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
#ifndef PAYLOAD_KE_H
#define PAYLOAD_KE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "diffiehellman.h"
#include "enums.h"

namespace openikev2 {

    /**
        This class represents a Key Exchange Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_KE : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ByteArray> public_key;     /**< Public key transported by the payload */

        public:
            Enums::DH_ID group;                 /**< Diffie-Hellan group id */

            /****************************** METHODS ******************************/
        protected:
            /**
            * Creates a new Payload_KE setting its parameters
            * @param group Group
            * @param public_key Public key
            */
            Payload_KE( Enums::DH_ID group, auto_ptr<ByteArray> public_key );

        public:
            /**
             * Creates a new Payload_KE using a DiffieHellman object to extract necesary info.
             * @param diffie_hellman DiffeHellman object.
             */
            Payload_KE( DiffieHellman& diffie_hellman );

            /**
             * Creates a new Payload_KE cloning another one
             * @param other Other Payload_KE
             */
            Payload_KE ( const Payload_KE& other );

            /**
             * Creates a new Payload_KE based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_KE> parse( ByteBuffer& byte_buffer );

            /**
             * Gets public key contained in the payload
             * @return The public key
             */
            virtual ByteArray& getPublicKey();

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_KE();
    };
}
#endif
