/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "cryptocontrollerimpl.h"

namespace openikev2 {

    CryptoControllerImpl::~CryptoControllerImpl() {}

    auto_ptr<Proposal> CryptoControllerImpl::selectBestTransfroms( Proposal& proposal ) {
        auto_ptr<Proposal> result ( new Proposal( proposal.protocol_id ) );
        result->spi = proposal.spi->clone();

        // We assume the first ENCR transform is the best
        Transform* best_encr_transform = proposal.getFirstTransformByType( Enums::ENCR );
        if ( best_encr_transform != NULL )
            result->addTransform( best_encr_transform->clone() );

        // We assume the first INTEG transform is the best
        Transform* best_integ_transform = proposal.getFirstTransformByType( Enums::INTEG );
        if ( best_integ_transform != NULL )
            result->addTransform( best_integ_transform->clone() );

        // We assume the first PRF transform is the best
        Transform* best_prf_transform = proposal.getFirstTransformByType( Enums::PRF );
        if ( best_prf_transform != NULL )
            result->addTransform( best_prf_transform->clone() );

        // We assume the first DH transform is the best
        Transform* best_dh_transform = proposal.getFirstTransformByType( Enums::D_H );
        if ( best_dh_transform != NULL )
            result->addTransform( best_dh_transform->clone() );

        // We assume the first ESN transform is the best
        Transform* best_esn_transform = proposal.getFirstTransformByType( Enums::ESN );
        if ( best_esn_transform != NULL )
            result->addTransform( best_esn_transform->clone() );

        return result;
    }

    auto_ptr<Proposal> CryptoControllerImpl::chooseProposal( Payload_SA& received_payload_sa, Proposal& desired_proposal ) {
        // For each received proposal, select it if matches protocols with at least one of the desired proposals
        for ( vector<Proposal*>::iterator proposal_iterator = received_payload_sa.proposals->begin(); proposal_iterator != received_payload_sa.proposals->end(); proposal_iterator++ ) {
            Proposal *current_received_proposal = ( *proposal_iterator );

            // If protocols mismatch, process the next
            if ( current_received_proposal->protocol_id != desired_proposal.protocol_id )
                continue;

            // gets the intersection between received proposal and desired proposal
            auto_ptr<Proposal> common_proposal = Proposal::intersection( *current_received_proposal, desired_proposal );

            // check if the best common proposal have the same transform types than the desired proposal after the intersection process
            if ( !desired_proposal.hasTheSameTransformTypes( *common_proposal ) )
                continue;

            // get the best transforms of the common proposal
            auto_ptr<Proposal> result = this->selectBestTransfroms( *common_proposal );

            return result;
        }

        // if no received proposal is suittable, then return a NULL proposal
        return auto_ptr<Proposal> ( NULL );
    }
}



