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
#include "bytearray.h"
#include <assert.h>
#include <string.h>


namespace openikev2 {

    ByteArray::ByteArray( uint32_t capacity ) {
        // Forbids creation of 0 bytes arrays
        if ( capacity == 0 )
            capacity = 1;

        this->begin_array = new uint8_t [ capacity ];
        this->end_data = this->begin_array;
        this->end_array = this->begin_array + capacity + 1;
    }

    ByteArray::ByteArray( uint32_t capacity, uint8_t pattern ) {
        // Forbids creation of 0 bytes arrays
        if ( capacity == 0 )
            capacity = 1;

        this->begin_array = new uint8_t [ capacity ];
        this->end_data = this->begin_array;
        this->end_array = this->begin_array + capacity + 1;

        memset( this->begin_array, pattern, capacity );
    }

    ByteArray::ByteArray( const void * array, uint32_t size, uint32_t capacity, bool owns_memory ) {
        assert ( array != NULL );

        if ( capacity == 0)
            capacity = size + 1;

        if ( owns_memory ) {
            this->begin_array = ( uint8_t* ) array;
        }
        else {
            this->begin_array = new uint8_t [ capacity ];
            // copy only the valid data
            memcpy( this->begin_array, array, size );
        }

        this->end_data = this->begin_array + size;
        this->end_array = this->begin_array + capacity + 1;
    }


    ByteArray::ByteArray( const ByteArray & other ) {
        this->begin_array = new uint8_t [ other.capacity() ];

        // copy only the valid data
        memcpy( this->begin_array, other.begin_array, other.size() );

        this->end_data = this->begin_array + other.size();
        this->end_array = this->begin_array + other.capacity();
    }

    uint8_t * ByteArray::getRawPointer( ) const {
        return this->begin_array;
    }

    bool ByteArray::operator==( const ByteArray &other ) const {
        if ( this->size() != other.size() )
            return false;

        if ( memcmp( this->begin_array, other.begin_array, this->size() ) != 0 )
            return false;

        return true;
    }

    bool ByteArray::operator<( const ByteArray &other ) const {
        // get the minimal size of both
        uint32_t minimal_size = ( this->size() < other.size() ) ? this->size() : other.size();

        // if "this" has the first minimal_size bytes lesser than "other", then return TRUE
        if ( memcmp( this->begin_array, other.begin_array, minimal_size ) < 0 )
            return true;

        // if "this" has the first minimal_size bytes greater than "other", then return FALSE
        else if ( memcmp( this->begin_array, other.begin_array, minimal_size ) > 0 )
            return false;

        // if both have the first minimal_size bytes equals, then return if "this" is shorter than "other"
        else
            return ( this->size() < other.size() );
    }

    bool ByteArray::operator>( const ByteArray &other ) const {
        // get the minimal size of both
        uint32_t minimal_size = ( this->size() < other.size() ) ? this->size() : other.size();

        // if "this" has the first minimal_size bytes lesser than "other", then return FALSE
        if ( memcmp( this->begin_array, other.begin_array, minimal_size ) < 0 )
            return false;

        // if "this" has the first minimal_size bytes greater than "other", then return TRUE
        else if ( memcmp( this->begin_array, other.begin_array, minimal_size ) > 0 )
            return true;

        // if both have the first minimal_size bytes equals, then return if "this" is shorter than "other"
        else
            return ( this->size() > other.size() );
    }

    uint8_t & ByteArray::operator [ ] ( const uint32_t position ) const {
        assert( this->begin_array + position < this->end_array );
        return this->begin_array[ position ];
    }

    auto_ptr< ByteArray > ByteArray::clone( ) const {
        auto_ptr<ByteArray> result ( new ByteArray ( *this ) );
        return result;
    }


    ByteArray::~ByteArray() {
        delete[] this->begin_array;
    }

    string ByteArray::toStringTab( uint8_t tabs ) const {
        return Printable::toHexString( this->begin_array, this->size(), 32, tabs );
    }

    uint32_t ByteArray::size( ) const {
        return this->end_data - this->begin_array;
    }

    void ByteArray::setSize( uint32_t size ) {
        assert( this->begin_array + size < this->end_array );

        this->end_data = this->begin_array + size;
    }

    uint32_t ByteArray::capacity( ) const {
        return this->end_array - this->begin_array;
    }


}






