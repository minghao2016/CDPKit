/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * PharmacophoreFitScore.cpp 
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

#include "CDPL/Pharm/PharmacophoreFitScore.hpp"
#include "CDPL/Pharm/Pharmacophore.hpp"
#include "CDPL/Pharm/Feature.hpp"
#include "CDPL/Pharm/FeatureFunctions.hpp"
#include "CDPL/Pharm/FeatureType.hpp"


using namespace CDPL;


const double Pharm::PharmacophoreFitScore::DEF_FTR_MATCH_COUNT_FACTOR;
const double Pharm::PharmacophoreFitScore::DEF_FTR_POS_MATCH_FACTOR;
const double Pharm::PharmacophoreFitScore::DEF_FTR_GEOM_MATCH_FACTOR;


Pharm::PharmacophoreFitScore::PharmacophoreFitScore(double match_cnt_factor, double pos_match_factor, 
						    double geom_match_factor):
    ftrMatchCntFactor(match_cnt_factor), ftrPosMatchFactor(pos_match_factor), ftrGeomMatchFactor(geom_match_factor)
{}

double Pharm::PharmacophoreFitScore::getFeatureMatchCountFactor() const
{
    return ftrMatchCntFactor;
}

void Pharm::PharmacophoreFitScore::setFeatureMatchCountFactor(double factor)
{
    ftrMatchCntFactor = factor;
}

double Pharm::PharmacophoreFitScore::getFeaturePositionMatchFactor() const
{
    return ftrPosMatchFactor;
}

void Pharm::PharmacophoreFitScore::setFeaturePositionMatchFactor(double factor)
{
    ftrPosMatchFactor = factor;
}

double Pharm::PharmacophoreFitScore::getFeatureGeometryMatchFactor() const
{
    return ftrGeomMatchFactor;
}

void Pharm::PharmacophoreFitScore::setFeatureGeometryMatchFactor(double factor)
{
    ftrGeomMatchFactor = factor;
}

double Pharm::PharmacophoreFitScore::operator()(const Pharmacophore& ref_pharm, const Pharmacophore& algnd_pharm, 
						const Math::Matrix4D& xform)
{
	geomFtrMappingExtractor.getMapping(ref_pharm, algnd_pharm, xform, geomFtrMapping);

	double cnt_score = 0.0;
	double pos_score = 0.0;
	double geom_score = 0.0;
	double num_ftrs = 0;

	for (Pharmacophore::ConstFeatureIterator f_it = ref_pharm.getFeaturesBegin(), f_end = ref_pharm.getFeaturesEnd(); f_it != f_end; ++f_it) {
		const Feature& ref_ftr = *f_it;
 
		if (getDisabledFlag(ref_ftr))
			continue;

		if (getType(ref_ftr) == FeatureType::X_VOLUME)
			continue;

		num_ftrs++;

		const Feature* algnd_ftr = geomFtrMapping[&ref_ftr];

		if (!algnd_ftr)
			continue;

		if (ftrMatchCntFactor != 0.0)
			cnt_score += 1.0;
		
		if (ftrPosMatchFactor != 0.0)
			pos_score += geomFtrMappingExtractor.getPositionMatchScore(ref_ftr, *algnd_ftr);

		if (ftrPosMatchFactor != 0.0)
			geom_score += geomFtrMappingExtractor.getGeometryMatchScore(ref_ftr, *algnd_ftr);
	}

	if (num_ftrs == 0)
		return 0.0;

	return ((cnt_score * ftrMatchCntFactor + pos_score * ftrPosMatchFactor + geom_score * ftrGeomMatchFactor) / num_ftrs);
}