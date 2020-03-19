/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/

#include "autolock.h"

namespace openikev2 {

    AutoLock::AutoLock( Mutex & managed_mutex )
            : mutex ( managed_mutex ) {
        this->mutex.acquire();
        this->must_release = true;
    }

    AutoLock::~AutoLock( ) {
        if ( this->must_release )
            this->mutex.release();
    }

    void AutoLock::release( ) {
        this->must_release = false;
        this->mutex.release();
    }

}


