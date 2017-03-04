/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
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
