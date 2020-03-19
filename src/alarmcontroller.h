/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef ALARMCONTROLLER_H
#define ALARMCONTROLLER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "alarm.h"
#include "alarmcontrollerimpl.h"

namespace openikev2 {

    /**
        This class represents an Alarm Manager, that controls system Alarms.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class AlarmController {
            /****************************** ATTRIBUTES ******************************/
        protected:
            static AlarmControllerImpl* implementation;     /**< Implementation of the AlarmController */

            /****************************** METHODS ******************************/
        public:
            /**
             * Establish the AlarmController implementation
             * @param implementation AlarmController implementation to be used
             */
            static void setImplementation( AlarmControllerImpl* implementation );

            /**
             * Adds the Alarm to the Alarm collection
             * @param alarm Alarm to be added
             */
            static void addAlarm( Alarm& alarm );

            /**
             * Removes the Alarm from the Alarm collection
             * @param alarm Alarm to be removed
             */
            static void removeAlarm( Alarm& alarm );

    };
}
#endif
