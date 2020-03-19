/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "buseventikesa.h"

namespace openikev2 {

    BusEventIkeSa::BusEventIkeSa( IKE_SA_EVENT_TYPE ike_sa_event_type, IkeSa & ike_sa )
            : ike_sa ( ike_sa ) {
        this->type = BusEvent::IKE_SA_EVENT;
        this->ike_sa_event_type = ike_sa_event_type;
        this->data = NULL;
    }

    BusEventIkeSa::BusEventIkeSa( IKE_SA_EVENT_TYPE ike_sa_event_type, IkeSa & ike_sa, void * data )
            : ike_sa ( ike_sa ) {
        this->type = BusEvent::IKE_SA_EVENT;
        this->ike_sa_event_type = ike_sa_event_type;
        this->data = data;
    }

    BusEventIkeSa::~ BusEventIkeSa( ) {}
}
