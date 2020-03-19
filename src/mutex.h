/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef MUTEX_H
#define MUTEX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace openikev2 {

    /**
    This abstract represents a Mutex, used to synchronize Threads.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Mutex {

            /****************************** METHODS ******************************/
        public:
            /**
             *    Locks the Mutex.
             */
            virtual void acquire() = 0;

            /**
             * Unlocks the Mutex
             */
            virtual void release() = 0;

            virtual ~Mutex();
    };
}
#endif
