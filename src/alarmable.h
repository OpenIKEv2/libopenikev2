/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef ALARMABLE_H
#define ALARMABLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

namespace openikev2 {
    class Alarm;

    /**
        This abstract class represents objects that want to be notified by an Alarm.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Alarmable {

            /****************************** METHODS ******************************/
        public:
            /**
             * This method is called when alarm finishes its countdown.
             */
            virtual void notifyAlarm( Alarm& alarm ) = 0;

            virtual ~Alarmable();
    };
}
#endif

