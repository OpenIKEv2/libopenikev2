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
#ifndef PAYLOAD_CERT_H
#define PAYLOAD_CERT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "enums.h"

namespace openikev2 {

    /**
        This class represents a Certificate Payload
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_CERT : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ByteArray> certificate_data;   /**< Certificate data */
        public:
            Enums::CERT_ENCODING cert_encoding;     /**< Certificate encoding */

        public:
            /****************************** METHODS ******************************/
            /**
             * Creates a new Payload_CERT setting its attributes
             * @param cert_encoding Certificate enconding
             * @param certificate_data Certificate data
             */
            Payload_CERT( Enums::CERT_ENCODING cert_encoding, auto_ptr<ByteArray> certificate_data );

            /**
             * Creates a new Payload_CERT cloning another one
             * @param other Other Payload_CERT to be cloned
             */
            Payload_CERT ( const Payload_CERT& other );

            /**
             * Creates a new Payload_CERT based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_CERT> parse ( ByteBuffer& byte_buffer );

            /**
             * Gets the certificate data
             * @return The certificate data
             */
            virtual ByteArray& getCertificateData() const;

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_CERT();

    };

};
#endif
