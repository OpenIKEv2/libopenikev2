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
#ifndef PAYLOAD_TSI_H
#define PAYLOAD_TSI_H

#include "payload_ts.h"

namespace openikev2 {

    /**
        This class represents a Initiator Traffic Selector Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Payload_TSi : public Payload_TS {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new empty Payload_TSi.
             */
            Payload_TSi();

            /**
             * Creates a new Payload_TSi inserting the indicated TrafficSelector.
             * @param traffic_selector TrafficSelector to be inserted.
             */
            Payload_TSi( auto_ptr<TrafficSelector> traffic_selector );

            /**
             * Clones a Payload_TSi.
             * @param other Payload_TS to be cloned.
             */
            Payload_TSi( const Payload_TS& other );

            /**
             * Creates a new Payload_TSi based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_TSi> parse( ByteBuffer& byte_buffer );

            virtual ~Payload_TSi();
    };
}
#endif
