/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * VectorArrayExport.cpp 
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

#include "CDPL/Math/VectorArray.hpp"

#include "Util/ArrayVisitor.hpp"

#include "ClassExports.hpp"


namespace
{

	template <typename ArrayType, std::size_t Dim>
	struct VectorArrayExport
	{

		VectorArrayExport(const char* name) {
			using namespace boost;
			using namespace CDPL;

			python::class_<ArrayType, typename ArrayType::SharedPointer>(name, python::no_init)
				.def(python::init<>(python::arg("self")))
				.def(python::init<const ArrayType&>((python::arg("self"), python::arg("array"))))
				.def(CDPLPythonUtil::ArrayVisitor<ArrayType, 
					 python::return_internal_reference<>, python::default_call_policies, 
					 python::default_call_policies, python::default_call_policies>())
				.def("__eq__", &ArrayType::operator==, (python::arg("self"), python::arg("array")))
				.def("__ne__", &ArrayType::operator!=, (python::arg("self"), python::arg("array")));
		}
	};
}


void CDPLPythonMath::exportVectorArrays()
{
	using namespace CDPL;
	
	VectorArrayExport<Math::Vector2FArray, 2>("Vector2FArray");
	VectorArrayExport<Math::Vector3FArray, 3>("Vector3FArray");

	VectorArrayExport<Math::Vector2DArray, 2>("Vector2DArray");
	VectorArrayExport<Math::Vector3DArray, 3>("Vector3DArray");

	VectorArrayExport<Math::Vector2LArray, 2>("Vector2LArray");
	VectorArrayExport<Math::Vector3LArray, 3>("Vector3LArray");

	VectorArrayExport<Math::Vector2ULArray, 2>("Vector2ULArray");
	VectorArrayExport<Math::Vector3ULArray, 3>("Vector3ULArray");
}
