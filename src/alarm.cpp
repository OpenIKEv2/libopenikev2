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
#include "alarm.h"
#include "threadcontroller.h"
#include "autolock.h"
#include "log.h"

#include <memory.h>

namespace openikev2 {

    Alarm::Alarm( Alarmable &alarmable, uint32_t msec_total ) : alarmable( alarmable ) {
        this->msec_total = msec_total;
        this->msec_left = msec_total;
        this->enabled = false;

        this->mutex = ThreadController::getMutex();

        Log::writeLockedMessage( this->getLogId(), "New Alarm created", Log::LOG_ALRM, true );
    }

    Alarm::~Alarm() {
        Log::writeLockedMessage( this->getLogId(), "Alarm deleted", Log::LOG_ALRM, true );
    }

    void Alarm::reset( ) {
        AutoLock auto_lock(  );

        this->msec_left = this->msec_total;
        this->enabled = true;

        Log::writeLockedMessage( this->getLogId(), "Alarm reseted", Log::LOG_ALRM, true );
    }

    void Alarm::disable( ) {
        AutoLock auto_lock( *this->mutex );

        this->enabled = false;

        Log::writeLockedMessage( this->getLogId(), "Alarm disabled", Log::LOG_ALRM, true );
    }

    void Alarm::setTime( uint32_t milliseconds ) {
        AutoLock auto_lock( *this->mutex );

        this->msec_total = milliseconds;
    }

    void Alarm::notifyAlarmable( ) {
        this->alarmable.notifyAlarm( *this );
    }

    uint32_t Alarm::getTotalTime( ) const {
        return this->msec_total;
    }

    string Alarm::getLogId() {
        uint32_t value = 0;
        memcpy(&value, this, 4);
        return "Alarm=" + Printable::toHexString( &value, 4 );
    }
}



