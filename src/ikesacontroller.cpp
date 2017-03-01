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

