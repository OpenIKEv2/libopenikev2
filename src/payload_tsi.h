/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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
