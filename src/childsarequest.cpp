/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
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



