/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "diffiehellman.h"

namespace openikev2 {
    DiffieHellman::DiffieHellman( Enums::DH_ID group_id ) {
        this->group_id = group_id;
    }

    DiffieHellman::~DiffieHellman() {}

}

