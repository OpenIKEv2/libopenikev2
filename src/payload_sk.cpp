/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#include "payload_sk.h"
#include "cryptocontroller.h"
#include "exception.h"

namespace openikev2 {

    Payload_SK::Payload_SK( Cipher& cipher, ByteArray& decrypted_body )
            : Payload ( PAYLOAD_SK, false ) {

        // creates a new random object
        auto_ptr<Random> random = CryptoController::getRandom();

        // creates the IV
        auto_ptr<ByteArray> initialization_vector = random->getRandomBytes( cipher.encr_block_size );

        // calculates the padding len
        uint16_t padding_len = ( cipher.encr_block_size - 1 ) - ( decrypted_body.size() % cipher.encr_block_size );

        // Creates the buffer to be encrypted
        ByteBuffer to_be_encrypted( decrypted_body.size() + padding_len + 1 );

        // appends the decripted data
        to_be_encrypted.writeByteArray( decrypted_body );

        // appends the padding
        to_be_encrypted.fillBytes( padding_len, 0x1F );

        // appends the padding len
        to_be_encrypted.writeInt8( padding_len );

        // generate the encrypted body
        auto_ptr<ByteArray> encrypted_body = cipher.encrypt( to_be_encrypted, *initialization_vector );

        // Create the payload data with the size = iv_size + encrypted_data_size + checksum_size
        auto_ptr<ByteBuffer> pdata ( new ByteBuffer( cipher.encr_block_size + encrypted_body->size() + cipher.integ_hash_size ) );

        // append the IV
        pdata->writeByteArray( *initialization_vector );

        // append the encrypted data
        pdata->writeByteArray( *encrypted_body );

        // append the 0 integrity checksum
        pdata->fillBytes( cipher.integ_hash_size, 0 );

        this->payload_data = pdata;
    }

    Payload_SK::Payload_SK( const Payload_SK& other )
            : Payload ( PAYLOAD_SK, false ) {

        this->payload_data = other.payload_data->clone();
    }

    Payload_SK::Payload_SK( auto_ptr<ByteArray> payload_data )
            : Payload ( PAYLOAD_SK, false ) {

        this->payload_data = payload_data;
    }

    auto_ptr<Payload_SK> Payload_SK::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_length < 4 )
            throw ParsingException( "Payload_SK length cannot be < 4 bytes." );

        // read all the data
        auto_ptr<ByteArray> payload_data = byte_buffer.readByteArray( payload_length - 4 );

        return auto_ptr<Payload_SK> ( new Payload_SK( payload_data ) );
    }

    Payload_SK::~Payload_SK() {}

    void Payload_SK::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 4 + this->payload_data->size() );

        // writes payload data
        byte_buffer.writeByteArray( *this->payload_data );
    }

    auto_ptr<ByteArray> Payload_SK::getDecryptedBody( Cipher& cipher ) {
        ByteBuffer byte_buffer ( this->payload_data->size() );
        byte_buffer.writeByteArray( *this->payload_data );

        // reads the initilization vector
        auto_ptr<ByteArray> initialization_vector = byte_buffer.readByteArray( cipher.encr_block_size );

        // reads the encrypted body
        auto_ptr<ByteArray> encrypted_body = byte_buffer.readByteArray( this->payload_data->size() - cipher.encr_block_size - cipher.integ_hash_size );

        // reads the decrypted body + padding + padding len
        auto_ptr<ByteArray> decrypted_body = cipher.decrypt( *encrypted_body, *initialization_vector );

        // get the padding len (the last byte in the array)
        uint8_t padding_len = ( *decrypted_body ) [ decrypted_body->size() - 1 ];

        // truncates the decrypted body
        decrypted_body->setSize( decrypted_body->size() - padding_len - 1 );

        return decrypted_body;
    }

    string Payload_SK::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_SK> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "payload_data=" << this->payload_data->toStringTab( tabs + 1 ) + "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_SK::clone( ) const {
        return auto_ptr<Payload> ( new Payload_SK( *this ) );
    }
}



