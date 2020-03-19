/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "ipaddress.h"

namespace openikev2 {

    IpAddress::~ IpAddress() {}

    bool IpAddress::operator ==( const IpAddress & other ) const {
        if ( this->getFamily() != other.getFamily() )
            return false;

        return ( *this->getBytes() == *other.getBytes() );
    }

    auto_ptr< Attribute > IpAddress::cloneAttribute( ) const {
        return auto_ptr<Attribute> ( this->clone() );
    }
}

