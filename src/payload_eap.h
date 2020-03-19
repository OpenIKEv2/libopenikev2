/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_EAP_H
#define PAYLOAD_EAP_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "eappacket.h"

namespace openikev2 {

    /**
        This class represents an EAP transport payload
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_EAP : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<EapPacket> eap_packet;            /**< EAP message code */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Payload_EAP containing an EapPacket
             * @param eap_packet EAP packet
             */
            Payload_EAP( auto_ptr<EapPacket> eap_packet );

            /**
             * Creates a new Payload_EAP cloning another one.
             * @param other Other Payload_EAP to be cloned.
             */
            Payload_EAP( const Payload_EAP& other );

            /**
             * Creates a new Payload_EAP based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_EAP> parse( ByteBuffer& byte_buffer );

            virtual EapPacket& getEapPacket() const;

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_EAP();

    };
}
#endif
