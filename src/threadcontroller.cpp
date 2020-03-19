/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "threadcontroller.h"
#include "assert.h"

namespace openikev2 {

    ThreadControllerImpl* ThreadController::implementation =  NULL;

    void ThreadController::setImplementation( ThreadControllerImpl* impl ) {
        implementation = impl;
    }

    auto_ptr< Condition > ThreadController::getCondition( ) {
        assert (implementation != NULL);
        return implementation->getCondition();
    }

    auto_ptr< Mutex > ThreadController::getMutex( ) {
        assert (implementation != NULL);
        return implementation->getMutex();
    }

    auto_ptr< Semaphore > ThreadController::getSemaphore( uint32_t initial_value ) {
        assert (implementation != NULL);
        return implementation->getSemaphore( initial_value );
    }
}
