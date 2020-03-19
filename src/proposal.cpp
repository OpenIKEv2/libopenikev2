/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "proposal.h"
#include "utils.h"
#include "exception.h"
#include <string.h>

namespace openikev2 {

    Proposal::Proposal( Enums::PROTOCOL_ID protocol_id ) {
        // assigns the proposal number
        this->proposal_number = 0;

        // Assigns protocol id
        this->protocol_id = protocol_id;

        // The SPI value is 0-length until it was assigned using setIkeSpi() or setIpsecSpi()
        if ( protocol_id == Enums::PROTO_IKE ) {
            this->spi.reset ( new ByteArray( 8 ) );
        }
        else if ( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP ) {
            this->spi.reset ( new ByteArray( 4 ) );
        }
        else
            assert( 0 );
    }

    Proposal::Proposal( Enums::PROTOCOL_ID protocol_id, auto_ptr<ByteArray> spi ) {
        // Checks if spi size agrees with protocol id
        if ( protocol_id == Enums::PROTO_IKE && spi->size() != 8 && spi->size() != 0 )
            throw ParsingException( "Proposal and SPI size don't match." );
        else if ( ( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP ) && spi->size() != 4 )
            throw ParsingException( "Proposal and SPI size don't match." );

        this->protocol_id = protocol_id;
        this->spi = spi;
        this->proposal_number = 0;
    }

    auto_ptr<Proposal> Proposal::parse( ByteBuffer& byte_buffer ) {
        // Pointer to the begin of the Proposal
        uint8_t * proposal_begin = byte_buffer.getReadPosition() - 2;

        // reads the proposal length
        uint32_t proposal_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( proposal_length < 8 )
            throw ParsingException( "Proposal length cannot be < 8 bytes. len=[" + intToString( proposal_length ) + "]" );

        // reads the proposal number
        uint8_t proposal_number = byte_buffer.readInt8();
        if ( proposal_number == 0 )
            throw ParsingException( "Proposal number cannot be 0" );


        // Reads protocol id from buffer
        Enums::PROTOCOL_ID protocol_id = ( Enums::PROTOCOL_ID ) byte_buffer.readInt8();

        // Reads spi size from buffer
        uint8_t spi_size = byte_buffer.readInt8();

        // Reads transform count
        uint8_t transform_count = byte_buffer.readInt8();

        // Reads spi value and stores it in the internal variable
        auto_ptr<ByteArray> spi = byte_buffer.readByteArray( spi_size );

        // Creates the Proposal
        auto_ptr<Proposal> result ( new Proposal( protocol_id, spi ) );
        result->proposal_number = proposal_number;

        // Reads transforms from buffer
        for ( int i = 0; i < transform_count; i++ ) {
            // reads has more transforms byte
            uint8_t has_more_transforms = byte_buffer.readInt8();

            // Checks if this is the last transform
            if ( i == transform_count - 1 && has_more_transforms )
                throw ParsingException( "Transform count field is lower than real transform number." );
            if ( i < transform_count - 1 && !has_more_transforms )
                throw ParsingException( "Transform count field is higher than real transform number." );

            // reads RESERVED byte
            byte_buffer.readInt8();

            // Reads Transform and adds to the collection (without fixed header of size 4)
            auto_ptr<Transform> transform = Transform::parse( byte_buffer );

            // adds this transform the the transform collection
            result->addTransform( transform );
        }

        // checks the size
        if ( byte_buffer.getReadPosition() != proposal_begin + proposal_length )
            throw ParsingException( "Proposal has different length than indicated in the header" );

        return result;
    }

    Proposal::~Proposal() {}

    auto_ptr<Proposal> Proposal::clone() const {
        auto_ptr<Proposal> result ( new Proposal( this->protocol_id ) );

        result->spi = this->spi->clone();

        for ( vector<Transform*>::const_iterator it = this->transforms->begin(); it != this->transforms->end(); it++ )
            result->addTransform( ( *it ) ->clone() );

        return result;
    }

    void Proposal::addTransform( auto_ptr<Transform> transform ) {
        assert ( transform.get() != NULL );

        switch ( this->protocol_id ) {
            case Enums::PROTO_AH:
                if ( transform->type == Enums::ENCR )
                    throw ProposalException( "AH protocol has ENCR transform" );
                if ( transform->type == Enums::PRF )
                    throw ProposalException( "AH protocol has PRF transform" );
                break;
            case Enums::PROTO_ESP:
                if ( transform->type == Enums::PRF )
                    throw ProposalException( "ESP protocol has PRF transform" );
                break;
            case Enums::PROTO_IKE:
                if ( transform->type == Enums::ESN )
                    throw ProposalException( "IKE protocol has ESN transform" );
                break;

        }

        this->transforms->push_back( transform.release() );
    }

    void Proposal::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // ASSERT: transform count must fits in uint8_t (octet)
        assert( this->transforms->size() > 0 && this->transforms->size() < 256 );

        // pointer to the beginning of the proposal
        uint8_t* proposal_begin = byte_buffer.getWritePosition();

        // writes a dummy value for the proposal length
        byte_buffer.writeInt16( 0 );

        // writes the proposal number
        byte_buffer.writeInt8( this->proposal_number );

        // Writes protocol id
        byte_buffer.writeInt8( this->protocol_id );

        // Writes spi size
        byte_buffer.writeInt8( this->spi->size() );

        // Writes transform count
        byte_buffer.writeInt8( this->transforms->size() );

        // Writes spi value
        byte_buffer.writeByteArray( *this->spi );

        // Writes each transform in the collection
        for ( vector<Transform*>::const_iterator it = this->transforms->begin(); it != this->transforms->end(); it++ ) {
            // Writes last transform flags (0=last, 3=more)
            if ( *it == this->transforms->back() )
                byte_buffer.writeInt8( 0 );
            else
                byte_buffer.writeInt8( 3 );

            // Fills RESERVED field with 0s
            byte_buffer.writeInt8( 0 );

            // Writes transform
            ( *it ) ->getBinaryRepresentation( byte_buffer );
        }

        // pointer to the end of the proposal
        uint8_t* proposal_end = byte_buffer.getWritePosition();

        // writes the real proposal length value
        byte_buffer.setWritePosition( proposal_begin );
        byte_buffer.writeInt16( proposal_end - proposal_begin + 2 );
        byte_buffer.setWritePosition( proposal_end );
    }


    Transform * Proposal::getFirstTransformByType( Enums::TRANSFORM_TYPE type ) const {
        for ( vector<Transform*>::const_iterator it = this->transforms->begin(); it != this->transforms->end(); it++ )
            if ( ( *it ) ->type == type )
                return ( *it );

        return NULL;
    }


    string Proposal::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PROPOSAL> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "proposal_number=" << ( int ) proposal_number << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "protocol_id=" << Enums::PROTOCOL_ID_STR( this->protocol_id ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "spi=" << this->spi->toStringTab( tabs + 1 ) + "\n";

        for ( vector<Transform*>::const_iterator it = this->transforms->begin(); it != this->transforms->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    void Proposal::deleteTransformsByType( Enums::TRANSFORM_TYPE type ) {
        vector<Transform*>::iterator it = this->transforms->begin();
        while ( it != this->transforms->end() ) {
            if ( ( *it ) ->type == type ) {
                delete ( *it );
                it = this->transforms->erase( it );
            }
            else {
                it++;
            }
        }
    }

    bool Proposal::hasTransform( const Transform & transform ) const {
        for ( vector<Transform*>::const_iterator it = this->transforms->begin(); it != this->transforms->end(); it++ ) {
            Transform* current_transform = ( *it );
            if ( *current_transform == transform )
                return true;
        }

        return false;
    }

    auto_ptr< Proposal > Proposal::intersection( const Proposal & proposal1, const Proposal & proposal2 ) {
        assert ( proposal1.protocol_id == proposal2.protocol_id );

        auto_ptr<Proposal> result ( new Proposal( proposal1.protocol_id ) );

        result->spi = proposal1.spi->clone();

        // for all the transform in the proposal1
        for ( vector<Transform*>::const_iterator it = proposal1.transforms->begin(); it != proposal1.transforms->end(); it++ ) {
            Transform* current_transform = ( *it );

            // if the proposal2 has the same transform type and id, then adds it to the result
            if ( proposal2.hasTransform( *current_transform ) )
                result->addTransform( current_transform->clone() );
        }

        return result;
    }


    bool Proposal::hasTheSameTransformTypes( const Proposal & other ) const {
        for ( vector<Transform*>::const_iterator it = this->transforms->begin(); it != this->transforms->end(); it++ ) {
            Transform* current_transform = ( *it );

            if ( other.getFirstTransformByType( current_transform->type ) == NULL )
                return false;
        }

        return true;
    }

    void Proposal::setIkeSpi( uint64_t spi ) {
        assert ( protocol_id == Enums::PROTO_IKE );
        this->spi.reset ( new ByteArray( &spi, 8 ) );
    }

    void Proposal::setIpsecSpi( uint32_t spi ) {
        assert( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP );
        auto_ptr<ByteBuffer> spi_value ( new ByteBuffer( 4 ) );
        spi_value->writeInt32( spi );
        this->spi = spi_value;
    }

    uint64_t Proposal::getIkeSpi( ) {
        assert ( protocol_id == Enums::PROTO_IKE );
        assert ( this->spi->size() == 8 );
        uint64_t spi = 0;
        memcpy( &spi, this->spi->getRawPointer(), 8 );
        return spi;
    }

    uint32_t Proposal::getIpsecSpi( ) {
        assert( protocol_id == Enums::PROTO_AH || protocol_id == Enums::PROTO_ESP );
        assert ( this->spi->size() == 4 );
        uint32_t spi = 0;
        ByteBuffer spi_value ( *this->spi );
        return spi_value.readInt32();
    }
}













