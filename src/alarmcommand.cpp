/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "alarmcommand.h"
#include "ikesa.h"

namespace openikev2 {

    AlarmCommand::AlarmCommand( Alarm& alarm )
            : Command ( false ), alarm( alarm ) {}

    AlarmCommand::~AlarmCommand() {}

    IkeSa::IKE_SA_ACTION AlarmCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.processAlarm( this->alarm );
    }

    string AlarmCommand::getCommandName() const {
        return "ALARM_TIMEOUT";
    }

}
