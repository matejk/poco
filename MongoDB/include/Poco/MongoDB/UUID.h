//
// UUID.h
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  UUID
//
// Definition of the UUID binary class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_UUID_INCLUDED
#define MongoDB_UUID_INCLUDED

#include "Poco/UUID.h"

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"

namespace Poco {
namespace MongoDB {

class MongoDB_API UUID
	/// Implements BSON UUID. It's a wrapper around a Poco::UUID.
{
public:
	typedef SharedPtr<UUID> Ptr;

	static const Poco::Int32    uuidSize;
	static const unsigned char  uuidSubtypeOld;
	static const unsigned char  uuidSubtype;

	UUID();
		/// Constructor

	UUID(const Poco::UUID& uuid);
		/// Constructor

	virtual ~UUID();
		/// Destructor

	Poco::UUID&  uuid();
		/// Returns a reference to the uuid

	std::string toString(int indent = 0) const;
		/// Returns the string representation of UUID

private:

	Poco::UUID  _uuid;
};

inline Poco::UUID& UUID::uuid()
{
	return _uuid;
}

// BSON Embedded Document
// spec: UUID
template<>
struct ElementTraits<UUID::Ptr>
{
	enum { TypeId = 0x05 };

	static std::string toString(const UUID::Ptr& value, int indent = 0)
	{
		return value.isNull() ? "" : value->toString();
	}
};


template<>
inline void BSONReader::read<UUID::Ptr>(UUID::Ptr& to)
{
	Poco::Int32 size;
	_reader >> size;

	// TODO: Verify that size is UUID::uuidSize!

	unsigned char subtype;
	_reader >> subtype;
	
	// TODO: Verify that subtype is UUID::uuidSubtype or UUID::uuidSubtypeOld!

	char uuidBuf[UUID::uuidSize];
	_reader.readRaw(uuidBuf, size);
	to->uuid().copyFrom(uuidBuf);
}


template<>
inline void BSONWriter::write<UUID::Ptr>(UUID::Ptr& from)
{
	_writer << from->uuidSize;
	_writer << from->uuidSubtype;
	char uuidBuf[UUID::uuidSize];
	from->uuid().copyTo(uuidBuf);
	_writer.writeRaw(uuidBuf, from->uuidSize);
}


} } // namespace Poco::MongoDB


#endif // MongoDB_UUID_INCLUDED
