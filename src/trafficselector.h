/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef TRAFFICSELECTOR_H
#define TRAFFICSELECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "ipaddress.h"
#include "bytebuffer.h"

namespace openikev2 {

    /**
        This class represents a Traffic selector
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class TrafficSelector : public Printable {

            /****************************** ENUMS ******************************/
        public:
            /** Traffic selector types */
            enum TS_TYPE {
                TS_IPV4_ADDR_RANGE = 7,           /**< IPv4 address range type */
                TS_IPV6_ADDR_RANGE = 8,           /**< IPv6 address range type */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            TS_TYPE ts_type;                /**< Traffic selector type */
            auto_ptr<ByteArray> start_addr;/**< Smallest address included in this TS */
            auto_ptr<ByteArray> end_addr;   /**< Largest address included in this TS */
            uint16_t start_port;            /**< Smallest port number allowed by this TS */
            uint16_t end_port;              /**< Largest port number allowed by this TS */
            uint8_t ip_protocol_id;         /**< IP protocol ID (e.g. UDP/TCP/ICMP) */

            /****************************** METHODS ******************************/
        protected:

            /**
             * Gets the expected address size for the TS type
             * @param ts_type TS TYPE
             * @return The expected size of the address
             */
            static uint16_t getExpectedAddressSize( TS_TYPE ts_type );

            /**
             * Set up the Traffic Selector attributes, setting its parameters. This is the full featured construct method
             * @param ts_type TrafficSelector type
             * @param start_addr Starting address.
             * @param end_addr Ending address.
             * @param start_port Starting port number.
             * @param end_port Ending port number.
             * @param ip_protocol_id IP protocol ID.
             */
            void construct( TS_TYPE ts_type, auto_ptr<ByteArray> start_addr, auto_ptr<ByteArray> end_addr, uint16_t start_port, uint16_t end_port, uint8_t ip_protocol_id );

            /**
             * Set up the Traffic Selector, based on an IP subnet
             * @param address IP subnet address
             * @param prefixlen Prefix length
             * @param port Port to generate the port range. If port=0, the range will be 0-65535, Otherwise the port range will be port-port.
             * @param ip_protocol_id IP protocol ID
             */
            void construct( IpAddress& address, uint8_t prefixlen, uint16_t port , uint8_t ip_protocol_id );

        public:
            /**
             * Creates a new Traffic Selector (for all the IP protocols)
             * @param ts_type TrafficSelector type
             * @param start_addr Starting address.
             * @param end_addr Ending address.
             * @param start_port Starting port number.
             * @param end_port Ending port number.
             * @param ip_protocol_id IP protocol ID.
             */
            TrafficSelector( TS_TYPE ts_type, auto_ptr<ByteArray> start_addr, auto_ptr<ByteArray> end_addr, uint16_t start_port, uint16_t end_port, uint8_t ip_protocol_id );

            /**
             * Creates a new Traffic Selector (for ICMP/ICMPv6 protocols)
             * @param ts_type TrafficSelector type
             * @param start_addr Starting address.
             * @param end_addr Ending address.
             * @param start_icmp_type Starting ICMP type
             * @param start_icmp_code Starting ICMP code
             * @param end_icmp_type Ending ICMP type
             * @param end_icmp_code Ending ICMP code
             * @param ip_protocol_id IP protocol ID (only ICMP or ICMPv6) allowed
             */
            TrafficSelector( TS_TYPE ts_type, auto_ptr<ByteArray> start_addr, auto_ptr<ByteArray> end_addr, uint8_t start_icmp_type, uint8_t start_icmp_code, uint8_t end_icmp_type, uint8_t end_icmp_code, uint8_t ip_protocol_id );

            /**
             * Creates a new Traffic Selector, based on an IP subnet
             * @param address IP network address
             * @param prefixlen Prefix length
             * @param port Port to generate the port range. If port=0, the range will be 0-65535, Otherwise the port range will be exactly the indicated port
             * @param ip_protocol_id IP protocol ID
             */
            TrafficSelector( IpAddress& address, uint8_t prefixlen, uint16_t port, uint8_t ip_protocol_id );

            /**
             * Creates a new Traffic Selector, based on an IP subnet (for ICMP/ICMPv6 protocols)
             * @param address IP network address
             * @param prefixlen Prefix length
             * @param icmp_type ICMP type to generate the type/code range.
             * @param icmp_code ICMP code to generate the type/code range. If type/code = 0/0 then the range will be 0/0 - 0XFFF/0xFFFF.
                                Otherwise, the range will be type/code - type/code.
             * @param ip_protocol_id IP protocol ID (only ICMP protocols)
             */
            TrafficSelector( IpAddress& address, uint8_t prefixlen, uint8_t icmp_type, uint8_t icmp_code, uint8_t ip_protocol_id );

            /**
             * Creates a new TrafficSelector cloning another one
             * @param other Other TrafficSelector to be cloned.
             */
            TrafficSelector( const TrafficSelector& other );

            /**
             * Creates a new TrafficSelector based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "TS type" field
             */
            TrafficSelector( ByteBuffer& byte_buffer );

            /**
             * Translate from TS_type IDs to strings
             * @param ts_type TS_type IDs
             * @return String with the TS_type ID
             */
            static string TS_TYPE_STR( TS_TYPE ts_type );

            /**
             * Returns the result of intersec two TrafficSelectors
             * @param ts1 One TrafficSelector.
             * @param ts2 Other TrafficSelector.
             * @return Intersection of the two TrafficSelector.
             */
            static auto_ptr<TrafficSelector> intersection( const TrafficSelector& ts1, const TrafficSelector& ts2 );

            /**
             * Indicates if this TrafficSelector contains (is a superset) of other one.
             * @param other Other TrafficSelector.
             * @return TRUE if this one contains other one. FALSE otherwise.
             */
            virtual bool operator>=( const TrafficSelector& other ) const;

            /**
             * Indicates if this TrafficSelector is contained (is a subset) of other one.
             * @param other Other TrafficSelector.
             * @return TRUE if this one is contained in the other one. FALSE otherwise.
             */
            virtual bool operator<=( const TrafficSelector& other ) const;

            /**
             * Compares this TrafficSelector with another one.
             * @param other Other TrafficSelector.
             * @return TRUE if both are equals. FALSE otherwise.
             */
            virtual bool operator==( const TrafficSelector& other ) const;

            /**
             * Gets the start port attribute value
             * @return The start port value
             */
            virtual uint16_t getStartPort() const ;

            /**
             * Gets the end port attribute value
             * @return The end port value
             */
            virtual uint16_t getEndPort() const ;

            /**
             * Gets the start ICMP type value
             * @return The start ICMP type value
             */
            virtual uint8_t getStartIcmpType() const ;

            /**
             * Gets the start ICMP code value
             * @return The start ICMP code value
             */
            virtual uint8_t getStartIcmpCode() const ;

            /**
             * Gets the end ICMP type value
             * @return The end ICMP type value
             */
            virtual uint8_t getEndIcmpType() const;

            /**
             * Gets the end ICMP code value
             * @return The end ICMP code value
             */
            virtual uint8_t getEndIcmpCode() const;

            /**
             * Clones a TrafficSelector
             * @return A clone of this TrafficSelector
             */
            virtual auto_ptr<TrafficSelector> clone() const;

            /**
             * Appends the binary representation of the TrafficSelector at the end of the buffer
             * @param byte_buffer Byte buffer where append the binary representation
             */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~TrafficSelector();
    };
}
#endif
