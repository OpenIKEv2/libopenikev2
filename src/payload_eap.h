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
