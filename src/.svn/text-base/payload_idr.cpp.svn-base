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
