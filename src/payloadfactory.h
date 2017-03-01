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
#ifndef PAYLOADFACTORY_H
#define PAYLOADFACTORY_H

#include "payload.h"

namespace openikev2 {

    /**
        This class allows to create different Payloads using an unique method.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class PayloadFactory {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Payload based on its binary representation and type
             * @param type Payload type
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             * @return The parsed payload
             */
            static auto_ptr<Payload> createPayload( Payload::PAYLOAD_TYPE type, ByteBuffer& byte_buffer );
    };
}
#endif
