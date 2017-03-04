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
#ifndef PAYLOAD_NONCE_H
#define PAYLOAD_NONCE_H

#include "payload.h"

namespace openikev2 {

    /**
        This class represents a Nonce Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_NONCE : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ByteArray> nonce;      /**< Random value */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new Payload_NONCE, setting the nonce value
             * @param nonce Nonce value
             */
            Payload_NONCE( auto_ptr<ByteArray> nonce );

        public:
            /**
             * Creates a new Payload_NONCE, calculating randomly its length and its value
             */
            Payload_NONCE();

            /**
             * Creates a new Payload_NONCE, setting its length.
             * @param nonce_length Length of the random value in bytes (must be between 16 and 256 both inclusive).
             */
            Payload_NONCE( uint16_t nonce_length );

            /**
             * Creates a new Payload_NONCE cloning another one
             * @param other Another Payload_NONCE to be copied
             */
            Payload_NONCE( const Payload_NONCE& other );

            /**
             * Creates a new Payload_NONCE based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_NONCE> parse( ByteBuffer& byte_buffer );

            /**
             * Gets the nonce value
             * @return The nonce value
             */
            virtual ByteArray& getNonceValue();

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_NONCE();

    };

};
#endif
