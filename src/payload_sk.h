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
#ifndef PAYLOAD_SK_H
#define PAYLOAD_SK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cipher.h"
#include "payload.h"
#include "random.h"


namespace openikev2 {

    /**
        This class represents an Encripted Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_SK : public Payload {
            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ByteArray> payload_data;     /**< All the payload data (IV + encrypted payloads + padding + padding len + checksum */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new Payload_SK setting the encrypted payload data
             * @param payload_data
             */
            Payload_SK( auto_ptr<ByteArray> payload_data );

        public:
            /**
             * Creates a new Payload_SK
             * @param cipher Cipher used to encrypt the data
             * @param decrypted_body Data to be encrypted
             */
            Payload_SK( Cipher& cipher, ByteArray& decrypted_body );

            /**
             * Creates a new Payload_SK cloning another one
             * @param other Other Payload_SK to be cloned
             */
            Payload_SK( const Payload_SK& other );

            /**
             * Creates a new Payload_SK based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_SK> parse( ByteBuffer& byte_buffer );

            /**
             * Gets the decripted body
             * @param cipher Cipher to be used
             * @return The decrypted body
             */
            virtual auto_ptr<ByteArray> getDecryptedBody( Cipher & cipher );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_SK();
    };
}
#endif
