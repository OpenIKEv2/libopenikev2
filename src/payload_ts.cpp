/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload_ts.h"

#include "exception.h"

namespace openikev2 {

    Payload_TS::Payload_TS()
            : Payload( PAYLOAD_NONE, false ) {}

    Payload_TS::Payload_TS( auto_ptr<TrafficSelector> traffic_selector )
            : Payload( PAYLOAD_NONE, false ) {
        this->addTrafficSelector( traffic_selector );
    }

    Payload_TS::Payload_TS( const Payload_TS & other )
            : Payload( other.type, false ) {

        // Clones all TrafficSelectors
        for ( vector<TrafficSelector*>::const_iterator it = other.traffic_selectors->begin(); it != other.traffic_selectors->end(); it++ )
            this->addTrafficSelector( ( *it ) ->clone() );
    }

    auto_ptr<Payload_TS> Payload_TS::parse( ByteBuffer& byte_buffer ) {
        // get a pointer to the begin of the payload
        uint8_t * payload_begin = byte_buffer.getReadPosition() - 2;

        // reads payload size
        uint16_t payload_size = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( payload_size < 8 )
            throw ParsingException( "Payload_TS length cannot be < 8 bytes." );

        // Reads TrafficSelector count
        uint8_t number_of_ts = byte_buffer.readInt8();

        // skips RESERVED bytes
        byte_buffer.skip( 3 );

        // adds all the traffic selectors
        auto_ptr<Payload_TS> result ( new Payload_TS() );
        for ( uint8_t i = 0; i < number_of_ts; i++ ) {
            auto_ptr<TrafficSelector> traffic_selector ( new TrafficSelector( byte_buffer ) );
            result->addTrafficSelector( traffic_selector );
        }

        // ckecks the size
        if ( byte_buffer.getReadPosition() != payload_begin + payload_size )
            throw ParsingException( "Payload_TS has different length than indicated in the header" );

        return result;
    }

    Payload_TS::~Payload_TS() { }


    void Payload_TS::addTrafficSelector( auto_ptr<TrafficSelector> traffic_selector ) {
        this->traffic_selectors->push_back( traffic_selector.release() );
    }

    void Payload_TS::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // pointer to the begin of the payload
        uint8_t * payload_begin = byte_buffer.getWritePosition();

        // writes dummy payload_length value
        byte_buffer.writeInt16( 0 );

        // writes the number of traffic selectors
        byte_buffer.writeInt8( this->traffic_selectors->size() );

        // writes RESERVED
        byte_buffer.fillBytes( 3, 0 );

        // writes all the traffic selectors
        for ( vector<TrafficSelector*>::const_iterator it = this->traffic_selectors->begin(); it != this->traffic_selectors->end(); it++ )
            ( *it ) ->getBinaryRepresentation( byte_buffer );

        // pointer to the end of the payload
        uint8_t* payload_end = byte_buffer.getWritePosition();

        // writes the real payload length value
        byte_buffer.setWritePosition( payload_begin );
        byte_buffer.writeInt16( payload_end - payload_begin + 2 );
        byte_buffer.setWritePosition( payload_end );
    }

    string Payload_TS::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        if ( this->type == PAYLOAD_TSi )
            oss << Printable::generateTabs( tabs ) << "<PAYLOAD_TSi> {\n";
        else if ( this->type == PAYLOAD_TSr )
            oss << Printable::generateTabs( tabs ) << "<PAYLOAD_TSr> {\n";

        for ( vector<TrafficSelector*>::const_iterator it = this->traffic_selectors->begin(); it != this->traffic_selectors->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    bool Payload_TS::operator==( const Payload_TS& other ) const {
        if ( this->traffic_selectors->size() != other.traffic_selectors->size() )
            return false;

        for ( vector<TrafficSelector*>::const_iterator it = this->traffic_selectors->begin(); it != this->traffic_selectors->end(); it++ )
            if ( !other.hasTrafficSelector( *( *it ) ) )
                return false;

        return true;
    }

    bool Payload_TS::hasTrafficSelector( const TrafficSelector & traffic_selector ) const {
        for ( vector<TrafficSelector*>::const_iterator it = this->traffic_selectors->begin(); it != this->traffic_selectors->end(); it++ )
            if ( traffic_selector == *( *it ) )
                return true;
        return false;
    }

    vector< TrafficSelector * > Payload_TS::getTrafficSelectors( ) const {
        return this->traffic_selectors.get();
    }

    auto_ptr<Payload> Payload_TS::clone( ) const {
        return auto_ptr<Payload> ( new Payload_TS( *this ) );
    }
}



