/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "peerconfiguration.h"

namespace openikev2 {

    PeerConfiguration::PeerConfiguration() {
        this->role = Enums::ROLE_ANY;
    }

    PeerConfiguration::~PeerConfiguration() { }

    string PeerConfiguration::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PEER_CONFIGURATION> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "role=" << Enums::ROLE_ID_STR( this->role ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "<PEER_ADDRESS> {\n";

        for ( vector<NetworkPrefix*>::const_iterator it = this->network_prefixes->begin(); it != this->network_prefixes->end(); it++ )
            oss << Printable::generateTabs( tabs + 2 ) << ( *it ) ->toStringTab( tabs + 2 ) << endl;

        oss << Printable::generateTabs( tabs + 1 ) << "}\n";

        if ( this->ike_sa_configuration.get() )
            oss << this->ike_sa_configuration->toStringTab( tabs + 1 );

        if ( this->child_sa_configuration.get() )
            oss << this->child_sa_configuration->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    void PeerConfiguration::deleteNetworkPrefix( const NetworkPrefix& network_prefix ) {
        for ( vector<NetworkPrefix*>::iterator it = this->network_prefixes->begin(); it != this->network_prefixes->end(); it++ ) {
            if ( network_prefix == *( *it ) ) {
                this->network_prefixes->erase( it );
                delete ( *it );
            }
        }
    }

    void PeerConfiguration::addNetworkPrefix( auto_ptr<NetworkPrefix> network_prefix ) {
        this->network_prefixes->push_back( network_prefix.release() );
    }

    bool PeerConfiguration::hasIpAddress( const IpAddress & ip_address ) const {
        for ( vector<NetworkPrefix*>::const_iterator it = this->network_prefixes->begin(); it != this->network_prefixes->end(); it++ ) {
            if ( ( *it )->containsIpAddress( ip_address ) )
                return true;
        }
        return false;
    }

    bool PeerConfiguration::hasNetworkPrefix( const NetworkPrefix & network_prefix ) const {
        for ( vector<NetworkPrefix*>::const_iterator it = this->network_prefixes->begin(); it != this->network_prefixes->end(); it++ ) {
            if ( *( *it ) == network_prefix )
                return true;
        }
        return false;
    }
    uint16_t PeerConfiguration::getNetworkPrefixCount() const {
        return this->network_prefixes->size();
    }

    auto_ptr< PeerConfiguration > PeerConfiguration::clone() const {
        auto_ptr<PeerConfiguration> result ( new PeerConfiguration() );

        result->role = this->role;
        result->ike_sa_configuration = this->ike_sa_configuration->clone();
        result->child_sa_configuration = this->child_sa_configuration->clone();

        for ( vector<NetworkPrefix*>::const_iterator it = this->network_prefixes->begin(); it != this->network_prefixes->end(); it++ )
            result->addNetworkPrefix( ( *it )->clone() );

        return result;
    }

    IkeSaConfiguration & PeerConfiguration::getIkeSaConfiguration() const {
        return *this->ike_sa_configuration;
    }

    ChildSaConfiguration & PeerConfiguration::getChildSaConfiguration() const {
        return *this->child_sa_configuration;
    }

    void PeerConfiguration::setChildSaConfiguration( auto_ptr< ChildSaConfiguration > child_sa_configuration ) {
        this->child_sa_configuration = child_sa_configuration;
    }

    void PeerConfiguration::setIkeSaConfiguration( auto_ptr< IkeSaConfiguration > ike_sa_configuration ) {
        this->ike_sa_configuration = ike_sa_configuration;
    }

    void PeerConfiguration::setRole( Enums::ROLE_ID role ) {
        this->role = role;
    }

    Enums::ROLE_ID PeerConfiguration::getRole() const {
        return this->role;
    }
}
