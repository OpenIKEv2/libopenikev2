/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_ID_H
#define PAYLOAD_ID_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "id.h"

namespace openikev2 {

    /**
        This class represents an Identification Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_ID : public Payload {

            /****************************** ATTRIBUTES ******************************/
        public:
            auto_ptr<ID> id;                /**< Identification information */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new Payload_ID
             * @param id ID
             */
            Payload_ID( auto_ptr<ID> id );

            /**
            * Creates a new Payload_ID based on its binary representation.
            * @param byte_buffer Buffer with its read pointer at the "payload length" field
            */
            static auto_ptr<Payload_ID> parse( ByteBuffer& byte_buffer );

        public:
            /**
             * Creates a new Payload_ID cloning another one
             * @param other Other Payload_ID to be cloned
             */
            Payload_ID ( const Payload_ID& other );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_ID();
    };
}
#endif
