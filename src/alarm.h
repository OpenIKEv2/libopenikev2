/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef ALARM_H
#define ALARM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mutex.h"
#include "alarmable.h"

#include <memory>
#include <stdint.h>
#include <iostream>


namespace openikev2 {

    /**
        This class represents an Alarm used to receive programmed events.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Alarm {
            /****************************** ATTRIBUTES ******************************/
        public:
            bool enabled;               /**< Indicates if this alarm must notify to the Alarmable object when its time expires */
            Alarmable &alarmable;       /**< Alarmable object to be notified */
            int64_t msec_total;         /**< Total time of the Alarm (in milliseconds).This time is used when the Alarm is reseted */
            int64_t msec_left;          /**< Remainig time of the Alarm (in milliseconds) */
            auto_ptr<Mutex> mutex;      /**< Mutex used to protect Alarm accesses */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Alarm, setting the receiver of the notification and the total Alarm time
             * @param alarmable Receiver of the alarm notification
             * @param msec_total Total time of the alarm (in milliseconds)
             */
            Alarm( Alarmable &alarmable, uint32_t msec_total );

            /**
             * Resets the count down and enables the Alarm
             */
            virtual void reset();

            /**
             * Disables the alarm. It will not alarm the Alarmable when the count down finishes.
             */
            virtual void disable();

            /**
             * Sets the alarm total time.
             * @param milliseconds Total time in milliseconds.
             */
            virtual void setTime( uint32_t milliseconds );

            /**
             * Returns the total time of the Alarm
             * @return The total time of the Alarm (in milliseconds)
             */
            virtual uint32_t getTotalTime() const;

            /**
             * Notifies alarm timeout to the receiver of the event.
             */
            virtual void notifyAlarmable();

            /**
             * Returns an string representing this Alarm
             * @return An string representing this Alarm
             */
            virtual string getLogId();

            /**
             * Deletes alarm.
             */
            virtual ~Alarm();
    };

}
#endif
