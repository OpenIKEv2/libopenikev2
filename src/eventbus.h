/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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
