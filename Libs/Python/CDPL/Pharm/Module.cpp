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

	exportFeature();
	exportBasicFeature();
	exportFeatureContainer();
	exportFeatureSet();
	exportPharmacophore();
	exportBasicPharmacophore();

	exportFeatureMapping();
	exportFeatureTypeHistogram();

	exportPharmacophoreIOManager();
	exportFeatureContainerIOManager();
	exportPharmacophoreReader();
	exportFeatureContainerWriter();
	exportPharmacophoreInputHandler();
	exportFeatureContainerOutputHandler();

	exportCDFPharmacophoreInputHandler();
	exportCDFFeatureContainerOutputHandler();
	exportCDFPharmacophoreReader();
	exportCDFFeatureContainerWriter();
	exportPMLPharmacophoreInputHandler();
	exportPMLFeatureContainerOutputHandler();
	exportPMLPharmacophoreReader();
	exportPMLFeatureContainerWriter();

	exportPharmacophoreFitScore();

	exportScreeningDBCreator();
	exportScreeningDBAccessor();
	exportScreeningProcessor();
	exportPharmacophoreFitScreeningScore();
	exportFileScreeningHitCollector();

#ifdef HAVE_SQLITE3
# if defined(HAVE_BOOST_SYSTEM) && defined(HAVE_BOOST_FILESYSTEM) && defined(HAVE_BOOST_IOSTREAMS)

	exportPSDPharmacophoreInputHandler();
	exportPSDMoleculeInputHandler();
	exportPSDMolecularGraphOutputHandler();
	exportPSDPharmacophoreReader();
	exportPSDMoleculeReader();
	exportPSDMolecularGraphWriter();

#endif // defined(HAVE_BOOST_SYSTEM) && defined(HAVE_BOOST_FILESYSTEM) && defined(HAVE_BOOST_IOSTREAMS)

	exportPSDScreeningDBCreator();
	exportPSDScreeningDBAccessor();

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
	exportInteractionAnalyzer();
	exportDefaultInteractionAnalyzer();
	exportInteractionConstraintConnector();
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

	exportFeatureContainerProperties();
	exportFeatureProperties();

	exportControlParameters();
	exportControlParameterDefaults();

	exportDataFormats();
	exportFeatureTypes();
	exportFeatureGeometries();

	exportPharmacophoreFunctions();
	exportFeatureFunctions();
	exportFeatureContainerFunctions();
	exportFeatureSetFunctions();
	exportControlParameterFunctions();

	exportBoostFunctionWrappers();

	registerToPythonConverters();
	registerFromPythonConverters();
}
