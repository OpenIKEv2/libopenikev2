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
#ifndef BUSEVENT_CHILD_SA_H
#define BUSEVENT_CHILD_SA_H

#include "busevent.h"
#include "enums.h"
#include "bytearray.h"

namespace openikev2 {
    class IkeSa;
    class ChildSa;

    /**
        This class represents an CHILD SA Bus Event
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class BusEventChildSa : public BusEvent {

            /****************************** ENUMS ******************************/
        public:
            /** CHILD SA event type */
            enum CHILD_SA_EVENT_TYPE {
                CHILD_SA_CREATED,                       /**< A new CHILD SA has been created */
                CHILD_SA_DELETED,                       /**< A CHILD SA has been deleted */
                CHILD_SA_ESTABLISHED,                   /**< The CHILD SA has been established successfully */
                CHILD_SA_REKEYED,                       /**< The CHILD SA has been rekeyed successfully */
                CHILD_SA_FAILED,                        /**< CHILD SA establishment failure */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            CHILD_SA_EVENT_TYPE child_sa_event_type;    /**< CHILD SA event type */
            IkeSa& ike_sa;                              /**< IKE_SA that controls the Child SA event */
            ChildSa& child_sa;                          /**< Child_SA that causes the event (it may be NULL) */
            void* data;                                 /**< Extra event data */

            /****************************** METHODS ******************************/
        public:

            /**
             * Constructor for a child_sa event without extra data
             * @param child_sa_event_type The type of the child_sa event
             * @param ike_sa The IkeSa that controls the child SA that launch the event
             * @param child_sa The child SA that launch the event
             */
            BusEventChildSa( CHILD_SA_EVENT_TYPE child_sa_event_type, IkeSa& ike_sa, ChildSa& child_sa );

            /**
             * Constructor for a child_sa event with extra data
             * @param child_sa_event_type The type of the child_sa event
             * @param ike_sa The IkeSa that controls the child SA that launch the event
             * @param child_sa The child SA that launch the event
             * @param data Extra event data
             */
            BusEventChildSa( CHILD_SA_EVENT_TYPE child_sa_event_type, IkeSa& ike_sa, ChildSa& child_sa, void* data );

            virtual ~BusEventChildSa();
    };
};
#endif
