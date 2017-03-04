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
#ifndef CRYPTOCONTROLLERIMPL_H
#define CRYPTOCONTROLLERIMPL_H

#include "diffiehellman.h"
#include "cipher.h"
#include "random.h"
#include "message.h"
#include "ipaddress.h"
#include "keyring.h"
#include "pseudorandomfunction.h"
#include "payload_sa.h"

namespace openikev2 {

    /**
        This abstract class represents a CryptoController concrete implementation.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class CryptoControllerImpl {

            /****************************** METHODS ******************************/
        protected:
            /**
             * Generates a Proposal with the best of the contained Transforms for each type
             * @param proposal Proposal
             * @return A proposal containing the best Transforms for each type
             */
            virtual auto_ptr<Proposal> selectBestTransfroms( Proposal& proposal );

        public:
            /**
             * Creates a new DiffieHellman object
             * @param group DiffieHellman group
             * @return A new DiffieHellman object
             */
            virtual auto_ptr<DiffieHellman> getDiffieHellman( Enums::DH_ID group ) = 0;

            /**
             * Creates a new Cipher object
             * @param proposal Proposal containing the ENCR and INTEG transforms
             * @param encr_key Encryption key
             * @param integ_key Integrity key
             * @return A new Cipher
             */
            virtual auto_ptr<Cipher> getCipher( Proposal& proposal, auto_ptr<ByteArray> encr_key, auto_ptr<ByteArray> integ_key ) = 0;

            /**
             * Creates a new Random object
             * @return A new Random object
             */
            virtual auto_ptr<Random> getRandom() = 0;

            /**
             * Creates a new PseudoRandomFunction object
             * @param prf_transform PRF tranform
             * @return A new PseudoRandomFunction object
             */
            virtual auto_ptr<PseudoRandomFunction> getPseudoRandomFunction( Transform& prf_transform ) = 0;

            /**
             * Creates a new KeyRing object
             * @param proposal Proposal containing all the transforms
             * @param prf PRF to be used
             * @return A new KeyRing object
             */
            virtual auto_ptr<KeyRing> getKeyRing( Proposal& proposal, const PseudoRandomFunction& prf ) = 0;

            /**
             * Creates a new cookie
             * @param message Full IKE_SA_INIT request message
             * @return A new COOKIE notify payload containing the new generated cookie
             */
            virtual auto_ptr<Payload_NOTIFY> generateCookie( Message& message ) = 0;

            /**
             * Creates a new Proposal containing the matching selection between a received Payload_SA and a desired Proposal
             * @param received_payload_sa Received Payload_SA
             * @param desired_proposal Our desired Proposal
             * @return A new negociated Proposal
             */
            virtual auto_ptr<Proposal> chooseProposal( Payload_SA& received_payload_sa, Proposal& desired_proposal );

            virtual ~CryptoControllerImpl();

    };
};
#endif
