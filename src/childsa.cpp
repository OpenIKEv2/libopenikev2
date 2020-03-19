/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "childsa.h"
#include "ipseccontroller.h"
#include "log.h"
#include "utils.h"
#include "exception.h"

#include <assert.h>

namespace openikev2 {

    ChildSa::ChildSa( uint32_t inbound_spi, bool child_sa_initiator ) {
        this->state = ChildSa::CHILD_SA_CREATING;

        this->inbound_spi = inbound_spi;
        this->outbound_spi = 0;
        this->rekeyed_spi = 0;

        this->mode = Enums::TUNNEL_MODE;
        this->attributemap.reset( new AttributeMap() );
        this->child_sa_initiator = child_sa_initiator;
        this->ipsec_protocol = Enums::PROTO_NONE;
    }

    ChildSa::ChildSa( uint32_t inbound_spi, Enums::PROTOCOL_ID ipsec_protocol, bool child_sa_initiator ) {
        this->state = ChildSa::CHILD_SA_CREATING;

        this->inbound_spi = inbound_spi;
        this->outbound_spi = 0;
        this->rekeyed_spi = 0;

        this->mode = Enums::TUNNEL_MODE;
        this->attributemap.reset( new AttributeMap() );
        this->child_sa_initiator = child_sa_initiator;
        this->ipsec_protocol = ipsec_protocol;
    }


    ChildSa::ChildSa( uint32_t inbound_spi, auto_ptr< ChildSaRequest > child_sa_request ) {
        this->state = ChildSa::CHILD_SA_CREATING;
        this->child_sa_initiator = true;
        this->inbound_spi = inbound_spi;
        this->outbound_spi = 0;
        this->rekeyed_spi = 0;

        this->mode = child_sa_request->mode;
        this->ipsec_protocol = child_sa_request->ipsec_protocol;
        this->my_traffic_selector = child_sa_request->my_traffic_selector;
        this->peer_traffic_selector = child_sa_request->peer_traffic_selector;
        this->attributemap.reset ( new AttributeMap() );
    }


    ChildSa::ChildSa( uint32_t inbound_spi, bool child_sa_initiator, const ChildSa & rekeyed_child_sa ) {
        this->state = ChildSa::CHILD_SA_CREATING;
        this->rekeyed_spi = rekeyed_child_sa.inbound_spi;
        this->inbound_spi = inbound_spi;
        this->outbound_spi = 0;
        this->mode = rekeyed_child_sa.mode;
        this->ipsec_protocol = rekeyed_child_sa.ipsec_protocol;
        this->attributemap.reset( new AttributeMap() );
        this->child_sa_initiator = child_sa_initiator;
        this->my_traffic_selector.reset( new Payload_TSi( *rekeyed_child_sa.my_traffic_selector ) );
        this->peer_traffic_selector.reset( new Payload_TSr( *rekeyed_child_sa.peer_traffic_selector ) );
    }

    ChildSa::~ChildSa() {}

    auto_ptr< ByteArray > ChildSa::getId( ) const {
        auto_ptr<ByteBuffer> result ( new ByteBuffer( 4 ) );
        result->writeInt32( this->inbound_spi );

        return auto_ptr<ByteArray> ( result );
    }

    string ChildSa::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        // Prints object name
        oss << Printable::generateTabs( tabs ) << "CHILD_SA{\n";

        ByteBuffer temp( 4 );
        temp.writeInt32( this->inbound_spi );
        oss << Printable::generateTabs( tabs + 1 ) << "inbound_spi=" << temp.toStringTab( tabs + 1 );

        temp.reset();
        temp.writeInt32( this->outbound_spi );
        oss << ",outbound_spi=" << temp.toStringTab( tabs + 1 ) << "\n";

        temp.reset();
        temp.writeInt32( this->rekeyed_spi );
        oss << Printable::generateTabs( tabs + 1 ) << "rekeyed_spi=" << temp.toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "mode=[" << Enums::IPSEC_MODE_STR( this->mode ) << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "ipsec protocol=[" << Enums::PROTOCOL_ID_STR( this->ipsec_protocol ) << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "is_initiator=[" << boolToString( this->child_sa_initiator ) << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "state=[" << ChildSa::CHILD_SA_STATE_STR( this->state ) << "]\n";

        oss << this->attributemap->toStringTab( tabs + 1 );

        if ( this->child_sa_configuration.get() )
            oss << this->child_sa_configuration->toStringTab( tabs + 1 );

        oss << this->my_traffic_selector->toStringTab( tabs + 1 );

        oss << this->peer_traffic_selector->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    Proposal& ChildSa::getProposal( ) const {
        return this->child_sa_configuration->getProposal();
    }

    void ChildSa::setProposal( auto_ptr<Proposal> new_proposal ) {
        this->ipsec_protocol = new_proposal->protocol_id;
        this->child_sa_configuration->setProposal( new_proposal );
    }

    ChildSaConfiguration & ChildSa::getChildSaConfiguration( ) const {
        return * this->child_sa_configuration;
    }

    void ChildSa::setChildSaConfiguration( auto_ptr< ChildSaConfiguration > child_sa_configuration ) {
        if ( this->ipsec_protocol != Enums::PROTO_NONE && this->ipsec_protocol != child_sa_configuration->getProposal().protocol_id )
            throw Exception( "Requested IPsec protocol <" + Enums::PROTOCOL_ID_STR( this->ipsec_protocol ) + "> doesn't match with the configured one <" + Enums::PROTOCOL_ID_STR( child_sa_configuration->getProposal().protocol_id ) + ">" );

        this->child_sa_configuration = child_sa_configuration;
        this->ipsec_protocol = this->child_sa_configuration->getProposal().protocol_id;
    }

    bool ChildSa::equals( const ChildSa& other ) const {
        // Check selectors
        if ( ! ( *this->my_traffic_selector == *other.my_traffic_selector ) )
            return false;

        if ( ! ( *this->peer_traffic_selector == *other.peer_traffic_selector ) )
            return false;

        if ( this->mode != other.mode )
            return false;

        if ( this->ipsec_protocol != other.ipsec_protocol )
            return false;

        return true;
    }

    string ChildSa::CHILD_SA_STATE_STR( CHILD_SA_STATE state ) {
        switch ( state ) {
            case ChildSa::CHILD_SA_CREATING:
                return "CHILD_SA_CREATING";
            case ChildSa::CHILD_SA_DELETING:
                return "CHILD_SA_DELETING";
            case ChildSa::CHILD_SA_ESTABLISHED:
                return "CHILD_SA_ESTABLISHED";
            case ChildSa::CHILD_SA_REKEYED:
                return "CHILD_SA_REKEYED";
            case ChildSa::CHILD_SA_REKEYING:
                return "CHILD_SA_REKEYING";
            default:
                return intToString( state );
        }
    }

    void ChildSa::setState( CHILD_SA_STATE next_state ) {
        Log::writeLockedMessage( this->getLogId(), "Transition: [" + CHILD_SA_STATE_STR( this->state ) + " ---> " + CHILD_SA_STATE_STR( next_state ) + "]", Log::LOG_STAT, true );
        this->state = next_state;
    }

    ChildSa::CHILD_SA_STATE ChildSa::getState( ) const {
        return this->state;
    }

    ChildSa& ChildSa::hasMinNonce( const ChildSa& child_sa1, const ChildSa& child_sa2 ) {
        ByteArray & min_nonce1 = ( *child_sa1.my_nonce < *child_sa1.peer_nonce ) ? *child_sa1.my_nonce : *child_sa1.peer_nonce;
        ByteArray & min_nonce2 = ( *child_sa2.my_nonce < *child_sa2.peer_nonce ) ? *child_sa2.my_nonce : *child_sa2.peer_nonce;

        // If the new Controller1 has the minimun nonce, then return it
        if ( min_nonce1 < min_nonce2 )
            return ( ChildSa& ) child_sa1;

        // Else if the Controller2 has the minimun nonce, then return it
        else if ( min_nonce2 < min_nonce1 )
            return ( ChildSa& ) child_sa2;

        // NONCES ARE EQUALS!!!!!!! Nearly imposible case
        else
            assert( 0 );
    }

    string openikev2::ChildSa::getLogId() const {
        return "CHILD_SA=" + this->getId()->toString();
    }

}






