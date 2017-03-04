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
#include "id.h"

namespace openikev2 {


    ID::ID( Enums::ID_TYPE type, auto_ptr<ByteArray> id_data ) {
        this->id_type = type;
        this->id_data = id_data;
    }

    ID::ID( Enums::ID_TYPE type, string id_data ) {
        this->id_type = type;
        this->id_data.reset ( new ByteArray( ( void * ) id_data.c_str(), id_data.size(), id_data.size(), false ) );
    }

    ID::ID( IpAddress& ip_address ) {
        this->id_type = ( ip_address.getFamily() == Enums::ADDR_IPV4 ) ? Enums::ID_IPV4_ADDR : Enums::ID_IPV6_ADDR;
        this->id_data = ip_address.getBytes();
    }

    ID::ID( const ID& other ) {
        this->id_type = other.id_type;
        this->id_data = other.id_data->clone();
    }

    ID::~ID() {}

    string ID::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<ID> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "id_type=" << Enums::ID_TYPE_STR( this->id_type ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "identification_data=" << this->id_data->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<ID> ID::clone( ) const {
        return auto_ptr<ID> ( new ID( *this ) );
    }

    bool ID::operator==( const ID & other ) const {
        if ( ( this->id_type == other.id_type ) && ( *this->id_data == *other.id_data ) )
            return true;

        return false;
    }
}

