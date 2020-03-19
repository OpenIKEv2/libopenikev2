/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_idi.h"

namespace openikev2 {

    Payload_IDi::Payload_IDi( auto_ptr<ID> id ) : Payload_ID( id ) {
        this->type = PAYLOAD_IDi;
    }

    Payload_IDi::Payload_IDi( const Payload_ID& other ) : Payload_ID( other ) {
        this->type = PAYLOAD_IDi;
    }

    auto_ptr<Payload_IDi> Payload_IDi::parse( ByteBuffer& byte_buffer ) {
        auto_ptr<Payload_ID> payload_id = Payload_ID::parse ( byte_buffer );
        return auto_ptr<Payload_IDi> ( new Payload_IDi ( *payload_id ) );
    }

    Payload_IDi::~Payload_IDi() {}
}
