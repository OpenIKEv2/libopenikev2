/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
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
#ifndef ENUMS_H
#define ENUMS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <sys/socket.h>

#define MAX_MESSAGE_SIZE        3000        // Max message size
#define WARNING_MESSAGE_SIZE    1500        // Warning message size

using namespace std;

namespace openikev2 {

    /**
        This class contains nearly all the IKEv2 related enumerations.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Enums {

            /****************************** ENUMS ******************************/
        public:

            /** IPsec modes */
            enum IPSEC_MODE {
		NONE_MODE = 0,
                TRANSPORT_MODE = 1,       /**< Transport mode. */
                TUNNEL_MODE = 2,          /**< Tunnel mode. */
            };

            /** Policy types */
            enum POLICY_TYPE {
		POLICY_MAIN = 0,
                POLICY_SUB = 1,
            };

            /** Policy action */
            enum POLICY_ACTION {
		POLICY_ALLOW = 0,
                POLICY_BLOCK = 1,
            };

            /** Attribute types */
            enum ATTRIBUTE_TYPE {
                ATTR_KEY_LEN = 14,        /**< Key length attribute type */
            };

            /** Transform types */
            enum TRANSFORM_TYPE {
                ENCR = 1,                 /**< Encryption algorithm */
                PRF = 2,                  /**< Pseudo-random function */
                INTEG = 3,                /**< Integrity algorithm */
                D_H = 4,                  /**< Diffie-Hellman group */
                ESN = 5,                  /**< Extended secuence numbers */
            };

            /** Transform type 1 (ENCR) IDs */
            enum ENCR_ID {
                ENCR_NONE = 0,            /**< NONE */
                ENCR_DES_IV64 = 1,        /**< DES algorithm with an 64 bit initialization vector (RFC 1827)*/
                ENCR_DES = 2,             /**< DES algorithm (RFC 2405)*/
                ENCR_3DES = 3,            /**< Triple DES algorithm (RFC 2451)*/
                ENCR_RC5 = 4,             /**< RC5 algorithm (RFC 2451)*/
                ENCR_IDEA = 5,            /**< IDEA algorithm (RFC 2451)*/
                ENCR_CAST = 6,            /**< CAST algorithm (RFC 2451)*/
                ENCR_BLOWFISH = 7,        /**< BLOWFISH algorithm (RFC 2451)*/
                ENCR_3IDEA = 8,           /**< Triple IDEA algorithm (RFC 2451)*/
                ENCR_DES_IV32 = 9,        /**< ? */
                ENCR_NULL = 11,           /**< NULL algorithm (RFC 2410) */
                ENCR_AES_CBC = 12,        /**< AES algorithm in CBC mode (RFC 3602) */
                ENCR_AES_CTR = 13,        /**< AES algorithm in CTR mode (RFC 3664) */
            };

            /** Transform type 2 (PRF) IDs */
            enum PRF_ID {
                PRF_NONE = 0,             /**< NONE */
                PRF_HMAC_MD5 = 1,         /**< Pseudo random function based on MD5 HMAC (RFC 2104) */
                PRF_HMAC_SHA1 = 2,        /**< Pseudo random function based on SHA1 HMAC (RFC 2104) */
                PRF_HMAC_TIGER = 3,       /**< Pseudo random function based on TIGER HMAC (RFC 2104) */
                PRF_AES128_CBC = 4,       /**< Pseudo random function based on AES128 in CBC mode (RFC 3664) */
            };

            /** Transform type 3 (INTEG) IDs */
            enum INTEG_ID {
                AUTH_NONE = 0,            /**< NONE */
                AUTH_HMAC_MD5_96 = 1,     /**< MD5 HMAC 96 algorithm (RFC 2403) */
                AUTH_HMAC_SHA1_96 = 2,    /**< SHA1 HMAC 96 algorithm (RFC 2404) */
                AUTH_DES_MAC = 3,         /**< DES MAC algorithm */
                AUTH_KPDK_MD5 = 4,        /**< MD5 KPDK algorithm (RFC 1826) */
                AUTH_AES_XCBC_96 = 5,     /**< AES XCBC 96 algorithm (RFC 3566) */
            };

            /** Transform type 4 (D_H) IDs */
            enum DH_ID {
                DH_NONE = 0,              /**< NONE */
                DH_GROUP_1 = 1,           /**< Diffie Hellman group 1 (MODP 768)  */
                DH_GROUP_2 = 2,           /**< Diffie Hellman group 2 (MODP 1024) */
                DH_GROUP_5 = 5,           /**< Diffie Hellman group 5 (MODP 1536) */
                DH_GROUP_14 = 14,         /**< Diffie Hellman group 14 (MODP 2048) */
                DH_GROUP_15 = 15,         /**< Diffie Hellman group 15 (MODP 3072) */
                DH_GROUP_16 = 16,         /**< Diffie Hellman group 16 (MODP 4096) */
                DH_GROUP_17 = 17,         /**< Diffie Hellman group 17 (MODP 6144) */
                DH_GROUP_18 = 18,         /**< Diffie Hellman group 18 (MODP 8192) */
            };

            /** Transform type 5 (ESN) IDs */
            enum ESN_ID {
                ESN_NO = 0,               /**< No extended sequence numbers */
                ESN_YES = 1,              /**< Extended secuence numbers */
            };

            /** Protocol IDs */
            enum PROTOCOL_ID {
                PROTO_NONE = 0,           /**< NONE */
                PROTO_IKE = 1,            /**< IKE Protocol */
                PROTO_AH = 2,             /**< AH Protocol */
                PROTO_ESP = 3,            /**< ESP Protocol */
            };

            /** Directions */
            enum DIRECTION {
                DIR_ALL = 0,              /**< Forward direction */
                DIR_IN = 1,               /**< Inbound direction */
                DIR_OUT = 2,              /**< Outbound direction */
                DIR_FWD = 3,              /**< Forward direction */
            };

            /** Role ID */
            enum ROLE_ID {
                ROLE_ANY = 0,             /**< Unspecified role */
                ROLE_INITIATOR = 1,       /**< Initiator role */
                ROLE_RESPONDER = 2,       /**< Responder role */
            };

            /** Identification Type */
            enum ID_TYPE {
                ID_RESERVED = 0,          /**< Reserved */
                ID_IPV4_ADDR = 1,         /**< IPv4 address */
                ID_FQDN = 2,              /**< Fully-qualified domain name */
                ID_RFC822_ADDR = 3,       /**< Fully-qualified RFC822 email address name */
                ID_IPV6_ADDR = 5,         /**< IPv6 address */
                ID_DER_ASN1_DN = 9,       /**< DER encoding of an ASN.1 X.500 Distinguised Name */
                ID_DER_ASN1_GN = 10,      /**< DER encoding of an ASN.1 X.500 General Name */
                ID_KEY_ID = 11,           /**< Key ID (opaque octet string) */
            };

            /** Ip address families */
            enum ADDR_FAMILY {
                ADDR_NONE,                  /**< No FAMILY */
                ADDR_IPV4,                  /**< IPv4 */
                ADDR_IPV6,                  /**< IPv6 */
            };

            /** Most used IP protocols */
            enum IP_PROTOCOL {
                IP_PROTO_ANY = 0,       /**< ANY */
                IP_PROTO_ICMP = 1,      /**< ICMP (v4)*/
                IP_PROTO_TCP = 6,       /**< TCP */
                IP_PROTO_UDP = 17,      /**< UDP */
                IP_PROTO_ICMPv6 = 58,   /**< ICMPv6 */
                IP_PROTO_MH = 135,      /**< MH */
            };

            /** Certificate encoding */
            enum CERT_ENCODING {
                CERT_RESERVED = 0,            /**< RESERVED */
                CERT_X509_PKCS_7 = 1,         /**< PKCS #7 wrapped X.509 certificate */
                CERT_PGP = 2,                 /**< PGP Certificate */
                CERT_DNS = 3,                 /**< DNS signed key */
                CERT_X509_SIGNATURE = 4,      /**< X.509 Certificate - Signature */
                CERT_KERBEROS = 6,            /**< Kerberos token */
                CERT_CRL = 7,                 /**< Certificate Revocation List (CRL) */
                CERT_ARL = 8,                 /**< Authority Revocation List (ARL) */
                CERT_SPKI = 9,                /**< SPKI Certificate */
                CERT_X509_ATTRIBUTE = 10,     /**< X.509 Certificate - Attribute */
                CERT_RAW_RSA = 11,            /**< Raw RSA Key */
                CERT_HASH_URL = 12,           /**< Hash and URL of X.509 certificate */
                CERT_HASH_URL_BUNDLE = 13,    /**< Hash and URL of X.509 bundle */
            };

            /** Authentication methods */
            enum AUTH_METHOD {
                AUTH_METHOD_NONE = 0,         /**< NONE */
                AUTH_METHOD_RSA = 1,          /**< RSA Authentication */
                AUTH_METHOD_PSK = 2,          /**< Pre-shared keys authentication */
                AUTH_METHOD_DSS = 3,          /**< DSS authentication */
            };

            /****************************** METHODS ******************************/
        public:
            /**
             * Returns the textual representation of a IPSEC_MODE value
             * @param mode IPSEC_MODE value
             * @return Textual respresentation
             */
            static string IPSEC_MODE_STR( IPSEC_MODE mode );

            /**
             * Returns the textual representation of a POLICY_TYPE value
             * @param mode POLICY_TYPE value
             * @return Textual respresentation
             */
            static string POLICY_TYPE_STR( POLICY_TYPE policy_type );



            /**
             * Returns the textual representation of a POLICY_ACTION value
             * @param mode POLICY_ACTION value
             * @return Textual respresentation
             */
            static string POLICY_ACTION_STR( POLICY_ACTION policy_action );


            /**
             * Returns the textual representation of a ATTRIBUTE_TYPE value
             * @param attr_type ATTRIBUTE_TYPE value
             * @return Textual respresentation
             */
            static string ATTRIBUTE_TYPE_STR( ATTRIBUTE_TYPE attr_type );

            /**
             * Returns the textual representation of a TRANSFORM_TYPE value
             * @param transform_type TRANSFORM_TYPE value
             * @return Textual respresentation
             */
            static string TRANSFORM_TYPE_STR( TRANSFORM_TYPE transform_type );

            /**
             * Returns the textual representation of a ENCR_ID value
             * @param encr_id ENCR_ID value
             * @return Textual respresentation
             */
            static string ENCR_ID_STR( ENCR_ID encr_id );

            /**
             * Returns the textual representation of a PRF_ID value
             * @param prf_id PRF_ID value
             * @return Textual respresentation
             */
            static string PRF_ID_STR( PRF_ID prf_id );

            /**
             * Returns the textual representation of a INTEG_ID value
             * @param integ_id INTEG_ID value
             * @return Textual respresentation
             */
            static string INTEG_ID_STR( INTEG_ID integ_id );

            /**
             * Returns the textual representation of a DH_ID value
             * @param dh_id DH_ID value
             * @return Textual respresentation
             */
            static string DH_ID_STR( DH_ID dh_id );

            /**
             * Returns the textual representation of a ESN_ID value
             * @param esn_id ESN_ID value
             * @return Textual respresentation
             */
            static string ESN_ID_STR( ESN_ID esn_id );

            /**
             * Returns the textual representation of a PROTOCOL_ID value
             * @param protocol PROTOCOL_ID value
             * @return Textual respresentation
             */
            static string PROTOCOL_ID_STR( PROTOCOL_ID protocol );

            /**
             * Returns the textual representation of a DIRECTION value
             * @param dir DIRECTION value
             * @return Textual respresentation
             */
            static string DIRECTION_STR( DIRECTION dir );

            /**
             * Returns the textual representation of a ROLE_ID value
             * @param role_id ROLE_ID value
             * @return Textual respresentation
             */
            static string ROLE_ID_STR( ROLE_ID role_id );

            /**
             * Returns the textual representation of a ID_TYPE value
             * @param id_type ID_TYPE value
             * @return Textual respresentation
             */
            static string ID_TYPE_STR( ID_TYPE id_type );

            /**
             * Returns the textual representation of a IP protocol ID
             * @param ip_proto IP protocol ID
             * @return Textual respresentation
             */
            static string IP_PROTO_STR( uint8_t ip_proto );

            /**
             * Translates from encoding ID to textual representation
             * @param encoding Certificate encoding
             * @return Textual representation
             */
            static string CERT_ENCODING_STR( uint16_t encoding );

            /**
             * Translate auth method ID to string
             * @param auth_method Auth method to translate
             * @return String with the translation
             */
            static string AUTH_METHOD_STR( AUTH_METHOD auth_method );
    };
}
#endif
