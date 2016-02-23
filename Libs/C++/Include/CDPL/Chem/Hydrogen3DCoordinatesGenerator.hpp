/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/*
 * Hydrogen3DCoordinatesGenerator.hpp 
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
 * \brief Definition of the class CDPL::Chem::Hydrogen3DCoordinatesGenerator.
 */

#ifndef CDPL_CHEM_HYDROGEN3DCOORDINATESGENERATOR_HPP
#define CDPL_CHEM_HYDROGEN3DCOORDINATESGENERATOR_HPP

#include <cstddef>
#include <vector>

#include "CDPL/Chem/APIPrefix.hpp"
#include "CDPL/Math/VectorArray.hpp"
#include "CDPL/Util/BitSet.hpp"


namespace CDPL 
{

	namespace Chem
	{
	 	
		class MolecularGraph;
		class Atom;

		/**
		 * \addtogroup CDPL_CHEM_3D_COORDINATES_GENERATION
		 * @{
		 */

		/**
		 * \brief Hydrogen3DCoordinatesGenerator.
		 */
		class CDPL_CHEM_API Hydrogen3DCoordinatesGenerator 
		{

		public:
			/**
			 * \brief Constructs the \c %Hydrogen3DCoordinatesGenerator instance.
			 */
			Hydrogen3DCoordinatesGenerator();

			/**
			 * \brief Constructs the \c %Hydrogen3DCoordinatesGenerator instance and generates 3D coordinates for
			 *        the hydrogen atoms of the molecular graph \a molgraph.
			 *
			 * The generated coordinates can be retrieved by a call to getResult().
			 *
			 * \param molgraph The molecular graph for which to generate 3D coordinates.
			 * \param undef_only Specifies whether or not to recalculate already defined hydrogen atom coordinates.
			 */
			Hydrogen3DCoordinatesGenerator(const MolecularGraph& molgraph, bool undef_only = true);

			/**
			 * \brief Allows to specify whether already defined hydrogen atom coordinates have to be recalculated or are left unchanged.
			 * \param undef_only \c true if already defined hydrogen atom coordinates have to be left unchanged, and \c false otherwise.
			 * \note The default setting is to calculate coordinates only for hydrogens with not yet defined positions.
			 */
			void undefinedOnly(bool undef_only);
			
			/**
			 * \brief Tells whether already defined hydrogen atom coordinates are recalculated or left unchanged.
			 * \return \c true if already defined hydrogen atom coordinates are left unchanged, and \c false otherwise.
			 */
			bool undefinedOnly() const;

			/**
			 * \brief Generates 3D coordinates for the hydrogen atoms of the molecular graph \a molgraph.
			 * \param molgraph The molecular graph for which to generate 3D coordinates.
			 * \return An array containing the heavy atom and generated hydrogen 3D coordinates. The coordinates
			 *         are stored in the same order as the atoms appear in the atom list of
			 *         the molecular graph (i.e. the coordinates of an atom are accessible via
			 *         its index).
			 */
			const Math::Vector3DArray& generate(const MolecularGraph& molgraph);

			/**
			 * \brief Returns the result of the last 3D coordinates calculation.
			 * \return An array containing the generated 3D coordinates. If a calculation
			 *         has not yet been performed, the returned array is empty.
			 * \see generate()
			 */
			const Math::Vector3DArray& getResult() const;

		private:
			/** \cond CDPL_PRIVATE_SECTION_DOC */

			typedef std::vector<std::size_t> AtomIndexList;
				
			void init(const MolecularGraph&);

			void assignHydrogenCoords();

			void assignDiatomicMolNbrCoords(const Atom&, std::size_t);
			void assignLinearGeomCoords(const Atom&, std::size_t, std::size_t);

			std::size_t getConnectedAtoms(const Atom&);

			double getHydrogenBondLength(const Atom&) const;

			const MolecularGraph* molGraph;
			bool                  undefOnly;
			Math::Vector3DArray   coordinates;
			Util::BitSet          defCoordsMask;
			AtomIndexList         centerAtoms;
			AtomIndexList         conctdAtoms;

			/** \endcond */
		};

		/**
		 * @}
		 */
	}
}

#endif // CDPL_CHEM_HYDROGEN3DCOORDINATESGENERATOR_HPP