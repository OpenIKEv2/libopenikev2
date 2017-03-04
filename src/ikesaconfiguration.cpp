/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
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
#include "ikesaconfiguration.h"
#include "utils.h"

namespace openikev2 {

    IkeSaConfiguration::IkeSaConfiguration( auto_ptr<Proposal> proposal ) {
        assert( proposal.get() != NULL );
        assert( proposal->protocol_id == Enums::PROTO_IKE );
        assert( proposal->getFirstTransformByType( Enums::ENCR ) != NULL );
        assert( proposal->getFirstTransformByType( Enums::INTEG ) != NULL );
        assert( proposal->getFirstTransformByType( Enums::D_H ) != NULL );
        assert( proposal->getFirstTransformByType( Enums::PRF ) != NULL );

        this->proposal = proposal;

        this->retransmition_time = 3;
        this->max_idle_time = 200;
        this->retransmition_factor = 2;
        this->rekey_time = 0xFFFF;
        this->ike_max_exchange_retransmitions = 3;

        this->attributemap.reset( new AttributeMap() );
    }

    IkeSaConfiguration::~IkeSaConfiguration() { }

    Proposal & IkeSaConfiguration::getProposal( ) const {
        return * this->proposal;
    }

    void IkeSaConfiguration::setProposal( auto_ptr< Proposal > proposal ) {
        assert( proposal->getFirstTransformByType( Enums::ENCR ) != NULL );
        assert( proposal->getFirstTransformByType( Enums::INTEG ) != NULL );
        assert( proposal->getFirstTransformByType( Enums::D_H ) != NULL );
        assert( proposal->getFirstTransformByType( Enums::PRF ) != NULL );

        this->proposal = proposal;
    }

    string IkeSaConfiguration::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<IKE_SA_CONFIGURATION> {\n";

        oss << this->proposal->toStringTab( tabs + 1 );

        if ( this->my_id.get() != NULL )
            oss << this->my_id->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs + 1 ) << "<PEER_ID> {\n";

        for ( vector<IdTemplate*>::const_iterator it = this->allowed_ids->begin(); it != this->allowed_ids->end(); it++ ) {
            oss << ( *it )->toStringTab( tabs + 2 );
        }

        oss << Printable::generateTabs( tabs + 1 ) << "}\n";

        oss << Printable::generateTabs( tabs + 1 ) << "max_idle_time=[" << this->max_idle_time << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "retransmition_time=[" << this->retransmition_time << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "retransmition_factor=[" << this->retransmition_factor << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "rekey_time=[" << this->rekey_time << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "ike_max_exchange_retransmitions=[" << this->ike_max_exchange_retransmitions << "]\n";

        oss << this->authenticator->toStringTab( tabs + 1 );

        oss << this->attributemap->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<IkeSaConfiguration> IkeSaConfiguration::clone( ) const {
        auto_ptr<IkeSaConfiguration> result ( new IkeSaConfiguration( auto_ptr<Proposal>( this->proposal->clone() ) ) );

        if ( this->my_id.get() != NULL )
            result->my_id = this->my_id->clone();

        result->rekey_time = this->rekey_time;
        result->max_idle_time = this->max_idle_time;
        result->retransmition_time = this->retransmition_time;
        result->retransmition_factor = this->retransmition_factor;
        result->ike_max_exchange_retransmitions = this->ike_max_exchange_retransmitions;

        result->authenticator = this->authenticator->clone();

        result->attributemap = this->attributemap->clone();

        for ( vector<IdTemplate*>::const_iterator it = this->allowed_ids->begin(); it != this->allowed_ids->end(); it++ ) {
            result->addAllowedId( ( *it )->clone() );
        }

        return result;
    }

    Authenticator & IkeSaConfiguration::getAuthenticator( ) {
        return *this->authenticator;
    }

    bool IkeSaConfiguration::checkId( const ID & id ) {
        for ( vector<IdTemplate*>::const_iterator it = this->allowed_ids->begin(); it != this->allowed_ids->end(); it++ ) {
            if ( ( *it )->match( id ) )
                return true;
        }

        return false;
    }

    void IkeSaConfiguration::addAllowedId( auto_ptr< IdTemplate > id_template ) {
        this->allowed_ids->push_back( id_template.release() );
    }

}







