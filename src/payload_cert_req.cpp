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
#include "payload_cert_req.h"
#include "utils.h"
#include "exception.h"

namespace openikev2 {

    Payload_CERT_REQ::Payload_CERT_REQ( Enums::CERT_ENCODING encoding )
            : Payload ( PAYLOAD_CERT_REQ, false ) {
        this->encoding = encoding;
    }

    Payload_CERT_REQ::Payload_CERT_REQ( const Payload_CERT_REQ & other )
            : Payload ( PAYLOAD_CERT_REQ, false ) {
        this->encoding = other.encoding;

        for ( vector<ByteArray*>::const_iterator it = other.ca_public_key_hash->begin(); it != other.ca_public_key_hash->end(); it ++ )
            this->addCaPublicKeyHash( ( *it ) ->clone() );
    }

    auto_ptr<Payload_CERT_REQ> Payload_CERT_REQ::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header and the certificate encoding
        if ( payload_length < 5 )
            throw ParsingException( "Payload_CERT_REQ length cannot be < 5 bytes." );

        // Certification Authority field must be multiple of 20
        if ( ( payload_length - 5 ) % 20 )
            throw ParsingException( "Certification Authority field must be multiple of 20." );

        // Reads certificate enconding
        Enums::CERT_ENCODING encoding = ( Enums::CERT_ENCODING ) byte_buffer.readInt8();

        auto_ptr<Payload_CERT_REQ> result ( new Payload_CERT_REQ ( encoding ) );

        // CA public key hash count
        uint16_t count = ( payload_length - 5 ) / 20;

        for ( uint16_t i = 0; i < count; i++ ) {
            auto_ptr<ByteArray> ca_hash = byte_buffer.readByteArray( 20 );
            result->addCaPublicKeyHash( ca_hash );
        }

        return result;
    }

    Payload_CERT_REQ::~Payload_CERT_REQ() {}

    void Payload_CERT_REQ::addCaPublicKeyHash( auto_ptr<ByteArray> hash ) {
        assert( hash->size() == 20 );

        this->ca_public_key_hash->push_back( hash.release() );
    }

    void Payload_CERT_REQ::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 5 + this->ca_public_key_hash->size() * 20 );

        // writes certificate encoding
        byte_buffer.writeInt8( this->encoding );

        // writes ca public key hashes
        for ( vector<ByteArray*>::const_iterator it = this->ca_public_key_hash->begin(); it != this->ca_public_key_hash->end(); it ++ )
            byte_buffer.writeByteArray( *( *it ) );
    }

    string Payload_CERT_REQ::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_CERT_REQ> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "encoding=" << Enums::CERT_ENCODING_STR( this->encoding ) << "\n";

        for ( vector<ByteArray*>::const_iterator it = this->ca_public_key_hash->begin(); it != this->ca_public_key_hash->end(); it ++ )
            oss << Printable::generateTabs( tabs + 1 ) << "ca_public_key_hash=" << ( *it ) ->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    vector< ByteArray * > Payload_CERT_REQ::getCaPublicKeyHashes( ) const {
        return this->ca_public_key_hash.get();
    }

    auto_ptr<Payload> Payload_CERT_REQ::clone( ) const {
        return auto_ptr<Payload> ( new Payload_CERT_REQ( *this ) );
    }
}



