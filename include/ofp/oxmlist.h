//  ===== ---- ofp/oxmlist.h -------------------------------*- C++ -*- =====  //
//
//  This file is licensed under the Apache License, Version 2.0.
//  See LICENSE.txt for details.
//  
//  ===== ------------------------------------------------------------ =====  //
/// \file
/// \brief Defines the OXMList class.
//  ===== ------------------------------------------------------------ =====  //

#ifndef OFP_OXMLIST_H
#define OFP_OXMLIST_H

#include "ofp/oxmrange.h"
#include "ofp/bytelist.h"


namespace ofp { // <namespace ofp>


// OXMList is an ordered sequence of values stored (without padding) in a
// memory buffer.
// It allows multiple values for the same key (used for prereqs).

class OXMList {
public:
	OXMList() = default;
	OXMList(const OXMRange &range);

	OXMIterator begin() const { return OXMIterator(buf_.begin()); }
	OXMIterator end() const { return OXMIterator(buf_.end()); }

	const UInt8 *data() const { return buf_.begin(); }
	size_t size() const { return buf_.size(); }
	
	template <class ValueType>
	void add(ValueType value);
	
	template <class ValueType>
	void add(ValueType value, ValueType mask);
	
	void add(OXMIterator iter);
	void addSignal(OXMType signal);
	void insertSignal(OXMIterator pos, OXMType signal);
	
	OXMIterator replace(OXMIterator pos, OXMIterator end, OXMType type, const void *data, size_t len);
	
	OXMRange toRange() const { return OXMRange{buf_.data(), buf_.size()}; }
	
	bool operator==(const OXMList &rhs) const { return buf_ == rhs.buf_; }
	bool operator!=(const OXMList &rhs) const { return !(*this == rhs); }

private:
	ByteList buf_;
	
	//void add(const void *data, size_t len);
	void insert(OXMIterator pos, const void *data, size_t len);

	void add(OXMType type, const void *data, size_t len);
	void add(OXMType type, const void *data, const void *mask, size_t len);
};

template <class ValueType>
inline
void OXMList::add(ValueType value)
{
	static_assert(sizeof(value) < 256, "oxm_length must be <= 255.");
	
	add(ValueType::type(), &value, sizeof(value));
}

template <class ValueType>
inline
void OXMList::add(ValueType value, ValueType mask)
{
	static_assert(sizeof(value) < 128, "oxm_length must be <= 255.");
	
	add(ValueType::type().withMask(), &value, &mask, sizeof(value));
}


inline void OXMList::add(OXMIterator iter)
{
	buf_.add(iter.data(), iter.size());
}

inline void OXMList::addSignal(OXMType signal)
{
	buf_.add(&signal, sizeof(signal));
}

inline void OXMList::insertSignal(OXMIterator pos, OXMType signal)
{
	insert(pos, &signal, sizeof(signal));
}

} // </namespace ofp>

#endif // OFP_OXMLIST_H
