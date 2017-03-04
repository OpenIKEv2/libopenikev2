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
#ifndef CIPHER_H
#define CIPHER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#include "bytearray.h"

namespace openikev2 {

    /**
        This abstract class represets a cipher, with methods to encrypt, decrypt, and to compute integrity of byte array.
        This class holds internally the keys, so it is no needed to pass them as argument.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Cipher {

            /****************************** ATTRIBUTES ******************************/
        public:
            uint32_t encr_block_size;       /**< Encryption block size */
            uint32_t integ_hash_size;       /**< Integrity hash size */

            /****************************** METHODS ******************************/
        public:

            /**
             * Encrypts a plain text using the internal crypto algorithm and the internal key.
             * @param plain_text Plain text to be encrypted. Its size must be multiple of the block size.
             * @param initialization_vector Initialization vector. Its size must be equal to the block size
             * @return The resulting cipher text. Its size is equal to the plain text one.
             */
            virtual auto_ptr<ByteArray> encrypt( ByteArray& plain_text, ByteArray& initialization_vector ) = 0;

            /**
             * Decrypts a cipher text using the internal crypto algorithm and the internal key.
             * @param cipher_text Cipher text to be decrypted. Its size must be multiple of the block size.
             * @param initialization_vector Initialization vector. Its size must be equal to the block size
             * @return The resulting plain text. Its size is equal to the cipher text one.
             */
            virtual auto_ptr<ByteArray> decrypt( ByteArray& cipher_text, ByteArray& initialization_vector ) = 0;

            /**
             * Computes the integrity of a data buffer using the internal auth algorithm and the internal key
             * @param data_buffer Data buffer to compute its integrity
             * @return Computed integrity data
             */
            virtual auto_ptr<ByteArray> computeIntegrity( ByteArray& data_buffer ) = 0;

            /**
             * Generates a HMAC value using the internal auth algorithm, a data buffer and a key
             * @param data_buffer Data buffer to compute the HMAC value
             * @param hmac_key Key to compute the HMAC value
             * @return Computed HMAC value
             */
            virtual auto_ptr<ByteArray> hmac( ByteArray& data_buffer, ByteArray& hmac_key ) = 0;

            virtual ~Cipher();
    };
};
#endif
