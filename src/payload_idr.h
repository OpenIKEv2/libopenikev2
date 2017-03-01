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
#ifndef PAYLOAD_IDR_H
#define PAYLOAD_IDR_H

#include "payload_id.h"

namespace openikev2 {

    /**
    This class represents a Responder Identification Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_IDr : public Payload_ID {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Payload_IDr with the specified ID.
             * @param id ID
             */
            Payload_IDr( auto_ptr<ID> id );

            /**
             * Creates a new Payload_IDr cloning another one
             * @param other Other Payload_IDr to be cloned
             */
            Payload_IDr( const Payload_ID& other );

            /**
             * Creates a new Payload_IDr based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_IDr> parse( ByteBuffer& byte_buffer );

            virtual ~Payload_IDr();
    };
}
#endif
