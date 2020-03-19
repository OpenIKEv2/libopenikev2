/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_conf.h"

#include "exception.h"
#include "utils.h"

namespace openikev2 {

    Payload_CONF::Payload_CONF( CONFIGURATION_TYPE configuration_type )
            : Payload( PAYLOAD_CONF, false ) {
        this->configuration_type = configuration_type;
    }

    Payload_CONF::Payload_CONF( const Payload_CONF & other )
            : Payload( PAYLOAD_CONF, false ) {
        this->configuration_type = other.configuration_type;

        for ( vector<ConfigurationAttribute*>::const_iterator it = other.attributes->begin(); it != other.attributes->end(); it++ )
            this->addConfigurationAttribute( ( *it ) ->clone() );
    }

    auto_ptr<Payload_CONF> Payload_CONF::parse( ByteBuffer& byte_buffer ) {
        // Pointer to the beginning of the payload
        uint8_t * payload_begin = byte_buffer.getReadPosition() - 2;

        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header, group id and reserved
        if ( payload_length < 8 )
            throw ParsingException( "Payload_CONF length cannot be < 8 bytes." );

        // read the configuration type
        Payload_CONF::CONFIGURATION_TYPE configuration_type = ( Payload_CONF::CONFIGURATION_TYPE ) byte_buffer.readInt8();
        auto_ptr<Payload_CONF> result ( new Payload_CONF ( configuration_type ) );

        // skips the reserved bytes
        byte_buffer.skip( 3 );

        // read all the configuration attributes
        while ( byte_buffer.getReadPosition() < payload_begin + payload_length ) {
            auto_ptr<ConfigurationAttribute> attribute = ConfigurationAttribute::parse( byte_buffer );
            result->addConfigurationAttribute( attribute );
        }

        // check size
        if ( byte_buffer.getReadPosition() != payload_begin + payload_length )
            throw ParsingException( "Indicated transform size differs with its real size." );

        return result;
    }

    void Payload_CONF::addConfigurationAttribute( auto_ptr<ConfigurationAttribute> attribute ) {
        this->attributes->push_back( attribute.release() );
    }

    void Payload_CONF::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // Pointer to the beginning of the payload
        uint8_t * payload_begin = byte_buffer.getWritePosition();

        // writes a dummy value for the transform length
        byte_buffer.writeInt16( 0 );

        // Writes configuration type type into buffer
        byte_buffer.writeInt8( this->configuration_type );

        // Writes RESERVED field
        byte_buffer.fillBytes( 3, 0 );

        // Inserts all attributes into buffer
        for ( vector<ConfigurationAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ ) {
            ( *it ) ->getBinaryRepresentation( byte_buffer );
        }

        // pointer to the end of the transform
        uint8_t* payload_end = byte_buffer.getWritePosition();

        // writes the real transform length value
        byte_buffer.setWritePosition( payload_begin );
        byte_buffer.writeInt16( payload_end - payload_begin + 2 );
        byte_buffer.setWritePosition( payload_end );
    }

    string Payload_CONF::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_CONF> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "type=" << Payload_CONF::CONFIGURATION_TYPE_STR( this->configuration_type ) << "\n";

        for ( vector<ConfigurationAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    Payload_CONF::~Payload_CONF() {}

    auto_ptr<Payload> Payload_CONF::clone( ) const {
        return auto_ptr<Payload> ( new Payload_CONF( *this ) );
    }

    string Payload_CONF::CONFIGURATION_TYPE_STR( CONFIGURATION_TYPE conf_type ) {
        switch ( conf_type ) {
            case Payload_CONF::CFG_ACK:
                return "CFG_ACK";
            case Payload_CONF::CFG_REPLY:
                return "CFG_REPLY";
            case Payload_CONF::CFG_REQUEST:
                return "CFG_REQUEST";
            case Payload_CONF::CFG_SET:
                return "CFG_SET";
            default:
                return intToString( conf_type );
        }
    }

    vector< ConfigurationAttribute * > Payload_CONF::getConfigurationAttributesByType( ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE type ) {
        vector<ConfigurationAttribute*> result;

        for ( vector<ConfigurationAttribute*>::iterator it = this->attributes->begin(); it != this->attributes->end(); it++ ) {
            if ( ( *it )->type == type )
                result.push_back( *it );
        }

        return result;
    }

    ConfigurationAttribute * Payload_CONF::getFirstConfigurationAttributeByType( ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE type ) {
        for ( vector<ConfigurationAttribute*>::iterator it = this->attributes->begin(); it != this->attributes->end(); it++ ) {
            if ( ( *it )->type == type )
                return *it;
        }
        return NULL;
    }

    ConfigurationAttribute & Payload_CONF::getUniqueConfigurationAttributeByType( ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE type ) {
        vector<ConfigurationAttribute*> temp = this->getConfigurationAttributesByType( type );
        if ( temp.size() != 1 )
            throw ParsingException( "There must be one and only one Configuration attribute of type " + ConfigurationAttribute::CONFIGURATION_ATTRIBUTE_TYPE_STR( type ) + " in the Payload_CONF" );

        return *temp.front();
    }

}








