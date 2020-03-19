/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "configuration.h"
#include "threadcontroller.h"
#include "autolock.h"
#include "log.h"
#include "exception.h"


namespace openikev2 {
    Configuration* Configuration::instance = NULL;

    Configuration::Configuration() {
        this->general_configuration.reset( new GeneralConfiguration() );
        this->mutex_config = ThreadController::getMutex();
    }

    Configuration::~Configuration() { }

    string Configuration::toStringTab( uint8_t tabs ) const {
        AutoLock auto_lock( *this->mutex_config );

        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<CONFIGURATION> {\n";

        oss << this->general_configuration->toStringTab( tabs + 1 );

        // generates the representation of the PeerConfiguration collection
        for ( vector<PeerConfiguration*>::const_iterator it = this->peer_configuration->begin(); it != this->peer_configuration->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";
        return oss.str();
    }

    void Configuration::addPeerConfiguration( auto_ptr<PeerConfiguration> peer_configuration ) {
        assert( peer_configuration.get() != NULL );
        AutoLock auto_lock( *this->mutex_config );
        this->peer_configuration->push_back( peer_configuration.release() );
    }

    void Configuration::setGeneralConfiguration( auto_ptr<GeneralConfiguration> general_configuration ) {
        assert( general_configuration.get() != NULL );
        AutoLock auto_lock( *this->mutex_config );
        this->general_configuration = general_configuration;
    }

    auto_ptr<PeerConfiguration> Configuration::getPeerConfiguration( const IpAddress& ip_address, Enums::ROLE_ID role ) {
        assert( role != Enums::ROLE_ANY );
        AutoLock auto_lock( *this->mutex_config );

        // Find a matching PeerConfiguration
        for ( vector<PeerConfiguration*>::iterator it = this->peer_configuration->begin(); it != this->peer_configuration->end(); it++ ) {
            PeerConfiguration * current_peer_configuration = ( *it );

            // Check role. If current peer configuration is not ANY and is not equal to parameter then omit current peer configuration
            if ( current_peer_configuration->role != Enums::ROLE_ANY && current_peer_configuration->role != role )
                continue;

            // If role are OK then find ID
            if ( current_peer_configuration->hasIpAddress( ip_address) )
                return current_peer_configuration->clone();
        }

        // If not found, throw exception
        throw NoConfigurationFoundException( "Requesting a Peer Configuration" );
    }

    auto_ptr<GeneralConfiguration> Configuration::getGeneralConfiguration( ) {
        AutoLock auto_lock( *this->mutex_config );
        return auto_ptr<GeneralConfiguration>( this->general_configuration->clone() );
    }

    void Configuration::deletePeerConfiguration( const NetworkPrefix& network_prefix ) {
        AutoLock auto_lock( *this->mutex_config );

        // Find a matching PeerConfiguration
        for ( vector<PeerConfiguration*>::iterator it = this->peer_configuration->begin(); it != this->peer_configuration->end(); it++ ) {
            PeerConfiguration * current_peer_configuration = ( *it );

            // If it contains the indicated network prefix
            if ( current_peer_configuration->hasNetworkPrefix( network_prefix ) ) {

                // delete the Network prefix from the Peer Configuration
                current_peer_configuration->deleteNetworkPrefix( network_prefix );

                // if the peer configuration has no more network prefixes, then delete the peer configuration
                if ( current_peer_configuration->getNetworkPrefixCount() == 0 ) {
                    this->peer_configuration->erase( it );
                    delete ( current_peer_configuration );
                }

                return ;
            }
        }

        // If not found, throw exception
        throw NoConfigurationFoundException( "Deleting a Peer Configuration" );
    }

    Configuration & Configuration::getInstance( ) {
        if ( instance == NULL )
            instance = new Configuration();
        return *instance;
    }

    void Configuration::deleteConfiguration( ) {
        delete instance;
        instance = NULL;
    }
}


