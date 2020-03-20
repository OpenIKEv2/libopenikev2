/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2RADIUSATTRIBUTE_H
#define OPENIKEV2RADIUSATTRIBUTE_H

#include "bytearray.h"
#include "ipaddress.h"
#include "bytebuffer.h"

namespace openikev2 {

    /**
        This class represents an RADIUS attribute in a RADIUS message
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class RadiusAttribute {
        public:
            enum RADIUS_ATTRIBUTE_TYPE {
                RADIUS_ATTR_USER_NAME = 1,
                RADIUS_ATTR_USER_PASSWORD = 2,
                RADIUS_ATTR_NAS_IP_ADDRESS = 4,
                RADIUS_ATTR_NAS_PORT = 5,
                RADIUS_ATTR_FRAMED_MTU = 12,
                RADIUS_ATTR_STATE = 24,
                RADIUS_ATTR_CLASS = 25,
                RADIUS_ATTR_VENDOR_SPECIFIC = 26,
                RADIUS_ATTR_SESSION_TIMEOUT = 27,
                RADIUS_ATTR_IDLE_TIMEOUT = 28,
                RADIUS_ATTR_TERMINATION_ACTION = 29,
                RADIUS_ATTR_CALLED_STATION_ID = 30,
                RADIUS_ATTR_CALLING_STATION_ID = 31,
                RADIUS_ATTR_NAS_IDENTIFIER = 32,
                RADIUS_ATTR_PROXY_STATE = 33,
                RADIUS_ATTR_ACCT_STATUS_TYPE = 40,
                RADIUS_ATTR_ACCT_DELAY_TIME = 41,
                RADIUS_ATTR_ACCT_INPUT_OCTETS = 42,
                RADIUS_ATTR_ACCT_OUTPUT_OCTETS = 43,
                RADIUS_ATTR_ACCT_SESSION_ID = 44,
                RADIUS_ATTR_ACCT_AUTHENTIC = 45,
                RADIUS_ATTR_ACCT_SESSION_TIME = 46,
                RADIUS_ATTR_ACCT_INPUT_PACKETS = 47,
                RADIUS_ATTR_ACCT_OUTPUT_PACKETS = 48,
                RADIUS_ATTR_ACCT_TERMINATE_CAUSE = 49,
                RADIUS_ATTR_ACCT_MULTI_SESSION_ID = 50,
                RADIUS_ATTR_ACCT_LINK_COUNT = 51,
                RADIUS_ATTR_ACCT_INPUT_GIGAWORDS = 52,
                RADIUS_ATTR_ACCT_OUTPUT_GIGAWORDS = 53,
                RADIUS_ATTR_EVENT_TIMESTAMP = 55,
                RADIUS_ATTR_NAS_PORT_TYPE = 61,
                RADIUS_ATTR_TUNNEL_TYPE = 64,
                RADIUS_ATTR_TUNNEL_MEDIUM_TYPE = 65,
                RADIUS_ATTR_CONNECT_INFO = 77,
                RADIUS_ATTR_EAP_MESSAGE = 79,
                RADIUS_ATTR_MESSAGE_AUTHENTICATOR = 80,
                RADIUS_ATTR_TUNNEL_PRIVATE_GROUP_ID = 81,
                RADIUS_ATTR_ACCT_INTERIM_INTERVAL = 85,
                RADIUS_ATTR_NAS_PORT_ID = 87,
                RADIUS_ATTR_NAS_IPV6_ADDRESS = 95,
            };

            enum MS_RADIUS_ATTRIBUTE_TYPE {
                MS_MPPE_SEND_KEY = 16,
                MS_MPPE_RECV_KEY = 17
            };

        protected:
            RADIUS_ATTRIBUTE_TYPE type;
            auto_ptr<ByteArray> value;

        public:
            RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, auto_ptr<ByteArray> value );
            RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, string value );
            RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, uint8_t value );
            RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, uint16_t value );
            RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, uint32_t value );
            RadiusAttribute( RADIUS_ATTRIBUTE_TYPE type, const IpAddress& value );

            static auto_ptr<RadiusAttribute> parse( ByteBuffer& byte_buffer );

            /**
            * Appends the binary representation at the end of byte_buffer
            * @param byte_buffer ByteBuffer to append the binary representation
            */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual RADIUS_ATTRIBUTE_TYPE getType() const;

            virtual ByteArray& getValue() const;

            virtual auto_ptr<RadiusAttribute> clone() const;

            virtual ~RadiusAttribute();

    };

}

#endif
