/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#ifndef DIFFIEHELLMAN_H
#define DIFFIEHELLMAN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bytearray.h"
#include "enums.h"

namespace openikev2 {

    /**
        This abstract class represents a Diffie-Hellman exchange.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class DiffieHellman {

            /****************************** ATTRIBUTES ******************************/
        public:
            Enums::DH_ID group_id;          /**< Diffie-Hellman group id */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Creates a new DiffieHellman object indicating its group id
             * @param group_id Group id
             */
            DiffieHellman(Enums::DH_ID group_id);
            
        public:
            /**
             * Gets the public key
             * @return The public key
             */
            virtual ByteArray& getPublicKey() const = 0;

            /**
             * Generates the DiffieHellman shared secret
             * @param peer_public_key The public key of the peer.
             */
            virtual void generateSharedSecret( const ByteArray& peer_public_key ) = 0;

            /**
             * Gets the shared secret.
             * It is needed to call generateSharedSecret() before call this method
             * @return The shared secret
             */
            virtual ByteArray& getSharedSecret() const = 0;

            virtual ~DiffieHellman();

    };
}
#endif
