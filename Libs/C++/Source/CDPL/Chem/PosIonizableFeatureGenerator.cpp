/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * PosIonizableFeatureGenerator.cpp 
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

#include "CDPL/Chem/PosIonizableFeatureGenerator.hpp"
#include "CDPL/Chem/UtilityFunctions.hpp"
#include "CDPL/Chem/FeatureType.hpp"
#include "CDPL/Chem/FeatureGeometry.hpp"


using namespace CDPL; 


Chem::PosIonizableFeatureGenerator::PosIonizableFeatureGenerator()
{
    init();
}

Chem::PosIonizableFeatureGenerator::PosIonizableFeatureGenerator(const MolecularGraph& molgraph, Pharmacophore& pharm)
{
    init();
    generate(molgraph, pharm);
}

void Chem::PosIonizableFeatureGenerator::init()
{
    addIncludePattern(parseSMARTS("[N;!$(NC(=[O,N]));!$(Nc);!$(N[#7,#8,#16]):3]1[C:3][C:3][N;!$(NC(=[O,N]));!$(Nc);!$(N[#7,#8,#16]):3][C:3][C:3]1"), FeatureType::POS_IONIZABLE, 2.0, FeatureGeometry::SPHERE);
    addIncludePattern(parseSMARTS("[NX3:3]([CX4])([CX4,#1])[CX4,#1]"), FeatureType::POS_IONIZABLE, 1.5, FeatureGeometry::SPHERE);
    addIncludePattern(parseSMARTS("[N:3]=[CX3:3]([NH0])[!N]"), FeatureType::POS_IONIZABLE, 1.5, FeatureGeometry::SPHERE);
    addIncludePattern(parseSMARTS("[N:3]=[CX3:3]([N;H1,H2:3])[!N]"), FeatureType::POS_IONIZABLE, 1.5, FeatureGeometry::SPHERE);
    addIncludePattern(parseSMARTS("[N:3]=[CX3:3]([N:3])[N:3]"), FeatureType::POS_IONIZABLE, 1.5, FeatureGeometry::SPHERE);
    addIncludePattern(parseSMARTS("[+,+2,+3,+4,+5,+6,+7;!$(*[-,-2,-3,-4,-5,-6,-7]):3]"), FeatureType::POS_IONIZABLE, 1.5, FeatureGeometry::SPHERE);

    addExcludePattern(parseSMARTS("N([R])([R])[C,S,P](=O)"));
}