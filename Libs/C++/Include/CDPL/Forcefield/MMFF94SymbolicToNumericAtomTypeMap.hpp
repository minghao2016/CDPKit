/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * MMFF94SymbolicToNumericAtomTypeMap.hpp 
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

/**
 * \file
 * \brief Definition of the class CDPL::Forcefield::MMFF94SymbolicToNumericAtomTypeMap.
 */

#ifndef CDPL_FORCEFIELD_MMFF94SYMBOLICTONUMERICATOMTYPEMAP_HPP
#define CDPL_FORCEFIELD_MMFF94SYMBOLICTONUMERICATOMTYPEMAP_HPP

#include <string>
#include <iosfwd>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include "CDPL/Forcefield/APIPrefix.hpp"


namespace CDPL 
{

    namespace Forcefield 
    {

	/**
	 * \addtogroup CDPL_FORCEFIELD_PARAMETER_TABLES
	 * @{
	 */

	class CDPL_FORCEFIELD_API MMFF94SymbolicToNumericAtomTypeMap
	{

	    typedef boost::unordered_map<std::string, unsigned int> DataStorage;

	  public:
	    typedef boost::shared_ptr<MMFF94SymbolicToNumericAtomTypeMap> SharedPointer;

	    typedef DataStorage::const_iterator ConstEntryIterator;
		typedef DataStorage::value_type Entry;

	    MMFF94SymbolicToNumericAtomTypeMap();

	    void addEntry(const std::string& sym_type, unsigned int num_type);

	    unsigned int getNumericType(const std::string& sym_type) const;

	    void clear();

	    ConstEntryIterator getEntriesBegin() const;

	    ConstEntryIterator getEntriesEnd() const;
			
	    void load(std::istream& is);

	    void loadDefaults();

	    static void set(const MMFF94SymbolicToNumericAtomTypeMap* table);

	    static const MMFF94SymbolicToNumericAtomTypeMap& get();

	  private:
	    static const MMFF94SymbolicToNumericAtomTypeMap* defaultTable;
	    DataStorage                                      entries;
	};
    
	/**
	 * @}
	 */
    }
}

#endif // CDPL_FORCEFIELD_MMFF94SYMBOLICTONUMERICATOMTYPEMAP_HPP
