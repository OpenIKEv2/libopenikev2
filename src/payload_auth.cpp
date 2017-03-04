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
#include "payload_auth.h"
#include "exception.h"

namespace openikev2 {

    Payload_AUTH::Payload_AUTH( Enums::AUTH_METHOD auth_method, auto_ptr<ByteArray> auth_field )
            : Payload( PAYLOAD_AUTH, false ) {

        // sets the auth method
        this->auth_method = auth_method;

        // sets the auth field
        this->auth_field = auth_field;

    }

    Payload_AUTH::Payload_AUTH( const Payload_AUTH & other )
            : Payload( PAYLOAD_AUTH, false ) {
        this->auth_method = other.auth_method;
        this->auth_field = other.auth_field->clone();
    }

    Payload_AUTH::~Payload_AUTH() { }

    auto_ptr<Payload_AUTH> Payload_AUTH::parse( ByteBuffer& byte_buffer ) {
        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header, group id and reserved
        if ( payload_length < 8 )
            throw ParsingException( "Payload_AUTH length cannot be < 8 bytes." );

        // Reads authentication method from buffer
        Enums::AUTH_METHOD auth_method = ( Enums::AUTH_METHOD ) byte_buffer.readInt8();

        // Skip RESERVED bytes
        byte_buffer.skip( 3 );

        // auth field size is equal to payload_length - 8 (fixed data)
        auto_ptr<ByteArray> auth_field = byte_buffer.readByteArray( payload_length - 8 );

        return auto_ptr<Payload_AUTH> ( new Payload_AUTH( auth_method, auth_field ) );
    }

    void Payload_AUTH::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes payload length
        byte_buffer.writeInt16( 8 + this->auth_field->size() );

        // writes authentication method
        byte_buffer.writeInt8( this->auth_method );

        // writes RESERVED
        byte_buffer.fillBytes( 3, 0 );

        // writes auth field
        byte_buffer.writeByteArray( *this->auth_field );
    }

    string Payload_AUTH::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_AUTH> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "authentication_method=" << Enums::AUTH_METHOD_STR( this->auth_method ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "authentication_field=" << this->auth_field->toStringTab( tabs + 2 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    bool Payload_AUTH::operator ==( const Payload_AUTH & other ) const {
        if ( ( this->auth_method == other.auth_method ) && ( *this->auth_field == *other.auth_field ) )
            return true;

        return false;
    }

    auto_ptr<Payload> Payload_AUTH::clone( ) const {
        return auto_ptr<Payload> ( new Payload_AUTH( *this ) );
    }

    Enums::AUTH_METHOD Payload_AUTH::getAuthMethod( ) const {
        return this->auth_method;
    }

    ByteArray & Payload_AUTH::getAuthField( ) const {
        return *this->auth_field;
    }

}









