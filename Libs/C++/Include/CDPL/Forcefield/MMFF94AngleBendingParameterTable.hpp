/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * MMFF94AngleBendingParameterTable.hpp 
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
 * \brief Definition of the class CDPL::Forcefield::MMFF94AngleBendingParameterTable.
 */

#ifndef CDPL_FORCEFIELD_MMFF94ANGLEBENDINGPARAMETERTABLE_HPP
#define CDPL_FORCEFIELD_MMFF94ANGLEBENDINGPARAMETERTABLE_HPP

#include <cstddef>
#include <iosfwd>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "CDPL/Forcefield/APIPrefix.hpp"
#include "CDPL/Base/IntTypes.hpp"


namespace CDPL 
{

    namespace Forcefield 
    {

		/**
		 * \addtogroup CDPL_FORCEFIELD_PARAMETER_TABLES
		 * @{
		 */

		class CDPL_FORCEFIELD_API MMFF94AngleBendingParameterTable
		{

		  public:
			class Entry;

		  private:
			typedef boost::unordered_map<Base::uint32, Entry> DataStorage;

		  public:
			typedef boost::shared_ptr<MMFF94AngleBendingParameterTable> SharedPointer;
	
			class CDPL_FORCEFIELD_API Entry
			{

			  public:
				Entry();
 
				Entry(unsigned int angle_type_idx, unsigned int nbr_atom1_type, unsigned int ctr_atom_type, 
					  unsigned int nbr_atom2_type, double force_const, double ref_angle);

				unsigned int getAngleTypeIndex() const;

				unsigned int getNeighborAtom1Type() const;

				unsigned int getCenterAtomType() const;

				unsigned int getNeighborAtom2Type() const;

				double getForceConstant() const;

				double getReferenceBondAngle() const;

				operator bool() const;

			  private:
				unsigned int angleTypeIdx;
				unsigned int nbrAtom1Type;
				unsigned int ctrAtomType;
				unsigned int nbrAtom2Type;
				double       forceConst;
				double       refAngle;
				bool         initialized;
			};			

			typedef boost::transform_iterator<boost::function1<const Entry&, const DataStorage::value_type&>, 
											  DataStorage::const_iterator> ConstEntryIterator;

			typedef boost::transform_iterator<boost::function1<Entry&, DataStorage::value_type&>, 
											  DataStorage::iterator> EntryIterator;
	
			MMFF94AngleBendingParameterTable();

			void addEntry(unsigned int angle_type_idx, unsigned int nbr_atom1_type, unsigned int ctr_atom_type, 
						  unsigned int nbr_atom2_type, double force_const, double ref_angle);

			const Entry& getEntry(unsigned int angle_type_idx, unsigned int nbr_atom1_type, unsigned int ctr_atom_type, 
								  unsigned int nbr_atom2_type) const;

			std::size_t getNumEntries() const;

			void clear();

			bool removeEntry(unsigned int angle_type_idx, unsigned int nbr_atom1_type, unsigned int ctr_atom_type, 
							 unsigned int nbr_atom2_type);

			EntryIterator removeEntry(const EntryIterator& it);

			ConstEntryIterator getEntriesBegin() const;

			ConstEntryIterator getEntriesEnd() const;
	
			EntryIterator getEntriesBegin();

			EntryIterator getEntriesEnd();

			void load(std::istream& is);

			void loadDefaults();

			static void set(const SharedPointer& table);

			static const SharedPointer& get();

		  private:
			static SharedPointer defaultTable;
			DataStorage          entries;
		};
    
		/**
		 * @}
		 */
    }
}

#endif // CDPL_FORCEFIELD_MMFF94ANGLEBENDINGPARAMETERTABLE_HPP
