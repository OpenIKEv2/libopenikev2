/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef CONFIGURATIONATTRIBUTE_H
#define CONFIGURATIONATTRIBUTE_H

#include "printable.h"
#include "bytebuffer.h"

namespace openikev2 {

    /**
        This class represents a Configuration Attribute in a Payload_CONF
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class ConfigurationAttribute : public Printable {

            /****************************** ENUMS ******************************/
        public:
            /**< Configuration attribute type */
            enum CONFIGURATION_ATTRIBUTE_TYPE {
                INTERNAL_IP4_ADDRESS    = 1,    /**< Internal IPv4 address */
                INTERNAL_IP4_NETMASK    = 2,    /**< Internal IPv4 netmask */
                INTERNAL_IP4_DNS        = 3,    /**< Internal IPv4 DNS address */
                INTERNAL_IP4_NBNS       = 4,    /**< Internal IPv4 NBNS address */
                INTERNAL_ADDRESS_EXPIRY = 5,    /**< Internal address time expiration */
                INTERNAL_IP4_DHCP       = 6,    /**< Internal IPv4 DHCP sever */
                APPLICATION_VERSION     = 7,    /**< */
                INTERNAL_IP6_ADDRESS    = 8,    /**<  Internal IPv6 address */
                INTERNAL_IP6_DNS        = 10,   /**< Internal IPv6 DNS address */
                INTERNAL_IP6_NBNS       = 11,   /**< Internal IPv6 NBNS address */
                INTERNAL_IP6_DHCP       = 12,   /**< Internal IPv6 DHCP sever */
                INTERNAL_IP4_SUBNET     = 13,   /**< Internal IPv4 subnet */
                SUPPORTED_ATTRIBUTES    = 14,   /**< */
                INTERNAL_IP6_SUBNET     = 15,   /**< Internal IPv6 subnet */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            CONFIGURATION_ATTRIBUTE_TYPE type;  /**< Configuration attribute type */
            auto_ptr<ByteArray> value;          /**< Value of the configuration attribute. */

            /****************************** METHODS ******************************/
        protected:
            /**
             * Checks the if the length is valid for this kind of attribute
             * @param type Attribute type
             * @param len Attribute len
             */
            static void checkLength( CONFIGURATION_ATTRIBUTE_TYPE type, uint16_t len );

        public:
            /**
             * Returns the textual representation of a CONFIGURATION_ATTRIBUTE_TYPE value
             * @param conf_attr CONFIGURATION_ATTRIBUTE_TYPE value
             * @return Textual respresentation
             */
            static string CONFIGURATION_ATTRIBUTE_TYPE_STR( CONFIGURATION_ATTRIBUTE_TYPE conf_attr );

            /**
             * Creates a new ConfigurationAttribute settings its value
             * @param type Type of the ConfigurationAttribute
             * @param value Value of the ConfigurationAttribute
             */
            ConfigurationAttribute( CONFIGURATION_ATTRIBUTE_TYPE type, auto_ptr<ByteArray> value );

            /**
             * Creates a new ConfigurationAttribute based on its binary representation
             * @param binary_representation Buffer with its read pointer at begin of a Configuration Attribute
             */
            static auto_ptr<ConfigurationAttribute> parse( ByteBuffer& binary_representation );

            /**
             * Creates a clone of this ConfigurationAttribute
             * @return The created ConfigurationAttribute
             */
            virtual auto_ptr<ConfigurationAttribute> clone();

            /**
             * Appends the binary representation at the end of byte_buffer
             * @param byte_buffer ByteBuffer to append the binary representation
             */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~ConfigurationAttribute();

    };
}

#endif
