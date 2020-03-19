/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "bytebuffer.h"
#include "netinet/in.h"
#include "exception.h"
#include <string.h>

#include <assert.h>

namespace openikev2 {

    ByteBuffer::ByteBuffer( uint32_t capacity ) : ByteArray( capacity ) {
        // points the real_begin_array to the begin_array
        this->real_begin_array = this->begin_array;
    }

    ByteBuffer::ByteBuffer( const ByteBuffer & other ) : ByteArray( other.capacity() ) {
        this->real_begin_array = this->begin_array;

        uint32_t other_real_size = other.end_data - other.real_begin_array;

        // copy only the valid data
        memcpy( this->real_begin_array, other.real_begin_array, other_real_size );

        // repositonate the begin_array pointer
        this->begin_array = this->real_begin_array + ( other.begin_array - other.real_begin_array );

        // repositonate the end_data pointer
        this->end_data = this->real_begin_array + other_real_size;
    }


    ByteBuffer::ByteBuffer( const ByteArray & byte_array ) : ByteArray( byte_array ) {
        this->real_begin_array = this->begin_array;
    }


    ByteBuffer::~ ByteBuffer( ) {
        delete[] this->real_begin_array;

        // to avoid double deletion
        this->begin_array = NULL;
    }

    void ByteBuffer::writeBuffer( const void * buffer, uint32_t buffer_size ) {
        // No real write when this ocurrs
        if ( buffer_size == 0 )
            return ;

        // If size is not 0, then buffer cannot be NULL
        assert( buffer != NULL );

        // If more memory is needed
        if ( this->end_data + buffer_size >= this->end_array )
            throw BufferExceededException( "ByteBuffer: Cannot write more data" );

        // Copy the buffer at the end of the array
        memcpy( this->end_data, buffer, buffer_size );

        // Increments the write position
        this->end_data += buffer_size;
    }

    void ByteBuffer::writeByteArray( const ByteArray & byte_array ) {
        this->writeBuffer( byte_array.begin_array, byte_array.size() );
    }

    void ByteBuffer::writeInt8( uint8_t value ) {
        this->writeBuffer( &value, 1 );
    }

    void ByteBuffer::writeInt16( uint16_t value ) {
        uint16_t temp = htons( value );
        this->writeBuffer( ( uint8_t* ) & temp, 2 );
    }

    void ByteBuffer::writeInt32( uint32_t value ) {
        uint32_t temp = htonl( value );
        this->writeBuffer( ( uint8_t* ) & temp, 4 );
    }

    void ByteBuffer::fillBytes( uint32_t num_bytes, uint8_t byte ) {
        // If more memory is needed
        if ( this->end_data + num_bytes >= this->end_array )
            throw BufferExceededException( "ByteBuffer: Cannot write more data" );

        memset( this->end_data, byte, num_bytes );

        this->end_data += num_bytes;
    }

    void ByteBuffer::readBuffer( uint32_t size_to_read, void * buffer ) {
        assert ( buffer != NULL );

        if ( this->begin_array + size_to_read > this->end_data )
            throw BufferExceededException( "ByteBuffer: Cannot read more data" );

        memcpy( buffer, this->begin_array, size_to_read );

        this->begin_array += size_to_read;
    }

    auto_ptr< ByteArray > ByteBuffer::readByteArray( uint32_t size ) {
        auto_ptr<ByteArray> result ( new ByteArray( size ) );
        this->readBuffer( size, result->begin_array );

        result->end_data += size;
        return result;
    }

    uint8_t ByteBuffer::readInt8( ) {
        uint8_t temp;
        this->readBuffer( 1, ( uint8_t* ) & temp );
        return temp;
    }

    uint16_t ByteBuffer::readInt16( ) {
        uint16_t temp;
        this->readBuffer( 2, ( uint8_t* ) & temp );
        return ntohs( temp );
    }

    uint32_t ByteBuffer::readInt32( ) {
        uint32_t temp;
        this->readBuffer( 4, ( uint8_t* ) & temp );
        return ntohl( temp );
    }

    uint8_t* ByteBuffer::getWritePosition( ) const {
        return this->end_data;
    }

    uint8_t* ByteBuffer::getReadPosition( ) const {
        return this->begin_array;
    }

    void ByteBuffer::setWritePosition( uint8_t* position ) {
        assert( position < this->end_array );
        assert( position >= this->begin_array );

        this->end_data = position;
    }

    void ByteBuffer::setReadPosition( uint8_t* position ) {
        assert( position < this->end_data );

        this->begin_array = position;
    }

    void ByteBuffer::reset() {
        this->begin_array = this->real_begin_array;
        this->end_data = this->real_begin_array;
    }

    uint32_t ByteBuffer::capacity( ) const {
        return this->end_array - this->real_begin_array;
    }

    void ByteBuffer::skip( uint32_t bytes ) {
        if ( this->begin_array + bytes > this->end_data )
            throw BufferExceededException( "ByteBuffer: Cannot read more data" );

        this->begin_array += bytes;
    }
}











