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
