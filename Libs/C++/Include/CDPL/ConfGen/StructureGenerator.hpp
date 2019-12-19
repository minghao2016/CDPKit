/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * StructureGenerator.hpp 
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

/**
 * \file
 * \brief Definition of the class CDPL::ConfGen::StructureGenerator.
 */

#ifndef CDPL_CONFGEN_STRUCTUREGENERATOR_HPP
#define CDPL_CONFGEN_STRUCTUREGENERATOR_HPP

#include <memory>

#include "CDPL/ConfGen/APIPrefix.hpp"
#include "CDPL/ConfGen/CallbackFunction.hpp"
#include "CDPL/ConfGen/StructureGeneratorSettings.hpp"
#include "CDPL/ConfGen/ConformerData.hpp"
#include "CDPL/ConfGen/FragmentLibrary.hpp"
#include "CDPL/ConfGen/TorsionLibrary.hpp"


namespace CDPL 
{

	namespace Chem
	{

		class MolecularGraph;
	}

    namespace ConfGen 
    {

		class ConformerGeneratorImpl;

		/**
		 * \addtogroup CDPL_CONFGEN_GENERATORS
		 * @{
		 */

		class CDPL_CONFGEN_API StructureGenerator
		{

		public:
			StructureGenerator();
		
			~StructureGenerator();

			const StructureGeneratorSettings& getSettings() const;

			StructureGeneratorSettings& getSettings();

			void clearFragmentLibraries();

			void addFragmentLibrary(const FragmentLibrary::SharedPointer& lib);

			void setTorsionLibrary(const TorsionLibrary::SharedPointer& lib);

			const TorsionLibrary::SharedPointer& getTorsionLibrary() const;

			void setAbortCallback(const CallbackFunction& func);

			const CallbackFunction& getAbortCallback() const;

			void setTimeoutCallback(const CallbackFunction& func);

			const CallbackFunction& getTimeoutCallback() const;
			
			unsigned int generate(const Chem::MolecularGraph& molgraph, ConformerData& conf_data);

			unsigned int generate(const Chem::MolecularGraph& molgraph, Math::Vector3DArray& coords);

		private:
			StructureGenerator(const StructureGenerator&);

			StructureGenerator& operator=(const StructureGenerator&);

			template <typename VecArrayType>
			unsigned int doGenerate(const Chem::MolecularGraph& molgraph, VecArrayType& coords);

			typedef std::auto_ptr<ConformerGeneratorImpl> ImplementationPointer;

			ImplementationPointer      impl;
			StructureGeneratorSettings settings;
		};

		/**
		 * @}
		 */
    }
}

#endif // CDPL_CONFGEN_STRUCTUREGENERATOR_HPP