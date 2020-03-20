/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "socketaddress.h"

namespace openikev2 {

    SocketAddress::~SocketAddress() {
    }

    bool SocketAddress::operator ==( const SocketAddress & other ) const {
        if ( this->getPort() != other.getPort() )
            return false;

        return ( this->getIpAddress() == other.getIpAddress() );
    }

}








