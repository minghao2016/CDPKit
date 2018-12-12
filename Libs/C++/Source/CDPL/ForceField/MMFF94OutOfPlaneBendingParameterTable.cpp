/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * MMFF94OutOfPlaneBendingParameterTable.cpp 
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

#include <cstring>
#include <sstream>
#include <algorithm>

#include "CDPL/Config.hpp"

#include <boost/bind.hpp>

#if defined(HAVE_BOOST_IOSTREAMS)

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#endif // defined(HAVE_BOOST_IOSTREAMS)

#include "CDPL/ForceField/MMFF94OutOfPlaneBendingParameterTable.hpp"
#include "CDPL/Base/Exceptions.hpp"

#include "MMFF94ParameterData.hpp"
#include "DataIOUtilities.hpp"


using namespace CDPL; 


namespace
{
 
    ForceField::MMFF94OutOfPlaneBendingParameterTable::SharedPointer builtinDynTable(new ForceField::MMFF94OutOfPlaneBendingParameterTable());
    ForceField::MMFF94OutOfPlaneBendingParameterTable::SharedPointer builtinStatTable(new ForceField::MMFF94OutOfPlaneBendingParameterTable());

    struct Init
    {

		Init() {
			builtinDynTable->loadDefaults(false);
			builtinStatTable->loadDefaults(true);
		}

    } init;

	Base::uint32 lookupKey(Base::uint32 term_atom1_type, Base::uint32 ctr_atom_type, Base::uint32 term_atom2_type, Base::uint32 oop_atom_type)
	{
		unsigned int nbr_types[] = { term_atom1_type, term_atom2_type, oop_atom_type };

		std::sort(nbr_types, nbr_types + 3); 

		return ((nbr_types[0] << 24) + (nbr_types[1] << 16) + (nbr_types[2] << 8) + ctr_atom_type);
	}

	const ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry NOT_FOUND;
}


ForceField::MMFF94OutOfPlaneBendingParameterTable::SharedPointer ForceField::MMFF94OutOfPlaneBendingParameterTable::defaultDynTable  = builtinDynTable;
ForceField::MMFF94OutOfPlaneBendingParameterTable::SharedPointer ForceField::MMFF94OutOfPlaneBendingParameterTable::defaultStatTable = builtinStatTable;


ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::Entry():
	termAtom1Type(0), ctrAtomType(0), termAtom2Type(0), oopAtomType(0), forceConst(0), initialized(false)
{}

ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::Entry(unsigned int term_atom1_type, unsigned int ctr_atom_type, unsigned int term_atom2_type, 
																unsigned int oop_atom_type, double force_const):
	termAtom1Type(term_atom1_type), ctrAtomType(ctr_atom_type), termAtom2Type(term_atom2_type), oopAtomType(term_atom2_type),
	forceConst(force_const), initialized(true)
{}

unsigned int ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::getTerminalAtom1Type() const
{
	return termAtom1Type;
}

unsigned int ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::getCenterAtomType() const
{
	return ctrAtomType;
}

unsigned int ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::getTerminalAtom2Type() const
{
	return termAtom2Type;
}

unsigned int ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::getOutOfPlaneAtomType() const
{
	return oopAtomType;
}

double ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::getForceConstant() const
{
	return forceConst;
}

ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry::operator bool() const
{
	return initialized;
}


ForceField::MMFF94OutOfPlaneBendingParameterTable::MMFF94OutOfPlaneBendingParameterTable()
{}

void ForceField::MMFF94OutOfPlaneBendingParameterTable::addEntry(unsigned int term_atom1_type, unsigned int ctr_atom_type, 
																 unsigned int term_atom2_type, unsigned int oop_atom_type, double force_const)
{
    entries.insert(DataStorage::value_type(lookupKey(term_atom1_type,  ctr_atom_type, term_atom2_type, oop_atom_type), 
										   Entry(term_atom1_type, ctr_atom_type, term_atom2_type, oop_atom_type, force_const)));
}

const ForceField::MMFF94OutOfPlaneBendingParameterTable::Entry& 
ForceField::MMFF94OutOfPlaneBendingParameterTable::getEntry(unsigned int term_atom1_type, unsigned int ctr_atom_type, 
															unsigned int term_atom2_type, unsigned int oop_atom_type) const
{
	DataStorage::const_iterator it = entries.find(lookupKey(term_atom1_type,  ctr_atom_type, term_atom2_type, oop_atom_type));

	if (it == entries.end())
		return NOT_FOUND;

	return it->second;
}

std::size_t ForceField::MMFF94OutOfPlaneBendingParameterTable::getNumEntries() const
{
    return entries.size();
}

void ForceField::MMFF94OutOfPlaneBendingParameterTable::clear()
{
    entries.clear();
}

bool ForceField::MMFF94OutOfPlaneBendingParameterTable::removeEntry(unsigned int term_atom1_type, unsigned int ctr_atom_type, 
																	unsigned int term_atom2_type, unsigned int oop_atom_type)
{
	return entries.erase(lookupKey(term_atom1_type,  ctr_atom_type, term_atom2_type, oop_atom_type));
}

ForceField::MMFF94OutOfPlaneBendingParameterTable::EntryIterator 
ForceField::MMFF94OutOfPlaneBendingParameterTable::removeEntry(const EntryIterator& it)
{
	return EntryIterator(entries.erase(it.base()), boost::bind<Entry&>(&DataStorage::value_type::second, _1));
}

ForceField::MMFF94OutOfPlaneBendingParameterTable::ConstEntryIterator 
ForceField::MMFF94OutOfPlaneBendingParameterTable::getEntriesBegin() const
{
	return ConstEntryIterator(entries.begin(), boost::bind(&DataStorage::value_type::second, _1));
}

ForceField::MMFF94OutOfPlaneBendingParameterTable::ConstEntryIterator 
ForceField::MMFF94OutOfPlaneBendingParameterTable::getEntriesEnd() const
{
	return ConstEntryIterator(entries.end(), boost::bind(&DataStorage::value_type::second, _1));
}
	
ForceField::MMFF94OutOfPlaneBendingParameterTable::EntryIterator 
ForceField::MMFF94OutOfPlaneBendingParameterTable::getEntriesBegin()
{
	return EntryIterator(entries.begin(), boost::bind<Entry&>(&DataStorage::value_type::second, _1));
}

ForceField::MMFF94OutOfPlaneBendingParameterTable::EntryIterator 
ForceField::MMFF94OutOfPlaneBendingParameterTable::getEntriesEnd()
{
	return EntryIterator(entries.end(), boost::bind<Entry&>(&DataStorage::value_type::second, _1));
}

void ForceField::MMFF94OutOfPlaneBendingParameterTable::load(std::istream& is)
{
    std::string line;
	unsigned int term_atom1_type;
	unsigned int ctr_atom_type;
	unsigned int term_atom2_type;
	unsigned int oop_atom_type;
	double force_const;

    while (readMMFF94DataLine(is, line, "MMFF94OutOfPlaneBendingParameterTable: error while reading out-of-plane bending parameter entry")) {
		std::istringstream line_iss(line);

		if (!(line_iss >> term_atom1_type))
			throw Base::IOError("MMFF94OutOfPlaneBendingParameterTable: error while reading terminal atom 1 type");

		if (!(line_iss >> ctr_atom_type))
			throw Base::IOError("MMFF94OutOfPlaneBendingParameterTable: error while reading center atom type");

		if (!(line_iss >> term_atom2_type))
			throw Base::IOError("MMFF94OutOfPlaneBendingParameterTable: error while reading terminal atom 2 type");
	
		if (!(line_iss >> oop_atom_type))
			throw Base::IOError("MMFF94OutOfPlaneBendingParameterTable: error while reading out-of-plane atom type");

		if (!(line_iss >> force_const))
			throw Base::IOError("MMFF94OutOfPlaneBendingParameterTable: error while reading force constant");

		addEntry(term_atom1_type, ctr_atom_type, term_atom2_type, oop_atom_type, force_const);
    }
}

void ForceField::MMFF94OutOfPlaneBendingParameterTable::loadDefaults(bool mmff94s)
{
	if (mmff94s) {
#if defined(HAVE_BOOST_IOSTREAMS)

		boost::iostreams::stream<boost::iostreams::array_source> is(MMFF94ParameterData::STATIC_OUT_OF_PLANE_BENDING_PARAMETERS, 
																	std::strlen(MMFF94ParameterData::STATIC_OUT_OF_PLANE_BENDING_PARAMETERS));
#else // defined(HAVE_BOOST_IOSTREAMS)

		std::istringstream is(std::string(MMFF94ParameterData::STATIC_OUT_OF_PLANE_BENDING_PARAMETERS));

#endif // defined(HAVE_BOOST_IOSTREAMS)

		load(is);

	} else {
#if defined(HAVE_BOOST_IOSTREAMS)

		boost::iostreams::stream<boost::iostreams::array_source> is(MMFF94ParameterData::OUT_OF_PLANE_BENDING_PARAMETERS, 
																	std::strlen(MMFF94ParameterData::OUT_OF_PLANE_BENDING_PARAMETERS));
#else // defined(HAVE_BOOST_IOSTREAMS)

		std::istringstream is(std::string(MMFF94ParameterData::OUT_OF_PLANE_BENDING_PARAMETERS));

#endif // defined(HAVE_BOOST_IOSTREAMS)

		load(is);
	}
}

void ForceField::MMFF94OutOfPlaneBendingParameterTable::set(const SharedPointer& table, bool mmff94s)
{	
	if (mmff94s) 
		defaultStatTable = (!table ? builtinStatTable : table);
	else
		defaultDynTable = (!table ? builtinDynTable : table);
}

const ForceField::MMFF94OutOfPlaneBendingParameterTable::SharedPointer& ForceField::MMFF94OutOfPlaneBendingParameterTable::get(bool mmff94s)
{
    return (mmff94s ? defaultStatTable : defaultDynTable);
}