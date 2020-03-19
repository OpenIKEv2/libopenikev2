/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/

#include "message.h"
#include "payloadfactory.h"
#include "exception.h"
#include "log.h"
#include <string.h>

namespace openikev2 {

    Message::Message( auto_ptr<SocketAddress> src_addr, auto_ptr<SocketAddress> dst_addr, uint64_t spi_i, uint64_t spi_r, uint8_t major_version, uint8_t minor_version, EXCHANGE_TYPE exchange_type, Message::MESSAGE_TYPE message_type, bool is_initiator, bool can_use_higher_major_version, uint32_t message_id ) {
        this->src_addr = src_addr;
        this->dst_addr = dst_addr;
        this->spi_i = spi_i;
        this->spi_r = spi_r;
        this->major_version = major_version;
        this->minor_version = minor_version;
        this->exchange_type = exchange_type;
        this->is_initiator = is_initiator;
        this->message_type = message_type;
        this->can_use_higher_major_version = can_use_higher_major_version;
        this->message_id = message_id;

        this->first_payload_type = Payload::PAYLOAD_NONE;
        this->first_payload_type_sk = Payload::PAYLOAD_NONE;
    }

    Message::Message( auto_ptr<SocketAddress> src_addr, auto_ptr<SocketAddress> dst_addr, ByteBuffer& byte_buffer ) {
        this->src_addr = src_addr;
        this->dst_addr = dst_addr;
        this->first_payload_type_sk = Payload::PAYLOAD_NONE;

        // get a copy of the message binary representation
        this->binary_representation = byte_buffer.clone();

        // Size must be at least size of fixed header
        if ( byte_buffer.size() < 28 )
            throw ParsingException( "Buffer seems to be too small to contain a Message: " + intToString( byte_buffer.size() ) );

        // Reads Initiator SPI
        byte_buffer.readBuffer( 8, &this->spi_i );

        // Reads Responder SPI
        byte_buffer.readBuffer( 8, &this->spi_r );

        // reads the first payload type
        this->first_payload_type = ( Payload::PAYLOAD_TYPE ) byte_buffer.readInt8();

        // Reads version numbers
        uint8_t version = byte_buffer.readInt8();
        this->major_version = ( version & 0xF0 ) >> 4;
        this->minor_version = ( version & 0x0F );

        // Reads exchange type
        this->exchange_type = ( EXCHANGE_TYPE ) byte_buffer.readInt8();

        // Reads Flags
        uint8_t flags = byte_buffer.readInt8();
        this->is_initiator = ( flags & 0x08 );
        this->message_type = ( MESSAGE_TYPE ) ( ( flags & 0x20 ) >> 5 );
        this->can_use_higher_major_version = ( flags & 0x10 );

        // Reads message ID
        this->message_id = byte_buffer.readInt32();

        // Reads message size
        uint32_t message_length = byte_buffer.readInt32();

        // Checks message length
        if ( byte_buffer.size() + 28 != message_length )
            throw ParsingException( "Indicated message length and real message length don't match. real_length=" + intToString( byte_buffer.size() + 28 ) + " indicated_length=" + intToString( message_length ) );

        // generate unencrypted payloads
        Payload::PAYLOAD_TYPE last_next_payload_type = Message::generatePayloads( this->first_payload_type, byte_buffer, this->unencrypted_payloads.get() );

        // get the payload_sk (if exists)
        Payload* payload_sk = this->getFirstPayloadByType( Payload::PAYLOAD_SK );

        // If there is a Payload_SK in the Message
        if ( payload_sk != NULL ) {
            // check if the payload_sk is in the last position
            if ( payload_sk != this->unencrypted_payloads->back() )
                throw ParsingException( "Invalid payload ordering in the Message: Payload_SK must be the last payload" );

            // store it in the special attribute and pop it from the collection
            this->payload_sk.reset( ( Payload_SK* ) payload_sk );
            this->unencrypted_payloads->pop_back();

            // stores the first payload type of the payload_sk
            this->first_payload_type_sk = last_next_payload_type;
        }

        // If there aren't any payload_sk, then the last next_payload_type must be 0
        else if ( last_next_payload_type != Payload::PAYLOAD_NONE ) {
            throw ParsingException( "The last next_payload_type must be 0 when no Payload_SK is present" );
        }
    }

    Message::Message( const Message & other ) {
        this->can_use_higher_major_version = other.can_use_higher_major_version;
        this->major_version = other.major_version;
        this->minor_version = other.minor_version;
        this->is_initiator = other.is_initiator;
        this->message_type = other.message_type;
        this->exchange_type = other.exchange_type;
        this->message_id = other.message_id;
        this->spi_i = other.spi_i;
        this->spi_r = other.spi_r;
        this->first_payload_type = other.first_payload_type;
        this->first_payload_type_sk = other.first_payload_type_sk;
        this->src_addr = other.src_addr->clone();
        this->dst_addr = other.dst_addr->clone();

        if ( other.payload_sk.get() )
            this->payload_sk.reset ( new Payload_SK( *other.payload_sk ) );

        for ( vector<Payload*>::const_iterator it = other.unencrypted_payloads->begin(); it != other.unencrypted_payloads->end(); it++ )
            this->addPayload( ( *it ) ->clone(), false );

        for ( vector<Payload*>::const_iterator it = other.encrypted_payloads->begin(); it != other.encrypted_payloads->end(); it++ )
            this->addPayload( ( *it ) ->clone(), true );

        // this must be performed in the last position, since other operations may invalidate the "cache"
        if ( other.binary_representation.get() )
            this->binary_representation = other.binary_representation->clone();
    }

    Message::~Message() { }

    Payload::PAYLOAD_TYPE Message::generatePayloads( Payload::PAYLOAD_TYPE first_payload_type, ByteBuffer& byte_buffer, vector<Payload*> &payloads ) {
        Payload::PAYLOAD_TYPE current_payload_type = first_payload_type;
        Payload::PAYLOAD_TYPE next_payload_type = Payload::PAYLOAD_NONE;

        // while data available
        while ( byte_buffer.size() > 0 && current_payload_type != Payload::PAYLOAD_NONE ) {
            // reads next payload type
            next_payload_type = ( Payload::PAYLOAD_TYPE ) byte_buffer.readInt8();

            // reads critical bit
            uint8_t critical = byte_buffer.readInt8() >> 7;

            auto_ptr<Payload> payload = PayloadFactory::createPayload( current_payload_type, byte_buffer );

            // if the payload is unknown
            if ( payload.get() == NULL ) {
                // if the payload is critical
                if ( critical )
                    throw UnknownPayloadException( "Unknonwn critical payload: " + Payload::PAYLOAD_TYPE_STR( current_payload_type ), current_payload_type );

                // if it is not critical
                Log::writeLockedMessage( "Message", "Unknown payload type=[" + intToString( current_payload_type ) + "]", Log::LOG_ERRO, true );
            }

            // If payload is known
            else {
                payloads.push_back( payload.release() );

            }

            // updates the current payload type
            current_payload_type = next_payload_type;
        }

        // If next payload = 0 and there is still data available, then report error
        if ( byte_buffer.size() > 0 )
            throw ParsingException( "Message has more data than needed. Found premature next_payload=0" );

        // if there isn't more data and the next_payload is not 0 and the last payload is not an payload_sk, then report error
        if ( byte_buffer.size() == 0 && current_payload_type != Payload::PAYLOAD_NONE && payloads.size() > 0 && 
                payloads.back()->type != Payload::PAYLOAD_SK )
            throw ParsingException( "Message must end with next_payload=0 or next_payload=PAYLOAD_SK" );

        // return the next payload type of the last fixed payload header
        return next_payload_type;
    }

    auto_ptr< ByteArray > Message::generateBinaryRepresentation( Payload::PAYLOAD_TYPE last_payload_type, const vector< Payload * > payloads ) {
        auto_ptr<ByteBuffer> byte_buffer ( new ByteBuffer( MAX_MESSAGE_SIZE ) );

        // writes all the payloads
        for ( vector<Payload*>::const_iterator it = payloads.begin(); it != payloads.end(); it++ ) {
            // writes the next_payload_type
            if ( it < payloads.end() - 1 )
                byte_buffer->writeInt8( ( *( it + 1 ) ) ->type );
            else
                byte_buffer->writeInt8( last_payload_type );

            // writes reserved bytes
            byte_buffer->fillBytes( 1, 0 );

            // writes payload
            ( *it ) ->getBinaryRepresentation( *byte_buffer );
        }

        return auto_ptr<ByteArray> ( byte_buffer );
    }


    vector< Payload * > Message::getPayloadsByType( Payload::PAYLOAD_TYPE type ) const {
        vector<Payload*> result;

        // search in the unencrypted payloads
        for ( vector<Payload*>::const_iterator it = this->unencrypted_payloads->begin(); it != this->unencrypted_payloads->end(); it++ ) {
            if ( ( *it ) ->type == type )
                result.push_back( *it );
        }

        // seatch in the encrypted payloads
        for ( vector<Payload*>::const_iterator it = this->encrypted_payloads->begin(); it != this->encrypted_payloads->end(); it++ ) {
            if ( ( *it ) ->type == type )
                result.push_back( *it );
        }

        return result;
    }

    Payload * Message::getFirstPayloadByType( Payload::PAYLOAD_TYPE type ) const {
        vector<Payload*> payloads = this->getPayloadsByType( type );

        if ( payloads.size() > 0 )
            return payloads.front();
        else
            return NULL;
    }

    Payload & Message::getUniquePayloadByType( Payload::PAYLOAD_TYPE type ) const {
        vector<Payload*> payloads = this->getPayloadsByType( type );

        if ( payloads.size() != 1 )
            throw ParsingException( "There must be one and only one Payload of the type " + Payload::PAYLOAD_TYPE_STR( type ) + " in the message" );

        return *payloads.front();
    }

    vector< Payload_NOTIFY * > Message::getNotifiesByType( Payload_NOTIFY::NOTIFY_TYPE notification_type ) const {
        vector<Payload_NOTIFY*> result;

        // gets all the notifies form the message
        vector<Payload*> notify_payloads = this->getPayloadsByType( Payload::PAYLOAD_NOTIFY );

        // find the notifies with the specified notification type
        for ( vector<Payload*>::const_iterator it = notify_payloads.begin(); it != notify_payloads.end(); it++ ) {
            Payload_NOTIFY* payload_notify = ( Payload_NOTIFY* ) ( *it );
            if ( payload_notify->notification_type == notification_type )
                result.push_back( payload_notify );
        }

        return result;
    }

    Payload_NOTIFY * Message::getFirstNotifyByType( Payload_NOTIFY::NOTIFY_TYPE notification_type ) const {
        vector<Payload_NOTIFY*> notify_payloads = this->getNotifiesByType( notification_type );

        if ( notify_payloads.size() > 0 )
            return notify_payloads.front();
        else
            return NULL;
    }


    Payload_NOTIFY& Message::getUniqueNotifyByType( Payload_NOTIFY::NOTIFY_TYPE notification_type ) const {
        vector<Payload_NOTIFY*> notify_payloads = this->getNotifiesByType( notification_type );

        if ( notify_payloads.size() != 1 )
            throw ParsingException( "There must be one and only one Notification Payload of the type " + Payload_NOTIFY::NOTIFY_TYPE_STR( notification_type ) + " in the message" );

        return *notify_payloads.front();
    }

    void Message::addPayload( auto_ptr<Payload> payload, bool is_encrypted ) {
        assert ( payload.get() );

        // this method invalidate the cached binary representation
        this->binary_representation.reset( NULL );

        AutoVector<Payload> &collection = ( is_encrypted ) ? this->encrypted_payloads : this->unencrypted_payloads;

        collection->push_back( payload.release() );
    }

    void Message::addPayloads( AutoVector< Payload > payloads, bool is_encrypted ) {
        for ( vector<Payload*>::iterator it = payloads->begin(); it != payloads->end(); it++ )
            this->addPayload( auto_ptr<Payload> ( *it ), is_encrypted );

        // release the vector
        payloads->clear();
    }

    void Message::addPayloadNotify( auto_ptr<Payload_NOTIFY> notify_payload, bool is_encrypted ) {
        assert ( notify_payload.get() );

        // this method invalidate the cached binary representation
        this->binary_representation.reset( NULL );

        AutoVector<Payload> &collection = ( is_encrypted ) ? this->encrypted_payloads : this->unencrypted_payloads;

        collection->insert( collection->begin(), notify_payload.release() );
    }

    void Message::addPayloadsNotify( AutoVector< Payload_NOTIFY > notifies, bool is_encrypted ) {
        for ( vector<Payload_NOTIFY*>::iterator it = notifies->begin(); it != notifies->end(); it++ )
            this->addPayloadNotify( auto_ptr<Payload_NOTIFY> ( *it ), is_encrypted );

        // release the vector
        notifies->clear();
    }

    ByteArray& Message::getBinaryRepresentation( Cipher * cipher ) {
        // if the binary representation of the Message is cached, then return it avoiding regenerate
        if ( this->binary_representation.get() )
            return * this->binary_representation;

        // Buffer to store the payload binary representation
        auto_ptr<ByteArray> payloads_binary_representation;

        // When a Payload_SK must be included
        if ( cipher != NULL ) {
            // creates the Payload_SK
            auto_ptr<ByteArray> to_be_encrypted = Message::generateBinaryRepresentation( Payload::PAYLOAD_NONE, this->encrypted_payloads.get() );
            this->payload_sk.reset( new Payload_SK( *cipher, *to_be_encrypted ) );

            // Adds it at the end of a temporal collection
            vector<Payload*> real_unencrypted_payloads = this->unencrypted_payloads.get();
            real_unencrypted_payloads.push_back( this->payload_sk.get() );

            // generates the binary representation of the temporal collection
            this->first_payload_type_sk = ( this->encrypted_payloads->size() > 0 ) ? this->encrypted_payloads->front() ->type : Payload::PAYLOAD_NONE;
            this->first_payload_type = ( this->unencrypted_payloads->size() > 0 ) ? this->unencrypted_payloads->front() ->type : Payload::PAYLOAD_SK;

            payloads_binary_representation = Message::generateBinaryRepresentation( this->first_payload_type_sk, real_unencrypted_payloads );
        }

        // When no Payload_SK is needed
        else {
            this->first_payload_type = ( this->unencrypted_payloads->size() > 0 ) ? this->unencrypted_payloads->front() ->type : Payload::PAYLOAD_NONE;
            payloads_binary_representation = Message::generateBinaryRepresentation( Payload::PAYLOAD_NONE, this->unencrypted_payloads.get() );
        }

        // generate the message in a new ByteBuffer
        auto_ptr<ByteBuffer> byte_buffer ( new ByteBuffer( MAX_MESSAGE_SIZE ) );

        // writes initiator SPI
        byte_buffer->writeBuffer ( &this->spi_i, 8 );

        // writes responder SPI
        byte_buffer->writeBuffer ( &this->spi_r, 8 );

        // Writes first payload type
        byte_buffer->writeInt8( this->first_payload_type );

        // Writes version numbers
        byte_buffer->writeInt8( ( this->major_version << 4 ) | ( this->minor_version & 0x0F ) );

        // Writes exchange type
        byte_buffer->writeInt8( this->exchange_type );

        // Writes Flags
        byte_buffer->writeInt8( ( this->is_initiator << 3 ) | ( ( uint8_t ) this->message_type << 5 ) | ( this->can_use_higher_major_version << 4 ) );

        // Writes message ID
        byte_buffer->writeInt32( this->message_id );

        // writes the message length
        byte_buffer->writeInt32( 28 + payloads_binary_representation->size() );

        // Generate the payloads binary representation
        byte_buffer->writeByteArray( *payloads_binary_representation );

        // Check if message is large
        if ( byte_buffer->size() > WARNING_MESSAGE_SIZE )
            Log::writeMessage( "Message", "A message exceeds the WARN limit size (" + intToString( WARNING_MESSAGE_SIZE ) + " bytes). You may want to use HASH & URL certificate.", Log::LOG_WARN, true );

        // writes the binary representation
        this->binary_representation = byte_buffer;

        // get the integrity checksum if needed
        if ( cipher != NULL ) {
            // obtain the checksum
            auto_ptr<ByteArray> message_data ( new ByteArray ( this->binary_representation->getRawPointer(), this->binary_representation->size() - cipher->integ_hash_size ) );
            auto_ptr<ByteArray> integrity_checksum = cipher->computeIntegrity( *message_data );

            // writes it in the end of the message
            uint8_t* position = &this->binary_representation->getRawPointer() [ this->binary_representation->size() - integrity_checksum->size() ];
            memcpy( position, integrity_checksum->getRawPointer(), integrity_checksum->size() );
        }

        return *this->binary_representation;
    }

    void Message::decryptPayloadSK( Cipher * cipher ) {
        // if cipher is NULL, then no action
        if ( cipher == NULL )
            return ;

        auto_ptr<ByteArray> decrypted_body = this->payload_sk->getDecryptedBody( *cipher );

        ByteBuffer byte_buffer( *decrypted_body );

        Message::generatePayloads( this->first_payload_type_sk, byte_buffer, this->encrypted_payloads.get() );
    }

    string Message::toStringTab( uint8_t tabs ) const {
        ByteBuffer temp( 100 );

        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<MESSAGE> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "src_addr=" << this->src_addr->toString() << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "dst_addr=" << this->dst_addr->toString() << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "exchange_type=" << Message::EXCHANGE_TYPE_STR( this->exchange_type ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "message_id=" << this->message_id << "\n";

        temp.reset();
        temp.writeBuffer( &this->spi_i, 8 );
        oss << Printable::generateTabs( tabs + 1 ) << "initiator_spi=" << temp.toStringTab( tabs + 1 ) << "\n";

        temp.reset();
        temp.writeBuffer( &this->spi_r, 8 );
        oss << Printable::generateTabs( tabs + 1 ) << "responder_spi=" << temp.toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "version=" << ( int ) this->major_version << "." << ( int ) this->minor_version << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "is_initiator=" << boolToString( this->is_initiator ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "message_type=" << MESSAGE_TYPE_STR( this->message_type ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "can_use_higher_major_version=" << boolToString( this->can_use_higher_major_version ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "<UNENCRYPTED PAYLOADS> {\n";
        for ( vector<Payload*>::const_iterator it = this->unencrypted_payloads->begin(); it != this->unencrypted_payloads->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 2 );
        oss << Printable::generateTabs( tabs + 1 ) << "}\n";

        oss << Printable::generateTabs( tabs + 1 ) << "<ENCRYPTED PAYLOADS> {\n";
        for ( vector<Payload*>::const_iterator it = this->encrypted_payloads->begin(); it != this->encrypted_payloads->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 2 );
        oss << Printable::generateTabs( tabs + 1 ) << "}\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    bool Message::checkIntegrity( Cipher * cipher ) const {
        //  Integrity is supposed OK when no cipher is specified
        if ( cipher == NULL )
            return true;

        ByteBuffer byte_buffer( *this->binary_representation );

        auto_ptr<ByteArray> message_data = byte_buffer.readByteArray( this->binary_representation->size() - cipher->integ_hash_size );
        auto_ptr<ByteArray> message_integrity_checksum = byte_buffer.readByteArray( cipher->integ_hash_size );

        // computes the expected integrity checksum
        auto_ptr<ByteArray> computed_integrity_checksum = cipher->computeIntegrity( *message_data );

        return ( *computed_integrity_checksum == *message_integrity_checksum );
    }

    auto_ptr<Message> Message::clone() const {
        return auto_ptr<Message> ( new Message( *this ) );
    }

    void Message::replaceFirstPayloadByType( Payload::PAYLOAD_TYPE type, auto_ptr< Payload > new_payload ) {
        // this method invalidate the cached binary representation
        this->binary_representation.reset( NULL );

        // search in the unencrypted payloads
        for ( vector<Payload*>::iterator it = this->unencrypted_payloads->begin(); it != this->unencrypted_payloads->end(); it++ ) {
            if ( ( *it ) ->type == type ) {
                delete ( *it );
                ( *it ) = new_payload.release();
                return ;
            }

            // seatch in the encrypted payloads
            for ( vector<Payload*>::iterator it = this->encrypted_payloads->begin(); it != this->encrypted_payloads->end(); it++ ) {
                if ( ( *it ) ->type == type ) {
                    delete ( *it );
                    ( *it ) = new_payload.release();
                    return ;
                }
            }
        }
    }

    void Message::replaceFirstNotifyByType( Payload_NOTIFY::NOTIFY_TYPE notify_type, auto_ptr< Payload_NOTIFY > new_payload ) {
        // this method invalidate the cached binary representation
        this->binary_representation.reset( NULL );

        // search in the unencrypted payloads
        for ( vector<Payload*>::iterator it = this->unencrypted_payloads->begin(); it != this->unencrypted_payloads->end(); it++ ) {
            if ( ( *it ) ->type == Payload::PAYLOAD_NOTIFY ) {
                Payload_NOTIFY * payload_notify = ( Payload_NOTIFY* ) ( *it );
                if ( payload_notify->notification_type == notify_type ) {
                    delete ( *it );
                    ( *it ) = new_payload.release();
                    return ;
                }
            }
        }

        // search in the encrypted payloads
        for ( vector<Payload*>::iterator it = this->encrypted_payloads->begin(); it != this->encrypted_payloads->end(); it++ ) {
            if ( ( *it ) ->type == Payload::PAYLOAD_NOTIFY ) {
                Payload_NOTIFY * payload_notify = ( Payload_NOTIFY* ) ( *it );
                if ( payload_notify->notification_type == notify_type ) {
                    delete ( *it );
                    ( *it ) = new_payload.release();
                    return ;
                }
            }
        }
    }

    SocketAddress & Message::getSrcAddress( ) const {
        return * this->src_addr;
    }

    SocketAddress & Message::getDstAddress( ) const {
        return * this->dst_addr;
    }

    string Message::EXCHANGE_TYPE_STR( EXCHANGE_TYPE exchange_type ) {
        switch ( exchange_type ) {
            case Message::CREATE_CHILD_SA:
                return "CREATE_CHILD_SA";
            case Message::IKE_AUTH:
                return "IKE_AUTH";
            case Message::IKE_SA_INIT:
                return "IKE_SA_INIT";
            case Message::INFORMATIONAL:
                return "INFORMATIONAL";
            default:
                return intToString( exchange_type );
        }
    }

    string Message::MESSAGE_TYPE_STR( MESSAGE_TYPE message_type ) {
        switch ( message_type ) {
            case Message::REQUEST:
                return "REQUEST";
            case Message::RESPONSE:
                return "RESPONSE";
            default:
                return intToString( message_type );
        }
    }
}










