/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * NegIonizableFeatureGeneratorExport.cpp 
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

#include "CDPL/Chem/NegIonizableFeatureGenerator.hpp"
#include "CDPL/Chem/MolecularGraph.hpp"
#include "CDPL/Chem/Pharmacophore.hpp"

#include "ClassExports.hpp"


void CDPLPythonChem::exportNegIonizableFeatureGenerator()
{
    using namespace boost;
    using namespace CDPL;

    python::class_<Chem::NegIonizableFeatureGenerator, python::bases<Chem::PatternBasedFeatureGenerator>,
		   boost::noncopyable>("NegIonizableFeatureGenerator", python::no_init)
		.def(python::init<bool>((python::arg("self"), python::arg("fuzzy"))))
		.def(python::init<const Chem::NegIonizableFeatureGenerator&>(
				 (python::arg("self"), python::arg("gen"))))
		.def(python::init<const Chem::MolecularGraph&, Chem::Pharmacophore&, bool>(
				 (python::arg("self"), python::arg("molgraph"), python::arg("pharm"), python::arg("fuzzy"))))
		.def("assign", &Chem::NegIonizableFeatureGenerator::operator=, 
			 (python::arg("self"), python::arg("gen")), python::return_self<>());
}
