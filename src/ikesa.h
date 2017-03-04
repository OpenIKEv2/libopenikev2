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
#ifndef IKE_SA_H
#define IKE_SA_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <deque>

#include "alarmable.h"
#include "message.h"
#include "ipaddress.h"
#include "diffiehellman.h"
#include "payload_nonce.h"
#include "payload_ke.h"
#include "payload_notify.h"
#include "mutex.h"
#include "keyring.h"
#include "cipher.h"
#include "childsa.h"
#include "alarm.h"
#include "id.h"
#include "peerconfiguration.h"
#include "attributemap.h"
#include "payload_conf.h"
#include "childsacollection.h"

namespace openikev2 {
    class Command;

    /**
        This class represents an IKE_SA Controller, that controls an IKE_SA and stores all its attributes and status.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class IkeSa : public Alarmable {

            /****************************** ENUMS ******************************/
        public:
            /** IKE_SA States */
            enum IKE_SA_STATE {
                // INITIAL STATE
                STATE_INITIAL,                                  /**< Initial state. All the newly created IKE_SAs start in this state.*/

                // HALF_OPEN STATES
                // initiator states
                STATE_IKE_SA_INIT_REQ_SENT,                     /**< IKE_SA_INIT exchange request has been sent */
                STATE_IKE_AUTH_REQ_SENT,                        /**< IKE_AUTH exchange request has been sent */
                STATE_IKE_AUTH_EAP_INIT_REQ_SENT,               /**< IKE_AUTH exchange request has been sent without AUTH payload (EAP init) */
                STATE_IKE_AUTH_EAP_CONT_REQ_SENT,               /**< IKE_AUTH exchange request has been sent with only an EAP payload (EAP continue) */
                STATE_IKE_AUTH_EAP_FINISH_REQ_SENT,             /**< IKE_AUTH exchange request has been sent with AUTH payload (EAP finish) */

                // responder states
                STATE_IKE_SA_INIT_RES_SENT,                     /**< IKE_SA_INIT exchange response has been sent */
                STATE_IKE_AUTH_EAP_INIT_RES_SENT,               /**< IKE_AUTH exchange response has been sent without AUTH paylaod (EAP init) */
                STATE_IKE_AUTH_EAP_CONT_RES_SENT,               /**< IKE_AUTH exchange response has been sent with only an EAP payload (EAP coninue) */
                STATE_IKE_AUTH_EAP_SUCCESS_RES_SENT,            /**< IKE_AUTH exchange response has been sent with AUTH payload (EAP success) */

                // ESTABLISHED STATES
                STATE_IKE_SA_ESTABLISHED,                       /**< IKE_SA is already established */
                STATE_NEW_CHILD_SA_REQ_SENT,                    /**< New CREATE_CHILD_SA exchange has been sent to create a new Child SA */
                STATE_REKEY_CHILD_SA_REQ_SENT,                  /**< New CREATE_CHILD_SA exchange has been sent to rekey a Child SA */
                STATE_REKEY_IKE_SA_REQ_SENT,                    /**< New CREATE_CHILD_SA exchange has been sent to rekey an IKE SA */
                STATE_DELETE_CHILD_SA_REQ_SENT,                 /**< New INFORMATIONAL exchange has been sent to delete a Child SA */
                STATE_DELETE_IKE_SA_REQ_SENT,                   /**< New INFORMATIONAL exchange has been sent to delete an IKE SA */
                STATE_GENERIC_INFORMATIONAL_REQ_SENT,           /**< New INFORMATIONAL exchange has been sent */
                STATE_REDUNDANT_CHILD_SA,                       /**< A redundant Child SA is being created */
                STATE_REDUNDANT_IKE_SA,                         /**< A redundant IKE SA is being created */

                STATE_WAITING_FOR_DELETION,                     /**< The IKE SA is waiting for deletion */
                STATE_CLOSED,                                   /**< The IKE SA has been closed */

                STATE_MAX,                                      /**< Maximun state value */
            };

            /** Received Payload Notify action */
            enum NOTIFY_ACTION {
                NOTIFY_ACTION_CONTINUE = 0,                     /**< Continue processing message */
                NOTIFY_ACTION_ERROR = 1,                        /**< Stop processing message */
                NOTIFY_ACTION_OMIT = 2,                         /**< Omit message (like if it had never been recived) */
            };

            enum NEGOTIATION_ACTION {
                NEGOTIATION_ACTION_CONTINUE = 0,                /**< Continue processing message */
                NEGOTIATION_ACTION_ERROR = 1,                   /**< Stop processing message */
            };

            /** Message action */
            enum MESSAGE_ACTION {
                MESSAGE_ACTION_COMMIT = 0,                      /**< Continue processing message */
                MESSAGE_ACTION_OMIT = 2,                        /**< Omit message */
                MESSAGE_ACTION_DELETE_IKE_SA = 1,             /**< Close IKE_SA */
            };

            enum IKE_SA_ACTION {
                IKE_SA_ACTION_CONTINUE,
                IKE_SA_ACTION_DELETE_IKE_SA,
            };



            /****************************** ATTRIBUTES ******************************/
        protected:
            IKE_SA_STATE state;                                     /**< IKE SA state */
            auto_ptr<PeerConfiguration> peer_configuration;         /**< Peer configuration */
            deque<Command*> command_queue;                          /**< Command Queue */
            deque<Command*> deferred_queue;                         /**< Deferred Command Queue */
            auto_ptr<Alarm> idle_ike_sa_alarm;                      /**< Idle IKE SA notification alarm */
            auto_ptr<Alarm> rekey_ike_sa_alarm;                     /**< Rekey IKE SA notification alarm */
            auto_ptr<Alarm> halfopen_alarm;                         /**< Alarm limiting the negotiation time of the IKE SA */
            auto_ptr<Mutex> mutex;                                  /**< Mutex to protect IKE_SA accesses */

        public:
            uint64_t my_spi;                                        /**< Our SPI */
            uint64_t peer_spi;                                      /**< Peer SPI */
            auto_ptr<SocketAddress> my_addr;                        /**< Our address */
            auto_ptr<SocketAddress> peer_addr;                      /**< Peer IP address */
            uint32_t my_message_id;                                 /**< Current own message id */
            uint32_t peer_message_id;                               /**< Expected peer message id */
            bool is_initiator;                                      /**< Indicates if we are the original initiator */
            bool is_auth_initiator;                                 /**< Indicates if we are the original authentication initiator */
            auto_ptr<Message> ike_sa_init_req;                      /**< IKE_SA_INIT request message. It is stored in order to generate and check the AUTH payload */
            auto_ptr<Message> ike_sa_init_res;                      /**< IKE_SA_INIT response message. It is stored in order to generete and check the AUTH payload */
            auto_ptr<Message> last_sent_request;                    /**< Last sent request */
            auto_ptr<Message> last_sent_response;                   /**< Last sent response */
            auto_ptr<Message> eap_init_req;                         /**< EAP INIT request message */
            uint32_t remaining_timeout_retries;                     /**< Remaining retries to send the current request */
            auto_ptr<Alarm> retransmition_alarm;                    /**< Retransmition alarm */
            bool is_half_open;                                      /**< Indicates if this IKE_SA is half open */
            auto_ptr<ID> my_id;                                     /**< Our identification */
            auto_ptr<ID> peer_id;                                   /**< Peer identification */
            auto_ptr<AttributeMap> attributemap;                    /**< Extra attributes. This attribute will be inherit by the new IKE_SA when rekeying */
            bool peer_supports_hash_url;                            /**< Indicates if peer supports HASH & URL certificates */
            auto_ptr<ChildSa> my_creating_child_sa;                 /**< CHILD SA being created by us */
            auto_ptr<ChildSa> peer_creating_child_sa;               /**< CHILD SA being created by the peer */
            auto_ptr<ByteArray> my_nonce;                           /**< Our nonce payload */
            auto_ptr<ByteArray> peer_nonce;                         /**< Peer nonce payload */
            auto_ptr<IkeSa> my_creating_ike_sa;                     /**< IKE SA being created by us */
            auto_ptr<IkeSa> peer_creating_ike_sa;                   /**< IKE SA being created by the peer */
            auto_ptr<ByteArray> my_vendor_id;                       /**< Our VENDOR ID */
            auto_ptr<ByteArray> peer_vendor_id;                     /**< Peer VENDOR ID */
            auto_ptr<DiffieHellman> dh;                             /**< Diffie Hellman used to compute IKE SA shared secret */
            auto_ptr<Cipher> send_cipher;                           /**< Cipher used to encrypt outgoing messages */
            auto_ptr<Cipher> receive_cipher;                        /**< Cipher used to decrypt incoming messages */
            auto_ptr<PseudoRandomFunction> prf;                     /**< PRF used to create AUTH payloads and more */
            auto_ptr<KeyRing> key_ring;                             /**< IKE SA KeyRing */
            auto_ptr<ChildSaCollection> child_sa_collection;        /**< Child SA collection */

            bool mobility;
            bool is_ha;
            auto_ptr<SocketAddress> home_address;
	        auto_ptr<SocketAddress> care_of_address;

            /****************************** METHODS ******************************/
        protected:
            /**
             * Base IkeSa creation method. Used in all the different constructors.
             * @param my_spi IKE_SA SPI
             * @param is_initiator Indicates if we are initiators
             * @param my_addr Own IP Address
             * @param peer_addr Peer IP Address
             */
            void base( uint64_t my_spi, bool is_initiator, auto_ptr<SocketAddress> my_addr, auto_ptr<SocketAddress> peer_addr );

            /**
            * Determines what IkeSa has the minimal nonce value
            * @param ike_sa1 IkeSa object to compare its nonces
            * @param ike_sa2 IkeSa object to compare its nonces
            * @return The IkeSa with the minimal nonce value
            */
            static IkeSa& hasMinNonce( const IkeSa& ike_sa1, const IkeSa& ike_sa2 );

            /**
             * Extracts the first Message from the deferred queue
             * @return Extracted Command
             */
            auto_ptr<Command> popDeferredCommand();

            /**
             * Extracts the first Message from the queue.
             * If there are deferred Commands, and the state == IKE_SA_ESTABLISHED, then search first in the deferred queue
             * @return Extracted Command
             */
            auto_ptr<Command> popCommand();

            /**
             * Checks the peer SPI value of the Message.
             * @param message Message to be checked.
             * @return TRUE if is valid. FALSE otherwise.
             */
            bool checkPeerIkeSpi( Message &message );

            /**
             * Checks the sequence number of the MEssage
             * @param message Message to be checked.
             * @return TRUE if is valid. FALSE otherwise.
             */
            bool checkMessageId( Message &message );

            /**
             * Creates physically a new ChildSa and adds it into collection.
             * The ChildSa must have updated the two SPI values, the proposal, the selectors and the DH value (if needed).
             * The keyring will be created by this method
             * @param child_sa ChildSa to be created
             */
            void createChildSa( auto_ptr<ChildSa> child_sa );

            void createRekeyChildSa( auto_ptr<ChildSa> child_sa, ChildSa& rekeyed_sa );

            /**
             * Inherit status from another IkeSa
             * @param other Another IkeSa
             */
            void inheritIkeSaStatus( IkeSa& other );

            /**
             * Obtains the IKE proposal for this IkeSa
             * @return The IKE proposal
             */
            Proposal& getProposal() const;

            /**
             * Sets the IKE proposal for this IkeSa
             * @param proposal The IKE proposal
             */
            void setProposal( auto_ptr<Proposal> proposal );

            /**
             * Creates the IKE_SA negotiation payloads and includes them into the request Message
             * @param message Request Message where include the payloads
             * @param ike_sa Negotiated IkeSa
             */
            void createIkeSaNegotiationRequest( Message& message, IkeSa & ike_sa );

            /**
             * Process the IKE_SA negotiation payloads response
             * @param message Response Message with the needed payloads
             * @param ike_sa Negotiated IkeSa
             * @return Action to be performed after message processing
             */
            NEGOTIATION_ACTION processIkeSaNegotiationResponse( Message& message, IkeSa & ike_sa );

            /**
             * Process the IKE_SA negotiation payloads request
             * @param message Request Message with the needed payloads
             * @param ike_sa Negotiated IkeSa
             * @return Action to be performed after message processing
             */
            NEGOTIATION_ACTION processIkeSaNegotiationRequest( Message& message, IkeSa & ike_sa );

            /**
             * Creates the IKE_SA negotiation payloads and includes them into the response Message
             * @param message Response Message where include the payloads
             * @param ike_sa Negotiated IkeSa
             */
            void createIkeSaNegotiationResponse( Message& message, IkeSa & ike_sa );

            /**
             * Creates the CHILD_SA negotiation payloads and includes them into the request Message
             * @param message Request Message where include the payloads
             */
            void createChildSaNegotiationRequest( Message& message );

            /**
             * Process the CHLD_SA negotiation payloads response
             * @param message Response Message with the needed payloads
             * @return Action to be performed after message processing
             */
            NEGOTIATION_ACTION processChildSaNegotiationResponse( Message& message );

            /**
             * Process the CHLD_SA negotiation payloads request
             * @param message Request Message with the needed payloads
             * @return Action to be performed after message processing
             */
            NEGOTIATION_ACTION processChildSaNegotiationRequest( Message& message );

            /**
             * Creates the CHILD_SA negotiation payloads and includes them into the response Message
             * @param message Response Message where include the payloads
             */
            void createChildSaNegotiationResponse( Message& message );

            void createRekeyIkeSa( auto_ptr<IkeSa> new_ike_sa );

            /**
             * Sends the message, writes the log output and stores it as last_sent_request or last_sent_response. If REQUEST, then also initiates the
             * retransmition alarm
             * @param message Message to be sent
             * @param text Texto to describe what it being sent
             */
            void sendMessage( auto_ptr<Message> message, string text );

        public:
            /**
             * Creates a new IkeSa setting its parameters.
             * @param my_spi SPI given to this IKE_SA.
             * @param is_initiator Indicates if this is the initiator.
             * @param my_addr Own IpAddress used to communicate with the Peer.
             * @param peer_addr Peer IpAddress wich communicates with us.
             */
            IkeSa( uint64_t my_spi, bool is_initiator, auto_ptr<SocketAddress> my_addr, auto_ptr<SocketAddress> peer_addr );

            /**
             * Creates a new IkeSa in a Rekey process.
             * @param my_spi SPI given to this IKE_SA.
             * @param is_initiator Indicates if this is the initiator.
             * @param rekeyed_ike_sa The IKE_SA being rekeyed
             */
            IkeSa( uint64_t my_spi, bool is_initiator, const IkeSa& rekeyed_ike_sa );

            /**
             * Gets a textual representation of an IKE_SA state
             * @param state IKE_SA state
             * @return The textual representation
             */
            static string IKE_SA_STATE_STR( IKE_SA_STATE state );

            /**
             * Processes the next Command in the queues
             * @return The IKE_SA action to be performed after the command execution
             */
            IKE_SA_ACTION processCommand();

            /**
             * Close current IkeSa.
             */
            IKE_SA_ACTION close();

            /**
             * Changes the IKE_SA state
             * @param state New IKE_SA state
             */
            void setState( IKE_SA_STATE state );

            /**
             * Gets the current IKE_SA state
             * @return Current IKE_SA state
             */
            IKE_SA_STATE getState() const;

            /**
             * Checks if this IkeSa has more commands to be executed.
             * @return TRUE if the IKE_SA has more commands to be executed. FALSE otherwise.
             */
            bool hasMoreCommands();

            /**
             * Returns the textual ID of the IKE_SA
             * @return
             */
            virtual string getLogId();

            /**
             * Pushes a new Command into the queue.
             * @param command Command to be inserted into the queue.
             * @param priority If TRUE, then the command is inserted at the front. Otherwise, is inserted at the back.
             */
            void pushCommand( auto_ptr<Command> command, bool priority );

            /**
             * Pushes new Command into the deferred command queue.
             * @param command Command to be inserted into queue
             */
            void pushDeferredCommand( auto_ptr<Command> command );

            /**
             * Creates a new Message using the IKE_SA information
             * @param exchange_type Exchange type
             * @param message_type Indicates if the Message is a request or a response
             * @return The new created Message object
             */
            auto_ptr<Message> createMessage( Message::EXCHANGE_TYPE exchange_type, Message::MESSAGE_TYPE message_type ) const;

            /**
             * Creates and sends a new IKE_SA_INIT request to the peer
             * Uses the internal proposal and the DH group indicated in the first DH transform
             * @param child_sa_request ChildSa to be created
             * @return Action to be performed after message creation
             */
            IKE_SA_ACTION createIkeSaInitRequest( auto_ptr<ChildSaRequest> child_sa_request );

            /**
             * Creates and sends a new IKE_SA_INIT response to the peer
             * @return Action to be performed after message creation
             */
            MESSAGE_ACTION createIkeSaInitResponse( );

            /**
             * Creates a new IKE_AUTH request.
             * @param received_payloads_cert_req_r Received PAYLOAD_CERT_REQ in the IKE_SA_INIT exchange response. NULL if not received
             * @return Action to be performed after message creation
             */
            IKE_SA_ACTION createIkeAuthRequest( vector<Payload_CERT_REQ*> received_payloads_cert_req_r );

            /**
             * Creates a new IKE_AUTH response.
             * @param payloads_cert_req_i CERT REQ payloads received in the IKE_AUTH request
             */
            MESSAGE_ACTION createIkeAuthNoEapResponse( const vector<Payload_CERT_REQ*> payloads_cert_req_i );

            /**
             * Creates a new CREATE_CHILD_SA exchange request to rekey a CHILD_SA
             * @param spi_rekey SPI of the CHILD_SA to be rekeyed
             * @return Action to be performed after message creation
             */
            IKE_SA_ACTION createRekeyChildSaRequest( uint32_t spi_rekey );

            /**
             * Creates a new CREATE_CHILD_SA exchange request to create a new CHILD_SA
             * @param child_sa_request CHILD_SA to be created
             * @return Action to be performed after message creation
             */
            IKE_SA_ACTION createNewChildSaRequest( auto_ptr<ChildSaRequest> child_sa_request );

            /**
             * Creates a new CREATE_CHILD_SA response.
             * @return Action to be performed after the message creation
            */
            MESSAGE_ACTION createNewChildSaResponse( );

            /**
             * Creates a Rekey CREATE_CHILD_SA response
             * @return Action to be performed after the message creation
             */
            MESSAGE_ACTION createRekeyChildSaResponse( );

            /**
             * Creates a new INFORMATIONAL response.
             * @return Action to be performed after the message creation
            */
            MESSAGE_ACTION createInformationalResponse( AutoVector<Payload> responses );

            /**
             * Creates a new IKE_SA REKEY request.
             * @return Action to be performed after the message creation
             */
            IKE_SA_ACTION createRekeyIkeSaRequest();

            /**
             * Creates a new IKE_SA REKEY response.
             * @return Action to be performed after the message creation
             */
            MESSAGE_ACTION createRekeyIkeSaResponse( );

            /**
             * Creates a new EAP_INIT response.
             * @return Action to be performed after the message creation
             */
            MESSAGE_ACTION createEapInitResponse( vector<Payload_CERT_REQ*> payloads_cert_req_i );

            /**
             * Creates a new EAP_CONTINUE request
             * @param payload_eap The EAP payload to be included in the message
             * @return Action to be performed after the message creation
             */
            IKE_SA_ACTION createEapContinueRequest( auto_ptr<Payload_EAP> payload_eap );

            /**
             * Creates a new EAP_CONTINUE response.
             * @param payload_eap The EAP payload to be included in the message
             * @return Action to be performed after the message creation
             */
            MESSAGE_ACTION createEapContinueResponse( auto_ptr<Payload_EAP> payload_eap );

            /**
             * Creates a new EAP_FINISH request
             * @return Action to be performed after the message creation
             */
            IKE_SA_ACTION createEapFinishRequest();

            /**
             * Creates a new EAP_FINISH response
             * @return Action to be performed after the message creation
             */
            MESSAGE_ACTION createEapFinishResponse(  );

            /**
             * Creates a new INFORMATIONAL request to delete a CHILD_SA
             * @param spi SPI of the SA to be deleted. If 0 value, then IKE_SA delete is assumed.
             * @return Action to be performed after the message creation
             */
            IKE_SA_ACTION createDeleteChildSaRequest( uint32_t spi );

            /**
             * Creates a new INFORMATIONAL request to delete the IKE_SA
             * @return Action to be performed after the message creation
             */
            IKE_SA_ACTION createDeleteIkeSaRequest();

            /**
             * Creates a new INFORMATIONAL request with generic information
             * @param payloads Payloads to be sent within the message
             * @return Action to be performed after the message creation
             */
            IKE_SA_ACTION createGenericInformationalRequest( AutoVector <Payload> payloads );

            /**
             * Process an IKE_SA_INIT request Message and performs adequated actions.
             * @param message IKE_SA_INIT request Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processIkeSaInitRequest( Message& message );

            /**
             * Process an IKE_SA_INIT response Message and performs adequated actions.
             * @param message IKE_SA_INIT response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processIkeSaInitResponse( Message& message );

            /**
             * Process an IKE_AUTH request Message and performs adequated actions.
             * @param message IKE_AUTH request Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processIkeAuthRequest( Message& message );

            /**
             * Process an IKE_AUTH request when no EAP is being used, and perform the adequated actions
             * @param message IKE_AUTH request Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processIkeAuthNoEapRequest( Message& message );

            /**
             * Process an IKE_AUTH response Message and performs adequated actions.
             * @param message IKE_AUTH response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processIkeAuthResponse( Message& message );

            /**
            * Process an IKE_AUTH response when no EAP is being used, and perform the adequated actions
            * @param message IKE_AUTH response Message.
            * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processIkeAuthNoEapResponse( Message& message );

            /**
             * Process a CREATE_CHILD_SA request Message and performs adequated actions.
             * @param message CREATE_CHILD_SA request Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processCreateChildSaRequest( Message& message );

            /**
             * Process a CREATE_CHILD_SA (new CHILD_SA) request message and performs adequated actions
             * @param message CREATE_CHILD_SA request Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processNewChildSaRequest( Message& message );

            /**
             * Process a CREATE_CHILD_SA (rekey CHILD_SA) request message and performs adequated actions
             * @param message CREATE_CHILD_SA request Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processRekeyChildSaRequest( Message& message );

            /**
             * Process a CREATE_CHILD_SA (rekey IKE_SA) request message and performs adequated actions
             * @param message CREATE_CHILD_SA request Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processRekeyIkeSaRequest( Message& message );

            /**
             * Process a CREATE_CHILD_SA response message and performs adequated actions
             * @param message CREATE_CHILD_SA response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processCreateChildSaResponse( Message& message );

            /**
             * Process a CREATE_CHILD_SA (new CHILD_SA) response message and performs adequated actions
             * @param message CREATE_CHILD_SA response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processNewChildSaResponse( Message& message );

            /**
             * Process a CREATE_CHILD_SA (rekey CHILD_SA) response message and performs adequated actions
             * @param message CREATE_CHILD_SA response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processRekeyChildSaResponse( Message& message );

            /**
             * Process a CREATE_CHILD_SA (rekey CHILD_SA) response message (in REDUNDANT CHILD SA state) and performs adequated actions
             * @param message CREATE_CHILD_SA response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processRedundantRekeyChildSaResponse( Message& message );

            /**
             * Process a CREATE_CHILD_SA (rekey IKE_SA) response message and performs adequated actions
             * @param message CREATE_CHILD_SA response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processRekeyIkeSaResponse( Message& message );

            /**
             * Process a CREATE_CHILD_SA (rekey IKE_SA) response message (in REDUNDANT IKE SA state) and performs adequated actions
             * @param message CREATE_CHILD_SA response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processRedundantRekeyIkeSaResponse( Message& message );

            /**
            * Process an INFORMATIONAL request Message and performs adequated actions.
            * @param message INFORMATIONAL request Message.
            * @return Action to be performed after message processing
            */
            MESSAGE_ACTION processInformationalRequest( Message &message );

            /**
             * Process an INFORMATIONAL response Message and performs adequated actions.
             * @param message INFORMATIONAL response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processInformationalResponse( Message &message );

            /**
             * Process an generic INFORMATIONAL response Message and performs adequated actions.
             * @param message INFORMATIONAL response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processGenericInformationalResponse( Message &message );

            /**
             * Process an DELETE_IKE_SA response Message and performs adequated actions.
             * @param message DELETE_IKE_SA response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processDeleteIkeSaResponse( Message& message );

            /**
             * Process an DELETE_CHILD_SA response Message and performs adequated actions.
             * @param message DELETE_CHILD_SA response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processDeleteChildSaResponse( Message& message );

            /**
             * Process an EAP_INIT request Message and performs adequated actions.
             * @param message EAP_INIT request Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processEapInitRequest( Message& message );

            /**
             * Process an EAP_INIT response Message and performs adequated actions.
             * @param message EAP_INIT response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processEapInitResponse( Message& message );

            /**
             * Process an EAP_CONTINUE request Message and performs adequated actions.
             * @param message EAP_CONTINUE response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processEapContinueRequest( Message& message );

            /**
             * Process an EAP_CONTINUE response Message and performs adequated actions
             * @param message EAP_CONTINUE response message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processEapContinueResponse( Message& message );

            /**
             * Process an EAP_FINISH request Message and performs adequated actions.
             * @param message EAP_FINISH response Message.
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processEapFinishRequest( Message& message );

            /**
             * Process and EAP_FINISH response Message and performs appropiated actions
             * @param message EAP_FINISH response Message
             * @return Action to be performed after message processing
             */
            MESSAGE_ACTION processEapFinishResponse( Message& message );

            /**
             * Process a received Message.
             * @param message Received Message.
             */
            IKE_SA_ACTION processMessage( Message& message );

            /**
             * Process the notification payloads included in a message regarding the current IKE_SA and perform the apropiated actions
             * @param message Received message
             * @param child_sa Related CHILD_SA (NULL if no CHILD_SA is related)
             * @return Action to be performed after the notification processing
             */
            NOTIFY_ACTION processNotifies( Message& message, ChildSa* child_sa );

            /**
            * Sends a response containing only a NOTIFICATION payload
            * @param exchange_type Exchange type
            * @param notify Notify to be included in the response message
            */
            void sendNotifyResponse( Message::EXCHANGE_TYPE exchange_type, auto_ptr<Payload_NOTIFY> notify );

            /**
            * Sends a response containing only a NOTIFICATION payload
            * @param exchange_type Exchange type
            * @param notify Notify to be included in the response message
            */
            void sendNotifyResponse( Message::EXCHANGE_TYPE exchange_type, Payload_NOTIFY::NOTIFY_TYPE notify_type );

            /**
             * Process delete requests.
             * @param payloads_delete Delete payload collection (outbound half)
             * @return Response delete payload collection (inbound half)
             */
            AutoVector<Payload> processDeleteRequest( vector<Payload*> payloads_delete );

            /**
             * Process delete requests.
             * @param payloads_delete Delete payload collection (outbound half)
             * @return Response delete payload collection (inbound half)
             */
            void processDeleteIkeSaRequest( vector<Payload*> payloads_delete, AutoVector<Payload> & result );

            /**
             * Process delete requests.
             * @param payloads_delete Delete payload collection (outbound half)
             * @return Response delete payload collection (inbound half)
             */
            void processDeleteChildSaRequest( vector<Payload*> payloads_delete, AutoVector<Payload> & result );

            /**
             * Process configuration requests
             * @param payloads_config Configuration payload collection
             * @return Response configuration payload collection
             */
            AutoVector<Payload> processConfigRequest( vector<Payload*> payloads_config );

            /**
             * Process configuration response
             * @param payloads_config Configuration payload collection
             */
            void processConfigResponse( vector<Payload*> payloads_config );

            /**
             * Retransmits last sent request.
             * @return Action to be performed after retransmit the last request
             */
            IKE_SA_ACTION retransmitLastRequest();

            /**
             * Retransmit las sent response.
             */
            void retransmitLastResponse();

            /**
             * Executes tasks associated to alarm events.
             * @param alarm Alarm that produces current event.
             */
            void notifyAlarm( Alarm& alarm );

            /**
             * Processes an Alarm signal
             * @param alarm Signalling Alarm
             * @return Action to be taken after alarm processing
             */
            IKE_SA_ACTION processAlarm( Alarm& alarm );

            /**
             * Obtains the IkeSaConfiguration for this IkeSa
             * @return The IkeSaConfiguration for this IkeSa
             */
            IkeSaConfiguration& getIkeSaConfiguration() const;

            /**
             * Obtains the ChildSaConfiguration for this IkeSa
             * @return The ChildSaConfiguration for this IkeSa
             */
            ChildSaConfiguration& getChildSaConfiguration() const;

            /**
             * Indicates if the IKE_SA controls the CHILD_SA
             * @param spi SPI of the CHILD_SA
             * @return TRUE if the IKE_SA controles such CHILD_SA. FALSE otherwise
             */
            bool controlsChildSa( uint32_t spi );

            virtual ~IkeSa();
    };

};
#endif
