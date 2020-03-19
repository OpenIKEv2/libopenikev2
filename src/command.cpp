/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/

#include "command.h"

namespace openikev2 {

    Command::Command( bool is_inheritable ) {
        this->is_inheritable = is_inheritable;
    }

    Command::~Command() {}

    bool Command::isInheritable() const {
        return this->is_inheritable;
    }
}
