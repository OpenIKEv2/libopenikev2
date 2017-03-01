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
#ifndef BUSEVENT_IKE_SA_H
#define BUSEVENT_IKE_SA_H

#include "busevent.h"
#include "enums.h"
#include "bytearray.h"

namespace openikev2 {
    class IkeSa;

    /**
        This class represents an IKE SA Bus Event
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
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
