/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef OPENIKEV2ALARMCOMMAND_H
#define OPENIKEV2ALARMCOMMAND_H

#include "command.h"
#include "alarm.h"

namespace openikev2 {

    /**
        This class represents an Alarm Command, indicating that an Alarm has became timeout
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class AlarmCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            Alarm& alarm;                           /**< Reference to the timeout Alarm */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new AlarmCommand indicating the timeout Alarm
             * @param alarm Timeout Alarm
             */
            AlarmCommand( Alarm& alarm );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );

            virtual string getCommandName() const;

            virtual ~AlarmCommand();
    };
}

#endif
