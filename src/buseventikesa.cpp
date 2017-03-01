/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Lesser General Public            *
*   License as published by the Free Software Foundation; either          *
*   version 2.1 of the License, or (at your option) any later version.    *
*                                                                         *
*   This library is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
*   Lesser General Public License for more details.                       *
*                                                                         *
*   You should have received a copy of the GNU Lesser General Public      *
*   License along with this library; if not, write to the Free Software   *
*   Foundation, Inc., 51 Franklin St, Fifth Floor,                        *
*   Boston, MA  02110-1301  USA                                           *
***************************************************************************/
#include "buseventikesa.h"

namespace openikev2 {

    BusEventIkeSa::BusEventIkeSa( IKE_SA_EVENT_TYPE ike_sa_event_type, IkeSa & ike_sa )
            : ike_sa ( ike_sa ) {
        this->type = BusEvent::IKE_SA_EVENT;
        this->ike_sa_event_type = ike_sa_event_type;
    }

    BusEventIkeSa::BusEventIkeSa( IKE_SA_EVENT_TYPE ike_sa_event_type, IkeSa & ike_sa, void * data )
            : ike_sa ( ike_sa ) {
        this->type = BusEvent::IKE_SA_EVENT;
        this->ike_sa_event_type = ike_sa_event_type;
        this->data = data;
    }

    BusEventIkeSa::~ BusEventIkeSa( ) {}
}