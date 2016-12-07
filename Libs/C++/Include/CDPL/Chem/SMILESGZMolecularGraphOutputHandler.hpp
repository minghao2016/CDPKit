/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * SMILESGZMolecularGraphOutputHandler.hpp 
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
 * \brief Definition of the class CDPL::Chem::SMILESGZMolecularGraphOutputHandler.
 */

#ifndef CDPL_CHEM_SMILESGZMOLECULARGRAPHOUTPUTHANDLER_HPP
#define CDPL_CHEM_SMILESGZMOLECULARGRAPHOUTPUTHANDLER_HPP

#include "CDPL/Chem/DataFormat.hpp"
#include "CDPL/Chem/SMILESGZMolecularGraphWriter.hpp"
#include "CDPL/Util/DefaultDataOutputHandler.hpp"


namespace CDPL 
{

	namespace Chem
	{

		/**
		 * \addtogroup CDPL_CHEM_SMILES_IO
		 * @{
		 */

		/**
		 * \brief A handler for the output of gzip-compressed molecular graph data in the <em>Daylight SMILES</em> [\ref SMILES] format.
		 */
		typedef Util::DefaultDataOutputHandler<SMILESGZMolecularGraphWriter, DataFormat::SMILES_GZ> SMILESGZMolecularGraphOutputHandler;

		/**
		 * @}
		 */
	}
}

#endif // CDPL_CHEM_SMILESGZMOLECULARGRAPHOUTPUTHANDLER_HPP
