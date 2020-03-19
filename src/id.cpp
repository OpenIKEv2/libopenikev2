/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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

