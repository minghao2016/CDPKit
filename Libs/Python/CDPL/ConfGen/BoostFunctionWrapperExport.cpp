/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * BoostFunctionWrapperExport.cpp 
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


#include <string>

#include <boost/function.hpp>

#include "CDPL/Chem/MolecularGraph.hpp"
#include "CDPL/Base/IntegerTypes.hpp"

#include "Base/BoostFunctionWrapperExport.hpp"

#include "ClassExports.hpp"


void CDPLPythonConfGen::exportBoostFunctionWrappers()
{
	using namespace boost;
    using namespace CDPL;

	CDPLPythonBase::BoostFunction0Export<boost::function0<bool> >("BoolVoidFunctor");
	CDPLPythonBase::BoostFunction2Export<boost::function2<bool, const Chem::MolecularGraph&, const std::string&> >("BoolMolecularGraphStringFunctor");
	CDPLPythonBase::BoostFunction4Export<boost::function4<void, Base::uint64, const Chem::MolecularGraph&, bool, std::size_t> >("VoidUInt64MolecularGraphBoolSizeTypeFunctor");
}