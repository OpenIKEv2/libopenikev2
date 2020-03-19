/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOADFACTORY_H
#define PAYLOADFACTORY_H

#include "payload.h"

namespace openikev2 {

    /**
        This class allows to create different Payloads using an unique method.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
