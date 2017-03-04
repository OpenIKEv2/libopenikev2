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
#include "payload_del.h"

#include "exception.h"

#include <assert.h>

namespace openikev2 {

    Payload_DEL::Payload_DEL( Enums::PROTOCOL_ID protocol_id )
            : Payload( PAYLOAD_DEL, false ) {
        assert ( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP || protocol_id == Enums::PROTO_IKE );

        // Assigns protocol id
        this->protocol_id = protocol_id;
    }

    Payload_DEL::Payload_DEL( const Payload_DEL& other )
            : Payload( PAYLOAD_DEL, false ) {
        this->protocol_id = other.protocol_id;
        this->ipsec_spi_values = other.ipsec_spi_values;
    }

    auto_ptr<Payload_DEL> Payload_DEL::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header, group id and reserved
        if ( payload_length < 8 )
            throw ParsingException( "Payload_DEL length cannot be < 8 bytes." );

        // Reads protocol id from buffer
        Enums::PROTOCOL_ID protocol_id = ( Enums::PROTOCOL_ID ) byte_buffer.readInt8();

        // Reads spi size
        uint8_t received_spi_size = byte_buffer.readInt8();

        // checks the spi size
        if ( ( protocol_id == Enums::PROTO_IKE && received_spi_size != 0 )
                || ( ( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP ) && received_spi_size != 4 ) )
            throw ParsingException( "Invalid spi size in Payload_DEL" );

        // reads the number of spis value
        uint16_t number_of_spis = byte_buffer.readInt16();

        // checks number of spis
        if ( received_spi_size == 0 && number_of_spis > 0 )
            throw ParsingException( "Invalid number of SPIs in Payload_DEL." );

        // check payload length
        if ( payload_length != 8 + number_of_spis * received_spi_size )
            throw ParsingException( "Invalid payload length. It is inconsistent with number of SPIs" );

        auto_ptr<Payload_DEL> result ( new Payload_DEL ( protocol_id ) );

        // read all the spi values
        for ( uint16_t i = 0; i < number_of_spis; i++ ) {
            uint32_t spi_value = byte_buffer.readInt32();
            result->ipsec_spi_values.push_back( spi_value );
        }

        return result;
    }

    Payload_DEL::~Payload_DEL() { }

    void Payload_DEL::addIpsecSpi( uint32_t ipsec_spi ) {
        assert( this->protocol_id == Enums::PROTO_AH || this->protocol_id == Enums::PROTO_ESP );

        // Insert it into the collection
        this->ipsec_spi_values.push_back( ipsec_spi );
    }

    void Payload_DEL::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 8 + this->ipsec_spi_values.size() * 4 );

        // Writes protocol id
        byte_buffer.writeInt8( this->protocol_id );

        // Writes spi size
        if ( this->protocol_id == Enums::PROTO_IKE )
            byte_buffer.writeInt8 ( 0 );
        else if ( this->protocol_id == Enums::PROTO_AH || this->protocol_id == Enums::PROTO_ESP )
            byte_buffer.writeInt8 ( 4 );
        else
            assert ( 0 );

        // write number of spis
        byte_buffer.writeInt16( this->ipsec_spi_values.size() );

        // Writes spi values
        for ( vector<uint32_t>::const_iterator it = this->ipsec_spi_values.begin(); it != this->ipsec_spi_values.end(); it++ )
            byte_buffer.writeInt32( *it );
    }

    string Payload_DEL::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_DEL> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "protocol_id=" << Enums::PROTOCOL_ID_STR( this->protocol_id ) << "\n";

        for ( vector<uint32_t>::const_iterator it = this->ipsec_spi_values.begin(); it != this->ipsec_spi_values.end(); it++ ) {
            ByteBuffer temp( 4 );
            temp.writeInt32( ( *it ) );
            oss << Printable::generateTabs( tabs + 1 ) << "spi=" << temp.toStringTab( tabs + 1 ) << endl;
        }

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_DEL::clone( ) const {
        return auto_ptr<Payload> ( new Payload_DEL( *this ) );
    }
}

