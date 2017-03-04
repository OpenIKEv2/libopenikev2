/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
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
#include "radiusattribute.h"
#include <assert.h>


namespace openikev2 {

    RadiusAttribute::RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, auto_ptr< ByteArray > value ) {
	assert (value->size() < 254);
        this->type = type;
        this->value = value;
    }

    RadiusAttribute::RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, string value ) {
	assert (value.size() < 254);
        this->type = type;
        this->value.reset( new ByteArray( value.data(), value.size() ) );
    }

    RadiusAttribute::RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, uint8_t value ) {
        this->type = type;
        auto_ptr<ByteBuffer> buffer( new ByteBuffer( 1 ) );
        buffer->writeInt8( value );
        this->value = buffer;
    }

    RadiusAttribute::RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, uint16_t value ) {
        this->type = type;

        auto_ptr<ByteBuffer> buffer( new ByteBuffer( 2 ) );
        buffer->writeInt16( value );
        this->value = buffer;
    }

    RadiusAttribute::RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, uint32_t value ) {
        this->type = type;

        auto_ptr<ByteBuffer> buffer( new ByteBuffer( 4 ) );
        buffer->writeInt32( value );
        this->value = buffer;
    }

    RadiusAttribute::RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, const IpAddress & value ) {
        this->type = type;
        this->value = value.getBytes();
    }

    RadiusAttribute::~RadiusAttribute() {
    }

    auto_ptr<RadiusAttribute> RadiusAttribute::parse( ByteBuffer& byte_buffer ) {
        // Get the attribute type
        RADIUS_ATTRIBUTE_TYPE type = ( RADIUS_ATTRIBUTE_TYPE ) byte_buffer.readInt8();

        // Get the attribute length
        uint8_t attribute_len = byte_buffer.readInt8();

        // Get the attribute value
        auto_ptr<ByteArray> value = byte_buffer.readByteArray( attribute_len - 2 );

        return auto_ptr<RadiusAttribute> ( new RadiusAttribute( type, value ) );
    }

    RadiusAttribute::RADIUS_ATTRIBUTE_TYPE RadiusAttribute::getType() const {
        return this->type;
    }

    ByteArray & RadiusAttribute::getValue() const {
        return *this->value;
    }

    void RadiusAttribute::getBinaryRepresentation( ByteBuffer & byte_buffer ) const {
        byte_buffer.writeInt8( this->type );
        byte_buffer.writeInt8( this->value->size() + 2 );
        byte_buffer.writeByteArray( *this->value );
    }

    auto_ptr< RadiusAttribute > RadiusAttribute::clone() const {
        return auto_ptr<RadiusAttribute> ( new RadiusAttribute( this->type, this->value->clone() ) );
    }
}





