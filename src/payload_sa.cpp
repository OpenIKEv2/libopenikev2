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
#include "payload_sa.h"

#include "exception.h"
#include "utils.h"
#include "log.h"
#include <assert.h>

namespace openikev2 {

    Payload_SA::Payload_SA()
            : Payload( PAYLOAD_SA, false ) {}

    Payload_SA::Payload_SA( const Payload_SA & other )
            : Payload( PAYLOAD_SA, false ) {
        for ( vector<Proposal*>::const_iterator it = other.proposals->begin(); it != other.proposals->end(); it++ )
            this->addProposal( ( *it ) ->clone() );
    }

    auto_ptr<Payload_SA> Payload_SA::parse( ByteBuffer& byte_buffer ) {
        auto_ptr<Payload_SA> result ( new Payload_SA() );
        AutoVector<Proposal> proposals;

        // get a pointer to the begin of the payload
        uint8_t * payload_begin = byte_buffer.getReadPosition() - 2;

        // reads payload size
        uint16_t payload_size = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_size < 4 )
            throw ParsingException( "Payload_SA length cannot be < 4 bytes: " + intToString( payload_size ) );

        // if the payload has no proposals, then return
        if ( payload_size == 4 )
            return result;

        // indicates if the payload contains more proposals 2 = yes
        uint8_t has_more_proposals = 2;

        // indicates the expected proposal number
        uint8_t expected_proposal_number = 1;

        // Read all the proposals
        do {
            // reads has more proposals byte
            has_more_proposals = byte_buffer.readInt8();

            // skips reserved byte
            byte_buffer.skip( 1 );

            // reads proposal
            auto_ptr<Proposal> proposal = Proposal::parse( byte_buffer );

            // check the proposal number
            if ( proposal->proposal_number == expected_proposal_number ) {
                proposals->push_back( proposal.release() );
                expected_proposal_number++;
            }

            // If this is a multiprotocol proposal, then omit it
            else if ( proposal->proposal_number == expected_proposal_number - 1 ) {
                Log::writeLockedMessage( "Payload_SA", "libopenikev2 doesn't support multiprotocol proposals. Ommiting it", Log::LOG_WARN, true );

                // get the last inserted proposals and, if the type matches with the received on
                if ( proposals->size() > 0 && proposals->back()->proposal_number == proposal->proposal_number )
                    proposals->pop_back();
            }

            // If proposal number is invalid
            else
                throw ParsingException( "Unexpected proposal number Received: " + intToString( proposal->proposal_number ) + " Expected: " + intToString( expected_proposal_number ) );
        }
        while ( has_more_proposals == 2 );

        // ckecks the size
        if ( byte_buffer.getReadPosition() != payload_begin + payload_size )
            throw ParsingException( "Payload_SA has different length than indicated in the header" );

        result->proposals = proposals;
        return result;
    }

    Payload_SA::Payload_SA( auto_ptr< Proposal > first_proposal )
            : Payload( PAYLOAD_SA, false ) {
        this->addProposal( first_proposal );
    }

    Payload_SA::~Payload_SA() { }

    void Payload_SA::addProposal( auto_ptr<Proposal> proposal ) {
        assert ( proposal.get() != NULL );

        // updates the proposal number
        proposal->proposal_number = this->proposals->size() + 1;

        // Add a proposal to the payload
        this->proposals->push_back( proposal.release() );
    }

    void Payload_SA::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // pointer to the begin of the payload
        uint8_t * payload_begin = byte_buffer.getWritePosition();

        // writes dummy payload_length value
        byte_buffer.writeInt16( 0 );

        // writes all the proposals
        for ( vector<Proposal*>::const_iterator it = this->proposals->begin(); it != this->proposals->end(); it++ ) {

            // if this is the last element
            if ( it == this->proposals->end() - 1 )
                byte_buffer.writeInt8( 0 );
            else
                byte_buffer.writeInt8( 2 );

            // writes reserved
            byte_buffer.writeInt8( 0 );

            // writes proposal
            ( *it ) ->getBinaryRepresentation( byte_buffer );
        }

        // pointer to the end of the payload
        uint8_t* payload_end = byte_buffer.getWritePosition();

        // writes the real payload length value
        byte_buffer.setWritePosition( payload_begin );
        byte_buffer.writeInt16( payload_end - payload_begin + 2 );
        byte_buffer.setWritePosition( payload_end );
    }

    string Payload_SA::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_SA> {\n";

        for ( vector<Proposal*>::const_iterator it = this->proposals->begin(); it != this->proposals->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_SA::clone( ) const {
        return auto_ptr<Payload> ( new Payload_SA( *this ) );
    }
}

