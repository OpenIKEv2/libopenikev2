/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "aaacontroller.h"
#include "threadcontroller.h"

namespace openikev2 {
    AAAControllerImpl* AAAController::implementation;

    void AAAController::setImplementation( AAAControllerImpl* implementation ) {
        AAAController::implementation = implementation;
    }

    void AAAController::AAA_send( AAASender& sender) {
        assert (implementation != NULL);
        return implementation->AAA_send( sender );
    }

}


