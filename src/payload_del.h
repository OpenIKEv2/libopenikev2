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
#ifndef PAYLOAD_DEL_H
#define PAYLOAD_DEL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "bytebuffer.h"
#include "enums.h"

#include <vector>

namespace openikev2 {

    /**
        This class represents a delete Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_DEL : public Payload {

            /****************************** ATTRIBUTES ******************************/
        public:
            Enums::PROTOCOL_ID protocol_id;     /**< Protocol type of the SA being deleted. */
            vector <uint32_t> ipsec_spi_values; /**< SPI values of the CHILD SAs to be deleted. */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Paylad_DEL setting its protocol id
             * @param protocol_id Protocol id
             */
            Payload_DEL( Enums::PROTOCOL_ID protocol_id );

            /**
             * Creates a new Payload_DEL cloning another one
             * @param other Other Payload_DEL to be cloned
             */
            Payload_DEL( const Payload_DEL& other );

            /**
             * Creates a new Payload_DEL based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_DEL> parse( ByteBuffer& byte_buffer );

            /**
             * Adds an ESP/AH spi to be added in the Payload_DEL
             * @param ipsec_spi SPI to be added in the Payload_DEL.
             */
            virtual void addIpsecSpi( uint32_t ipsec_spi );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_DEL();

    };

};
#endif
