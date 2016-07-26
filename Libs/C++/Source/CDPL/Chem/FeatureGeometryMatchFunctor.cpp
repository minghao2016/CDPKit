/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * FeatureGeometryMatchFunctor.cpp 
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

#include <cmath>

#include "CDPL/Chem/FeatureGeometryMatchFunctor.hpp"
#include "CDPL/Chem/Feature.hpp"
#include "CDPL/Chem/FeatureFunctions.hpp"
#include "CDPL/Chem/FeatureGeometry.hpp"
#include "CDPL/Chem/FeatureType.hpp"
#include "CDPL/Math/VectorAdapter.hpp"
#include "CDPL/Math/VectorProxy.hpp"


using namespace CDPL; 


const double Chem::FeatureGeometryMatchFunctor::DEF_HBA_ANGLE_TOLERANCE;
const double Chem::FeatureGeometryMatchFunctor::DEF_HBD_ANGLE_TOLERANCE;
const double Chem::FeatureGeometryMatchFunctor::DEF_AR_PLANE_ANGLE_TOLERANCE;


double Chem::FeatureGeometryMatchFunctor::getHBondAcceptorAngleTolerance() const
{
	return hbaVecAngleTol;
}

double Chem::FeatureGeometryMatchFunctor::getHBondDonorAngleTolerance() const
{
	return hbdVecAngleTol;
}

double Chem::FeatureGeometryMatchFunctor::getAromPlaneAngleTolerance() const
{
	return arPlaneAngleTol;
}

bool Chem::FeatureGeometryMatchFunctor::queryMode() const
{
	return qryMode;
}

bool Chem::FeatureGeometryMatchFunctor::operator()(const Feature& ftr1, const Feature& ftr2, const Math::Matrix4D& xform) const
{
	double ang_tol = 0.0;
	unsigned int ftr1_type = getType(ftr1);

	switch (ftr1_type) {

		case FeatureType::H_BOND_DONOR:
			ang_tol = hbdVecAngleTol;
			break;

		case FeatureType::H_BOND_ACCEPTOR:
			ang_tol = hbdVecAngleTol;
			break;

		case FeatureType::AROMATIC:
			ang_tol = arPlaneAngleTol;
			break;

		default:
			return true;
	}

	if (ftr1_type != getType(ftr2))
		return true;

	unsigned int ftr1_geom = getGeometry(ftr1);
	unsigned int ftr2_geom = getGeometry(ftr2);

	if (qryMode && ftr1_geom != ftr2_geom)
		return false;

	if (ftr1_geom != FeatureGeometry::VECTOR && ftr1_geom != FeatureGeometry::PLANE)
		return true;

	if (ftr2_geom != FeatureGeometry::VECTOR && ftr2_geom != FeatureGeometry::PLANE)
		return true;

	if (ftr1_geom != ftr2_geom)
		return false;

	if (!hasOrientation(ftr1))
		return false;

	if (!hasOrientation(ftr2))
		return false;

	const Math::Vector3D orient2 = getOrientation(ftr2);
	Math::Vector3D trans_or2;

	trans_or2.assign(range(prod(xform, homog(orient2)), 0, 3));
	trans_or2.minusAssign(range(prod(xform, Math::UnitVector<double>(4, 3)), 0, 3));

	double ang = std::acos(angleCos(getOrientation(ftr1), range(trans_or2, 0, 3), 1.0)) / M_PI * 180.0;

	return (ang <= ang_tol);
}