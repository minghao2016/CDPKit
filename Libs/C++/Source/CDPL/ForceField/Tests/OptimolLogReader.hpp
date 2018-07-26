/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * OptimolLogReader.hpp
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


#ifndef CDPL_FORCEFIELD_TEST_OPTIMOLLOGREADER_HPP
#define CDPL_FORCEFIELD_TEST_OPTIMOLLOGREADER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <cstddef>

#include <boost/unordered_map.hpp>


namespace MMFF94TestUtils
{

    class OptimolLogReader
    {

    public:
		struct BondStretchingInteraction
		{

			std::size_t  atom1Idx;
			std::size_t  atom2Idx;
			unsigned int ffClass;
			double       forceConst;
			double       refLength;
		};

		struct AngleBendingInteraction
		{

			std::string  termAtom1Name;
			std::size_t  ctrAtomIdx;
			std::string  termAtom2Name;
			unsigned int ffClass;
			double       forceConst;
			double       refAngle;
		};

		struct StretchBendInteraction
		{

			std::string  termAtom1Name;
			std::size_t  ctrAtomIdx;
			std::string  termAtom2Name;
			unsigned int ffClass;
			double       forceConst;
		};
	
		struct OutOfPlaneBendingInteraction
		{

			std::string  termAtom1Name;
			std::string  ctrAtomName;
			std::string  termAtom2Name;
			std::size_t  oopAtomIdx;
			double       forceConst;
		};

		struct TorsionInteraction
		{

			std::string  termAtom1Name;
			std::size_t  ctrAtom1Idx;
			std::size_t  ctrAtom2Idx;
			std::string  termAtom2Name;
			unsigned int ffClass;
			double       torParams[3];
		};

		typedef std::vector<std::string> SymbolicAtomTypeArray;
		typedef std::vector<unsigned int> NumericAtomTypeArray;
		typedef std::vector<double> AtomChargeArray;
		typedef std::vector<BondStretchingInteraction> BondStretchingInteractionList;
		typedef std::vector<AngleBendingInteraction> AngleBendingInteractionList;
		typedef std::vector<StretchBendInteraction> StretchBendInteractionList;
		typedef std::vector<OutOfPlaneBendingInteraction> OutOfPlaneBendingInteractionList;
		typedef std::vector<TorsionInteraction> TorsionInteractionList;

		OptimolLogReader(const std::string& log_file);

		bool getSymbolicAtomTypes(const std::string& mol_name, SymbolicAtomTypeArray& sym_types);
		bool getNumericAtomTypes(const std::string& mol_name, NumericAtomTypeArray& num_types);
		bool getPartialAtomCharges(const std::string& mol_name, AtomChargeArray& charges);
		bool getFormalAtomCharges(const std::string& mol_name, AtomChargeArray& charges);
		bool getBondStretchingInteractions(const std::string& mol_name, BondStretchingInteractionList& iactions);
		bool getAngleBendingInteractions(const std::string& mol_name, AngleBendingInteractionList& iactions);
		bool getStretchBendInteractions(const std::string& mol_name, StretchBendInteractionList& iactions);
		bool getOutOfPlaneBendingInteractions(const std::string& mol_name, OutOfPlaneBendingInteractionList& iactions);
		bool getTorsionInteractions(const std::string& mol_name, TorsionInteractionList& iactions);

    private:
		void buildIndex();

		bool skipLines(std::size_t n);
		bool skipTokens(std::istream& is, std::size_t n) const;
		bool readLine(std::string& line);
		bool skipToLine(std::string& line, const char* srch_str);
		bool seekToRecord(const std::string& mol_name);

		typedef boost::unordered_map<std::string, std::istream::pos_type> RecordFileOffsetMap;

		std::ifstream       logIStream;
		RecordFileOffsetMap recordOffsets;
    };
}

#endif // CDPL_FORCEFIELD_TEST_OPTIMOLLOGREADER_HPP
