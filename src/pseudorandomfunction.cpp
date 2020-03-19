/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "pseudorandomfunction.h"
#include "bytebuffer.h"
#include "bytearray.h"

namespace openikev2 {
    PseudoRandomFunction::~PseudoRandomFunction() {}

    auto_ptr< ByteArray > PseudoRandomFunction::prfPlus( const ByteArray & key, const ByteArray & sequence, uint32_t needed_size ) const {
        uint32_t iterations = ( needed_size / this->prf_size ) + 1;

        // The result size will be iter * prf_size
        auto_ptr<ByteBuffer> result ( new ByteBuffer( iterations * this->prf_size ) );

        // New sequence must have, at least, prf_size + sequence size + 1
        ByteBuffer new_sequence( this->prf_size + sequence.size() + 1 );

        // For the first iteration, temp must be an empty ByteArray
        auto_ptr<ByteArray> temp( new ByteArray( 0 ) );

        // Make the iteratins
        for ( uint8_t current_iter = 1; current_iter <= iterations; current_iter++ ) {
            // Resets new_sequence
            new_sequence.reset();

            // New sequence = Ti | S | iter
            new_sequence.writeByteArray( *temp );
            new_sequence.writeByteArray( sequence );
            new_sequence.writeInt8( current_iter );

            // Calculate temp = prf(K, S | 0x01) = prf(K, new_sequence)
            auto_ptr<ByteArray> new_sequence_array = new_sequence.readByteArray( new_sequence.size() );
            temp = this->prf( key, *new_sequence_array );

            // Adds temp to the result
            result->writeByteArray( *temp );
        }

        return auto_ptr<ByteArray> ( result );
    }

}

