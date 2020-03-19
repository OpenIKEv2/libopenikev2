/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef CRYPTOCONTROLLER_H
#define CRYPTOCONTROLLER_H

#include "cryptocontrollerimpl.h"
// #include "message.h"
#include "ipaddress.h"

namespace openikev2 {

    /**
        This class allows to perform cryptographical operations using several concrete implementations
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class CryptoController {

            /****************************** ATTRIBUTES ******************************/
        protected:
            static CryptoControllerImpl* implementation;        /**< Implementation of the CryptoController */

            /****************************** METHODS ******************************/
        public:

            /**
             * Sets the CryptoController implementation. The old one will be deleted.
             * @param impl CryptoController implementation
             */
            static void setImplementation( CryptoControllerImpl* impl );

            /**
             * Creates a new DiffieHellman object using the current implementation
             * @param group DiffieHellman group
             * @return A new DiffieHellman object
             */
            static auto_ptr<DiffieHellman> getDiffieHellman( Enums::DH_ID group );

            /**
             * Creates a new Cipher object
             * @param proposal Proposal containing the ENCR and INTEG transforms
             * @param encr_key Encryption key
             * @param integ_key Integrity key
             * @return A new Cipher
             */
            static auto_ptr<Cipher> getCipher( Proposal& proposal, auto_ptr<ByteArray> encr_key, auto_ptr<ByteArray> integ_key );


            /**
             * Creates a new Random object using the current implementation
             * @return A new Random object
             */
            static auto_ptr<Random> getRandom();

            /**
             * Creates a new PseudoRandomFunction object
             * @param prf_transform PRF trandform
             * @return A new PseudoRandomFunction object
             */
            static auto_ptr<PseudoRandomFunction> getPseudoRandomFunction( Transform& prf_transform );

            /**
             * Creates a new KeyRing object
             * @param proposal Proposal containing all the transforms
             * @param prf PRF to be used
             * @return A new KeyRing object
             */
            static auto_ptr<KeyRing> getKeyRing( Proposal& proposal, const PseudoRandomFunction& prf );

            /**
            * Creates a new cookie using the current implementation
            * @param message Full IKE_SA_INIT request message
            * @return A new COOKIE notify payload containing the new generated cookie
            */
            static auto_ptr<Payload_NOTIFY> generateCookie( Message& message );

            /**
             * Creates a new Proposal containing the matching selection between a received Payload_SA and a desired Proposal
             * @param received_payload_sa Received Payload_SA
             * @param desired_proposal Our desired Proposal
             * @return A new negociated Proposal
             */
            static auto_ptr<Proposal> chooseProposal( Payload_SA& received_payload_sa, Proposal& desired_proposal );
    };
}

#endif
