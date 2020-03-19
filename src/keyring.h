/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef OPENIKEV2KEYRING_H
#define OPENIKEV2KEYRING_H

#include "cipher.h"
#include "pseudorandomfunction.h"
#include "bytearray.h"
#include "printable.h"
#include "proposal.h"

namespace openikev2 {

    /**
        This class represents a KeyRing
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class KeyRing: public Printable {

            /****************************** ATTRIBUTES ******************************/
        protected:
            PseudoRandomFunction* prf;              /**< Pseudo random function used for key derivation */
            uint32_t encr_key_size;                 /**< Encryption key size */
            uint32_t integ_key_size;                /**< Integrity key size */
        public:
            auto_ptr<ByteArray> sk_ai;              /**< Key used for integrity operations in the initiator */
            auto_ptr<ByteArray> sk_ar;              /**< Key used for integrity operations in the responder */
            auto_ptr<ByteArray> sk_ei;              /**< Key used for encryption operations in the initiator */
            auto_ptr<ByteArray> sk_er;              /**< Key used for encryption operations in the responder */

            auto_ptr<ByteArray> sk_pi;              /**< Key used for Payload_AUTH generation in the initiator (IKE_SA only) */
            auto_ptr<ByteArray> sk_pr;              /**< Key used for Payload_AUTH generation in the responder (IKE_SA only) */
            auto_ptr<ByteArray> sk_d;               /**< Key used for deriving new keys for the CHILD_SAs (IKE_SA only) */
            auto_ptr<ByteArray> skeyseed;           /**< Skeyseed (IKE_SA only) */

            /****************************** METHODS ******************************/
        public:
            /**
             * Generate the IKE_SA keys
             * @param nonce_i Initiator nonce
             * @param nonce_r Responder nonce
             * @param spi_i Initiator SPI
             * @param spi_r Responder SPI
             * @param shared_secret Shared secret
             * @param old_sk_d Old SK_d, for rekeying (use a NULL if not needed)
             */
            virtual void generateIkeSaKeys( ByteArray& nonce_i, ByteArray& nonce_r, uint64_t spi_i, uint64_t spi_r, ByteArray& shared_secret, ByteArray* old_sk_d );

            /**
             * Generate the CHILD_SA keys
             * @param nonce_i Initiator nonce
             * @param nonce_r Initiator nonce
             * @param sk_d IKE_SA SK_d key
             * @param shared_secret Shared secret (use NULL if not needed)
             */
            virtual void generateChildSaKeys( ByteArray& nonce_i, ByteArray& nonce_r, ByteArray& sk_d, ByteArray* shared_secret );

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~KeyRing();

    };

}

#endif
