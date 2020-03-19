/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef BUSEVENT_IKE_SA_H
#define BUSEVENT_IKE_SA_H

#include "busevent.h"
#include "enums.h"
#include "bytearray.h"

namespace openikev2 {
    class IkeSa;

    /**
        This class represents an IKE SA Bus Event
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class BusEventIkeSa : public BusEvent {

            /****************************** ENUMS ******************************/
        public:
            /** IKE SA event type */
            enum IKE_SA_EVENT_TYPE {
                IKE_SA_CREATED,                             /**< New IKE SA event */
                IKE_SA_DELETED,                             /**< IKE SA deleted event */
                IKE_SA_REKEYED,                             /**< IKE SA rekeyed event */
                IKE_SA_ESTABLISHED,                         /**< IKE SA created (authenticated) event */
                IKE_SA_FAILED,                              /** IKE SA establishment failure */
            };

            /****************************** ENUMS ******************************/
        public:
            IKE_SA_EVENT_TYPE ike_sa_event_type;      /**< IKE SA event type */
            IkeSa& ike_sa;       /**< IKE SA that throws the event*/
            void* data;                               /**< Extra event data */

            /****************************** METHODS ******************************/
        public:
            /**
             * Constructor for a IKE_SA event
             * @param ike_sa_event_type Event type
             * @param ike_sa IkeSa that throws the event
             */
            BusEventIkeSa( IKE_SA_EVENT_TYPE ike_sa_event_type, IkeSa& ike_sa );

            /**
             * Constructor for a IKE_SA event
             * @param ike_sa_event_type Event type
             * @param ike_sa IkeSa that throws the event
             * @param data Extra event data
             * @return
             */
            BusEventIkeSa( IKE_SA_EVENT_TYPE ike_sa_event_type, IkeSa& ike_sa, void* data );

            virtual ~BusEventIkeSa();
    };
}
#endif
