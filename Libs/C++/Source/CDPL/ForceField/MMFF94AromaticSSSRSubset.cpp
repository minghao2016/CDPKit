/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * MMFF94AromaticSSSRSubset.cpp 
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

#include <algorithm>

#include <boost/bind.hpp>

#include "CDPL/ForceField/MMFF94AromaticSSSRSubset.hpp"
#include "CDPL/Chem/Bond.hpp"
#include "CDPL/Chem/MolecularGraphFunctions.hpp"
#include "CDPL/Chem/UtilityFunctions.hpp"


using namespace CDPL;


ForceField::MMFF94AromaticSSSRSubset::MMFF94AromaticSSSRSubset(const Chem::MolecularGraph& molgraph)
{
    extract(molgraph);
}

void ForceField::MMFF94AromaticSSSRSubset::extract(const Chem::MolecularGraph& molgraph)
{
    clear();

    if (molgraph.getNumAtoms() == 0 || molgraph.getNumBonds() == 0)
		return;

	init(molgraph);
    perceiveAromaticRings();
}

void ForceField::MMFF94AromaticSSSRSubset::init(const Chem::MolecularGraph& molgraph)
{
	aromBondMask.resize(molgraph.getNumBonds());
	aromBondMask.reset();

	candidateRings.clear();

	molGraph = &molgraph;
}

void ForceField::MMFF94AromaticSSSRSubset::perceiveAromaticRings()
{
    using namespace Chem;

    const FragmentList::BaseType& sssr = *getSSSR(*molGraph);

    for (FragmentList::BaseType::ConstElementIterator it = sssr.getElementsBegin(), end = sssr.getElementsEnd(); it != end; ++it) {
		const Fragment::SharedPointer& ring = *it;
		std::size_t r_size = ring->getNumAtoms();

		if (r_size < 4 && r_size > 6)
			continue;

		if (!isNotAromatic(*ring, *molGraph)) {
			if (Chem::isAromatic(*ring, *molGraph, aromBondMask)) {
				addToAromaticBondMask(*ring);
				addElement(ring);

			} else
				candidateRings.push_back(ring);
		}
    }
	
    for (bool new_aro_rings = true; new_aro_rings; ) {
		new_aro_rings = false;

		for (RingList::iterator it = candidateRings.begin(); it != candidateRings.end(); ) {
			const Fragment::SharedPointer& ring = *it;

			if (!Chem::isAromatic(*ring, *molGraph, aromBondMask)) {
				++it;
				continue;
			}

			addToAromaticBondMask(*ring);
			addElement(ring);

			new_aro_rings = true;
			it = candidateRings.erase(it);
		}
    }
}

void ForceField::MMFF94AromaticSSSRSubset::addToAromaticBondMask(const Chem::Fragment& ring)
{
    using namespace Chem;

    std::for_each(ring.getBondsBegin(), ring.getBondsEnd(),
				  boost::bind(&Util::BitSet::set, boost::ref(aromBondMask),
							  boost::bind(&BondContainer::getBondIndex, molGraph, _1), true));
}
