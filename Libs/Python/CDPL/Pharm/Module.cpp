/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * Module.cpp 
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


#include <boost/python.hpp>

#include "ClassExports.hpp"
#include "FunctionExports.hpp"
#include "NamespaceExports.hpp"
#include "ConverterRegistration.hpp"


BOOST_PYTHON_MODULE(_pharm)
{
	using namespace CDPLPythonPharm;

	exportPharmacophore();
	exportFeature();
	exportBasicPharmacophore();
	exportBasicFeature();

	exportFeatureMapping();
	exportFeatureTypeHistogram();

	exportPharmacophoreIOManager();
	exportPharmacophoreReader();
	exportPharmacophoreWriter();
	exportPharmacophoreInputHandler();
	exportPharmacophoreOutputHandler();

	exportCDFPharmacophoreInputHandler();
	exportCDFPharmacophoreOutputHandler();
	exportCDFPharmacophoreReader();
	exportCDFPharmacophoreWriter();

	exportScreeningDBCreator();
	exportScreeningDBAccessor();
	exportScreeningDBProcessor();

#ifdef HAVE_SQLITE3

	exportSQLiteScreeningDBCreator();
	exportSQLiteScreeningDBAccessor();

#endif // HAVE_SQLITE3

	exportFeatureGenerator();
	exportPharmacophoreGenerator();
	exportDefaultPharmacophoreGenerator();
	exportPatternBasedFeatureGenerator();
	exportAromaticFeatureGenerator();
	exportHydrophobicFeatureGenerator();
	exportHBondAcceptorFeatureGenerator();
	exportHBondDonorFeatureGenerator();
	exportPosIonizableFeatureGenerator();
	exportNegIonizableFeatureGenerator();
	exportSurfaceXVolumeCoatGenerator();
	exportFeatureInteractionAnalyzer();
	exportDefaultFeatureInteractionAnalyzer();
	exportFeatureInteractionConstraintConnector();
	exportFeatureDistanceConstraint();
	exportHydrophobicInteractionConstraint();
	exportIonicInteractionConstraint();
	exportOrthogonalPiPiInteractionConstraint();
	exportParallelPiPiInteractionConstraint();
	exportCationPiInteractionConstraint();
	exportHBondingInteractionConstraint();

	exportTopologicalEntityAlignment();
	exportGeometricalEntityAlignment();
	exportPharmacophoreAlignment();
    exportGeometricalFeatureMappingExtractor();
	exportFeatureTypeMatchFunctor();
	exportFeaturePairDistanceMatchFunctor();
	exportFeaturePositionMatchFunctor();
	exportFeatureGeometryMatchFunctor();

	exportPharmacophoreProperties();
	exportFeatureProperties();

	exportControlParameters();
	exportControlParameterDefaults();

	exportDataFormats();
	exportFeatureTypes();
	exportFeatureGeometries();

	exportPharmacophoreFunctions();
	exportFeatureFunctions();
	exportControlParameterFunctions();

	exportBoostFunctionWrappers();

	registerToPythonConverters();
	registerFromPythonConverters();
}