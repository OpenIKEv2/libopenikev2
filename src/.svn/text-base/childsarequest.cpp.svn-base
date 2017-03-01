/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
 *   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
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
#include "childsarequest.h"

namespace openikev2 {

    ChildSaRequest::ChildSaRequest( Enums::PROTOCOL_ID ipsec_protocol, Enums::IPSEC_MODE mode, auto_ptr< Payload_TS > my_traffic_selector, auto_ptr< Payload_TS > peer_traffic_selector ) {
        this->ipsec_protocol = ipsec_protocol;
        this->mode = mode;
        this->my_traffic_selector = my_traffic_selector;
        this->peer_traffic_selector = peer_traffic_selector;
    }

    ChildSaRequest::ChildSaRequest( const ChildSaRequest & other ) {
        this->ipsec_protocol = other.ipsec_protocol;
        this->mode = other.mode;
        this->my_traffic_selector.reset( new Payload_TSi( *other.my_traffic_selector ) );
        this->peer_traffic_selector.reset( new Payload_TSr( *other.peer_traffic_selector ) );
    }

    ChildSaRequest::~ChildSaRequest() {
    }

    auto_ptr< ChildSaRequest > ChildSaRequest::clone() {
        return auto_ptr<ChildSaRequest> ( new ChildSaRequest( *this ) );
    }

    string ChildSaRequest::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        // Prints object name
        oss << Printable::generateTabs( tabs ) << "CHILD_SA_REQUEST{\n";

        oss << Printable::generateTabs( tabs + 1 ) << "ipsec protocol=[" << Enums::PROTOCOL_ID_STR( this->ipsec_protocol ) << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "mode=[" << Enums::IPSEC_MODE_STR( this->mode ) << "]\n";

        oss << this->my_traffic_selector->toStringTab( tabs + 1 );

        oss << this->peer_traffic_selector->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }
}



