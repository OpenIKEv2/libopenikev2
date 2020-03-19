/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <cstdlib>

#include "bytearray.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents a byte buffer.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ByteBuffer : public ByteArray {

            /****************************** ATTRIBUTES ******************************/
        protected:
            uint8_t* real_begin_array;      /**< Internal array */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Returns the internal array capacity
             * @return The internal array capacity
             */
            virtual uint32_t capacity() const;

        public:
            /**
             * Creates a new ByteBuffer
             * @param capacity Capacity
             */
            ByteBuffer( uint32_t capacity );

            /**
             * Creates a new ByteBuffer cloning another one
             * @param other Another ByteBuffer
             */
            ByteBuffer( const ByteBuffer& other );

            /**
             * Creates a new ByteBuffer copying the data from a ByteArray
             * @param byte_array ByteArray to copy the data
             */
            ByteBuffer( const ByteArray& byte_array );

            /**
             * Appends a standard byte array at the end of this ByteBuffer
             * @param buffer Standard byte arrray
             * @param buffer_size Size of the byte array
             */
            virtual void writeBuffer( const void* buffer, uint32_t buffer_size );

            /**
             * Appends a ByteArray at the end of this ByteBuffer
             * @param byte_array Other ByteBuffer
             */
            virtual void writeByteArray( const ByteArray& byte_array );

            /**
             * Appends a 8 bit integer at the end of the ByteBuffer
             * @param value 8 bit integer
             */
            virtual void writeInt8( uint8_t value );

            /**
             * Appends a 16 bit integer at the end of the ByteBuffer, in network byte order
             * @param value 16 bit integer in host byte order
             */
            virtual void writeInt16( uint16_t value );

            /**
             * Appends a 32 bit integer at the end of the ByteBuffer, in network byte order
             * @param value 32 bit integer in host byte order
             */
            virtual void writeInt32( uint32_t value );

            /**
             * Appends a 8 bit integer repeated num_bytes times
             * @param num_bytes Number of bytes
             * @param byte 8 bit value
             */
            virtual void fillBytes( uint32_t num_bytes, uint8_t byte );

            /**
             * Reads size bytes and stores them in destination_buffer
             * @param size_to_read Size to read
             * @param destination_buffer Destination buffer
             */
            virtual void readBuffer( uint32_t size_to_read, void* destination_buffer );

            /**
             * Reads size bytes and stores them in a new ByteArray
             * @param size Size to read
             * @return New ByteArray with the read bytes
             */
            virtual auto_ptr<ByteArray> readByteArray( uint32_t size );

            /**
             * Reads a 8 bit integer from the begin of the ByteBuffer
             * @return 8 bit integer
             */
            virtual uint8_t readInt8();

            /**
             * Reads a 16 bit integer from the begin of the ByteBuffer, in network byte order
             * @return 16 bit integer in host byte order
             */
            virtual uint16_t readInt16();

            /**
             * Reads a 32 bit integer from the begin of the ByteBuffer, in network byte order
             * @return 32 bit integer in host byte order
             */
            virtual uint32_t readInt32();

            /**
             * Gets the current write position
             * @return Write position
             */
            virtual uint8_t* getWritePosition() const;

            /**
             * Sets the write position
             * @param position New write position
             */
            virtual void setWritePosition( uint8_t* position );

            /**
             * Gets the current read position
             * @return Read position
             */
            virtual uint8_t* getReadPosition() const;

            /**
             * Sets the read position
             * @param position New read position
             */
            virtual void setReadPosition( uint8_t* position );

            /**
             * Resets the pointers
             */
            virtual void reset();

            /**
             * Skips the read of the indicated bytes
             * @param bytes Number of bytes to be skipped
             */
            virtual void skip( uint32_t bytes );

            virtual ~ByteBuffer();
    };
}

#endif
