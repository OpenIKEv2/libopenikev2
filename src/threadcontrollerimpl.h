/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef THREADCONTROLLERIMPL_H
#define THREADCONTROLLERIMPL_H

#include "condition.h"
#include "mutex.h"
#include "semaphore.h"
#include "ikesa.h"
#include "command.h"
#include "payload_tsi.h"
#include "payload_tsr.h"

using namespace std;

namespace openikev2 {

    /**
        This abstract class represents a ThreadController concrete implementation
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ThreadControllerImpl {

            /****************************** METHODS ******************************/
        public:
            /**
             * Gets a new Condition object.
             * @return A new Condition object.
             */
            virtual auto_ptr<Condition> getCondition() = 0;

            /**
             * Gets a new Mutex object.
             * @return A new Mutex object.
             */
            virtual auto_ptr<Mutex> getMutex() = 0;

            /**
             * Creates a new implementation dependent Semaphore object.
             * @return A new Semaphore object.
             */
            virtual auto_ptr<Semaphore> getSemaphore( uint32_t initial_value ) = 0;

            virtual ~ThreadControllerImpl();
    };
};
#endif
