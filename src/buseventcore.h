/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef BUSEVENT_CORE_H
#define BUSEVENT_CORE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "busevent.h"

namespace openikev2 {

    /**
        This class respresents a Core Bus Event
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class BusEventCore : public BusEvent {

            /****************************** ENUMS ******************************/
        public:
            /** Core event type */
            enum CORE_EVENT_TYPE {
                ALL_SAS_CLOSED,                   /**< All SAs has been closes successfully */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            CORE_EVENT_TYPE core_event_type;    /**< Core event type */

        public:
            /**
             * Creates a new Core Bus Event.
             * @param type Core event type
             */
            BusEventCore( CORE_EVENT_TYPE type );

            virtual ~BusEventCore();
    };
};
#endif
