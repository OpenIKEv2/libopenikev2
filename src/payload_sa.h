/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PAYLOAD_SA_H
#define PAYLOAD_SA_H

#include "payload.h"
#include "proposal.h"

#include "autovector.h"

namespace openikev2 {

    /**
        This class represents a SA Payload.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Payload_SA : public Payload {
            /****************************** ATTRIBUTES ******************************/
        public:
            AutoVector <Proposal> proposals;    /**< Proposals collections */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Renumbers the proposals
             */
            void renumberProposals();

        public:
            /**
             * Creates a new empty Payload_SA.
             */
            Payload_SA();

            /**
             * Creates a new Payload_SA inserting the fisrt proposal
             * @param first_proposal First proposal to be inserted in the Payload_SA
             */
            Payload_SA( auto_ptr<Proposal> first_proposal );

            /**
             * Creates a new Payload_SA cloning another one
             * @param other Other Payload_SA to be cloned
             */
            Payload_SA( const Payload_SA& other );

            /**
             * Creates a new Payload_SA based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_SA> parse( ByteBuffer& byte_buffer );

            /**
             * Adds a proposal to the Payload SA.
             * @param proposal Proposal to be added.
             */
            void addProposal( auto_ptr<Proposal> proposal );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_SA();
    };
}
#endif
