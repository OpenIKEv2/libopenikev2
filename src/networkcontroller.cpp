/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "networkcontroller.h"
#include "threadcontroller.h"

namespace openikev2 {

    NetworkControllerImpl* NetworkController::implementation( NULL );

    void NetworkController::setImplementation( NetworkControllerImpl* impl ) {
        implementation = impl;
    }

    void NetworkController::refreshInterfaces(){
        assert ( implementation != NULL );
        implementation->refreshInterfaces();
    }

    IpAddress* NetworkController::getCurrentCoA(){
        assert ( implementation != NULL );
        return implementation->getCurrentCoA();
    }

    IpAddress* NetworkController::getHoAbyCoA(const IpAddress& current_coa){
        assert ( implementation != NULL );
        return implementation->getHoAbyCoA(current_coa);
    }

    auto_ptr<IpAddress> NetworkController::getIpAddress( Enums::ADDR_FAMILY family, auto_ptr<ByteArray> data ) {
        assert ( implementation != NULL );
        return implementation->getIpAddress( family, data );
    }

    auto_ptr<SocketAddress> NetworkController::getSocketAddress( string address, int port ) {
        assert ( implementation != NULL );
        return implementation->getSocketAddress( address, port );
    }

    auto_ptr<SocketAddress> NetworkController::getSocketAddress( auto_ptr<IpAddress> address, int port ) {
        assert ( implementation != NULL );
        return implementation->getSocketAddress( address, port );
    }

    auto_ptr<IpAddress> NetworkController::getIpAddress( string address ) {
        assert ( implementation != NULL );
        return implementation->getIpAddress( address );
    }

    void NetworkController::createConfigurationRequest( Message& message, IkeSa & ike_sa ) {
        assert ( implementation != NULL );
        return implementation->createConfigurationRequest( message, ike_sa );
    }

    IkeSa::NEGOTIATION_ACTION NetworkController::processConfigurationRequest(  Message& message, IkeSa& ike_sa ) {
        assert ( implementation != NULL );
        return implementation->processConfigurationRequest( message, ike_sa );
    }

    IkeSa::NEGOTIATION_ACTION NetworkController::processConfigurationResponse( Message& message, IkeSa & ike_sa ) {
        assert ( implementation != NULL );
        return implementation->processConfigurationResponse( message, ike_sa );
    }

    void NetworkController::createConfigurationResponse( Message& message, IkeSa & ike_sa ) {
        assert ( implementation != NULL );
        return implementation->createConfigurationResponse( message, ike_sa );
    }

    void NetworkController::addNotifies( Message & message, IkeSa & ike_sa, ChildSa* child_sa ) {
        assert ( implementation != NULL );
        implementation->addNotifies( message, ike_sa, child_sa );
    }

    NotifyController * NetworkController::getNotifyController( uint16_t type ) {
        assert ( implementation != NULL );
        return implementation->getNotifyController( type );
    }

    void NetworkController::registerNotifyController( uint16_t type, auto_ptr<NotifyController> notify_controller ) {
        assert ( implementation != NULL );
        implementation->registerNotifyController( type, notify_controller );
    }

    void NetworkController::sendMessage( Message & message, Cipher* cipher ) {
        assert ( implementation != NULL );
        implementation->sendMessage( message, cipher );
    }

    void NetworkController::addSrcAddress( auto_ptr< IpAddress > new_src_address ) {
        assert ( implementation != NULL );
        implementation->addSrcAddress ( new_src_address );
    }

    void NetworkController::removeSrcAddress( const IpAddress & src_address ) {
        assert ( implementation != NULL );
        implementation->removeSrcAddress( src_address );
    }

}

