/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "cryptocontroller.h"

namespace openikev2 {

    CryptoControllerImpl* CryptoController::implementation ( NULL );

    void CryptoController::setImplementation( CryptoControllerImpl* impl ) {
        implementation = impl;
    }

    auto_ptr<DiffieHellman> CryptoController::getDiffieHellman( Enums::DH_ID group ) {
        assert (implementation != NULL);
        return implementation->getDiffieHellman( group );
    }

    auto_ptr<Random> CryptoController::getRandom( ) {
        assert (implementation != NULL);
        return implementation->getRandom();
    }

    auto_ptr<Cipher> CryptoController::getCipher( Proposal& proposal, auto_ptr<ByteArray> encr_key, auto_ptr<ByteArray> integ_key ) {
        assert (implementation != NULL);
        return implementation->getCipher( proposal, encr_key, integ_key );
    }

    auto_ptr< PseudoRandomFunction > CryptoController::getPseudoRandomFunction( Transform& prf_transform ) {
        assert (implementation != NULL);
        return implementation->getPseudoRandomFunction( prf_transform );
    }

    auto_ptr<Payload_NOTIFY> CryptoController::generateCookie( Message& message ) {
        assert (implementation != NULL);
        return implementation->generateCookie( message );
    }

    auto_ptr<Proposal> CryptoController::chooseProposal( Payload_SA& received_payload_sa, Proposal& desired_proposal ) {
        assert (implementation != NULL);
        return implementation->chooseProposal( received_payload_sa, desired_proposal );
    }

    auto_ptr< KeyRing > CryptoController::getKeyRing( Proposal & proposal, const PseudoRandomFunction& prf ) {
        assert (implementation != NULL);
        return implementation->getKeyRing( proposal, prf );
    }
}




