/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * PharmacophoreFeatureFunctionExport.cpp 
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

#include "CDPL/Chem/PharmacophoreFeature.hpp"
#include "CDPL/Chem/PharmacophoreFeatureFunctions.hpp"
#include "CDPL/Chem/StereoDescriptor.hpp"

#include "FunctionExports.hpp"
#include "FunctionWrapper.hpp"


#define MAKE_FEATURE_FUNC_WRAPPERS(TYPE, FUNC_SUFFIX)                       \
TYPE get##FUNC_SUFFIX##Wrapper(CDPL::Chem::PharmacophoreFeature& feature)   \
{                                                                           \
	return get##FUNC_SUFFIX(feature);                                       \
}                                                                           \
                                                                            \
bool has##FUNC_SUFFIX##Wrapper(CDPL::Chem::PharmacophoreFeature& feature)   \
{                                                                           \
	return has##FUNC_SUFFIX(feature);                                       \
}

#define EXPORT_FEATURE_FUNCS(FUNC_SUFFIX, ARG_NAME)                                                           \
python::def("get"#FUNC_SUFFIX, &get##FUNC_SUFFIX##Wrapper, python::arg("feature"));                           \
python::def("has"#FUNC_SUFFIX, &has##FUNC_SUFFIX##Wrapper, python::arg("feature"));                           \
python::def("clear"#FUNC_SUFFIX, &Chem::clear##FUNC_SUFFIX, python::arg("feature"));                          \
python::def("set"#FUNC_SUFFIX, &Chem::set##FUNC_SUFFIX, (python::arg("feature"), python::arg(#ARG_NAME))); 

#define EXPORT_FEATURE_FUNCS_INT_REF(FUNC_SUFFIX, ARG_NAME)                                                   \
python::def("get"#FUNC_SUFFIX, &get##FUNC_SUFFIX##Wrapper, python::arg("feature"),                            \
            python::return_internal_reference<1>());                                                          \
python::def("has"#FUNC_SUFFIX, &has##FUNC_SUFFIX##Wrapper, python::arg("feature"));                           \
python::def("clear"#FUNC_SUFFIX, &Chem::clear##FUNC_SUFFIX, python::arg("feature"));                          \
python::def("set"#FUNC_SUFFIX, &Chem::set##FUNC_SUFFIX, (python::arg("feature"), python::arg(#ARG_NAME))); 

#define EXPORT_FEATURE_FUNCS_COPY_REF_CW(FUNC_SUFFIX, ARG_NAME)                                               \
python::def("get"#FUNC_SUFFIX, &get##FUNC_SUFFIX##Wrapper, python::arg("feature"),                            \
            python::return_value_policy<python::copy_const_reference,                                         \
			python::with_custodian_and_ward_postcall<0, 1> >());                                              \
python::def("has"#FUNC_SUFFIX, &has##FUNC_SUFFIX##Wrapper, python::arg("feature"));                           \
python::def("clear"#FUNC_SUFFIX, &Chem::clear##FUNC_SUFFIX, python::arg("feature"));                          \
python::def("set"#FUNC_SUFFIX, &Chem::set##FUNC_SUFFIX, (python::arg("feature"), python::arg(#ARG_NAME)),     \
			python::with_custodian_and_ward<1, 2>());                                                            


namespace
{

    MAKE_FEATURE_FUNC_WRAPPERS(unsigned int, Type)
    MAKE_FEATURE_FUNC_WRAPPERS(unsigned int, Geometry)
    MAKE_FEATURE_FUNC_WRAPPERS(double, Length)
    MAKE_FEATURE_FUNC_WRAPPERS(double, Tolerance)
    MAKE_FEATURE_FUNC_WRAPPERS(bool, DisabledFlag)
    MAKE_FEATURE_FUNC_WRAPPERS(bool, OptionalFlag)
	MAKE_FEATURE_FUNC_WRAPPERS(const CDPL::Math::Vector3D&, Orientation)
	MAKE_FEATURE_FUNC_WRAPPERS(const CDPL::Chem::Fragment::SharedPointer&, Substructure)
}


void CDPLPythonChem::exportPharmacophoreFeatureFunctions()
{
    using namespace boost;
    using namespace CDPL;
    
    EXPORT_FEATURE_FUNCS(Type, type)
    EXPORT_FEATURE_FUNCS(Geometry, geom)
    EXPORT_FEATURE_FUNCS(Length, length)
    EXPORT_FEATURE_FUNCS(Tolerance, tol)
    EXPORT_FEATURE_FUNCS(DisabledFlag, flag)
    EXPORT_FEATURE_FUNCS(OptionalFlag, flag)
	EXPORT_FEATURE_FUNCS_INT_REF(Orientation, orient)
	EXPORT_FEATURE_FUNCS_COPY_REF_CW(Substructure, substruct)
}
