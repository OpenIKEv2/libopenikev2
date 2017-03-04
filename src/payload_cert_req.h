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
#ifndef PAYLOAD_CERT_REQ_H
#define PAYLOAD_CERT_REQ_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "enums.h"

#include "autovector.h"

namespace openikev2 {

    /**
        This class represents a Certificate Request Payload
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_CERT_REQ : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            AutoVector<ByteArray> ca_public_key_hash;   /**< CA public key SHA1 hash collection */
        public:
            Enums::CERT_ENCODING encoding;              /**< The requested certificate enconding */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Payload_CERT_REQ setting its enconding
             * @param encoding Requested encoding
             */
            Payload_CERT_REQ( Enums::CERT_ENCODING encoding );

            /**
             * Creates a new Payload_CERT_REQ cloning another one
             * @param other Other payload
             */
            Payload_CERT_REQ( const Payload_CERT_REQ& other );

            /**
             * Creates a new Payload_CERT_REQ based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_CERT_REQ> parse( ByteBuffer& byte_buffer );

            /**
             * Adds a CA public key hash to the collection
             * @param hash Public key hash (must be 20 bytes length)
             */
            virtual void addCaPublicKeyHash( auto_ptr<ByteArray> hash );

            /**
             * Gets all the CA public key hash from the collection
             * @return THe public key hash collection
             */
            virtual vector<ByteArray*> getCaPublicKeyHashes() const;

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_CERT_REQ();
    };
}
#endif
