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
#ifndef PAYLOAD_VENDOR_H
#define PAYLOAD_VENDOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"

namespace openikev2 {

    /**
        This class represents a VENDOR payload
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_VENDOR : public Payload {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ByteArray> vendor_id;  /**< Vendor ID */

            /****************************** METHODS ******************************/
        public:

            /**
             * Creates a new Payload_VENDOR
             * @param vendor_id Binary vendor ID
             */
            Payload_VENDOR( auto_ptr<ByteArray> vendor_id );

            /**
             * Creates a new Payload_VENDOR
             * @param vendor_id_str String vendor ID
             */
            Payload_VENDOR( string vendor_id_str );

            /**
             * Creates a new Payload_VENDOR clonning another one
             * @param other Other Payload_VENDOR
             */
            Payload_VENDOR( const Payload_VENDOR& other );

            /**
             * Creates a new Payload_VENDOR based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_VENDOR> parse( ByteBuffer & byte_buffer );

            /**
             * Gets the vendor id value
             * @return Binary vendor ID value
             */
            virtual ByteArray& getVendorId();

            virtual void getBinaryRepresentation( ByteBuffer & byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_VENDOR();
    };
}
#endif
