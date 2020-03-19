/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef OPENIKEV2AUTOLOCK_H
#define OPENIKEV2AUTOLOCK_H

#include <iostream>
#include <cstdlib>
#include <sstream>

#include "mutex.h"

namespace openikev2 {

    /**
        This class automatically acquires a Mutex and release it when it goes out of scope (if not released before)
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class AutoLock {

            /****************************** ATTRIBUTES ******************************/
        private:
            Mutex& mutex;           /**< Mutex to be managed */
            bool must_release;      /**< Indicates if the mutex must be relased when the AutoLock goes out of scope */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new AutoLock to manage the indicated Mutex
             * @param managed_mutex Mutex to be managed
             */
            AutoLock( Mutex& managed_mutex );

            /**
             * Releases the managed Mutex before the AutoLock destruction
             */
            void release();

            ~AutoLock();
    };
}

#endif
