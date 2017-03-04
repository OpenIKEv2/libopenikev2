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
#include "configurationattribute.h"
#include "exception.h"
#include "utils.h"

namespace openikev2 {

    ConfigurationAttribute::ConfigurationAttribute( CONFIGURATION_ATTRIBUTE_TYPE type, auto_ptr<ByteArray> value ) {
        this->type = type;
        this->value = value.get() ? value : auto_ptr<ByteArray> ( new ByteArray( 0 ) );

        // Check the length of the attribute and converts a parsing exception in an assert
        try {
            ConfigurationAttribute::checkLength( type, this->value->size() );
        }
        catch ( ParsingException & ex ) {
            cout << ex.what() << endl;
            assert( 0 );
        }
    }

    auto_ptr<ConfigurationAttribute> ConfigurationAttribute::parse( ByteBuffer& binary_representation ) {
        // Get the attribute type, omitting the reserved bit
        CONFIGURATION_ATTRIBUTE_TYPE type = ( CONFIGURATION_ATTRIBUTE_TYPE ) ( binary_representation.readInt16() & 0x7FFF );

        // Get the attribute length
        uint16_t attribute_len = binary_representation.readInt16();

        // Checks length consistency
        checkLength( type, attribute_len );

        // Get the attribute value
        auto_ptr<ByteArray> value = binary_representation.readByteArray( attribute_len );

        return auto_ptr<ConfigurationAttribute> ( new ConfigurationAttribute( type, value ) );
    }

    ConfigurationAttribute::~ConfigurationAttribute() { }

    void ConfigurationAttribute::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // Writes type and reserved bit
        byte_buffer.writeInt16( this->type & 0x7FFF );

        // Writes length
        byte_buffer.writeInt16( this->value->size() );

        // Writes attribute value
        byte_buffer.writeByteArray( *this->value );
    }

    string ConfigurationAttribute::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<CONFIGURATION ATTRIBUTE> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "type=[" << ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE_STR( this->type ) << "]\n";

        oss << Printable::generateTabs( tabs + 1 ) << "value=" << this->value->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<ConfigurationAttribute> ConfigurationAttribute::clone( ) {
        return auto_ptr<ConfigurationAttribute> ( new ConfigurationAttribute( this->type, this->value->clone() ) );
    }

    void ConfigurationAttribute::checkLength( CONFIGURATION_ATTRIBUTE_TYPE type, uint16_t len ) {
        switch ( type ) {
            case INTERNAL_IP4_ADDRESS:
            case INTERNAL_IP4_NETMASK:
            case INTERNAL_IP4_DNS:
            case INTERNAL_IP4_NBNS:
            case INTERNAL_IP4_DHCP:
            case INTERNAL_ADDRESS_EXPIRY:
                if ( len != 0 && len != 4 )
                    throw ParsingException( "Configuration Attribute has invalid length: expected=0/4, received=" + intToString( len ) );
                break;

            case INTERNAL_IP6_DNS:
            case INTERNAL_IP6_NBNS:
            case INTERNAL_IP6_DHCP:
                if ( len != 0 && len != 16 )
                    throw ParsingException( "Configuration Attribute has invalid length: expected=0/16, received=" + intToString( len ) );
                break;

            case INTERNAL_IP6_ADDRESS:
                if ( len != 0 && len != 17 )
                    throw ParsingException( "Configuration Attribute has invalid length: expected=0/17, received=" + intToString( len ) );
                break;

            case INTERNAL_IP4_SUBNET:
                if ( len != 0 && len != 8 )
                    throw ParsingException( "Configuration Attribute has invalid length: expected=0/8, received=" + intToString( len ) );
                break;

            case INTERNAL_IP6_SUBNET:
                if ( len != 17 )
                    throw ParsingException( "Configuration Attribute has invalid length: expected=17, received=" + intToString( len ) );
                break;

            case SUPPORTED_ATTRIBUTES:
                if ( len % 2 != 0 )
                    throw ParsingException( "Configuration Attribute has invalid length: expected=multiple of 2, received=" + intToString( len ) );
                break;

        }
    }

    string ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE_STR( CONFIGURATION_ATTRIBUTE_TYPE conf_attr ) {
        switch ( conf_attr ) {
            case ConfigurationAttribute::INTERNAL_ADDRESS_EXPIRY:
                return "INTERNAL_ADDRESS_EXPIRY";
            case ConfigurationAttribute::APPLICATION_VERSION:
                return "APPLICATION_VERSION";
            case ConfigurationAttribute::INTERNAL_IP4_ADDRESS:
                return "INTERNAL_IP4_ADDRESS";
            case ConfigurationAttribute::INTERNAL_IP4_DHCP:
                return "INTERNAL_IP4_DHCP";
            case ConfigurationAttribute::INTERNAL_IP4_DNS:
                return "INTERNAL_IP4_DNS";
            case ConfigurationAttribute::INTERNAL_IP4_NBNS:
                return "INTERNAL_IP4_NBNS";
            case ConfigurationAttribute::INTERNAL_IP4_NETMASK:
                return "INTERNAL_IP4_NETMASK";
            case ConfigurationAttribute::INTERNAL_IP4_SUBNET:
                return "INTERNAL_IP4_SUBNET";
            case ConfigurationAttribute::INTERNAL_IP6_ADDRESS:
                return "INTERNAL_IP6_ADDRESS";
            case ConfigurationAttribute::INTERNAL_IP6_DHCP:
                return "INTERNAL_IP6_DHCP";
            case ConfigurationAttribute::INTERNAL_IP6_DNS:
                return "INTERNAL_IP6_DNS";
            case ConfigurationAttribute::INTERNAL_IP6_NBNS:
                return "INTERNAL_IP6_NBNS";
            case ConfigurationAttribute::INTERNAL_IP6_SUBNET:
                return "INTERNAL_IP6_SUBNET";
            case ConfigurationAttribute::SUPPORTED_ATTRIBUTES:
                return "SUPPORTED_ATTRIBUTES";
            default:
                return intToString( conf_attr );
        }
    }
}
