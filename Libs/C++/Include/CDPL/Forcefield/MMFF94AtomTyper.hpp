/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * MMFF94AtomTyper.hpp 
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
 * \brief Definition of the class CDPL::Forcefield::MMFF94AtomTyper.
 */

#ifndef CDPL_FORCEFIELD_MMFF94ATOMTYPER_HPP
#define CDPL_FORCEFIELD_MMFF94ATOMTYPER_HPP

#include "CDPL/Forcefield/APIPrefix.hpp"
#include "CDPL/Chem/MolecularGraph.hpp"


namespace CDPL 
{

    namespace Forcefield 
    {

	/**
	 * \addtogroup CDPL_FORCEFIELD_ATOM_TYPE_PERCEPTION
	 * @{
	 */

	class CDPL_FORCEFIELD_API MMFF94AtomTyper
	{

	  public:
	    MMFF94AtomTyper();

	  private:
	};
    
	/**
	 * @}
	 */
    }
}

#endif // CDPL_FORCEFIELD_MMFF94ATOMTYPER_HPP
