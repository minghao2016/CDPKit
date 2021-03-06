/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*- */

/* 
 * FragmentLibrary.cpp 
 *
 * This file is part of the Chemical Data Processing Toolkit
 *
 * Copyright (C) 2003-2020 Thomas A. Seidel <thomas.seidel@univie.ac.at>
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

#include <string>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "CDPL/Config.hpp"

#if defined(HAVE_BOOST_IOSTREAMS)

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#else // defined(HAVE_BOOST_IOSTREAMS)

#include <sstream>

#endif // defined(HAVE_BOOST_IOSTREAMS)

#include "CDPL/ConfGen/FragmentLibrary.hpp"
#include "CDPL/Base/Exceptions.hpp"

#include "CDFFragmentLibraryDataReader.hpp"
#include "CDFFragmentLibraryDataWriter.hpp"
#include "CDFMoleculeDataFunctions.hpp"


using namespace CDPL;


namespace
{

	const char BUILTIN_FRAG_LIB_DATA[] =                 
        #include "FragmentLibrary.cdf.str" 
		;

    Chem::MolecularGraph::SharedPointer NO_ENTRY;

    ConfGen::FragmentLibrary::SharedPointer builtinFragLib(new ConfGen::FragmentLibrary());

	boost::once_flag initBuiltinFragLibFlag = BOOST_ONCE_INIT;
	
	void initBuiltinFragLib()
	{
		builtinFragLib->loadDefaults();
	}
}


ConfGen::FragmentLibrary::SharedPointer ConfGen::FragmentLibrary::defaultLib = builtinFragLib;


ConfGen::FragmentLibrary::FragmentLibrary() {}

ConfGen::FragmentLibrary::FragmentLibrary(const FragmentLibrary& lib) 
{
	for (HashToFragmentMap::iterator it = lib.hashToFragMap.begin(), end = lib.hashToFragMap.end(); it != end; ++it)
		hashToFragMap.insert(Entry(it->first, hasCDFMoleculeData(*it->second) ? it->second->clone() : it->second));
}

ConfGen::FragmentLibrary::~FragmentLibrary() {}

ConfGen::FragmentLibrary& ConfGen::FragmentLibrary::operator=(const FragmentLibrary& lib)
{
	if (this == &lib)
		return *this;

	hashToFragMap.clear();

	for (HashToFragmentMap::iterator it = lib.hashToFragMap.begin(), end = lib.hashToFragMap.end(); it != end; ++it)
		hashToFragMap.insert(Entry(it->first, hasCDFMoleculeData(*it->second) ? it->second->clone() : it->second));

	return *this;
}

void ConfGen::FragmentLibrary::addEntries(const FragmentLibrary& lib)
{
	for (HashToFragmentMap::iterator it = lib.hashToFragMap.begin(), end = lib.hashToFragMap.end(); it != end; ++it)
		hashToFragMap.insert(Entry(it->first, hasCDFMoleculeData(*it->second) ? it->second->clone() : it->second));
}

bool ConfGen::FragmentLibrary::addEntry(Base::uint64 frag_hash, const Chem::MolecularGraph::SharedPointer& frag)
{
	if (!frag)
		return false;

    return hashToFragMap.insert(Entry(frag_hash, frag)).second;
}

const Chem::MolecularGraph::SharedPointer& ConfGen::FragmentLibrary::getEntry(Base::uint64 frag_hash) const
{
    HashToFragmentMap::iterator it = hashToFragMap.find(frag_hash);

    if (it == hashToFragMap.end())
		return NO_ENTRY;

	loadMolStructure(*it);

    return it->second;
}

bool ConfGen::FragmentLibrary::containsEntry(Base::uint64 frag_hash) const
{
    return (hashToFragMap.find(frag_hash) != hashToFragMap.end());
}

std::size_t ConfGen::FragmentLibrary::getNumEntries() const
{
    return hashToFragMap.size();
}

void ConfGen::FragmentLibrary::clear()
{
    hashToFragMap.clear();
}

bool ConfGen::FragmentLibrary::removeEntry(Base::uint64 frag_hash)
{
   return (hashToFragMap.erase(frag_hash) > 0);
}

ConfGen::FragmentLibrary::EntryIterator ConfGen::FragmentLibrary::removeEntry(const EntryIterator& it)
{
    return EntryIterator(hashToFragMap.erase(it.base()), boost::bind(&FragmentLibrary::loadMolStructure, this, _1));
}

ConfGen::FragmentLibrary::ConstEntryIterator ConfGen::FragmentLibrary::getEntriesBegin() const
{
    return ConstEntryIterator(hashToFragMap.begin(), boost::bind(&FragmentLibrary::loadMolStructure, this, _1));
}

ConfGen::FragmentLibrary::ConstEntryIterator ConfGen::FragmentLibrary::getEntriesEnd() const
{
    return ConstEntryIterator(hashToFragMap.end(), boost::bind(&FragmentLibrary::loadMolStructure, this, _1));
}
	
ConfGen::FragmentLibrary::EntryIterator ConfGen::FragmentLibrary::getEntriesBegin()
{
    return EntryIterator(hashToFragMap.begin(), boost::bind(&FragmentLibrary::loadMolStructure, this, _1));
}

ConfGen::FragmentLibrary::EntryIterator ConfGen::FragmentLibrary::getEntriesEnd()
{
    return EntryIterator(hashToFragMap.end(), boost::bind(&FragmentLibrary::loadMolStructure, this, _1));
}

boost::mutex& ConfGen::FragmentLibrary::getMutex()
{
	return mutex;
}

void ConfGen::FragmentLibrary::load(std::istream& is)
{
	CDFFragmentLibraryDataReader reader;
	Entry entry;

	while (true) {
		try {
			if (!reader.read(is, entry))
				break;

			hashToFragMap.insert(entry);

		} catch (const std::exception& e) {
			throw Base::IOError("FragmentLibrary: error while loading fragment library: " + std::string(e.what()));
		}
	}
}

void ConfGen::FragmentLibrary::save(std::ostream& os) const
{
	boost::lock_guard<boost::mutex> lock(mutex);
	CDFFragmentLibraryDataWriter writer;

	for (HashToFragmentMap::const_iterator it = hashToFragMap.begin(), end = hashToFragMap.end(); it != end; ++it) {
		try {
			if (!writer.write(os, *it))
				throw Base::IOError("FragmentLibrary: unspecified error while saving fragment library");

		} catch (const std::exception& e) {
			throw Base::IOError("FragmentLibrary: error while saving fragment library: " + std::string(e.what()));
		}
	}
}

void ConfGen::FragmentLibrary::loadDefaults()
{
#if defined(HAVE_BOOST_IOSTREAMS)

    boost::iostreams::stream<boost::iostreams::array_source> is(BUILTIN_FRAG_LIB_DATA, sizeof(BUILTIN_FRAG_LIB_DATA) - 1);

#else // defined(HAVE_BOOST_IOSTREAMS)

	std::istringstream is(std::string(BUILTIN_FRAG_LIB_DATA, sizeof(BUILTIN_FRAG_LIB_DATA) - 1), std::ios_base::in | std::ios_base::binary);

#endif // defined(HAVE_BOOST_IOSTREAMS)

	load(is);
}

void ConfGen::FragmentLibrary::set(const SharedPointer& lib)
{
    defaultLib = (!lib ? builtinFragLib : lib);
}

const ConfGen::FragmentLibrary::SharedPointer& ConfGen::FragmentLibrary::get()
{
	boost::call_once(&initBuiltinFragLib, initBuiltinFragLibFlag);

    return defaultLib;
}

ConfGen::FragmentLibrary::Entry& ConfGen::FragmentLibrary::loadMolStructure(Entry& entry) const
{
	boost::lock_guard<boost::mutex> lock(mutex);

	if (!hasCDFMoleculeData(*entry.second))
		return entry;

	entry.second = createMoleculeFromCDFData(*entry.second);

	return entry;
}
