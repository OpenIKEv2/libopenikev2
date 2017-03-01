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
#ifndef PAYLOAD_TSR_H
#define PAYLOAD_TSR_H

#include "payload_ts.h"

namespace openikev2 {

    /**
        This class represents a Responder Traffic Selector Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_TSr : public Payload_TS {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new empty Payload_TSr.
             */
            Payload_TSr();

            /**
             * Creates a new Payload_TSr inserting the indicated TrafficSelector.
             * @param traffic_selector TrafficSelector to be inserted.
             */
            Payload_TSr( auto_ptr<TrafficSelector> traffic_selector );

            /**
             * Clones a Payload_TSr.
             * @param other Payload_TSr to be cloned.
             */
            Payload_TSr( const Payload_TS& other );

            /**
             * Creates a new Payload_TSr based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_TSr> parse( ByteBuffer& byte_buffer );

            virtual ~Payload_TSr();
    };
}
#endif
