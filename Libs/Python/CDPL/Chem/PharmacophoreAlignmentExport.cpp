 /* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * PharmacophoreAlignmentExport.cpp 
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

#include "CDPL/Chem/PharmacophoreAlignment.hpp"
#include "CDPL/Chem/Pharmacophore.hpp"
#include "CDPL/Chem/Feature.hpp"

#include "ClassExports.hpp"


namespace
{

    void addPharmacophore(CDPL::Chem::PharmacophoreAlignment& align, CDPL::Chem::Pharmacophore& pharm, bool first_set)
    {
	align.addPharmacophore(pharm, first_set);
    }
}


void CDPLPythonChem::exportPharmacophoreAlignment()
{
    using namespace boost;
    using namespace CDPL;

    python::class_<Chem::PharmacophoreAlignment, python::bases<Chem::GeometricalEntityAlignment<Chem::Feature> >,
		   boost::noncopyable>("PharmacophoreAlignment", python::no_init)
	.def(python::init<bool>((python::arg("self"), python::arg("query_mode"))))
	.def(python::init<const Chem::PharmacophoreAlignment&>((python::arg("self"), python::arg("alignment"))))
	.def("addPharmacophore", &addPharmacophore, (python::arg("self"), python::arg("pharm"), python::arg("first_set")))
	.def("assign", &Chem::PharmacophoreAlignment::operator=, 
	     (python::arg("self"), python::arg("alignment")), python::return_self<>());
}
