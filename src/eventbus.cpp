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

