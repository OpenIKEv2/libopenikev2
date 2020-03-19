/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_nonce.h"
#include "cryptocontroller.h"
#include "exception.h"

namespace openikev2 {

    Payload_NONCE::Payload_NONCE()
            : Payload( PAYLOAD_NONCE, false ) {

        // Creates a new random object
        auto_ptr<Random> random ( CryptoController::getRandom() );

        // Generate nonce length
        uint16_t nonce_length = random->getRandomInt32( 16, 256 );

        // Fills nonce value with random bytes
        this->nonce = random->getRandomBytes( nonce_length );
    }

    Payload_NONCE::Payload_NONCE( auto_ptr<ByteArray> nonce )
            : Payload( PAYLOAD_NONCE, false ) {
        this->nonce = nonce;
    }

    Payload_NONCE::Payload_NONCE( uint16_t nonce_length )
            : Payload( PAYLOAD_NONCE, false ) {

        // Checks if nonce length has correct value
        assert( nonce_length >= 16 && nonce_length <= 256 );

        // Creates a new random object
        auto_ptr<Random> random ( CryptoController::getRandom() );

        // Fills nonce value with random bytes
        this->nonce = random->getRandomBytes( nonce_length );
    }

    Payload_NONCE::Payload_NONCE( const Payload_NONCE& other )
            : Payload( PAYLOAD_NONCE, false ) {

        this->nonce = other.nonce->clone();
    }

    auto_ptr<Payload_NONCE> Payload_NONCE::parse( ByteBuffer& byte_buffer ) {

        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_length < 20 || payload_length > 260 )
            throw ParsingException( "Payload_NONCE length cannot be < 20 bytes nor > 260." );

        // reads nonce value
        auto_ptr<ByteArray> nonce = byte_buffer.readByteArray( payload_length - 4 );

        return auto_ptr<Payload_NONCE> ( new Payload_NONCE( nonce ) );
    }

    Payload_NONCE::~Payload_NONCE() { }

    void Payload_NONCE::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 4 + this->nonce->size() );

        // writes nonce value
        byte_buffer.writeByteArray( *this->nonce );
    }

    ByteArray & Payload_NONCE::getNonceValue( ) {
        return * this->nonce;
    }


    string Payload_NONCE::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_NONCE> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "nonce_value=" << this->nonce->toStringTab( tabs + 2 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_NONCE::clone( ) const {
        return auto_ptr<Payload> ( new Payload_NONCE( *this ) );
    }

}

