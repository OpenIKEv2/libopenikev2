/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
