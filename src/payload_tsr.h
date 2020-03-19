/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_TSR_H
#define PAYLOAD_TSR_H

#include "payload_ts.h"

namespace openikev2 {

    /**
        This class represents a Responder Traffic Selector Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
