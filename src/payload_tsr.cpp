/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_tsr.h"

namespace openikev2 {

    Payload_TSr::Payload_TSr() : Payload_TS() {
        this->type = PAYLOAD_TSr;
    }

    Payload_TSr::Payload_TSr( auto_ptr<TrafficSelector> traffic_selector ) : Payload_TS( traffic_selector ) {
        this->type = PAYLOAD_TSr;
    }

    Payload_TSr::Payload_TSr( const Payload_TS& other ) : Payload_TS( other ) {
        this->type = PAYLOAD_TSr;
    }

    auto_ptr<Payload_TSr> Payload_TSr::parse( ByteBuffer& byte_buffer ) {
        auto_ptr<Payload_TS> payload_ts = Payload_TS::parse( byte_buffer );
        return auto_ptr<Payload_TSr> ( new Payload_TSr( *payload_ts ) );
    }

    Payload_TSr::~Payload_TSr() {}
}

