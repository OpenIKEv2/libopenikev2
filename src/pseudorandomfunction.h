/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2PSEUDORANDOMFUNCTION_H
#define OPENIKEV2PSEUDORANDOMFUNCTION_H

#include <stdint.h>

#include "enums.h"
#include "bytearray.h"

namespace openikev2 {

    /**
        This class represents a Pseudo-Random Function (PRF)
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class PseudoRandomFunction {

            /****************************** ATTRIBUTES ******************************/
        public:
            uint32_t prf_size;              /**< PRF result value size */

            /****************************** ATTRIBUTES ******************************/
        public:
            /**
             * Computes the PRF value, using the indicated parameters
             * @param key Key
             * @param data Data to compute the PRF value
             * @return PRF value
             */
            virtual auto_ptr<ByteArray> prf( const ByteArray& key, const ByteArray& data ) const = 0;

            /**
             * Computes the PRF+ value, using the indicated parameters
             * @param key Key
             * @param sequence Sequence (S)
             * @param needed_size Needed key material size
             * @return Key material
             */
            virtual auto_ptr<ByteArray> prfPlus( const ByteArray& key, const ByteArray& sequence, uint32_t needed_size ) const;

            virtual ~PseudoRandomFunction();
    };
}

#endif
