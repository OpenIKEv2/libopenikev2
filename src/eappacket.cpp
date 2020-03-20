/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "eappacket.h"
#include "exception.h"
#include "utils.h"

#include <assert.h>

namespace openikev2
{
    EapPacket::EapPacket( EAP_CODE code, uint8_t identifier, EAP_TYPE eap_type, auto_ptr<ByteArray> eap_type_data ) {
        assert( code == EAP_CODE_REQUEST || code == EAP_CODE_RESPONSE );

        this->code = code;
        this->identifier = identifier;
        this->eap_type = eap_type;
        this->eap_type_data = eap_type_data;
    }

    EapPacket::EapPacket( EAP_CODE code, uint8_t identifier ) {
        assert( code == EAP_CODE_SUCCESS || code == EAP_CODE_FAILURE );

        this->code = code;
        this->identifier = identifier;
        this->eap_type = EAP_TYPE_NONE;
    }

    EapPacket::EapPacket( const EapPacket& other ) {
        this->code = other.code;
        this->identifier = other.identifier;
        this->eap_type = other.eap_type;
        if ( this->eap_type_data.get() != NULL )
            this->eap_type_data = other.eap_type_data->clone();
    }

    auto_ptr<EapPacket> EapPacket::parse( ByteBuffer& byte_buffer ) {
        // reads the code
        EAP_CODE code = ( EAP_CODE ) byte_buffer.readInt8();

        // reads identifier
        uint8_t identifier = byte_buffer.readInt8();

        // reads the eap message length
        uint16_t eap_packet_len = byte_buffer.readInt16();

        // Check payload length
        if ( ( code == EAP_CODE_REQUEST || code == EAP_CODE_RESPONSE ) && eap_packet_len <= 5 )
            throw ParsingException( "This EAP packet must have more than 5 bytes " );
        else if ( ( code == EAP_CODE_SUCCESS || code == EAP_CODE_FAILURE ) && eap_packet_len != 4 )
            throw ParsingException( "This EAP packet must have exactly 4 bytes" );

        EAP_TYPE eap_type = EAP_TYPE_NONE;
        auto_ptr<ByteArray> eap_type_data;

        if ( eap_packet_len > 4 ) {
            eap_type = ( EAP_TYPE ) byte_buffer.readInt8();
            eap_type_data = byte_buffer.readByteArray( eap_packet_len - 5 );
        }

        if ( code == EAP_CODE_REQUEST || code == EAP_CODE_RESPONSE )
            return auto_ptr<EapPacket> ( new EapPacket( code, identifier, eap_type, eap_type_data ) );
        else
            return auto_ptr<EapPacket> ( new EapPacket( code, identifier ) );
    }

    EapPacket::~EapPacket() { }

    void EapPacket::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // Writes code
        byte_buffer.writeInt8( this->code );

        // Writes Identifier
        byte_buffer.writeInt8( this->identifier );

        // writes the EAP message length
        uint16_t packet_length = 4;

        // if code is request or response, then adds (1 + eap_type_data length) to the total length
        if ( this->code == EAP_CODE_REQUEST || this->code == EAP_CODE_RESPONSE )
            packet_length += ( 1 + this->eap_type_data->size() );

        byte_buffer.writeInt16( packet_length );

        // if the type is not NONE, writes the EAP TYPE
        if ( this->eap_type != EAP_TYPE_NONE ) {
            byte_buffer.writeInt8( this->eap_type );
            byte_buffer.writeByteArray( *this->eap_type_data );
        }
    }

    string EapPacket::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<EAP_PACKET> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "code=" << EapPacket::EAP_CODE_STR( this->code ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "identifier=" << ( int ) this->identifier << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "eap_type=" << EapPacket::EAP_TYPE_STR( this->eap_type ) << "\n";

        if ( this->eap_type_data.get() != NULL )
            oss << Printable::generateTabs( tabs + 1 ) << "eap_type_data=" << this->eap_type_data->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<EapPacket> EapPacket::clone( ) const {
        return auto_ptr<EapPacket> ( new EapPacket ( *this ) );
    }

    string EapPacket::EAP_CODE_STR( uint16_t eap_code ) {
        switch ( eap_code ) {
            case EapPacket::EAP_CODE_NONE:
                return "EAP_CODE_NONE";
            case EapPacket::EAP_CODE_REQUEST:
                return "EAP_CODE_REQUEST";
            case EapPacket::EAP_CODE_RESPONSE:
                return "EAP_CODE_RESPONSE";
            case EapPacket::EAP_CODE_SUCCESS:
                return "EAP_CODE_SUCCESS";
            case EapPacket::EAP_CODE_FAILURE:
                return "EAP_CODE_FAILURE";
            default:
                return intToString( eap_code );
        }
    }

    string EapPacket::EAP_TYPE_STR( uint16_t eap_type ) {
        switch ( eap_type ) {
            case EapPacket::EAP_TYPE_NONE:
                return "EAP_TYPE_NONE";
            case EapPacket::EAP_TYPE_IDENTITY:
                return "EAP_TYPE_IDENTITY";
            case EapPacket::EAP_TYPE_NOTIFICATION:
                return "EAP_TYPE_NOTIFICATION";
            case EapPacket::EAP_TYPE_NAK:
                return "EAP_TYPE_NAK";
            case EapPacket::EAP_TYPE_MD5_CHALLENGE:
                return "EAP_TYPE_MD5_CHALLENGE";
            case EapPacket::EAP_TYPE_ONE_TIME_PASSWD:
                return "EAP_TYPE_ONE_TIME_PASSWD";
            case EapPacket::EAP_TYPE_GENERIC_TOKEN_CARD:
                return "EAP_TYPE_GENERIC_TOKEN_CARD";
            default:
                return intToString( eap_type );
        }
    }



}
