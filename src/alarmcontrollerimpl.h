/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef OPENIKEV2ALARMCONTROLLERIMPL_H
#define OPENIKEV2ALARMCONTROLLERIMPL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "alarm.h"

using namespace std;

namespace openikev2 {

    /**
        This class contains the AlarmController implementation.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class AlarmControllerImpl {
            /****************************** METHODS ******************************/
        public:
            /**
             * Adds the Alarm to the Alarm collection
             * @param alarm Alarm to be added
             */
            virtual void addAlarm( Alarm& alarm ) = 0;

            /**
             * Removes the Alarm from the Alarm collection
             * @param alarm Alarm to be removed
             */
            virtual void removeAlarm( Alarm& alarm ) = 0;

            virtual ~AlarmControllerImpl();
    };

}

#endif
