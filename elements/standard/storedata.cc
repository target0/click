// -*- c-basic-offset: 4 -*-
/*
 * storedata.{cc,hh} -- element changes packet data
 * Eddie Kohler
 *
 * Copyright (c) 2004 Regents of the University of California
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "storedata.hh"
#include <click/confparse.hh>
CLICK_DECLS

StoreData::StoreData()
    : Element(1, 1)
{
    MOD_INC_USE_COUNT;
}

StoreData::~StoreData()
{
    MOD_DEC_USE_COUNT;
}

int
StoreData::configure(Vector<String> &conf, ErrorHandler *errh)
{
    return cp_va_parse(conf, this, errh,
		       cpUnsigned, "offset", &_offset,
		       cpString, "data", &_data,
		       0);
}

Packet *
StoreData::simple_action(Packet *p)
{
    if (p->length() <= _offset)
	return p;
    else if (WritablePacket *q = p->uniqueify()) {
	int len = q->length() - _offset;
	memcpy(q->data() + _offset, _data.data(), (_data.length() < len ? _data.length() : len));
	return q;
    } else
	return 0;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(StoreData)
ELEMENT_MT_SAFE(StoreData)