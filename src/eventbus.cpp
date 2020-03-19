/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "eventbus.h"
#include "log.h"
#include "threadcontroller.h"
#include "autolock.h"

namespace openikev2 {

    EventBus* EventBus::instance = NULL;

    EventBus::EventBus() {
        this->map_mutex = ThreadController::getMutex();
    }

    EventBus::~EventBus() { }

    void EventBus::registerBusObserver( BusObserver & observer, BusEvent::EVENT_TYPE event_type ) {
        AutoLock auto_lock( *this->map_mutex );

        this->event_observers[ event_type ].push_back( &observer );
    }

    void EventBus::removeBusObserver( BusObserver & observer ) {
        AutoLock auto_lock( *this->map_mutex );

        for ( map<BusEvent::EVENT_TYPE, BusObserverVector>::iterator it_map = this->event_observers.begin(); it_map != this->event_observers.end(); it_map++ ) {
            // Find in the list
            vector<BusObserver*>::iterator it_observers = ( *it_map ).second.begin();
            while ( it_observers != ( *it_map ).second.end() ) {
                if ( *it_observers == &observer ) {
                    Log::writeLockedMessage( "EventBus", "Deleting bus observer from EventBus lists\n", Log::LOG_EBUS, true );
                    it_observers = ( *it_map ).second.erase( it_observers );
                }
                else
                    it_observers++;
            }
        }
    }

    void EventBus::sendBusEvent( auto_ptr<BusEvent> event ) {
        AutoLock auto_lock( *this->map_mutex );

        for ( BusObserverVector::iterator it = this->event_observers[ event->type ].begin(); it != this->event_observers[ event->type ].end(); it++ ) {
            ( *it )->notifyBusEvent( *event );
        }


    }

    EventBus & EventBus::getInstance( ) {
        if ( instance == NULL )
            instance = new EventBus();
        return *instance;
    }
}

