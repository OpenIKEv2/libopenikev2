/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "buseventcore.h"

namespace openikev2 {

    BusEventCore::BusEventCore( CORE_EVENT_TYPE type ) {
        this->type = BusEvent::CORE_EVENT;
        this->core_event_type = type;
    }

    BusEventCore::~BusEventCore() {}
}
