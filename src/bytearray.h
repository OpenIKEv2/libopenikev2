/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/

#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include <memory>


namespace openikev2 {

    /**
        This class represents a ByteArray. It takes care with memory
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ByteArray : public Printable {
            friend class ByteBuffer;

            /****************************** ATTRIBUTES ******************************/
        protected:
            uint8_t* begin_array;       /**< Internal pointer to the begin of the array */
            uint8_t* end_data;          /**< Internal pointer to the end of valid data */
            uint8_t* end_array;          /**< Internal pointer to the end of the array */


            /****************************** METHODS ******************************/
        protected:
            /**
             * Returns the internal array capacity
             * @return The internal array capacity
             */
            virtual uint32_t capacity() const;

        public:
            /**
             * Creates a new ByteArray with the indicated capacity.
             * @param capacity Capacity of the array.
             */
            ByteArray( uint32_t capacity );

            /**
             * Creates a new ByteArray with the indicated capacity.
             * @param capacity Capacity of the array.
             * @param pattern Patter to fill the array
             */
            ByteArray( uint32_t capacity, uint8_t pattern );

            /**
             * Creates a new ByteArray using an existing raw byte array
             * @param array Pointer to the raw array
             * @param size Size of the actual data in the array
             * @param capacity Capacity of the array. If set to 0, the use size + 1
             * @param owns_memory Indicates if the ByteArray must own the memory of "array". Defaults to FALSE (make copy)
             */
            ByteArray( const void * array, uint32_t size, uint32_t capacity = 0, bool owns_memory = false );

            /**
             * Creates a new ByteArray cloning another one
             * @param other The other ByteArray
             */
            ByteArray( const ByteArray& other );

            /**
             * Gets the raw pointer to the array
             * @return The raw pointer
             */
            virtual uint8_t* getRawPointer() const;

            /**
            * Compares if this ByteArray object is equal to other
            * @param other The other ByteArray
            * @return TRUE if both are equals, FALSE otherwise
            */
            virtual bool operator==( const ByteArray &other ) const;

            /**
             * Compares if this ByteArray object is lesser than other (lexicographically)
             * @param other The other ByteArray
             * @return TRUE if this ByteArray is lesser than the other. FALSE otherwise
             */
            virtual bool operator<( const ByteArray &other ) const;

            /**
             * Compares if this ByteArray object is greater than other  (lexicographically)
             * @param other The other ByteArray
             * @return TRUE if this ByteArray is greater than the other. FALSE otherwise
             */
            virtual bool operator>( const ByteArray &other ) const;

            /**
             * Gets the i-nth element in the array
             * @param position Array position
             * @return Element in the array
             */
            virtual uint8_t& operator[] ( const uint32_t position ) const;

            /**
             * Gets a clone of this object
             * @return A new cloned ByteArray
             */
            virtual auto_ptr<ByteArray> clone() const;

            /**
             * Returns the size of the valid data
             * @return The size of the valid data
             */
            virtual uint32_t size() const;

            /**
             * Sets the size of the array
             * @param size The new size
             */
            virtual void setSize( uint32_t size );

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~ByteArray();

    };

}

#endif
