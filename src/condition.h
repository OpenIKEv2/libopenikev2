/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef CONDITION_H
#define CONDITION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mutex.h"

namespace openikev2 {

    /**
        This abstract class represents a Condition, used to synchronize Threads.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Condition : public Mutex {

            /****************************** METHODS ******************************/
        public:
            /**
             *    Wait until notify() is executed
             */
            virtual void wait() = 0;

            /**
             *  Wake up one of the Threads waiting on this Condition
             */
            virtual void notify() = 0;

            /**
             *    Locks the internal Mutex.
             */
            virtual void acquire() = 0;

            /**
             *    Unlocks the internal Mutex.
             */
            virtual void release() = 0;

            virtual ~Condition();
    };

};
#endif
