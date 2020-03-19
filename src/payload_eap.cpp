/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_eap.h"

#include "exception.h"

#include "utils.h"

namespace openikev2 {

    Payload_EAP::Payload_EAP( auto_ptr<EapPacket> eap_packet )
            : Payload( PAYLOAD_EAP, false ) {
        this->eap_packet = eap_packet;
    }

    Payload_EAP::Payload_EAP( const Payload_EAP& other )
            : Payload( PAYLOAD_EAP, false ) {
        this->eap_packet = other.eap_packet->clone();
    }

    auto_ptr<Payload_EAP> Payload_EAP::parse( ByteBuffer& byte_buffer ) {
        uint8_t *payload_begin = byte_buffer.getReadPosition();

        // reads payload size
        uint16_t payload_length = byte_buffer.readInt16();

        // Size must be at least size of fixed header, group id and reserved
        if ( payload_length < 4 )
            throw ParsingException( "Payload_EAP length cannot be < 4 bytes." );

        auto_ptr<EapPacket> eap_packet = EapPacket::parse( byte_buffer );

        uint8_t *payload_end = byte_buffer.getReadPosition();

        if ( payload_end - payload_begin != payload_length - 2 )
            throw ParsingException( "Payload_EAP length and EAP packet length don't match" );

        return auto_ptr<Payload_EAP> ( new Payload_EAP( eap_packet ) );
    }

    Payload_EAP::~Payload_EAP() { }

    void Payload_EAP::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        uint8_t* payload_begin = byte_buffer.getWritePosition();

        // writes payload length (foo)
        byte_buffer.writeInt16( 0 );

        // Writes EAP packet
        this->eap_packet->getBinaryRepresentation( byte_buffer );

        // writes real payload length
        uint8_t* current_position = byte_buffer.getWritePosition();
        byte_buffer.setWritePosition( payload_begin );
        byte_buffer.writeInt16( current_position - payload_begin + 2 );
        byte_buffer.setWritePosition( current_position );
    }

    string Payload_EAP::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<PAYLOAD_EAP> {\n";

        oss << this->eap_packet->toStringTab( tabs + 1 ) << endl;

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    auto_ptr<Payload> Payload_EAP::clone( ) const {
        return auto_ptr<Payload> ( new Payload_EAP( *this ) );
    }

    EapPacket & Payload_EAP::getEapPacket() const {
        return *this->eap_packet;
    }
}






