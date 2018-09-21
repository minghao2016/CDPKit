/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * MolecularGraphDGConstraintGenerator.cpp 
 *
 * This file is part of the Chemical Data Processing Toolkit
 *
 * Copyright (C) 2003-2010 Thomas A. Seidel <thomas.seidel@univie.ac.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING. If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

 
#include "StaticInit.hpp"

#include <cmath>
#include <algorithm>

#include <boost/tuple/tuple_comparison.hpp>
#include <boost/bind.hpp>

#include "CDPL/ConfGen/MolecularGraphDGConstraintGenerator.hpp"
#include "CDPL/Chem/FragmentList.hpp"
#include "CDPL/Chem/MolecularGraph.hpp"
#include "CDPL/Chem/Atom.hpp"
#include "CDPL/Chem/Bond.hpp"
#include "CDPL/Chem/AtomFunctions.hpp"
#include "CDPL/Chem/BondFunctions.hpp"
#include "CDPL/Chem/MolecularGraphFunctions.hpp"
#include "CDPL/Chem/AtomContainerFunctions.hpp"
#include "CDPL/Chem/AtomDictionary.hpp"
#include "CDPL/Chem/StereoDescriptor.hpp"
#include "CDPL/Chem/HybridizationState.hpp"
#include "CDPL/Chem/AtomConfiguration.hpp"
#include "CDPL/Chem/BondConfiguration.hpp"
#include "CDPL/Chem/AtomType.hpp"
#include "CDPL/ForceField/MMFF94InteractionData.hpp"
#include "CDPL/ForceField/MMFF94BondStretchingInteractionData.hpp"
#include "CDPL/ForceField/MMFF94BondStretchingInteraction.hpp"
#include "CDPL/ForceField/MMFF94AngleBendingInteractionData.hpp"
#include "CDPL/ForceField/MMFF94AngleBendingInteraction.hpp"


using namespace CDPL;


std::size_t ConfGen::MolecularGraphDGConstraintGenerator::BondAngleKeyHash::operator()(const BondAngleKey& k) const 
{
	size_t hash = 0;

	boost::hash_combine(hash, k.get<0>());
	boost::hash_combine(hash, k.get<1>());
	boost::hash_combine(hash, k.get<2>());

	return hash;
}


ConfGen::MolecularGraphDGConstraintGenerator::MolecularGraphDGConstraintGenerator():
    molGraph(0), noHydrogens(false), regAtomStereo(true), regBondStereo(true)
{}

void ConfGen::MolecularGraphDGConstraintGenerator::excludeHydrogens(bool exclude)
{
    noHydrogens = exclude;
}

bool ConfGen::MolecularGraphDGConstraintGenerator::hydrogensExcluded() const
{
    return noHydrogens;
}

void ConfGen::MolecularGraphDGConstraintGenerator::regardAtomStereo(bool regard)
{
    regAtomStereo = regard;
}

bool ConfGen::MolecularGraphDGConstraintGenerator::atomStereoRegarded() const
{
    return regAtomStereo;
}

void ConfGen::MolecularGraphDGConstraintGenerator::regardBondStereo(bool regard)
{
    regBondStereo = regard;
}

bool ConfGen::MolecularGraphDGConstraintGenerator::bondStereoRegarded() const
{
    return regBondStereo;
}

void CDPL::ConfGen::MolecularGraphDGConstraintGenerator::setup(const Chem::MolecularGraph& molgraph, 
															   const ForceField::MMFF94InteractionData& ia_data)
{
    init(molgraph);
  
	if (regAtomStereo)
		getAtomStereoDescriptors();

	if (regBondStereo)
		getBondStereoDescriptors();

	assignBondLengths(ia_data);
    assignBondAngles(ia_data);
}

void ConfGen::MolecularGraphDGConstraintGenerator::addBondLengthConstraints(Util::DG3DCoordinatesGenerator& coords_gen)
{
	for (BondLengthTable::const_iterator it = bondLengthTable.begin(), end = bondLengthTable.end(); it != end; ++it) {
		double bond_len = it->second;
		std::size_t atom1_idx = it->first.first;
		std::size_t atom2_idx = it->first.second;

		if (atomPairProcessed(atom1_idx, atom2_idx))
			continue;

		coords_gen.addDistanceConstraint(atom1_idx, atom2_idx, bond_len, bond_len);
		markAtomPairProcessed(atom1_idx, atom2_idx);
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::addBondAngleConstraints(Util::DG3DCoordinatesGenerator& coords_gen)
{
	for (BondAngleTable::const_iterator it = bondAngleTable.begin(), end = bondAngleTable.end(); it != end; ++it) {
		std::size_t atom1_idx = it->first.get<0>();
		std::size_t atom2_idx = it->first.get<1>();
		std::size_t atom3_idx = it->first.get<2>();

		if (atomPairProcessed(atom1_idx, atom3_idx))
			continue;

		double angle = it->second;
		double bond1_len = getBondLength(atom2_idx, atom1_idx);
		double bond2_len = getBondLength(atom2_idx, atom3_idx);

		double dist = calc13AtomDistance(bond1_len, bond2_len, angle);

		coords_gen.addDistanceConstraint(atom1_idx, atom3_idx, dist, dist);
		markAtomPairProcessed(atom1_idx, atom3_idx);
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::addDefaultDistanceConstraints(Util::DG3DCoordinatesGenerator& coords_gen)
{
	using namespace Chem;

	if (!molGraph)
		return;

	double bond_length_sum = std::accumulate(bondLengthTable.begin(), bondLengthTable.end(), double(),
											 boost::bind(std::plus<double>(), _1,
														 boost::bind(&BondLengthTable::value_type::second, _2)));
	for (std::size_t i = 0; i < numAtoms; i++) {
		if (noHydrogens && hAtomMask.test(i))
			continue;

		double vdw_rad1 = AtomDictionary::getVdWRadius(getType(molGraph->getAtom(i)));

		for (std::size_t j = i + 1; j < numAtoms; j++) {
			if (noHydrogens && hAtomMask.test(j))
				continue;

			if (atomPairProcessed(i, j))
				continue;

			double vdw_rad2 = AtomDictionary::getVdWRadius(getType(molGraph->getAtom(j)));

			coords_gen.addDistanceConstraint(i, j, vdw_rad1 + vdw_rad2, bond_length_sum);
			markAtomPairProcessed(i, j);
		}
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::add14DistanceConstraints(Util::DG3DCoordinatesGenerator& coords_gen)
{
	using namespace Chem;

	if (!molGraph)
		return;

	const FragmentList& sssr = *getSSSR(*molGraph);

	for (MolecularGraph::ConstBondIterator it = molGraph->getBondsBegin(), end = molGraph->getBondsEnd(); it != end; ++it) {
		const Bond& bond = *it;
		const Atom& atom1 = bond.getBegin();

		if (!molGraph->containsAtom(atom1))
			continue;

		std::size_t atom1_idx = molGraph->getAtomIndex(atom1);

		if (noHydrogens && hAtomMask.test(atom1_idx))
			continue;

		const Atom& atom2 = bond.getEnd();

		if (!molGraph->containsAtom(atom2))
			continue;

		std::size_t atom2_idx = molGraph->getAtomIndex(atom2);

		if (noHydrogens && hAtomMask.test(atom2_idx))
			continue;

		if (getNeighborAtoms(atom1, atomIndexList1, &atom2) == 0)
			continue;
	
		if (getNeighborAtoms(atom2, atomIndexList2, &atom1) == 0)
			continue;

		double bond_len = getBondLength(atom1_idx, atom2_idx);
		bool ring_check = getRingFlag(bond);

		for (std::size_t i = 0, num_nbrs1 = atomIndexList1.size(); i < num_nbrs1; i++) {
			std::size_t nbr_atom1_idx = atomIndexList1[i];
			double nbr_bond1_angle = getBondAngle(nbr_atom1_idx, atom1_idx, atom2_idx);

			if (nbr_bond1_angle <= 0.0)
				continue;

			double nbr_bond1_len = getBondLength(nbr_atom1_idx, atom1_idx);

			for (std::size_t j = 0, num_nbrs2 = atomIndexList2.size(); j < num_nbrs2; j++) {
				std::size_t nbr_atom2_idx = atomIndexList2[j];

				if (atomPairProcessed(nbr_atom1_idx, nbr_atom2_idx))
					continue;

				double nbr_bond2_angle = getBondAngle(nbr_atom2_idx, atom2_idx, atom1_idx);
				
				if (nbr_bond2_angle <= 0.0)
					continue;

				double nbr_bond2_len = getBondLength(nbr_atom2_idx, atom2_idx);
				bool cis_only = false;

				if (ring_check) {
					std::size_t ring_size = getSmallestRingSize(sssr, nbr_atom1_idx, nbr_atom2_idx);

					if (ring_size > 0 && ring_size < 8)
						cis_only = true;
				}
			
				double cis_dist = calcCis14AtomDistance(nbr_bond1_len, bond_len, nbr_bond2_len, nbr_bond1_angle, nbr_bond2_angle);				

				if (cis_only) {
					coords_gen.addDistanceConstraint(nbr_atom1_idx, nbr_atom2_idx, cis_dist, cis_dist);
					markAtomPairProcessed(nbr_atom1_idx, nbr_atom2_idx);
					continue;
				}

				double trans_dist = calcTrans14AtomDistance(nbr_bond1_len, bond_len, nbr_bond2_len, nbr_bond1_angle, nbr_bond2_angle);

				coords_gen.addDistanceConstraint(nbr_atom1_idx, nbr_atom2_idx, cis_dist, trans_dist);
				markAtomPairProcessed(nbr_atom1_idx, nbr_atom2_idx);
			}
		}
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::addAtomPlanarityConstraints(Util::DG3DCoordinatesGenerator& coords_gen)
{
	using namespace Chem;

	if (!molGraph)
		return;

	for (std::size_t i = 0; i < numAtoms; i++) {
		if (noHydrogens && hAtomMask.test(i))
			continue;

		const Atom& atom = molGraph->getAtom(i);

		if (!isPlanar(atom))
			continue;

		if (getNeighborAtoms(atom, atomIndexList1) != 3)
			continue;

		coords_gen.addVolumeConstraint(i, atomIndexList1[0], atomIndexList1[1], atomIndexList1[2], 0.0, 0.0);
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::addBondPlanarityConstraints(Util::DG3DCoordinatesGenerator& coords_gen)
{
	using namespace Chem;

	if (!molGraph)
		return;

	for (MolecularGraph::ConstBondIterator it = molGraph->getBondsBegin(), end = molGraph->getBondsEnd(); it != end; ++it) {
		const Bond& bond = *it;
		const Atom& atom1 = bond.getBegin();

		if (!molGraph->containsAtom(atom1))
			continue;

		std::size_t atom1_idx = molGraph->getAtomIndex(atom1);

		if (noHydrogens && hAtomMask.test(atom1_idx))
			continue;

		const Atom& atom2 = bond.getEnd();

		if (!molGraph->containsAtom(atom2))
			continue;

		std::size_t atom2_idx = molGraph->getAtomIndex(atom2);

		if (noHydrogens && hAtomMask.test(atom2_idx))
			continue;

		if (!isPlanar(bond))
			continue;

		std::size_t num_nbrs1 = getNeighborAtoms(atom1, atomIndexList1, &atom2);

		if (num_nbrs1 < 1 && num_nbrs1 > 2)
			continue;
	
		std::size_t num_nbrs2 = getNeighborAtoms(atom2, atomIndexList2, &atom1);

		if (num_nbrs2 < 1 && num_nbrs2 > 2)
			continue;
	
		for (std::size_t i = 0; i < num_nbrs1; i++) {
			std::size_t nbr_atom1_idx = atomIndexList1[i];

			for (std::size_t j = 0; j < num_nbrs2; j++) {
				std::size_t nbr_atom2_idx = atomIndexList2[j];

				coords_gen.addVolumeConstraint(nbr_atom1_idx, atom1_idx, atom2_idx, nbr_atom2_idx, 0.0, 0.0);
			}
		}
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::init(const Chem::MolecularGraph& molgraph)
{
    if (noHydrogens)  {
		hAtomMask.reset();
		buildAtomTypeMask(molgraph, hAtomMask, Chem::AtomType::H);
	}

	molGraph = &molgraph;

	bondLengthTable.clear();
	bondAngleTable.clear();
	atomStereoDescrs.clear();
	bondStereoDescrs.clear();

    numAtoms = molgraph.getNumAtoms();
	
	procAtomPairMask.resize(numAtoms * numAtoms);
	procAtomPairMask.reset();
}

void ConfGen::MolecularGraphDGConstraintGenerator::assignBondLengths(const ForceField::MMFF94InteractionData& ia_data)
{
	using namespace ForceField;
	using namespace Chem;

	const MMFF94BondStretchingInteractionData& bs_data = ia_data.getBondStretchingInteractions();
	
	for (MMFF94BondStretchingInteractionData::ConstElementIterator it = bs_data.getElementsBegin(), end = bs_data.getElementsEnd(); it != end; ++it) {
		const MMFF94BondStretchingInteraction& iactn = *it;

		std::size_t atom1_idx = iactn.getAtom1Index();

		if (atom1_idx >= numAtoms)
			continue;

		std::size_t atom2_idx = iactn.getAtom2Index();

		if (atom2_idx >= numAtoms)
			continue;
		
		if (noHydrogens && (hAtomMask.test(atom1_idx) || hAtomMask.test(atom2_idx)))
			continue;

		setBondLength(atom1_idx, atom2_idx, iactn.getReferenceLength());
	} 

	// Fallback

	for (MolecularGraph::ConstBondIterator it = molGraph->getBondsBegin(), end = molGraph->getBondsEnd(); it != end; ++it) {
		const Bond& bond = *it;
		const Atom& atom1 = bond.getBegin();

		if (!molGraph->containsAtom(atom1))
			continue;

		const Atom& atom2 = bond.getEnd();

		if (!molGraph->containsAtom(atom2))
			continue;

		std::size_t atom1_idx = molGraph->getAtomIndex(atom1);
		std::size_t atom2_idx = molGraph->getAtomIndex(atom2);

		if (noHydrogens && (hAtomMask.test(atom1_idx) || hAtomMask.test(atom2_idx)))
			continue;
	
		if (getBondLength(atom1_idx, atom2_idx) >= 0.0)
			continue;

		std::size_t order = getOrder(bond);
		double cov_rad1 = AtomDictionary::getCovalentRadius(getType(atom1), order);
		double cov_rad2 = AtomDictionary::getCovalentRadius(getType(atom2), order);

		setBondLength(atom1_idx, atom2_idx, cov_rad1 + cov_rad2);
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::assignBondAngles(const ForceField::MMFF94InteractionData& ia_data)
{
	using namespace ForceField;
	using namespace Chem;

	const FragmentList& sssr = *getSSSR(*molGraph);
	const MMFF94AngleBendingInteractionData& bs_data = ia_data.getAngleBendingInteractions();
	
	for (MMFF94AngleBendingInteractionData::ConstElementIterator it = bs_data.getElementsBegin(), end = bs_data.getElementsBegin(); it != end; ++it) {
		const MMFF94AngleBendingInteraction& iactn = *it;

		std::size_t term_atom1_idx = iactn.getTerminalAtom1Index();

		if (term_atom1_idx >= numAtoms)
			continue;

		std::size_t ctr_atom_idx = iactn.getCenterAtomIndex();

		if (ctr_atom_idx >= numAtoms)
			continue;
	
		std::size_t term_atom2_idx = iactn.getTerminalAtom2Index();

		if (term_atom2_idx >= numAtoms)
			continue;
	
		if (noHydrogens && (hAtomMask.test(term_atom1_idx) || hAtomMask.test(ctr_atom_idx) || hAtomMask.test(term_atom2_idx)))
			continue;

		if (iactn.isLinearAngle())
			setBondAngle(term_atom1_idx, ctr_atom_idx, term_atom2_idx, M_PI);
		else
			setBondAngle(term_atom1_idx, ctr_atom_idx, term_atom2_idx, std::abs(iactn.getReferenceAngle()) * M_PI / 180.0);
	} 

	// Fallback

	for (std::size_t i = 0; i < numAtoms; i++) {
		if (noHydrogens && hAtomMask.test(i))
			continue;

		const Atom& ctr_atom = molGraph->getAtom(i);
		double angle = 0.0;

		switch (getHybridizationState(ctr_atom)) {

			case HybridizationState::SP1:
				angle = M_PI;
				break;

			case HybridizationState::SP2:
				angle = M_PI * 120.0 / 180.0;
				break;

			case HybridizationState::SP3:
				angle = M_PI * 109.5 / 180.0;
				break;
				
			default:
				continue;
		}

		bool ring_check = getRingFlag(ctr_atom);
		Atom::ConstBondIterator nb_it1 = ctr_atom.getBondsBegin();

		for (Atom::ConstAtomIterator na_it1 = ctr_atom.getAtomsBegin(), na_end = ctr_atom.getAtomsEnd(); na_it1 != na_end; ++na_it1) {
			const Atom& term_atom1 = *na_it1;
			const Bond& term_bond1 = *nb_it1;

			if (!molGraph->containsAtom(term_atom1) || !molGraph->containsBond(term_bond1))
				continue;

			std::size_t term_atom1_idx = molGraph->getAtomIndex(term_atom1);

			if (noHydrogens && hAtomMask.test(term_atom1_idx))
				continue;

			bool term_bond1_in_ring = (ring_check ? getRingFlag(term_bond1) : false);
			Atom::ConstBondIterator nb_it2 = ++nb_it1;

			for (Atom::ConstAtomIterator na_it2 = na_it1 + 1; na_it2 != na_end; ++na_it2) {
				const Atom& term_atom2 = *na_it2;
				const Bond& term_bond2 = *nb_it2;

				if (!molGraph->containsAtom(term_atom2) || !molGraph->containsBond(term_bond2))
					continue;

				std::size_t term_atom2_idx = molGraph->getAtomIndex(term_atom2);

				if (noHydrogens && hAtomMask.test(term_atom2_idx))
					continue;

				if (getBondAngle(term_atom1_idx, i, term_atom2_idx) >= 0.0)
					continue;

				if (term_bond1_in_ring && getRingFlag(term_bond2)) {
					switch (getSmallestRingSize(sssr, term_bond1, term_bond2)) {

						case 3:
							setBondAngle(term_atom1_idx, i, term_atom2_idx,  M_PI * 60.0 / 180.0);
							continue;

						case 4:
							if (angle == M_PI * 109.5 / 180.0) 
								setBondAngle(term_atom1_idx, i, term_atom2_idx,  M_PI * 88.5 / 180.0);
							else
								setBondAngle(term_atom1_idx, i, term_atom2_idx,  M_PI * 90.0 / 180.0);
							continue;

						default:
							break;
					}
				}

				setBondAngle(term_atom1_idx, i, term_atom2_idx, angle);
			}
		}
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::getAtomStereoDescriptors()
{
	using namespace Chem;

	for (MolecularGraph::ConstAtomIterator it = molGraph->getAtomsBegin(), end = molGraph->getAtomsEnd(); it != end; ++it) {
		const StereoDescriptor& descr = getStereoDescriptor(*it);

		if (descr.getConfiguration() != AtomConfiguration::R && descr.getConfiguration() != AtomConfiguration::S)
			continue;

		std::size_t num_ref_atoms = descr.getNumReferenceAtoms();

		if (num_ref_atoms < 3 || num_ref_atoms > 4)
			continue;

		bool valid = false;

		if (valid)
			atomStereoDescrs.push_back(&descr);
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::getBondStereoDescriptors()
{
	using namespace Chem;

	for (MolecularGraph::ConstBondIterator it = molGraph->getBondsBegin(), end = molGraph->getBondsEnd(); it != end; ++it) {
		const StereoDescriptor& descr = getStereoDescriptor(*it);

		if (descr.getConfiguration() != BondConfiguration::CIS && descr.getConfiguration() != BondConfiguration::TRANS)
			continue;

		bool valid = false;

		if (valid)
			bondStereoDescrs.push_back(&descr);
	}
}

void ConfGen::MolecularGraphDGConstraintGenerator::setBondLength(std::size_t atom1_idx, std::size_t atom2_idx, double length)
{
	if (atom1_idx > atom2_idx)
		std::swap(atom1_idx, atom2_idx);

	bondLengthTable[std::make_pair(atom1_idx, atom2_idx)] = length;
}

double ConfGen::MolecularGraphDGConstraintGenerator::getBondLength(std::size_t atom1_idx, std::size_t atom2_idx) const
{
	if (atom1_idx > atom2_idx)
		std::swap(atom1_idx, atom2_idx);

	BondLengthTable::const_iterator it = bondLengthTable.find(std::make_pair(atom1_idx, atom2_idx));

	if (it == bondLengthTable.end())
		return -1.0;

	return it->second;
}

void ConfGen::MolecularGraphDGConstraintGenerator::setBondAngle(std::size_t atom1_idx, std::size_t atom2_idx, std::size_t atom3_idx, double angle)
{
	if (atom1_idx > atom3_idx)
		std::swap(atom1_idx, atom3_idx);

	bondAngleTable[BondAngleKey(atom1_idx, atom2_idx, atom3_idx)] = angle;
}

double ConfGen::MolecularGraphDGConstraintGenerator::getBondAngle(std::size_t atom1_idx, std::size_t atom2_idx, std::size_t atom3_idx) const
{
	if (atom1_idx > atom3_idx)
		std::swap(atom1_idx, atom3_idx);

	BondAngleTable::const_iterator it = bondAngleTable.find(BondAngleKey(atom1_idx, atom2_idx, atom3_idx));

	if (it == bondAngleTable.end())
		return -1.0;

	return it->second;
}

std::size_t ConfGen::MolecularGraphDGConstraintGenerator::getSmallestRingSize(const Chem::FragmentList& sssr, const Chem::Bond& bond1, 
																			  const Chem::Bond& bond2) const
{
	using namespace Chem;

	std::size_t sm_size = 0;

	for (FragmentList::ConstElementIterator it = sssr.getElementsBegin(), end = sssr.getElementsEnd(); it != end; ++it) {
		const Fragment& ring = *it;

		if (!ring.containsBond(bond1) || !ring.containsBond(bond2))
			continue;

		if (sm_size == 0)
			sm_size = ring.getNumAtoms();
		else
			sm_size = std::min(sm_size, ring.getNumAtoms());
	}

	return sm_size;
} 

std::size_t ConfGen::MolecularGraphDGConstraintGenerator::getSmallestRingSize(const Chem::FragmentList& sssr, std::size_t atom1_idx, std::size_t atom2_idx) const
{
	using namespace Chem;

	std::size_t sm_size = 0;
	const Atom& atom1 = molGraph->getAtom(atom1_idx);
	const Atom& atom2 = molGraph->getAtom(atom2_idx);

	for (FragmentList::ConstElementIterator it = sssr.getElementsBegin(), end = sssr.getElementsEnd(); it != end; ++it) {
		const Fragment& ring = *it;

		if (!ring.containsAtom(atom1) || !ring.containsAtom(atom2))
			continue;

		if (sm_size == 0)
			sm_size = ring.getNumAtoms();
		else
			sm_size = std::min(sm_size, ring.getNumAtoms());
	}

	return sm_size;
} 

void ConfGen::MolecularGraphDGConstraintGenerator::markAtomPairProcessed(std::size_t atom1_idx, std::size_t atom2_idx)
{
	if (atom1_idx > atom2_idx)
		std::swap(atom1_idx, atom2_idx);

	procAtomPairMask.set(atom1_idx * numAtoms + atom2_idx);
}

bool ConfGen::MolecularGraphDGConstraintGenerator::atomPairProcessed(std::size_t atom1_idx, std::size_t atom2_idx) const
{
	if (atom1_idx > atom2_idx)
		std::swap(atom1_idx, atom2_idx);

	return procAtomPairMask.test(atom1_idx * numAtoms + atom2_idx);
}

double ConfGen::MolecularGraphDGConstraintGenerator::calc13AtomDistance(double bond1_len, double bond2_len, double angle) const
{
	double r1_2 = bond1_len * bond1_len;
	double r2_2 = bond2_len * bond2_len;
	double x = 2 * bond1_len * bond2_len * std::cos(angle);

	return std::sqrt(r1_2 + r2_2 - x);
}

double ConfGen::MolecularGraphDGConstraintGenerator::calcCis14AtomDistance(double bond1_len, double bond2_len, double bond3_len, 
																		   double angle_12, double angle_23) const
{
	return (bond2_len - bond1_len * std::cos(angle_12) - bond3_len * std::cos(angle_23));
}

double ConfGen::MolecularGraphDGConstraintGenerator::calcTrans14AtomDistance(double bond1_len, double bond2_len, double bond3_len, 
																			 double angle_12, double angle_23) const
{
	double x1 = bond1_len * std::cos(angle_12);
	double y1 = bond1_len * std::sin(angle_12);
	double x4 = bond2_len - bond3_len * std::cos(angle_23);
	double y4 = -bond3_len * std::sin(angle_23);

	double dx = x4 - x1;
	double dy = y4 - y1;

	return std::sqrt(dx * dx + dy * dy);
}

bool ConfGen::MolecularGraphDGConstraintGenerator::isPlanar(const Chem::Atom& atom) const
{
	using namespace Chem;

	if (getHybridizationState(atom) == HybridizationState::SP2)
		return true;

	if (getAromaticityFlag(atom))
		return true;

	unsigned int atom_type = getType(atom);

	if ((atom_type == AtomType::N || atom_type == AtomType::O) && hasNeighborWithDoubleBond(atom))
		return true;

	return false;
}

bool ConfGen::MolecularGraphDGConstraintGenerator::isPlanar(const Chem::Bond& bond) const
{
	using namespace Chem;

	if (getAromaticityFlag(bond))
		return true;

	std::size_t order = getOrder(bond);
	const Atom& atom1 = bond.getBegin();
	const Atom& atom2 = bond.getEnd();

	if (getAromaticityFlag(bond))
		return true;

	if (order == 2 && (getHybridizationState(atom1) == HybridizationState::SP2 || getAromaticityFlag(atom1)) &&
		(getHybridizationState(atom2) == HybridizationState::SP2 || getAromaticityFlag(atom2)))
		return true;

	if (order == 3)
		return true;

	if (order == 1) {
		unsigned int atom_type = getType(atom1);
	
		if ((atom_type == AtomType::N || atom_type == AtomType::O) && 
			((getAromaticityFlag(atom2) && getHeavyAtomCount(atom1, *molGraph) == 1)
			 || (getHybridizationState(atom2) == HybridizationState::SP2 && getExplicitBondCount(atom2, *molGraph, 2, AtomType::HET, false) == 1)))
			return true;

		atom_type = getType(atom2);

		if ((atom_type == AtomType::N || atom_type == AtomType::O) && 
			((getAromaticityFlag(atom1) && getHeavyAtomCount(atom2, *molGraph) == 1)
			 || (getHybridizationState(atom1) == HybridizationState::SP2 && getExplicitBondCount(atom1, *molGraph, 2, AtomType::HET, false) == 1)))
			return true;
	}

	return false;
}

bool ConfGen::MolecularGraphDGConstraintGenerator::hasNeighborWithDoubleBond(const Chem::Atom& atom) const
{
	using namespace Chem;

	Atom::ConstBondIterator b_it1 = atom.getBondsBegin();

	for (Atom::ConstAtomIterator a_it1 = atom.getAtomsBegin(), a_end1 = atom.getAtomsEnd(); a_it1 != a_end1; ++a_it1, ++b_it1) {
		const Atom& nbr_atom = *a_it1;

		if (!molGraph->containsAtom(nbr_atom))
			continue;

		if (!molGraph->containsBond(*b_it1))
			continue;

		Atom::ConstBondIterator b_it2 = nbr_atom.getBondsBegin();

		for (Atom::ConstAtomIterator a_it2 = nbr_atom.getAtomsBegin(), a_end2 = nbr_atom.getAtomsEnd(); a_it2 != a_end2; ++a_it2, ++b_it2) {
			const Atom& nbr_nbr_atom = *a_it2;

			if (&nbr_nbr_atom == &atom)
				continue;

			if (!molGraph->containsAtom(nbr_nbr_atom))
				continue;

			const Bond& nbr_nbr_bond = *b_it2;
			
			if (!molGraph->containsBond(nbr_nbr_bond))
				continue;

			if (getOrder(nbr_nbr_bond) > 1)
				return true;
		}
	}

	return false;
}

std::size_t ConfGen::MolecularGraphDGConstraintGenerator::getNeighborAtoms(const Chem::Atom& atom, AtomIndexList& idx_list, const Chem::Atom* x_atom) const
{
	using namespace Chem;

	idx_list.clear();

	Atom::ConstBondIterator b_it = atom.getBondsBegin();

	for (Atom::ConstAtomIterator a_it = atom.getAtomsBegin(), a_end = atom.getAtomsEnd(); a_it != a_end; ++a_it, ++b_it) {
		const Atom& nbr_atom = *a_it;

		if (&nbr_atom == x_atom)
			continue;

		if (!molGraph->containsAtom(nbr_atom))
			continue;

		if (!molGraph->containsBond(*b_it))
			continue;

		std::size_t atom_idx = molGraph->getAtomIndex(nbr_atom);

		if (noHydrogens && hAtomMask.test(atom_idx))
			continue;

		idx_list.push_back(atom_idx);
	}

	return idx_list.size();
}
