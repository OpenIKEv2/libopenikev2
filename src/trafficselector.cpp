/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "trafficselector.h"

#include "exception.h"
#include "enums.h"
#include "utils.h"
#include "networkprefix.h"

#include <assert.h>

namespace openikev2 {

    TrafficSelector::TrafficSelector( TS_TYPE ts_type, auto_ptr<ByteArray> start_addr, auto_ptr<ByteArray> end_addr, uint16_t start_port, uint16_t end_port, uint8_t ip_protocol_id ) {
        assert( ( start_port <= end_port ) || ( start_port == 65535 && end_port == 0 ) );

        this->construct( ts_type, start_addr, end_addr, start_port, end_port, ip_protocol_id );
    }

    TrafficSelector::TrafficSelector( TS_TYPE ts_type, auto_ptr<ByteArray> start_addr, auto_ptr<ByteArray> end_addr, uint8_t start_icmp_type, uint8_t start_icmp_code, uint8_t end_icmp_type, uint8_t end_icmp_code, uint8_t ip_protocol_id ) {
        assert ( ip_protocol_id == Enums::IP_PROTO_ICMP || ip_protocol_id == Enums::IP_PROTO_ICMPv6 );

        uint16_t start_port = ( start_icmp_type << 8 ) | start_icmp_code;
        uint16_t end_port = ( end_icmp_type << 8 ) | end_icmp_code;
        this->construct( ts_type, start_addr, end_addr, start_port, end_port, ip_protocol_id );
    }

    TrafficSelector::TrafficSelector( IpAddress& address, uint8_t prefixlen, uint8_t icmp_type, uint8_t icmp_code, uint8_t ip_protocol_id ) {
        assert ( ip_protocol_id == Enums::IP_PROTO_ICMP || ip_protocol_id == Enums::IP_PROTO_ICMPv6 );

        uint16_t start_port = ( icmp_type << 8 ) | icmp_code;
        this->construct( address, prefixlen, start_port, ip_protocol_id );

        if ( icmp_type == 0 )
            this->end_port |= 0xFF00;

        if ( icmp_code == 0 )
            this->end_port |= 0xFF;
    }


    TrafficSelector::TrafficSelector( IpAddress& address, uint8_t prefixlen, uint16_t port , uint8_t ip_protocol_id ) {
        this->construct( address, prefixlen, port, ip_protocol_id );
    }

    TrafficSelector::TrafficSelector( const TrafficSelector& other ) {
        this->ts_type = other.ts_type;
        this->ip_protocol_id = other.ip_protocol_id;
        this->start_port = other.start_port;
        this->end_port = other.end_port;

        this->start_addr = other.start_addr->clone();
        this->end_addr = other.end_addr->clone();
    }

    TrafficSelector::TrafficSelector( ByteBuffer& byte_buffer ) {
        // reads ts type
        this->ts_type = ( TS_TYPE ) byte_buffer.readInt8();

        // check TS TYPE
        if ( this->ts_type != TS_IPV4_ADDR_RANGE && this->ts_type != TS_IPV6_ADDR_RANGE )
            throw ParsingException( "TS type not supported" );

        // reads the IP protocol ID
        this->ip_protocol_id = byte_buffer.readInt8();

        // reads selector length
        uint16_t selector_length = byte_buffer.readInt16();

        // check length
        if ( selector_length != 8 + TrafficSelector::getExpectedAddressSize( this->ts_type ) * 2 )
            throw ParsingException ( " Invalid Traffic Selector length" );

        // reads the start port
        this->start_port = byte_buffer.readInt16();

        // reads the  port
        this->end_port = byte_buffer.readInt16();

        // checks the port range
        if ( ( this->start_port > this->end_port ) && ( this->start_port != 65535 && this->end_port != 0 ) )
            throw ParsingException( "TS has invalid port range." );

        // reads the start address
        this->start_addr = byte_buffer.readByteArray( TrafficSelector::getExpectedAddressSize( this->ts_type ) );
        this->end_addr = byte_buffer.readByteArray( TrafficSelector::getExpectedAddressSize( this->ts_type ) );

        // checks the address range
        if ( *this->start_addr > *this->end_addr )
            throw ParsingException( "TS has invalid address range." );
    }

    TrafficSelector::~TrafficSelector( ) {}

    void TrafficSelector::construct( TS_TYPE ts_type, auto_ptr<ByteArray> start_addr, auto_ptr<ByteArray> end_addr, uint16_t start_port, uint16_t end_port, uint8_t ip_protocol_id ) {
        assert ( start_addr->size() == getExpectedAddressSize( ts_type ) );
        assert ( end_addr->size() == getExpectedAddressSize( ts_type ) );

        this->ts_type = ts_type;

        this->ip_protocol_id = ip_protocol_id;

        this->start_port = start_port;

        this->end_port = end_port;

        this->start_addr = start_addr;

        this->end_addr = end_addr;
    }

    void TrafficSelector::construct( IpAddress& address, uint8_t prefixlen, uint16_t port, uint8_t ip_protocol_id ) {
        this->ts_type = ( address.getFamily() == Enums::ADDR_IPV4 ) ? TS_IPV4_ADDR_RANGE : TS_IPV6_ADDR_RANGE;

        this->ip_protocol_id = ip_protocol_id;

        if ( port == 0 ) {
            this->start_port = 0;
            this->end_port = 65535;
        }
        else {
            this->start_port = port;
            this->end_port = port;
        }

        NetworkPrefix prefix ( address.clone(), prefixlen );

        this->start_addr = prefix.getNetworkAddress().getBytes();
        this->end_addr = prefix.getBroadCastAddress()->getBytes();

    }

    auto_ptr<TrafficSelector> TrafficSelector::intersection( const TrafficSelector& ts1, const TrafficSelector& ts2 ) {
        // check the TS type
        if ( ts1.ts_type != ts2.ts_type )
            return auto_ptr<TrafficSelector> ( NULL );

        // check if both have compatibles ip_protocol_id
        if ( ( ts1.ip_protocol_id != ts2.ip_protocol_id ) && ts1.ip_protocol_id != Enums::IP_PROTO_ANY && ts2.ip_protocol_id != Enums::IP_PROTO_ANY )
            return auto_ptr<TrafficSelector> ( NULL );

        // the selected ip_protocol_id will be the non 0 one (if available)
        uint8_t selected_ip_protocol_id = ( ts1.ip_protocol_id != 0 ) ? ts1.ip_protocol_id : ts2.ip_protocol_id;

        // the selected start port will be the maximun
        uint16_t selected_start_port = ( ts1.start_port > ts2.start_port ) ? ts1.start_port : ts2.start_port;

        // the selected end port will be the minimun
        uint16_t selected_end_port = ( ts1.end_port < ts2.end_port ) ? ts1.end_port : ts2.end_port;

        // check selected ports
        if ( ( selected_start_port > selected_end_port ) && ( selected_start_port != 65535 ) && ( selected_end_port != 0 ) )
            return auto_ptr<TrafficSelector> ( NULL );

        // the selected start address will be the maximun
        auto_ptr<ByteArray> selected_start_address = ( *ts1.start_addr > *ts2.start_addr ) ? ts1.start_addr->clone() : ts2.start_addr->clone();

        // the selected end address will be the minimun
        auto_ptr<ByteArray> selected_end_address = ( *ts1.end_addr < *ts2.end_addr ) ? ts1.end_addr->clone() : ts2.end_addr->clone();

        // check selected addresses
        if ( ( *selected_start_address > *selected_end_address ) )
            return auto_ptr<TrafficSelector> ( NULL );

        // creates the result
        return auto_ptr<TrafficSelector> ( new TrafficSelector( ts1.ts_type, selected_start_address, selected_end_address, selected_start_port, selected_end_port, selected_ip_protocol_id ) );
    }

    bool TrafficSelector::operator>=( const TrafficSelector& other ) const {
        if ( this->ts_type != other.ts_type )
            return false;

        if ( ( this->ip_protocol_id != other.ip_protocol_id ) && this->ip_protocol_id != Enums::IP_PROTO_ANY )
            return false;

        if ( this->start_port > other.start_port || this->end_port < other.end_port )
            return false;

        if ( *this->start_addr > *other.start_addr )
            return false;

        if ( *this->end_addr < *other.end_addr )
            return false;

        return true;
    }

    bool TrafficSelector::operator<=( const TrafficSelector& other ) const {
        return ( other >= *this );
    }

    bool TrafficSelector::operator==( const TrafficSelector& other ) const {
        if ( this->ts_type != other.ts_type )
            return false;

        if ( this->ip_protocol_id != other.ip_protocol_id )
            return false;

        if ( this->start_port != other.start_port || this->end_port != other.end_port )
            return false;

        if ( !( *this->start_addr == *other.start_addr ) )
            return false;

        if ( !( *this->end_addr == *other.end_addr ) )
            return false;

        return true;
    }

    void TrafficSelector::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes TS type
        byte_buffer.writeInt8( this->ts_type );

        // writes IP protocol ID
        byte_buffer.writeInt8( this->ip_protocol_id );

        // writes selector length
        byte_buffer.writeInt16( 8 + this->start_addr->size() + this->end_addr->size() );

        // writes start port
        byte_buffer.writeInt16( this->start_port );

        // writes end port
        byte_buffer.writeInt16( this->end_port );

        // writes strart address
        byte_buffer.writeByteArray( *this->start_addr );

        // writes end address
        byte_buffer.writeByteArray( *this->end_addr );
    }

    string TrafficSelector::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<TRAFFIC_SELECTOR> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "ts_type=" << TrafficSelector::TS_TYPE_STR( this->ts_type ) << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "ip_protocol_id=" << Enums::IP_PROTO_STR( this->ip_protocol_id ) << "\n";

        ByteBuffer temp_sport( 2 );
        ByteBuffer temp_dport( 2 );
        temp_sport.writeInt16( this->start_port );
        temp_dport.writeInt16( this->end_port );
        oss << Printable::generateTabs( tabs + 1 ) << "port range = " << temp_sport.toString() << "-" << temp_dport.toString() << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "start_address=" << this->start_addr->toStringTab( tabs + 1 ) << endl;

        oss << Printable::generateTabs( tabs + 1 ) << "end_address=" << this->end_addr->toStringTab( tabs + 1 ) << endl;

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    uint16_t TrafficSelector::getExpectedAddressSize( TS_TYPE ts_type ) {
        if ( ts_type == TS_IPV4_ADDR_RANGE )
            return 4;
        else if ( ts_type == TS_IPV6_ADDR_RANGE )
            return 16;
        else
            assert( 0 );
    }

    string TrafficSelector::TS_TYPE_STR( TS_TYPE ts_type ) {
        switch ( ts_type ) {
            case TrafficSelector::TS_IPV4_ADDR_RANGE:
                return "TS_IPV4_ADDR_RANGE";
            case TrafficSelector::TS_IPV6_ADDR_RANGE:
                return "TS_IPV6_ADDR_RANGE";
            default:
                return intToString( ts_type );
        }
    }

    uint8_t TrafficSelector::getStartIcmpType( ) const {
        return ( this->start_port >> 8 );
    }

    uint8_t TrafficSelector::getStartIcmpCode( ) const  {
        return ( this->start_port & 0xFF );
    }

    uint8_t TrafficSelector::getEndIcmpType( ) const  {
        return ( this->end_port >> 8 );
    }

    uint8_t TrafficSelector::getEndIcmpCode( ) const  {
        return ( this->end_port & 0xFF );
    }

    uint16_t TrafficSelector::getStartPort( ) const  {
        return this->start_port;
    }

    uint16_t TrafficSelector::getEndPort( ) const  {
        return this->end_port;
    }

    auto_ptr<TrafficSelector> TrafficSelector::clone( ) const {
        return auto_ptr<TrafficSelector> ( new TrafficSelector( *this ) );
    }
}
