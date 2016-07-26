/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * FeaturePositionMatchFunctor.hpp 
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
 * \brief Definition of the class CDPL::Chem::FeaturePositionMatchFunctor.
 */

#ifndef CDPL_CHEM_FEATUREPOSITIONMATCHFUNCTOR_HPP
#define CDPL_CHEM_FEATUREPOSITIONMATCHFUNCTOR_HPP

#include "CDPL/Chem/APIPrefix.hpp"

#include "CDPL/Math/Matrix.hpp"


namespace CDPL 
{

    namespace Chem
    {

		class Feature;

		/**
		 * \addtogroup CDPL_CHEM_ALIGNMENT
		 * @{
		 */

		/**
		 * \brief FeaturePositionMatchFunctor.
		 */
		class CDPL_CHEM_API FeaturePositionMatchFunctor
		{

		  public:
			FeaturePositionMatchFunctor(bool query_mode): qryMode(query_mode) {}

			bool queryMode() const;

			/**
			 * \brief Checks if both \a ftr1 and \a ftr2 have a feature position within the respective tolerances.
			 * \param ftr1 The first feature.
			 * \param ftr2 The second feature.
			 * \param xform The transformation to apply to the position of the second feature.
			 * \return \c true if the feature positions are equivalent, and \c false otherwise.
			 * \see Chem::getPosition(const Feature&),  Chem::getTolerance(const Feature&)
			 */
			bool operator()(const Feature& ftr1, const Feature& ftr2, const Math::Matrix4D& xform) const;

		  private:
			bool qryMode;
		};

		/**
		 * @}
		 */
    }
}

#endif // CDPL_CHEM_FEATUREPOSITIONMATCHFUNCTOR_HPP