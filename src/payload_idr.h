/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_IDR_H
#define PAYLOAD_IDR_H

#include "payload_id.h"

namespace openikev2 {

    /**
    This class represents a Responder Identification Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
