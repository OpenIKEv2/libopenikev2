/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_ke.h"

#include "utils.h"
#include "exception.h"

namespace openikev2 {

    Payload_KE::Payload_KE( DiffieHellman& diffie )
            : Payload ( PAYLOAD_KE, false ) {

        // Copies group id
        this->group = diffie.group_id;

        // Copies key into internal variable
        this->public_key = diffie.getPublicKey().clone();
    }

    Payload_KE::Payload_KE( Enums::DH_ID group, auto_ptr<ByteArray> public_key )
            : Payload ( PAYLOAD_KE, false ) {
        this->group = group;
        this->public_key = public_key;
    }

    Payload_KE::Payload_KE( const Payload_KE & other )
            : Payload ( PAYLOAD_KE, false ) {

        this->group = other.group;
        this->public_key = other.public_key->clone();
    }

    auto_ptr<Payload_KE> Payload_KE::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header, group id and reserved
        if ( payload_length < 8 )
            throw ParsingException( "Payload_KE length cannot be < 8 bytes." );

        // Reads group id from buffer
        Enums::DH_ID group = ( Enums::DH_ID ) byte_buffer.readInt16();

        // skips the reserved bytes
        byte_buffer.skip( 2 );

        // public key size is equal to payload_length - 8 (fixed data)
        auto_ptr<ByteArray> public_key = byte_buffer.readByteArray( payload_length - 8 );

        return auto_ptr<Payload_KE> ( new Payload_KE( group, public_key ) );
    }

    Payload_KE::~Payload_KE() {}

    ByteArray& Payload_KE::getPublicKey() {
        return * this->public_key;
    }

    void Payload_KE::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 8 + this->public_key->size() );

        // writes group id
        byte_buffer.writeInt16( this->group );

        // writes RESERVED
        byte_buffer.writeInt16( 0 );

        // writes public key
        byte_buffer.writeByteArray( *this->public_key );
    }

    string Payload_KE::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_KE> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "gruop_id=" << this->group << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "public_key=" << this->public_key->toStringTab( tabs + 2 ) << endl;

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_KE::clone( ) const {
        return auto_ptr<Payload> ( new Payload_KE( *this ) );
    }
}




