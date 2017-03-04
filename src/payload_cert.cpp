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
#include "payload_cert.h"
#include "utils.h"
#include "exception.h"

namespace openikev2 {

    Payload_CERT::Payload_CERT( Enums::CERT_ENCODING cert_encoding, auto_ptr<ByteArray> certificate_data )
            : Payload ( PAYLOAD_CERT, false ) {
        this->cert_encoding = cert_encoding;
        this->certificate_data = certificate_data;
    }

    Payload_CERT::Payload_CERT( const Payload_CERT& other )
            : Payload ( PAYLOAD_CERT, false ) {
        this->cert_encoding = other.cert_encoding;
        this->certificate_data = other.certificate_data->clone();
    }

    auto_ptr<Payload_CERT> Payload_CERT::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header and the certificate encoding
        if ( payload_length < 5 )
            throw ParsingException( "Payload_CERT length cannot be < 5 bytes." );

        // Reads certificate enconding
        Enums::CERT_ENCODING cert_encoding = ( Enums::CERT_ENCODING ) byte_buffer.readInt8();

        // reads the certificate data
        auto_ptr<ByteArray> certificate_data = byte_buffer.readByteArray( payload_length - 5 );

        return auto_ptr<Payload_CERT> ( new Payload_CERT ( cert_encoding, certificate_data ) );
    }

    Payload_CERT::~Payload_CERT() {}

    void Payload_CERT::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 5 + this->certificate_data->size() );

        // writes certificate encoding
        byte_buffer.writeInt8( this->cert_encoding );

        // writes certificate data
        byte_buffer.writeByteArray( *this->certificate_data );
    }

    string Payload_CERT::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_CERT> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "encoding=" << Enums::CERT_ENCODING_STR( this->cert_encoding ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "certificate_data=" << this->certificate_data->toStringTab( tabs + 2 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    ByteArray & Payload_CERT::getCertificateData( ) const {
        return * this->certificate_data;
    }

    auto_ptr<Payload> Payload_CERT::clone( ) const {
        return auto_ptr<Payload> ( new Payload_CERT( *this ) );
    }
}

