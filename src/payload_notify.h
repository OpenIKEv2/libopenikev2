/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#ifndef PAYLOAD_NOTIFY_H
#define PAYLOAD_NOTIFY_H

#include "payload.h"
#include "proposal.h"
#include "utils.h"

namespace openikev2 {

    /**
        This class represents a NOTIFY Payload.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Payload_NOTIFY : public Payload {

            /****************************** ENUMS ******************************/
        public:
            /** Notify Types */
            enum NOTIFY_TYPE {
                UNSUPPORTED_CRITICAL_PAYLOAD = 1,         /**< Unsupported critical payload  */
                INVALID_IKE_SPI = 4,                      /**< Invalid IKE SPI */
                INVALID_MAJOR_VERSION = 5,                /**< Invalid Major Version */
                INVALID_SYNTAX = 7,                       /**< Invalid syntax */
                INVALID_MESSAGE_ID = 9,                   /**< Invalid message ID */
                INVALID_SPI = 11,                         /**< Invalid SPI */
                NO_PROPOSAL_CHOSEN = 14,                  /**< No proposal chosen */
                INVALID_KE_PAYLOAD = 17,                  /**< Invalid KE payload */

                AUTHENTICATION_FAILED = 24,               /**< Authentication failed */
                SINGLE_PAIR_REQUIRED = 34,                /**< Single pair required */
                NO_ADDITIONAL_SAS = 35,                   /**< No additional SAs */
                INTERNAL_ADDRESS_FAILURE = 36,            /**< Internal address failure */
                FAILED_CP_REQUIRED = 37,                  /**< Failed Configuration Payload required */
                TS_UNACCEPTABLE = 38,                     /**< Traffic selector unacceptable */
                INVALID_SELECTORS = 39,                   /**< Invalid selectors */

                INITIAL_CONTACT = 16384,                  /**< Initial contact */
                SET_WINDOW_SIZE = 16385,                  /**< Set window size */
                ADDITIONAL_TS_POSSIBLE = 16386,           /**< Additional Traffic selector possible */
                IPCOMP_SUPPORTED = 16387,                 /**< IPcomp supported */
                NAT_DETECTION_SOURCE_IP = 16388,          /**< NAT detection source ip */
                NAT_DETECTION_DESTINATION_IP = 16389,     /**< NAT detection destination ip */
                COOKIE = 16390,                           /**< Cookie */
                USE_TRANSPORT_MODE = 16391,               /**< Use transport mode */
                HTTP_CERT_LOOKUP_SUPPORTED = 16392,       /**< HTTP certificate lookup supported  */
                REKEY_SA = 16393,                         /**< Rekey SA */
                ESP_TFC_PADDING_NOT_SUPPORTED = 16394,    /**< ESP TFC padding not supported */
                NON_FIRST_FRAGMENT_ALSO = 16395,          /**< Non first fragment also */
            };

            /****************************** ATTRIBUTES ******************************/
        public:
            Enums::PROTOCOL_ID protocol_id;         /**< Protocol type of the SA being notificated. If not applicable, then PROTO_NONE */
            NOTIFY_TYPE notification_type;          /**< Notification type */
            auto_ptr<ByteArray> spi_value;          /**< SPI of the notificated SA */
            auto_ptr<ByteArray> notification_data;  /**< Information or error data transmitted */

            /****************************** METHODS ******************************/
        public:

            /**
             * Creates a new Payload_NOTIFY setting its parameters.
             * @param notification_type Notify type.
             * @param protocol_id Protocol type.
             * @param spi_value SPI value (NULL when not applicable)
             * @param notification_data Notification data (NULL when not applicable)
             */
            Payload_NOTIFY( NOTIFY_TYPE notification_type, Enums::PROTOCOL_ID protocol_id, auto_ptr<ByteArray> spi_value, auto_ptr<ByteArray> notification_data );

            /**
             * Creates a new Payload_NOTIFY setting its parameters when no SPI and DATA is available
             * @param notification_type Notify type.
             * @param protocol_id Protocol type.
             */
            Payload_NOTIFY( NOTIFY_TYPE notification_type, Enums::PROTOCOL_ID protocol_id );

            /**
             * Creates a new Payload_NOTIFY cloning another one.
             * @param other Other Payload_NOTIFY to be cloned.
             */
            Payload_NOTIFY( const Payload_NOTIFY& other );

            /**
             * Creates a new Payload_NOTIFY based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "payload length" field
             */
            static auto_ptr<Payload_NOTIFY> parse( ByteBuffer& byte_buffer );

            /**
             * Indicates if the type of this Payload_NOTIFY is an error notify.
             * @return True if it is an error notify. False otherwise.
             */
            virtual bool isError();

            /**
             * Translate the enumeration values to String
             * @param notify_type Enumeration value
             * @return String with the notify type
             */
            static string NOTIFY_TYPE_STR( NOTIFY_TYPE notify_type );

            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual auto_ptr<Payload> clone() const;

            virtual ~Payload_NOTIFY();

    };
};
#endif
