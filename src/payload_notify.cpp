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
#include "payload_notify.h"

#include "exception.h"

namespace openikev2 {

    Payload_NOTIFY::Payload_NOTIFY( NOTIFY_TYPE notification_type, Enums::PROTOCOL_ID protocol_id, auto_ptr<ByteArray> spi_value, auto_ptr<ByteArray> notification_data )
            : Payload( PAYLOAD_NOTIFY, false ) {

        // check the spi value
        if ( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP )
            assert ( spi_value.get() != NULL && spi_value->size() == 4 );
        else if ( protocol_id == Enums::PROTO_IKE || protocol_id == Enums::PROTO_NONE )
            assert ( spi_value.get() == NULL );

        this->notification_type = notification_type;
        this->protocol_id = protocol_id;
        this->spi_value = spi_value;
        this->notification_data = notification_data;
    }

    Payload_NOTIFY::Payload_NOTIFY( NOTIFY_TYPE notification_type, Enums::PROTOCOL_ID protocol_id )
            : Payload( PAYLOAD_NOTIFY, false ) {

        assert ( protocol_id == Enums::PROTO_IKE || protocol_id == Enums::PROTO_NONE );
        this->notification_type = notification_type;
        this->protocol_id = protocol_id;
    }

    Payload_NOTIFY::Payload_NOTIFY( const Payload_NOTIFY& other )
            : Payload( PAYLOAD_NOTIFY, false ) {

        this->notification_type = other.notification_type;
        this->protocol_id = other.protocol_id;

        if ( other.spi_value.get() )
            this->spi_value = other.spi_value->clone();

        if ( other.notification_data.get() )
            this->notification_data = other.notification_data->clone();
    }

    auto_ptr<Payload_NOTIFY> Payload_NOTIFY::parse( ByteBuffer& byte_buffer ) {

        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_length < 8 )
            throw ParsingException( "Payload_KE length cannot be < 8 bytes." );

        // Reads the protocol id from buffer
        Enums::PROTOCOL_ID protocol_id = ( Enums::PROTOCOL_ID ) byte_buffer.readInt8();

        // Reads the SPI size from buffer
        uint8_t spi_size = byte_buffer.readInt8();

        // Reads the notify type id from buffer
        NOTIFY_TYPE notification_type = ( NOTIFY_TYPE ) byte_buffer.readInt16();

        // Check for spi size
        if ( ( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP ) && spi_size != 4 )
            throw ParsingException( "Protocol id doesn't match spi size in notify payload" );
        if ( ( protocol_id == Enums::PROTO_NONE || protocol_id == Enums::PROTO_IKE ) && spi_size != 0 )
            throw ParsingException( "Protocol id doesn't match spi size in notify payload" );

        auto_ptr<ByteArray> spi_value;
        if ( spi_size > 0 )
            spi_value = byte_buffer.readByteArray( spi_size );

        uint16_t data_size = payload_length - 8 - spi_size;
        auto_ptr<ByteArray> notification_data;
        if ( data_size > 0 )
            notification_data = byte_buffer.readByteArray( data_size );

        return auto_ptr<Payload_NOTIFY> ( new Payload_NOTIFY( notification_type, protocol_id, spi_value, notification_data ) );
    }

    Payload_NOTIFY::~Payload_NOTIFY() {}

    void Payload_NOTIFY::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        uint16_t spi_size = this->spi_value.get() ? this->spi_value->size() : 0;
        uint16_t data_size = this->notification_data.get() ? this->notification_data->size() : 0;

        // writes payload length
        byte_buffer.writeInt16( 8 + spi_size + data_size );

        // writes protocol ID
        byte_buffer.writeInt8( this->protocol_id );

        // writes SPI size
        byte_buffer.writeInt8( spi_size );

        // writes notification type
        byte_buffer.writeInt16( this->notification_type );

        // writes SPI if available
        if ( this->spi_value.get() )
            byte_buffer.writeByteArray( *this->spi_value );

        // writes notification data
        if ( this->notification_data.get() )
            byte_buffer.writeByteArray( *this->notification_data );
    }

    string Payload_NOTIFY::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_NOTIFY> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "type=" << Payload_NOTIFY::NOTIFY_TYPE_STR( this->notification_type ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "protocol_id=" << Enums::PROTOCOL_ID_STR( this->protocol_id ) << "\n";

        if ( this->spi_value.get() )
            oss << Printable::generateTabs( tabs + 1 ) << "spi_value=" << this->spi_value->toStringTab( tabs + 1 ) << "\n";

        if ( this->notification_data.get() )
            oss << Printable::generateTabs( tabs + 1 ) << "notification_data=" << this->notification_data->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    bool Payload_NOTIFY::isError( ) {
        return ( this->notification_type < INITIAL_CONTACT );
    }

    auto_ptr<Payload> Payload_NOTIFY::clone( ) const {
        return auto_ptr<Payload> ( new Payload_NOTIFY ( *this ) );
    }

    string Payload_NOTIFY::NOTIFY_TYPE_STR( NOTIFY_TYPE notify_type ) {
        switch ( notify_type ) {
            case Payload_NOTIFY::ADDITIONAL_TS_POSSIBLE:
                return "ADDITIONAL_TS_POSSIBLE";
            case Payload_NOTIFY::AUTHENTICATION_FAILED:
                return "AUTHENTICATION_FAILED";
            case Payload_NOTIFY::COOKIE:
                return "COOKIE";
            case Payload_NOTIFY::ESP_TFC_PADDING_NOT_SUPPORTED:
                return "ESP_TFC_PADDING_NOT_SUPPORTED";
            case Payload_NOTIFY::FAILED_CP_REQUIRED:
                return "FAILED_CP_REQUIRED";
            case Payload_NOTIFY::HTTP_CERT_LOOKUP_SUPPORTED:
                return "HTTP_CERT_LOOKUP_SUPPORTED";
            case Payload_NOTIFY::INITIAL_CONTACT:
                return "INITIAL_CONTACT";
            case Payload_NOTIFY::INTERNAL_ADDRESS_FAILURE:
                return "INTERNAL_ADDRESS_FAILURE";
            case Payload_NOTIFY::INVALID_IKE_SPI:
                return "INVALID_IKE_SPI";
            case Payload_NOTIFY::INVALID_KE_PAYLOAD:
                return "INVALID_KE_PAYLOAD";
            case Payload_NOTIFY::INVALID_MAJOR_VERSION:
                return "INVALID_MAJOR_VERSION";
            case Payload_NOTIFY::INVALID_MESSAGE_ID:
                return "INVALID_MESSAGE_ID";
            case Payload_NOTIFY::INVALID_SELECTORS:
                return "INVALID_SELECTORS";
            case Payload_NOTIFY::INVALID_SPI:
                return "INVALID_SPI";
            case Payload_NOTIFY::IPCOMP_SUPPORTED:
                return "IPCOMP_SUPPORTED";
            case Payload_NOTIFY::INVALID_SYNTAX:
                return "INVALID_SYNTAX";
            case Payload_NOTIFY::NAT_DETECTION_DESTINATION_IP:
                return "NAT_DETECTION_DESTINATION_IP";
            case Payload_NOTIFY::NAT_DETECTION_SOURCE_IP:
                return "NAT_DETECTION_SOURCE_IP";
            case Payload_NOTIFY::NON_FIRST_FRAGMENT_ALSO:
                return "NON_FIRST_FRAGMENT_ALSO";
            case Payload_NOTIFY::NO_ADDITIONAL_SAS:
                return "NO_ADDITIONAL_SAS";
            case Payload_NOTIFY::NO_PROPOSAL_CHOSEN:
                return "NO_PROPOSAL_CHOSEN";
            case Payload_NOTIFY::REKEY_SA:
                return "REKEY_SA";
            case Payload_NOTIFY::SET_WINDOW_SIZE:
                return "SET_WINDOW_SIZE";
            case Payload_NOTIFY::SINGLE_PAIR_REQUIRED:
                return "SINGLE_PAIR_REQUIRED";
            case Payload_NOTIFY::TS_UNACCEPTABLE:
                return "TS_UNACCEPTABLE";
            case Payload_NOTIFY::UNSUPPORTED_CRITICAL_PAYLOAD:
                return "UNSUPPORTED_CRITICAL_PAYLOAD";
            case Payload_NOTIFY::USE_TRANSPORT_MODE:
                return "USE_TRANSPORT_MODE";
            default:
                return intToString( notify_type );
        }
    }
}
