/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#include "payload_tsi.h"

namespace openikev2 {

    Payload_TSi::Payload_TSi() : Payload_TS() {
        this->type = PAYLOAD_TSi;
    }

    Payload_TSi::Payload_TSi( auto_ptr<TrafficSelector> traffic_selector ) : Payload_TS( traffic_selector ) {
        this->type = PAYLOAD_TSi;
    }

    Payload_TSi::Payload_TSi( const Payload_TS& other ) : Payload_TS( other ) {
        this->type = PAYLOAD_TSi;
    }

    auto_ptr<Payload_TSi> Payload_TSi::parse( ByteBuffer& byte_buffer ) {
        auto_ptr<Payload_TS> payload_ts = Payload_TS::parse( byte_buffer );
        return auto_ptr<Payload_TSi> ( new Payload_TSi( *payload_ts ) );
    }

    Payload_TSi::~Payload_TSi() {}
}


