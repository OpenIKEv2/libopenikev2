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
#ifndef PAYLOAD_SA_H
#define PAYLOAD_SA_H

#include "payload.h"
#include "proposal.h"

#include "autovector.h"

namespace openikev2 {

    /**
        This class represents a SA Payload.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj.fernandez@dif.um.es, alejandro_perez@dif.um.es>
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
