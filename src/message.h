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
#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "payload.h"
#include "payload_sk.h"
#include "cipher.h"
#include "payload_notify.h"
#include "ipaddress.h"
#include "autovector.h"
#include "socketaddress.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents an IKE message.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Message : public Printable {

            /****************************** ENUMS ******************************/
        public:
            /** Exhange types */
            enum EXCHANGE_TYPE {
                IKE_SA_INIT = 34,   /**< IKE_SA_INIT exchange */
                IKE_AUTH,           /**< IKE_AUTH exchange */
                CREATE_CHILD_SA,    /**< CREATE_CHILD_SA exchange */
                INFORMATIONAL       /**< INFORMATIONAL exchange */
            };

            /** Message types */
            enum MESSAGE_TYPE {
                REQUEST = 0,        /**< Request */
                RESPONSE            /**< Response */
            };

            /****************************** ATTRIBUTES ******************************/
        protected:
            AutoVector<Payload> unencrypted_payloads;   /**< Unencrypted Payload collection. */
            AutoVector<Payload> encrypted_payloads;     /**< Encrypted Payload collection. */
            Payload::PAYLOAD_TYPE first_payload_type;   /**< The type of the first payload in the mensage */
            Payload::PAYLOAD_TYPE first_payload_type_sk;/**< The type of the first payload in the payload_sk */
            auto_ptr<ByteArray> binary_representation;  /**< Message binary representation. */
            auto_ptr<Payload_SK> payload_sk;            /**< Message Payload_SK */

        public:
            auto_ptr<SocketAddress> src_addr;           /**< Source address */
            auto_ptr<SocketAddress> dst_addr;           /**< Destination address */
            bool can_use_higher_major_version;          /**< This flag indicates that the transmiter is able to speaking a higher major version. */
            uint8_t major_version;                      /**< Message protocol major version. */
            uint8_t minor_version;                      /**< Message protocol minor version. */
            EXCHANGE_TYPE exchange_type;                /**< Message exchange type. */
            MESSAGE_TYPE message_type;                  /**< Indicates if the message is a request or a response. */
            bool is_initiator;                          /**< This flag indicates that the transmitter is the original initiator. */
            uint64_t spi_i;                             /**< Initiator SPI. */
            uint64_t spi_r;                             /**< Responder SPI. */
            uint32_t message_id;                        /**< Message ID. */


            /****************************** METHODS ******************************/
        protected:
            /**
             * Generates all the payloads based on its binary representation
             * @param first_payload_type Type of the first payload
             * @param byte_buffer Concatenated binary representation of the payloads
             * @param payloads Collection to be filled by this method with the generated payloads
             * @return The next payload type of the last fixed payload header
             */
            static Payload::PAYLOAD_TYPE generatePayloads( Payload::PAYLOAD_TYPE first_payload_type, ByteBuffer& byte_buffer, vector<Payload*> &payloads );

            /**
             * Generates the binary representation of a Payload collection
             * @param last_payload_type The last payload type
             * @param payloads Payload Collection
             * @return Binary representation of the Payloads
             */
            static auto_ptr<ByteArray> generateBinaryRepresentation( Payload::PAYLOAD_TYPE last_payload_type, const vector<Payload*> payloads );

        public:
            /**
             * Creates a new Message, setting its attributes
             * @param src_addr Source address
             * @param dst_addr Destination address
             * @param spi_i Initiator SPI.
             * @param spi_r Responder SPI.
             * @param major_version Message protocol major version.
             * @param minor_version Message protocol minor version.
             * @param exchange_type Message exchange type.
             * @param is_initiator Is the original initiator.
             * @param message_type  Message type (REQUEST or RESPONSE)
             * @param can_use_higher_major_version The transmiter is able to speaking a higher major version.
             * @param message_id Message ID.
             */
            Message( auto_ptr<SocketAddress> src_addr, auto_ptr<SocketAddress> dst_addr, uint64_t spi_i, uint64_t spi_r, uint8_t major_version, uint8_t minor_version, EXCHANGE_TYPE exchange_type, MESSAGE_TYPE message_type, bool is_initiator, bool can_use_higher_major_version, uint32_t message_id );

            /**
             * Creates a new Message based on its binary representation.
             * @param src_addr Source address
             * @param dst_addr Destination address
             * @param byte_buffer Buffer containing the binary representation of the message
             */
            Message( auto_ptr<SocketAddress> src_addr, auto_ptr<SocketAddress> dst_addr, ByteBuffer& byte_buffer );

            /**
             * Creates a new Message cloning another one
             * @param other Other Message object to be cloned
             */
            Message( const Message& other );

            /**
             * Gets all the Payloads of the indicated type found in the Message
             * @param type Type of the payloads
             * @return  A payload collection
             */
            vector<Payload*> getPayloadsByType( Payload::PAYLOAD_TYPE type ) const;

            /**
             * Gets the first Payload with that type found in the Message
             * @param type Type of the Payload
             * @return  The first payload found. NULL if none is found.
             */
            Payload* getFirstPayloadByType( Payload::PAYLOAD_TYPE type ) const;

            /**
             * Gets the unique Payload with the indicated type found in the Message.
             * There must be one and only one Payload of the indicated type, otherwise ParsingException is thrown
             * @param type Type of the Payload
             * @return The searched Payload
             */
            Payload& getUniquePayloadByType( Payload::PAYLOAD_TYPE type ) const;

            /**
             * Gets a collection of all the notification payloads of the indicated type.
             * @param notification_type Type of the notification payloads.
             * @return Collection of notification payloads.
             */
            vector<Payload_NOTIFY*> getNotifiesByType( Payload_NOTIFY::NOTIFY_TYPE notification_type ) const ;

            /**
             * Gets the first notification payload of the indicated type.
             * @param notification_type Type of the notification payload.
             * @return The first notification payloa of the specified type. NULL if none if found
             */
            Payload_NOTIFY* getFirstNotifyByType( Payload_NOTIFY::NOTIFY_TYPE notification_type ) const;

            /**
             * Gets the unique notification payload with the indicated notification type found in the Message.
             * There must be one and only one notification payload of the indicated type, otherwise ParsingException is thrown
             * @param notification_type Type of the notification payload
             * @return The searched notification payload
             */
            Payload_NOTIFY& getUniqueNotifyByType( Payload_NOTIFY::NOTIFY_TYPE notification_type ) const;

            /**
             * Adds a Payload to the Message
             * @param payload Payload to be added.
             * @param is_encrypted Indicates if payload must be encrypted.
             */
            void addPayload( auto_ptr<Payload> payload, bool is_encrypted );

            /**
             * Adds some Payloads to the Message
             * @param payloads Payloads to be added.
             * @param is_encrypted Indicates if payloads must be encrypted.
             */
            void addPayloads( AutoVector<Payload> payloads, bool is_encrypted );

            /**
             * Replaces the first payload of the indicated type.
             * @param type Payload type to be replaced.
             * @param new_payload New payload to be included in the Message.
             */
            void replaceFirstPayloadByType( Payload::PAYLOAD_TYPE type, auto_ptr<Payload> new_payload );

            /**
             * Replaces the first notify payload of the indicated type
             * @param notify_type
             * @param new_payload
             */
            void replaceFirstNotifyByType( Payload_NOTIFY::NOTIFY_TYPE notify_type, auto_ptr< Payload_NOTIFY > new_payload );

            /**
             * Adds a notification payload. This Payload will be inserted in the first position of the Message.
             * @param notify_payload Notification Payload to be inserted.
             * @param is_encrypted Indicates if notify payload must be encrypted.
             */
            void addPayloadNotify( auto_ptr<Payload_NOTIFY> notify_payload, bool is_encrypted );

            /**
             * Add a notification payload collection. This payloads will be inserted in the firsts positions of the Message
             * @param notifies Payload NOTIFY collection
             * @param is_encrypted Indicates if the notifies must be encrypted
             */
            void addPayloadsNotify( AutoVector<Payload_NOTIFY> notifies, bool is_encrypted );

            /**
             * Gets the binary representation of the Message. If still not generated, the generate it.
             * @param cipher Cipher to be used to generate the Payload_SK. NULL if not applicable
             * @return Binary representation of the Message
             */
            ByteArray& getBinaryRepresentation( Cipher *cipher );

            /**
             * Decrypts the Payload_SK and generates the contained Payloads
             * The geneated Payloads are stored into the encrypted_payloads collection
             * @param cipher Cipher used to decrypt the Payload_SK (NULL if not applicable)
             */
            void decryptPayloadSK( Cipher *cipher );

            /**
             * Checks the Message integrity
             * @param cipher Cipher used to check integrity (NULL if not applicable)
             * @return TRUE if integrity is ok. FALSE otherwise
             */
            bool checkIntegrity( Cipher * cipher ) const;

            /**
             * Clone this message
             * @return Message clon
             */
            auto_ptr<Message> clone() const;

            /**
             * Gets the source addres of the Message
             * @return The source address of the Message
             */
            SocketAddress& getSrcAddress() const;

            /**
             * Gets the destiantion addres of the Message
             * @return The destiantion address of the Message
             */
            SocketAddress& getDstAddress() const;


            /**
             * Translates from exchange type IDs to strings in order to be easily recognized
             * @param exchange_type Exchange type ID
             * @return Returns the name of the specified exchange type ID
             */
            static string EXCHANGE_TYPE_STR( EXCHANGE_TYPE exchange_type );

            /**
             * Translates from message type to strings in order to be easily recognized
             * @param message_type Message type
             * @return Returns the name of the specified message type
             */
            static string MESSAGE_TYPE_STR( MESSAGE_TYPE message_type );

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~Message();

    };
};
#endif
