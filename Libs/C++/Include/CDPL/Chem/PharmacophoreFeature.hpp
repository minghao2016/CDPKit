/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * PharmacophoreFeature.hpp
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
 * \brief Definition of the class CDPL::Chem::PharmacophoreFeature.
 */

#ifndef CDPL_CHEM_PHARMACOPHOREFEATURE_HPP
#define CDPL_CHEM_PHARMACOPHOREFEATURE_HPP

#include "CDPL/Chem/APIPrefix.hpp"
#include "CDPL/Base/PropertyContainer.hpp"


namespace CDPL 
{

    namespace Chem 
    {

	class Pharmacophore;

	/**
	 * \addtogroup CDPL_CHEM_PHARMACOPHORE_INTERFACES
	 * @{
	 */

	/**
	 * \brief PharmacophoreFeature.
	 */
	class CDPL_CHEM_API PharmacophoreFeature : public Base::PropertyContainer
	{

	  public:
	    /**
	     * \brief Returns a \c const reference to the parent pharmacophore.
	     * \return A \c const reference to the parent pharmacophore.
	     */
	    virtual const Pharmacophore& getPharmacophore() const = 0;

	    /**
	     * \brief Returns a non-\c const reference to the parent pharmacophore.
	     * \return A non-\c const reference to the parent pharmacophore.
	     */
	    virtual Pharmacophore& getPharmacophore() = 0;
	
	    /**
	     * \brief Returns the index of the feature in its parent pharmacophore.
	     * \return The zero-based index of the feature.
	     */
	    virtual std::size_t getIndex() const = 0;
		
	    /**
	     * \brief Assignment operator that replaces the current set of properties with the properties of \a feature;
	     * \param feature The feature whose properties get copied.
	     * \return A reference to itself.
	     */
	    PharmacophoreFeature& operator=(const PharmacophoreFeature& feature);

	  protected:
	    /**
	     * \brief Virtual destructor.
	     */
	    virtual ~PharmacophoreFeature() {}
	};

	/**
	 * @}
	 */
    }
}

#endif // CDPL_CHEM_PHARMACOPHOREFEATURE_HPP