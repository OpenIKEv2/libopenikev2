/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "payload_idr.h"

namespace openikev2 {

    Payload_IDr::Payload_IDr( auto_ptr<ID> id ) : Payload_ID( id ) {
        this->type = PAYLOAD_IDr;
    }

    Payload_IDr::Payload_IDr( const Payload_ID& other ) : Payload_ID( other ) {
        this->type = PAYLOAD_IDr;
    }

    auto_ptr<Payload_IDr> Payload_IDr::parse( ByteBuffer& byte_buffer ) {
        auto_ptr<Payload_ID> payload_id = Payload_ID::parse( byte_buffer );
        return auto_ptr<Payload_IDr> ( new Payload_IDr ( *payload_id ) );
    }

    Payload_IDr::~Payload_IDr() {}

}
