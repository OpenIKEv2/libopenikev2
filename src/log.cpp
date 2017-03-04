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
#include "log.h"

#include "threadcontroller.h"
#include "utils.h"
#include "autolock.h"
#include "logimpl.h"

namespace openikev2 {

    LogImpl* Log::implementation( NULL );
    auto_ptr<Mutex> Log::log_mutex( NULL );

    void Log::setImplementation( LogImpl* implementation ) {
        Log::implementation = implementation;
        log_mutex = ThreadController::getMutex();
    }

    void Log::writeLockedMessage( string who, string message, uint16_t type, bool main_info ) {
        AutoLock auto_lock( *log_mutex );
        implementation->writeMessage( who, message, type, main_info );
    }

    void Log::writeMessage( string who, string message, uint16_t type, bool main_info ) {
        implementation->writeMessage( who, message, type, main_info );
    }

    void Log::acquire( ) {
        Log::log_mutex->acquire();
    }

    void Log::release( ) {
        Log::log_mutex->release();
    }

    string Log::LOG_TYPE_STR( uint16_t type ) {
        if ( type & Log::LOG_ALRM )
            return "ALARM";
        else if ( type & Log::LOG_CONF )
            return "CONFG";
        else if ( type & Log::LOG_CRYP )
            return "CRYPT";
        else if ( type & Log::LOG_DHCP )
            return "DHCPC";
        else if ( type & Log::LOG_EBUS )
            return "EVBUS";
        else if ( type & Log::LOG_ERRO )
            return "ERROR";
        else if ( type & Log::LOG_HALF )
            return "HOPEN";
        else if ( type & Log::LOG_IPSC )
            return "IPSEC";
        else if ( type & Log::LOG_MESG )
            return "MESSG";
        else if ( type & Log::LOG_POLI )
            return "POLIC";
        else if ( type & Log::LOG_STAT )
            return "STATE";
        else if ( type & Log::LOG_THRD )
            return "THRDS";
        else if ( type & Log::LOG_WARN )
            return "WARNG";
        else if ( type & Log::LOG_INFO )
            return "INFOR";
        else
            return intToString( type );
    }
}
