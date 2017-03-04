/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
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
#ifndef OPENIKEV2EAPPACKET_H
#define OPENIKEV2EAPPACKET_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "bytebuffer.h"

namespace openikev2 {
    /**
        This class represents an EAP packet
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class EapPacket : public Printable {

            /****************************** ENUMS ******************************/
        public:
            /** EAP codes */
            enum EAP_CODE {
                EAP_CODE_NONE = 0,      /**< EAP NONE */
                EAP_CODE_REQUEST = 1,   /**< EAP request */
                EAP_CODE_RESPONSE = 2,  /**< EAP response */
                EAP_CODE_SUCCESS = 3,   /**< EAP success */
                EAP_CODE_FAILURE = 4,   /**< EAP failure */
            };

            /** EAP types */
            enum EAP_TYPE {
                EAP_TYPE_NONE = 0,                      /**< RESERVED  */
                EAP_TYPE_IDENTITY = 1,                  /**< Identity  */
                EAP_TYPE_NOTIFICATION = 2,              /**< Notification  */
                EAP_TYPE_NAK = 3,                       /**< Legacy NAK  */
                EAP_TYPE_MD5_CHALLENGE = 4,             /**< MD5-Challenge  */
                EAP_TYPE_ONE_TIME_PASSWD = 5,           /**< One time password (OTP)  */
                EAP_TYPE_GENERIC_TOKEN_CARD = 6,        /**< Generic Token Card (GTC)  */
                EAP_TYPE_RSA_PK_AUTH = 9,               /**< RSA Public Key Authentication */
                EAP_TYPE_DSS_UNILATERAL = 10,           /**< DSS Unilateral */
                EAP_TYPE_KEA = 11,                      /**< KEA */
                EAP_TYPE_KEA_VALIDATE = 12,             /**< KEA-VALIDATE */
                EAP_TYPE_EAP_TLS = 13,                  /**< EAP-TLS */
                EAP_TYPE_DEFENDER_TOKEN = 14,           /**< Defender token (AXENT) */
                EAP_TYPE_RSA_SECURID = 15,              /**< RSA Security SecurID EAP */
                EAP_TYPE_ARCOT_SYSTEMS = 16,            /**< Arcot Systems EAP */
                EAP_TYPE_CISCO_WIRELESS = 17,           /**< EAP-Cisco Wireless */
                EAP_TYPE_EAP_SIM = 18,                  /**< GSM Subscriber Identity Modules (EAP-SIM) */
                EAP_TYPE_SRP_SHA1 = 19,                 /**< SRP-SHA1 */
                EAP_TYPE_EAP_TTLS = 21,                 /**< EAP-TTLS */
                EAP_TYPE_RAS = 22,                      /**< Remote Access Service */
                EAP_TYPE_EAP_AKA = 23,                  /**< EAP-AKA Authentication */
                EAP_TYPE_EAP_3COM_WIRELESS = 24,        /**< EAP-3Com Wireless */
                EAP_TYPE_PEAP = 25,                     /**< PEAP */
                EAP_TYPE_MS_EAP_AUTH = 26,              /**< MS-EAP-Authentication */
                EAP_TYPE_MAKE = 27,                     /**< Mutual Authentication w/Key Exchange (MAKE) */
                EAP_TYPE_CRYPTO_CARD = 28,              /**< CRYPTOCard */
                EAP_TYPE_EAP_MSCHAP_V2 = 29,            /**< EAP-MSCHAP-V2 */
                EAP_TYPE_DYNAM_ID = 30,                 /**< DynamID */
                EAP_TYPE_ROB_EAP = 31,                  /**< Rob EAP */
                EAP_TYPE_POTP = 32,                     /**< Protected One-Time Password */
                EAP_TYPE_MS_AUTH_TLV = 33,              /**< MS-Authentication-TLV */
                EAP_TYPE_SENTRINET = 34,                /**< SentriNET */
                EAP_TYPE_EAP_ACTIONTEC_WIRELESS = 35,   /**< EAP-Actiontec Wireless */
                EAP_TYPE_COGENT_BIO_AUTH = 36,          /**< Cogent Systems Biometrics Authentication EAP */
                EAP_TYPE_AIRFORTRESS = 37,              /**< AirFortress EAP */
                EAP_TYPE_HTTP_DIGEST = 38,              /**< EAP-HTTP Digest */
                EAP_TYPE_SECURE_SUITE = 39,             /**< SecureSuite EAP */
                EAP_TYPE_DEVICE_CONNECT = 40,           /**< DeviceConnect EAP */
                EAP_TYPE_SPEKE = 41,                    /**< EAP-SPEKE */
                EAP_TYPE_MOBAC = 42,                    /**< EAP-MOBAC */
                EAP_TYPE_FAST = 43,                     /**< EAP-FAST */
                EAP_TYPE_ZLXEAP = 44,                   /**< ZoneLabs EAP (ZLXEAP) */
                EAP_TYPE_LINK = 45,                     /**< EAP-Link */
                EAP_TYPE_PAX = 46,                      /**< EAP-PAX */
		// EAP_FRM (necesita concretarse el numero)
		EAP_TYPE_FRM = 253,			/**< EAP-FRM */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            EAP_CODE code;                      /**< EAP message code */
            uint8_t identifier;                 /**< EAP message identifier */
            EAP_TYPE eap_type;                  /**< EAP message type */
            auto_ptr<ByteArray> eap_type_data;  /**< EAP message type data */

        public:
            /**
             * Creates a new EapPacket containing a REQUEST/RESPONSE
             * @param code EAP message code (only REQUEST or RESPONSE)
             * @param identifier EAP message identifier
             * @param eap_type EAP message type
             * @param eap_type_data EAP message data type
             */
            EapPacket( EAP_CODE code, uint8_t identifier, EAP_TYPE eap_type, auto_ptr<ByteArray> eap_type_data );

            /**
             * Creates a new EapPacket containing a SUCCESS/FAILURE
             * @param code EAP message code (only SUCCESS or FAILURE)
             * @param identifier EAP message identifier
             */
            EapPacket( EAP_CODE code, uint8_t identifier );

           /**
             * Creates a new EapPacket cloning another one.
             * @param other Other EapPacket to be cloned.
             */
            EapPacket( const EapPacket& other );

            /**
             * Creates a new EapPacket based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<EapPacket> parse( ByteBuffer& byte_buffer );

            /**
             * Translate the enumeration values to String
             * @param eap_code Enumeration value
             * @return String with the eap_code
             */
            static string EAP_CODE_STR( uint16_t eap_code );

            /**
             * Translate the enumeration values to String
             * @param eap_type Enumeration value
             * @return String with the eap_type
             */
            static string EAP_TYPE_STR( uint16_t eap_type );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual auto_ptr<EapPacket> clone() const;

            virtual ~EapPacket();

    };

}

#endif
