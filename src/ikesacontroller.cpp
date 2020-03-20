/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "ikesacontroller.h"
#include "command.h"

namespace openikev2 {
    IkeSaControllerImpl* IkeSaController::implementation ( NULL );

    void IkeSaController::setImplementation(  IkeSaControllerImpl* ike_sa_controller ) {
        implementation = ike_sa_controller;
    }

    void IkeSaController::incHalfOpenCounter() {
        assert (implementation != NULL);
        implementation->incHalfOpenCounter();
    }

    void IkeSaController::decHalfOpenCounter() {
        assert (implementation != NULL);
        implementation->decHalfOpenCounter();
    }

    bool IkeSaController::useCookies() {
        assert (implementation != NULL);
        return implementation->useCookies();
    }

    void IkeSaController::requestChildSa( IpAddress & ike_sa_src_addr, IpAddress & ike_sa_dst_addr, auto_ptr<ChildSaRequest> child_sa_request) {
        assert (implementation != NULL);
        implementation->requestChildSa( ike_sa_src_addr, ike_sa_dst_addr, child_sa_request);
    }

    void IkeSaController::requestChildSaMobility( IpAddress & ike_sa_src_addr, IpAddress & ike_sa_dst_addr, auto_ptr<ChildSaRequest> child_sa_request, IpAddress & ike_sa_coa_addr, bool is_ha) {
        assert (implementation != NULL);
        implementation->requestChildSaMobility( ike_sa_src_addr, ike_sa_dst_addr, child_sa_request , ike_sa_coa_addr, is_ha);
    }

    void IkeSaController::addIkeSa( auto_ptr< IkeSa > ike_sa ) {
        assert (implementation != NULL);
        implementation->addIkeSa( ike_sa );
    }

    uint64_t IkeSaController::nextSpi() {
        assert (implementation != NULL);
        return implementation->nextSpi();
    }

    bool IkeSaController::pushCommandByIkeSaSpi( uint64_t spi, auto_ptr< Command > command, bool priority ) {
        assert (implementation != NULL);
        return implementation->pushCommandByIkeSaSpi( spi, command, priority );
    }

    IkeSa* IkeSaController::getIkeSaByIkeSaSpi( uint64_t spi ) {
        assert (implementation != NULL);
        return implementation->getIkeSaByIkeSaSpi( spi );
    }


    bool IkeSaController::pushCommandByChildSaSpi( uint32_t spi, auto_ptr< Command > command, bool priority ) {
        assert (implementation != NULL);
        return implementation->pushCommandByChildSaSpi( spi, command, priority );
    }
}

