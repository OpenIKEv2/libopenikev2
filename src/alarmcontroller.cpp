/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "alarmcontroller.h"
#include "threadcontroller.h"

namespace openikev2 {
    AlarmControllerImpl* AlarmController::implementation;

    void AlarmController::setImplementation( AlarmControllerImpl* implementation ) {
        AlarmController::implementation = implementation;
    }

    void AlarmController::addAlarm( Alarm& alarm ) {
        assert (implementation != NULL);
        return implementation->addAlarm( alarm );
    }

    void AlarmController::removeAlarm( Alarm& alarm ) {
        assert (implementation != NULL);
        return implementation->removeAlarm( alarm );
    }
}






