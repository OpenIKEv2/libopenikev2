/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
 *   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
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
#ifndef OPENIKEV2PSEUDORANDOMFUNCTION_H
#define OPENIKEV2PSEUDORANDOMFUNCTION_H

#include <stdint.h>

#include "enums.h"
#include "bytearray.h"

namespace openikev2 {

    /**
        This class represents a Pseudo-Random Function (PRF)
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
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
