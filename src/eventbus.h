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
#ifndef EVENTBUS_H
#define EVENTBUS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <map>
#include <vector>
#include <memory>

#include "busevent.h"
#include "busobserver.h"
#include "mutex.h"

using namespace std;

namespace openikev2 {
    typedef vector<BusObserver*> BusObserverVector;

    /**
        This class represents a Event Bus. It follows the Singleton design pattern.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class EventBus {

            /****************************** ENUMS ******************************/
        protected:
            map<BusEvent::EVENT_TYPE, BusObserverVector> event_observers;   /**< Registered observers collection by Event type */
            auto_ptr<Mutex> map_mutex;                                      /**< Mutex to protect observers collection*/
            static EventBus* instance;                                      /**< Unique EventBus instance */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new EventBus
             */
            EventBus();

        public:

            /**
             * Gets the unique EventBus instance. If the instance doesn't exist, this method creates one and returns it.
             * @return The unique EventBus instance.
             */
            static EventBus& getInstance();

            /**
             * Register a BusObserver in the EventBus
             * @param observer BusObserver to be registered
             * @param event_type Type of the events observer wants to receive
             */
            void registerBusObserver( BusObserver& observer, BusEvent::EVENT_TYPE event_type );

            /**
             * Removes an observer from the observers lists. Note that object will not be deleted.
             * @param observer Observer to be removed
             */
            void removeBusObserver( BusObserver& observer );

            /**
             * Sends a new Event in the EventBus. All the registered observers will be notified.
             * @param event Notified Event
             */
            void sendBusEvent( auto_ptr<BusEvent> event );

            virtual ~EventBus();
    };
};
#endif
