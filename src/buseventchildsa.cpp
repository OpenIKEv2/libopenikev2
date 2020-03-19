/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "buseventchildsa.h"

namespace openikev2 {

    BusEventChildSa::BusEventChildSa( CHILD_SA_EVENT_TYPE child_sa_event_type, IkeSa & ike_sa, ChildSa & child_sa )
            : ike_sa( ike_sa ), child_sa ( child_sa ) {
        this->type = BusEvent::CHILD_SA_EVENT;
        this->child_sa_event_type = child_sa_event_type;
        this->data = NULL;
    }

    BusEventChildSa::BusEventChildSa( CHILD_SA_EVENT_TYPE child_sa_event_type, IkeSa & ike_sa, ChildSa & child_sa, void* data )
            : ike_sa( ike_sa ), child_sa ( child_sa ) {
        this->type = BusEvent::CHILD_SA_EVENT;
        this->child_sa_event_type = child_sa_event_type;
        this->data = data;
    }

    BusEventChildSa::~BusEventChildSa() {}
}
