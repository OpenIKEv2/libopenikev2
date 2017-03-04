/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Lesser General Public            *
*   License as published by the Free Software Foundation; either          *
*   version 2.1 of the License, or (at your option) any later version.    *
*                                                                         *
*   This library is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
*   Lesser General Public License for more details.                       *
*                                                                         *
*   You should have received a copy of the GNU Lesser General Public      *
*   License along with this library; if not, write to the Free Software   *
*   Foundation, Inc., 51 Franklin St, Fifth Floor,                        *
*   Boston, MA  02110-1301  USA                                           *
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

