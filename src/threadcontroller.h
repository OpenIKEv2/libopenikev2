/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include "threadcontrollerimpl.h"

namespace openikev2 {

    /**
        This class allows to manage threads using several concrete implementations
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ThreadController {
            /****************************** ATTRIBUTES ******************************/
        protected:
            static ThreadControllerImpl* implementation;            /**< Thread Controller implementation to be used. */

            /****************************** METHODS ******************************/
        public:
            /**
             * Sets the desired Thread Controller implementation.
             * @param implementation Desired Thread Controller implementation.
             */
            static void setImplementation( ThreadControllerImpl* implementation );

            /**
             * Gets a new Condition object.
             * @return A new Condition object.
             */
            static auto_ptr<Condition> getCondition();

            /**
             * Gets a new Mutex object.
             * @return A new Mutex object.
             */
            static auto_ptr<Mutex> getMutex();

            /**
             * Creates a new implementation dependent Semaphore object.
             * @return A new Semaphore object.
             */
            static auto_ptr<Semaphore> getSemaphore( uint32_t initial_value );
    };
};
#endif
