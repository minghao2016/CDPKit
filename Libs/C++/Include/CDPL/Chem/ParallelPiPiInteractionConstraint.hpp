/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * ParallelPiPiInteractionConstraint.hpp 
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
 * \brief Definition of the class CDPL::Chem::ParallelPiPiInteractionConstraint.
 */

#ifndef CDPL_CHEM_PARALLELPIPIINTERACTIONCONSTRAINT_HPP
#define CDPL_CHEM_PARALLELPIPIINTERACTIONCONSTRAINT_HPP

#include <functional>

#include "CDPL/Chem/APIPrefix.hpp"


namespace CDPL 
{

    namespace Chem
    {

		class Feature;

		/**
		 * \addtogroup CDPL_CHEM_PHARMACOPHORE_PERCEPTION
		 * @{
		 */

		/**
		 * \brief ParallelPiPiInteractionConstraint.
		 */
		class CDPL_CHEM_API ParallelPiPiInteractionConstraint : public std::binary_function<Feature, Feature, bool>
		{

		  public:
			static const double DEF_MAX_H_DISTANCE  = 2.8;
			static const double DEF_MIN_V_DISTANCE  = 3.0;
			static const double DEF_MAX_V_DISTANCE  = 5.5;
			static const double DEF_ANGLE_TOLERANCE = 20.0;
	
			/**
			 * \brief Constructs a \c %ParallelPiPiInteractionConstraint functor with the specified constraints.
			 * \param min_v_dist The minimum allowed distance of the two feature-positions orthogonal to the ring-planes.
			 * \param max_v_dist The maximum allowed distance of the two feature-positions orthogonal to the ring-planes.
			 * \param max_h_dist The maximum allowed distance of the feature-position along the their ring-planes.
			 * \param ang_tol The maximum allowed angle deviation from 0° of the two ring-plane orientation vectors.
			 */
			ParallelPiPiInteractionConstraint(double min_v_dist = DEF_MIN_V_DISTANCE, double max_v_dist = DEF_MAX_V_DISTANCE,
											  double max_h_dist = DEF_MAX_H_DISTANCE,double ang_tol = DEF_ANGLE_TOLERANCE):
				minVDist(min_v_dist), maxVDist(max_v_dist),  maxHDist(max_h_dist), angleTol(ang_tol) {}

			double getMinVDistance() const;

			double getMaxVDistance() const;

			double getMaxHDistance() const;

			double getAngleTolerance() const;

			bool operator()(const Feature& ftr1, const Feature& ftr2) const;

		  private:
			/** \cond CDPL_PRIVATE_SECTION_DOC */
		
			double minVDist;
			double maxVDist;
			double maxHDist;
			double angleTol;

			/** \endcond */			
		};

		/**
		 * @}
		 */
    }
}

#endif // CDPL_CHEM_PARALLELPIPIINTERACTIONCONSTRAINT_HPP