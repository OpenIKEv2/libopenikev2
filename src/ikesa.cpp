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
#include "ikesa.h"

#include "cryptocontroller.h"
#include "alarmcontroller.h"
#include "ipseccontroller.h"
#include "ikesacontroller.h"
#include "utils.h"
#include "log.h"
#include "autolock.h"

#include "payload_conf.h"
#include "payload_ke.h"
#include "payload_nonce.h"
#include "payload_sa.h"
#include "payload_vendor.h"
#include "payload_idi.h"
#include "payload_idr.h"
#include "payload_del.h"
#include "payload_notify.h"

#include "threadcontroller.h"
#include "networkcontroller.h"
#include "configuration.h"
#include "eventbus.h"
#include "buseventchildsa.h"
#include "buseventikesa.h"

#include "senddeletechildsareqcommand.h"
#include "senddeleteikesareqcommand.h"
#include "sendeapcontinuereqcommand.h"
#include "sendeapfinishreqcommand.h"
#include "sendikeauthreqcommand.h"
#include "sendikesainitreqcommand.h"
#include "sendinformationalreqcommand.h"
#include "sendnewchildsareqcommand.h"
#include "sendrekeychildsareqcommand.h"
#include "sendrekeyikesareqcommand.h"
#include "exitikesacommand.h"
#include "alarmcommand.h"

#include "boolattribute.h"
#include "stringattribute.h"

#include "exception.h"

#include <sys/time.h>
#include <stdio.h>


namespace openikev2 {

    void IkeSa::base( uint64_t my_spi, bool is_initiator, auto_ptr<SocketAddress> my_addr, auto_ptr<SocketAddress> peer_addr ) {
        this->my_spi = my_spi;
        this->peer_spi = 0;

        this->my_addr = my_addr;
        this->peer_addr = peer_addr;

        this->is_initiator = is_initiator;

        this->my_message_id = 0;
        this->peer_message_id = 0;

        this->attributemap.reset( new AttributeMap() );

        this->child_sa_collection.reset ( new ChildSaCollection() );

        auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

        if ( general_conf->vendor_id.get() )
            this->my_vendor_id = general_conf->vendor_id->clone();

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
		      is_ha = is_ha_attr->value;

	    this->is_ha = is_ha;


    	BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
    	if (mobility_attr  != NULL )
        	this->mobility = mobility_attr->value;
    	if ( this->mobility ) {

		if (!is_ha){ // It is MR
			Log::writeLockedMessage( this->getLogId(), "During the IKE_SA creation.....", Log::LOG_THRD, true );

       			StringAttribute* string_attr = general_conf->attributemap->getAttribute<StringAttribute>( "home_address" );
    			if (string_attr!=NULL ){
				Log::writeLockedMessage( this->getLogId(), "1.....", Log::LOG_THRD, true );
               			this->home_address = NetworkController::getSocketAddress(string_attr->value,500);
				Log::writeLockedMessage( this->getLogId(), "2.....", Log::LOG_THRD, true );
    			    	//auto_ptr<IpAddress> coa (NetworkController::getCurrentCoA());
			    	//NetworkController::getSocketAddress(coa,500);
    			    	this->care_of_address = this->my_addr->clone();
				Log::writeLockedMessage( this->getLogId(), "3.....", Log::LOG_THRD, true );
                	}
    		}
    	}


        this->remaining_timeout_retries = this->getIkeSaConfiguration().ike_max_exchange_retransmitions;
        this->peer_supports_hash_url = false;

        // calculates rekeying time
        auto_ptr<Random> random = CryptoController::getRandom();
        uint32_t temp_10percent = ( uint32_t ) ( ( float ) this->getIkeSaConfiguration().rekey_time * 0.1 );
        uint32_t jitter = random->getRandomInt32( 0, temp_10percent * 2 );
        uint32_t rekey_time = this->getIkeSaConfiguration().rekey_time - temp_10percent + jitter;

        this->retransmition_alarm.reset( new Alarm( *this, this->getIkeSaConfiguration().retransmition_time * 1000 ) );
        AlarmController::addAlarm( *this->retransmition_alarm );

        this->rekey_ike_sa_alarm.reset( new Alarm( ( *this ), rekey_time * 1000 ) );
        AlarmController::addAlarm( *this->rekey_ike_sa_alarm );

        this->halfopen_alarm.reset( new Alarm( ( *this ), general_conf->ike_max_halfopen_time * 1000 ) );
        AlarmController::addAlarm( *this->halfopen_alarm );

        this->idle_ike_sa_alarm.reset( new Alarm( ( *this ), this->getIkeSaConfiguration().max_idle_time * 1000 ) );
        AlarmController::addAlarm( *this->idle_ike_sa_alarm );

        this->mutex = ThreadController::getMutex();

        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_CREATED, *this ) ) );
    }

    IkeSa::IkeSa( uint64_t my_spi, bool is_initiator, auto_ptr< SocketAddress > my_addr, auto_ptr< SocketAddress > peer_addr ) {
        this->is_half_open = true;
        this->state = STATE_INITIAL;
        this->my_id.reset( new ID( my_addr->getIpAddress() ) );
        this->peer_id.reset( new ID( peer_addr->getIpAddress() ) );

        this->peer_configuration = Configuration::getInstance().getPeerConfiguration( peer_addr->getIpAddress(), is_initiator ? Enums::ROLE_INITIATOR : Enums::ROLE_RESPONDER );

        this->base( my_spi, is_initiator, my_addr, peer_addr );

        this->is_auth_initiator = is_initiator;

        Log::acquire();
        Log::writeMessage( this->getLogId(), "New IKE_SA", Log::LOG_INFO, true );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "Local peer:\n" + Printable::generateTabs( 1 ) + "IP=[" + this->my_addr->toString() + "]\n" + this->my_id->toStringTab( 1 ), Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "Remote peer:\n" + Printable::generateTabs( 1 ) + "IP=[" + this->peer_addr->toString() + "]\n" + this->peer_id->toStringTab( 1 ) , Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "REKEY_TIME=[" + intToString( this->getIkeSaConfiguration().rekey_time ) + "]" , Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "TIMEOUT=[" + intToString( this->getIkeSaConfiguration().retransmition_time ) + "]" , Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "RETRIES=[" + intToString( this->getIkeSaConfiguration().ike_max_exchange_retransmitions ) + "]", Log::LOG_INFO, false );
        Log::release();
    }

    IkeSa::IkeSa( uint64_t my_spi, bool is_initiator, const IkeSa& rekeyed_ike_sa ) {
        this->is_half_open = false;
        this->state = STATE_IKE_SA_ESTABLISHED;
        this->my_id = rekeyed_ike_sa.my_id->clone();
        this->peer_id = rekeyed_ike_sa.peer_id->clone();

        this->peer_configuration = rekeyed_ike_sa.peer_configuration->clone();

        this->base( my_spi, is_initiator, rekeyed_ike_sa.my_addr->clone(), rekeyed_ike_sa.peer_addr->clone() );

        // restore the is_initiator value
        this->is_auth_initiator = rekeyed_ike_sa.is_auth_initiator;

        Log::acquire();
        Log::writeMessage( this->getLogId(), "New IKE_SA: (Rekeying)", Log::LOG_INFO, true );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "Local peer:\n" + Printable::generateTabs( 1 ) + "IP=[" + this->my_addr->toString() + "]\n" + this->my_id->toStringTab( 1 ), Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "Remote peer:\n" + Printable::generateTabs( 1 ) + "IP=[" + this->peer_addr->toString() + "]\n" + this->peer_id->toStringTab( 1 ) , Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "REKEY_TIME=[" + intToString( this->getIkeSaConfiguration().rekey_time ) + "]" , Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "TIMEOUT=[" + intToString( this->getIkeSaConfiguration().retransmition_time ) + "]" , Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "RETRIES=[" + intToString( this->getIkeSaConfiguration().ike_max_exchange_retransmitions ) + "]", Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "Is original initiator=[" + boolToString( is_initiator ) + "]", Log::LOG_INFO, false );
        Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + "Is original auth initiator=[" + boolToString( is_auth_initiator ) + "]", Log::LOG_INFO, false );
        Log::release();
    }

    IkeSa::~IkeSa() {
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_DELETED, *this ) ) );

        // If this IkeSa is half open, the deletes it from the half open counter
        if ( this->is_half_open )
            IkeSaController::decHalfOpenCounter();

        // Deletes all alarms
        AlarmController::removeAlarm( *this->retransmition_alarm );
        AlarmController::removeAlarm( *this->rekey_ike_sa_alarm );
        AlarmController::removeAlarm( *this->halfopen_alarm );
        AlarmController::removeAlarm( *this->idle_ike_sa_alarm );

        // Deletes remainig commands
        for ( deque<Command*>::iterator it = this->command_queue.begin(); it != this->command_queue.end(); it++ )
            delete ( *it );
        for ( deque<Command*>::iterator it = this->deferred_queue.begin(); it != this->deferred_queue.end(); it++ )
            delete ( *it );

        // Deletes all CHILD SAs physically
        while ( child_sa_collection->size() > 0 ) {
            ChildSa* current = child_sa_collection->getFirstChildSa();
            IpsecController::deleteIpsecSa( this->my_addr->getIpAddress(), this->peer_addr->getIpAddress(), current->ipsec_protocol, current->outbound_spi );
            IpsecController::deleteIpsecSa( this->peer_addr->getIpAddress(), this->my_addr->getIpAddress(), current->ipsec_protocol, current->inbound_spi );
            child_sa_collection->deleteChildSa( current->inbound_spi );
        }

        Log::writeLockedMessage( this->getLogId(), "IKE_SA deleted", Log::LOG_INFO, true );
    }

    IkeSa & IkeSa::hasMinNonce( const IkeSa & ike_sa1, const IkeSa & ike_sa2 ) {
        ByteArray & min_nonce1 = ( *ike_sa1.my_nonce < *ike_sa1.peer_nonce ) ? *ike_sa1.my_nonce : *ike_sa1.peer_nonce;
        ByteArray & min_nonce2 = ( *ike_sa2.my_nonce < *ike_sa2.peer_nonce ) ? *ike_sa2.my_nonce : *ike_sa2.peer_nonce;

        // If the new Controller1 has the minimun nonce, then return it
        if ( min_nonce1 < min_nonce2 )
            return ( IkeSa& ) ike_sa1;

        // Else if the Controller2 has the minimun nonce, then return it
        else if ( min_nonce2 < min_nonce1 )
            return ( IkeSa& ) ike_sa2;

        // NONCES ARE EQUALS!!!!!!! Nearly imposible case
        else
            assert( 0 );
    }

    string IkeSa::IKE_SA_STATE_STR( IKE_SA_STATE state ) {
        switch ( state ) {
            case IkeSa::STATE_DELETE_CHILD_SA_REQ_SENT:
                return "STATE_DELETE_CHILD_SA_REQ_SENT";

            case IkeSa::STATE_DELETE_IKE_SA_REQ_SENT:
                return "STATE_DELETE_IKE_SA_REQ_SENT";

            case IkeSa::STATE_WAITING_FOR_DELETION:
                return "STATE_WAITING_FOR_DELETION";

            case IkeSa::STATE_IKE_AUTH_EAP_INIT_RES_SENT:
                return "STATE_IKE_AUTH_EAP_INIT_RES_SENT";

            case IkeSa::STATE_IKE_AUTH_EAP_INIT_REQ_SENT:
                return "STATE_IKE_AUTH_EAP_INIT_REQ_SENT";

            case IkeSa::STATE_IKE_AUTH_REQ_SENT:
                return "STATE_IKE_AUTH_REQ_SENT";

            case IkeSa::STATE_IKE_AUTH_EAP_CONT_REQ_SENT:
                return "STATE_IKE_AUTH_EAP_CONT_REQ_SENT";

            case IkeSa::STATE_IKE_AUTH_EAP_CONT_RES_SENT:
                return "STATE_IKE_AUTH_EAP_CONT_RES_SENT";

            case IkeSa::STATE_IKE_AUTH_EAP_FINISH_REQ_SENT:
                return "STATE_IKE_AUTH_EAP_FINISH_REQ_SENT";

            case IkeSa::STATE_IKE_AUTH_EAP_SUCCESS_RES_SENT:
                return "STATE_IKE_AUTH_EAP_SUCCESS_RES_SENT";

            case IkeSa::STATE_IKE_SA_ESTABLISHED:
                return "STATE_IKE_SA_ESTABLISHED";

            case IkeSa::STATE_IKE_SA_INIT_RES_SENT:
                return "STATE_IKE_SA_INIT_RES_SENT";

            case IkeSa::STATE_IKE_SA_INIT_REQ_SENT:
                return "STATE_IKE_SA_INIT_REQ_SENT";

            case IkeSa::STATE_GENERIC_INFORMATIONAL_REQ_SENT:
                return "STATE_GENERIC_INFORMATIONAL_REQ_SENT";

            case IkeSa::STATE_INITIAL:
                return "STATE_INITIAL";

            case IkeSa::STATE_NEW_CHILD_SA_REQ_SENT:
                return "STATE_NEW_CHILD_SA_REQ_SENT";

            case IkeSa::STATE_REDUNDANT_CHILD_SA:
                return "STATE_REDUNDANT_CHILD_SA";

            case IkeSa::STATE_REDUNDANT_IKE_SA:
                return "STATE_REDUNDANT_IKE_SA";

            case IkeSa::STATE_REKEY_CHILD_SA_REQ_SENT:
                return "STATE_REKEY_CHILD_SA_REQ_SENT";

            case IkeSa::STATE_REKEY_IKE_SA_REQ_SENT:
                return "STATE_REKEY_IKE_SA_REQ_SENT";

            case IkeSa::STATE_CLOSED:
                return "STATE_CLOSED";

            default:
                return intToString( state );
        }
    }

    auto_ptr<Command> IkeSa::popDeferredCommand() {
        // Gets the first command in the deferred queue
        auto_ptr<Command> result ( this->deferred_queue.front() );

        // removes the first command in the queue
        this->deferred_queue.pop_front();

        Log::writeLockedMessage( this->getLogId(), "Pop deferred command=[" + result->getCommandName() + "]", Log::LOG_THRD, true );

        return result;
    }

    auto_ptr<Command> IkeSa::popCommand( ) {
        AutoLock auto_lock( *this->mutex );

        if ( ( this->state == STATE_IKE_SA_ESTABLISHED ) && !( deferred_queue.empty() ) ) {
            return this->popDeferredCommand();
        }
        else {
            assert( !command_queue.empty() );

            // Gets the first command in the queue
            auto_ptr<Command> result ( this->command_queue.front() );

            // removes the first command in the queue
            this->command_queue.pop_front();

            return result;
        }
    }

    void IkeSa::pushDeferredCommand( auto_ptr<Command> command ) {
        Log::writeLockedMessage( this->getLogId(), "Push deferred command=[" + command->getCommandName() + "]", Log::LOG_THRD, true );

        this->deferred_queue.push_back( command.release() );
    }

    void IkeSa::pushCommand( auto_ptr<Command> command , bool priority ) {
        AutoLock auto_lock( *this->mutex );

        if ( priority )
            command_queue.push_front( command.release() );
        else
            command_queue.push_back( command.release() );
    }

    IkeSa::IKE_SA_ACTION IkeSa::processCommand( ) {
        try {
            // Gets a command, deferred or not
            auto_ptr<Command> command = this->popCommand();
            Log::writeLockedMessage( this->getLogId(), "Processing command=[" + command->getCommandName() + "]", Log::LOG_THRD, true );

            // If a command is processed (and it is not a ALARM COMMAND), then the IKE SA is not idle
            if ( command->getCommandName() != "ALARM_TIMEOUT" )
                this->idle_ike_sa_alarm->reset();

            return command->executeCommand( *this );
        }
        catch ( exception & ex ) {
            Log::writeLockedMessage( this->getLogId(), ex.what(), Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
    }

    IkeSaConfiguration & IkeSa::getIkeSaConfiguration( ) const {
        return this->peer_configuration->getIkeSaConfiguration();
    }

    ChildSaConfiguration & IkeSa::getChildSaConfiguration( ) const {
        return this->peer_configuration->getChildSaConfiguration();
    }

    IkeSa::IKE_SA_ACTION IkeSa::close( ) {
        if ( this->state >= STATE_IKE_SA_ESTABLISHED ) {
            this->pushCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ), true );
            return IKE_SA_ACTION_CONTINUE;
        }
        else {
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
    }

    bool IkeSa::checkPeerIkeSpi( Message& message ) {
        // not a a IKE_SA_INIT req or REKEY IKE SA req
        if ( message.message_id != 0 ) {
            // check if the peer spi is correct
            uint64_t received_peer_spi = message.is_initiator ? message.spi_i : message.spi_r;

            if ( received_peer_spi != this->peer_spi ) {
                Log::writeLockedMessage( this->getLogId(), "Invalid Peer SPI: expected=" + Printable::toHexString( &this->peer_spi, 8 ) +
                                         " received=" + Printable::toHexString( &received_peer_spi, 8 ), Log::LOG_WARN, true );
                return false;
            }
        }
        return true;
    }

    bool IkeSa::checkMessageId( Message & message ) {
        // This message has a invalid sequence number
        if ( ( message.message_type == Message::REQUEST && message.message_id != this->peer_message_id && message.message_id != this->peer_message_id - 1 ) ) {
            Log::writeLockedMessage( this->getLogId(), "Invalid Request ID: expected=" + intToString( this->peer_message_id ) + " received=" + intToString( message.message_id ), Log::LOG_WARN, true );
            return false;
        }

        if ( message.message_type == Message::RESPONSE && message.message_id != this->my_message_id ) {
            Log::writeLockedMessage( this->getLogId(), "Invalid Response ID: expected=" + intToString( this->my_message_id ) + " received=" + intToString( message.message_id ), Log::LOG_WARN, true );
            return false;
        }

        return true;
    }

    void IkeSa::setState( IKE_SA_STATE next_state ) {
        Log::writeLockedMessage( this->getLogId(), "Transition: [" + IKE_SA_STATE_STR( this->state ) + " ---> " + IKE_SA_STATE_STR( next_state ) + "]", Log::LOG_STAT, true );
        this->state = next_state;

        // If STATE_IKE_SA_ESTABLISHED and halfopen, then full open
        if ( this->state == STATE_IKE_SA_ESTABLISHED && this->is_half_open ) {
            this->is_half_open = false;
            IkeSaController::decHalfOpenCounter();
            this->rekey_ike_sa_alarm->reset();
        }
    }

    IkeSa::IKE_SA_STATE IkeSa::getState( ) const {
        return this->state;
    }

    Proposal & IkeSa::getProposal( ) const {
        return this->getIkeSaConfiguration().getProposal();
    }

    void IkeSa::setProposal( auto_ptr< Proposal > proposal ) {
        this->getIkeSaConfiguration().setProposal( proposal );
    }

    auto_ptr< Message > IkeSa::createMessage( Message::EXCHANGE_TYPE exchange_type, Message::MESSAGE_TYPE message_type ) const {
         // Choose CoA address in case of mobility protection enabled

        auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
		is_ha = is_ha_attr->value;

	if ( this->mobility ) {


		if (exchange_type == Message::IKE_SA_INIT || ((exchange_type == Message::IKE_AUTH) && (message_type == Message::REQUEST)) ){
			// Special case for IKE_SA_INIT and IKE_AUTH, no change the HoA to CoA
              		return auto_ptr<Message> ( new Message( this->my_addr->clone() , //Our address
                                                this->peer_addr->clone(),  // Peer address
                                                this->is_initiator ? this->my_spi : this->peer_spi,  // Initiator SPI
                                                this->is_initiator ? this->peer_spi : this->my_spi,    // Peer SPI
                                                2,    // Major version
                                                0,    // Minor version
                                                exchange_type,  // Exchange type
                                                message_type,  // Message type
                                                this->is_initiator,  // Is initiator?
                                                false,   // Can use higher version?
                                                ( message_type == Message::REQUEST ) ? this->my_message_id : this->peer_message_id   //  Message ID
                                              )
                                 );


		}
		else {

			return auto_ptr<Message> ( new Message( (is_ha ? this->my_addr->clone() : this->care_of_address->clone()) , //Our address
                                                (is_ha ? this->care_of_address->clone() : this->peer_addr->clone() ),  // Peer address
                                                this->is_initiator ? this->my_spi : this->peer_spi,  // Initiator SPI
                                                this->is_initiator ? this->peer_spi : this->my_spi,    // Peer SPI
                                                2,    // Major version
                                                0,    // Minor version
                                                exchange_type,  // Exchange type
                                                message_type,  // Message type
                                                this->is_initiator,  // Is initiator?
                                                false,   // Can use higher version?
                                                ( message_type == Message::REQUEST ) ? this->my_message_id : this->peer_message_id   //  Message ID
                                              )
                                 );

		}


	 }
         else {
              return auto_ptr<Message> ( new Message( this->my_addr->clone() , //Our address
                                                this->peer_addr->clone(),  // Peer address
                                                this->is_initiator ? this->my_spi : this->peer_spi,  // Initiator SPI
                                                this->is_initiator ? this->peer_spi : this->my_spi,    // Peer SPI
                                                2,    // Major version
                                                0,    // Minor version
                                                exchange_type,  // Exchange type
                                                message_type,  // Message type
                                                this->is_initiator,  // Is initiator?
                                                false,   // Can use higher version?
                                                ( message_type == Message::REQUEST ) ? this->my_message_id : this->peer_message_id   //  Message ID
                                              )
                                 );
         }
    }

    IkeSa::NOTIFY_ACTION IkeSa::processNotifies( Message & message, ChildSa* child_sa ) {
        // Updates PEER VENDOR ID INFORMATION
        Payload_VENDOR * payload_vendor = ( Payload_VENDOR* ) message.getFirstPayloadByType( Payload::PAYLOAD_VENDOR );

        if ( payload_vendor != NULL ) {
            this->peer_vendor_id = payload_vendor->getVendorId().clone();
            Log::writeMessage( this->getLogId(), "Peer indicates its vendor ID", Log::LOG_INFO, true );
            Log::writeMessage( this->getLogId(), Printable::generateTabs( 1 ) + this->peer_vendor_id->toStringTab( 1 ), Log::LOG_INFO, false );
        }

        // Gets all notifies in the message
        vector<Payload*> notifies = message.getPayloadsByType( Payload::PAYLOAD_NOTIFY );

        // For each notify in the message do
        for ( vector<Payload*>::iterator it = notifies.begin(); it != notifies.end(); it++ ) {
            // Gets current notify
            Payload_NOTIFY *notify = ( Payload_NOTIFY* ) ( *it );

            // get the notify controller
            NotifyController* notify_controller = NetworkController::getNotifyController( notify->notification_type );

            // If exists notify controller for that notify type
            if ( notify_controller != NULL ) {
                NOTIFY_ACTION action = notify_controller->processNotify( *notify, message, *this, child_sa );
                if ( action != NOTIFY_ACTION_CONTINUE )
                    return action;
            }

            // if doesn't exist notify controller for the notification type
            else {
                // If an unknown notify is received
                if ( notify->isError() ) {
                    // We don't allow ERROR notifies in REQUESTs
                    if ( message.message_type == Message::REQUEST ) {
                        Log::writeLockedMessage( this->getLogId(), "Notify found in a request: " + intToString( notify->notification_type ), Log::LOG_ERRO, true );
                        this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
                    }
                    else if ( message.message_type == Message::RESPONSE ) {
                        Log::writeLockedMessage( this->getLogId(), "Unknown error notify received: " + intToString( notify->notification_type ), Log::LOG_ERRO, true );
                    }
                    return NOTIFY_ACTION_ERROR;
                }
                else {
                    Log::writeLockedMessage( this->getLogId(), "Unknown status notify received: " + intToString( notify->notification_type ), Log::LOG_WARN, true );
                }
            }
        }

        return NOTIFY_ACTION_CONTINUE;
    }

    void IkeSa::createChildSa( auto_ptr<ChildSa> child_sa ) {

	auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
		is_ha = is_ha_attr->value;


    	BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
    	bool mobility = false;
	auto_ptr<SocketAddress> hoa (NULL);
	auto_ptr<SocketAddress>	coa (NULL);
        if (mobility_attr  != NULL )
		mobility = mobility_attr->value;

	if (mobility && (child_sa->mode == Enums::TUNNEL_MODE)){
		if (is_ha) {
			// creates outbound SA in the kernel
			IpsecController::createIpsecSa( this->my_addr->getIpAddress(), this->care_of_address->getIpAddress(), *child_sa );
		}
		else {
			// creates outbound SA in the kernel
			IpsecController::createIpsecSa( this->care_of_address->getIpAddress(), this->peer_addr->getIpAddress(), *child_sa );
		}
	}
	else {
		// creates outbound SA in the kernel
		IpsecController::createIpsecSa( this->my_addr->getIpAddress(), this->peer_addr->getIpAddress(), *child_sa );

	}

        // change child sa state
        child_sa->setState( ChildSa::CHILD_SA_ESTABLISHED );

        uint32_t count = this->child_sa_collection->size() + 1;

        Log::acquire();
        Log::writeMessage( this->getLogId(), "New Child SA: Count=[" + intToString( count ) + "]", Log::LOG_INFO, true );
        Log::writeMessage( this->getLogId(), child_sa->toStringTab( 1 ), Log::LOG_INFO, false );
        Log::release();

        // send CHILD_SA creation event
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_ESTABLISHED, *this, *child_sa, &count ) ) );

        // Insert created ChildSa into global ChildSa collection
        this->child_sa_collection->addChildSa( child_sa );
    }

    void IkeSa::createRekeyChildSa( auto_ptr<ChildSa> child_sa , ChildSa& rekeyed_sa ) {
        // creates outbound SA in the kernel
        IpsecController::createIpsecSa( this->my_addr->getIpAddress(), this->peer_addr->getIpAddress(), *child_sa );

        // change child sa state
        child_sa->setState( ChildSa::CHILD_SA_ESTABLISHED );

        uint32_t count = this->child_sa_collection->size() + 1;

        Log::acquire();
        Log::writeMessage( this->getLogId(), "New Child SA: Count=[" + intToString( count ) + "]", Log::LOG_INFO, true );
        Log::writeMessage( this->getLogId(), child_sa->toStringTab( 1 ), Log::LOG_INFO, false );
        Log::release();

        // send CHILD_SA creation event
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_ESTABLISHED, *this, *child_sa, &count ) ) );
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_REKEYED, *this, rekeyed_sa, child_sa.get() ) ) );

        // Inherits the AttributeMap
        child_sa->attributemap->inherit( *rekeyed_sa.attributemap );

        // Insert created ChildSa into global ChildSa collection
        this->child_sa_collection->addChildSa( child_sa );
    }

    IkeSa::IKE_SA_ACTION IkeSa::createIkeSaInitRequest( auto_ptr<ChildSaRequest> child_sa_request ) {
        // Check state
        if ( this->state != STATE_INITIAL ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start IKE_SA_INIT] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }

        Log::writeLockedMessage( this->getLogId(), "Create the IKE_SA request message ", Log::LOG_ERRO, true );

 	auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();
        BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
        bool mobility = false;
        if (mobility_attr  != NULL )
        	mobility = mobility_attr->value;

	BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
	bool is_ha = false;
	if (is_ha_attr  != NULL )
	is_ha = is_ha_attr->value;

	if (mobility){

		if (!is_ha){
			// is MR
		        Log::writeLockedMessage( this->getLogId(), "Changin CoA to Hoa for child sa creation (MR)", Log::LOG_ERRO, true );
	       		this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->home_address->getIpAddress(),child_sa_request->ipsec_protocol), child_sa_request ) );
                        Log::writeLockedMessage( this->getLogId(), "Get SPI using HoA instead of CoA.", Log::LOG_WARN, true );

		}
		else {
		        Log::writeLockedMessage( this->getLogId(), "Changin CoA to Hoa for child sa creation (HA)", Log::LOG_ERRO, true );
	        	this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(),child_sa_request->ipsec_protocol), child_sa_request ) );
                        Log::writeLockedMessage( this->getLogId(), "The IKE_SA_INIT exchange can not be started by the HA using mobility.", Log::LOG_WARN, true );

		}
	}
	else {

        	this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(),child_sa_request->ipsec_protocol), child_sa_request ) );

	}


        // Creates Message (IKE_SA_INIT request)
        auto_ptr<Message> message = this->createMessage( Message::IKE_SA_INIT, Message::REQUEST );

        // Include all the notify (N) requests in the message
        NetworkController::addNotifies( *message, *this, NULL );

        // Include the IKE_SA negotiation payloads (SA, KE, NONCE)
        this->createIkeSaNegotiationRequest( *message, *this );

        // Include payload vendor (V)
        if ( this->my_vendor_id.get() ) {
            message->addPayload( auto_ptr<Payload> ( new Payload_VENDOR( this->my_vendor_id->clone() ) ), false );
        }



	// sends the message to the peer
        this->sendMessage( message, "Send: IKE_SA_INIT request" );

        // Store the IKE_SA_INIT request
        this->ike_sa_init_req = this->last_sent_request->clone();

        // Commit state transition
        this->setState( STATE_IKE_SA_INIT_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processIkeSaInitResponse( Message& message ) {
        // Check state
        if ( this->state != STATE_IKE_SA_INIT_REQ_SENT ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive IKE_SA_INIT response] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }

        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_SA_INIT response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        //  Process notify (N) and vendor (V) payloads
        NOTIFY_ACTION notify_action = this->processNotifies( message, NULL );
        if ( notify_action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( notify_action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // Process IKE_SA negotiation response (SA, KE, NONCE)
        NEGOTIATION_ACTION negotiation_action = this->processIkeSaNegotiationResponse( message, *this );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // Stores the IKE_SA_INIT response
        this->ike_sa_init_res = message.clone();

        // Creates IKE_AUTH request command
        vector<Payload*> payloads_cert_req = message.getPayloadsByType( Payload::PAYLOAD_CERT_REQ );
        AutoVector<Payload_CERT_REQ> auto_payloads_cert_req;
        for ( vector<Payload*>::iterator it = payloads_cert_req.begin(); it != payloads_cert_req.end(); it++ ) {
            Payload_CERT_REQ* payload_cert_req = ( Payload_CERT_REQ* ) * it;
            auto_payloads_cert_req->push_back( new Payload_CERT_REQ( *payload_cert_req ) );
        }

        // creates the command to send the IKE AUTH request
        this->pushCommand( auto_ptr<Command> ( new SendIkeAuthReqCommand( auto_payloads_cert_req ) ), false );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processIkeSaInitRequest( Message &message ) {
        // Check state
        if ( this->state != STATE_INITIAL ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive IKE_SA_INIT request] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_SA_INIT, Payload_NOTIFY::INVALID_SYNTAX );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_SA_INIT request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // Check if cookies are needed, check that there is unless one
        if ( IkeSaController::useCookies() ) {
            if ( message.getFirstNotifyByType( Payload_NOTIFY::COOKIE ) == NULL ) {
                Log::writeLockedMessage( this->getLogId(), "Cookie needed and not received. Sending cookie", Log::LOG_WARN, true );
                auto_ptr<Payload_NOTIFY> expected_cookie = CryptoController::generateCookie( message );
                this->sendNotifyResponse( Message::IKE_SA_INIT, expected_cookie );
                return MESSAGE_ACTION_DELETE_IKE_SA;
            }
        }

        //  process IKE_SA notifies (N) and vendor (V) payloads
        NOTIFY_ACTION action = this->processNotifies( message, NULL );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;


        // process the IKE_SA negotiation request (SA, KE, NONCE)
        NEGOTIATION_ACTION negotiation_action = this->processIkeSaNegotiationRequest( message, *this );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // saves a copy of the message for future uses
        this->ike_sa_init_req = message.clone();

        // start the countdown to perform initial exchanges
        this->halfopen_alarm->reset();

        // creates the response to the IKE_SA_INIT request
        return this->createIkeSaInitResponse( );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createIkeSaInitResponse( ) {
        // creates the response message
        auto_ptr<Message> message = this->createMessage( Message::IKE_SA_INIT, Message::RESPONSE );

        // Include all the notify payloads (N)
        NetworkController::addNotifies( *message, *this, NULL );

        // Include the IKE_SA negotiation response payloads (SA, KE, NONCE)
        this->createIkeSaNegotiationResponse( *message, *this );

        // Includes the certificate request payloads (CERTREQ+)
        AutoVector<Payload_CERT_REQ> payloads_cert_req_r = this->getIkeSaConfiguration().getAuthenticator().generateCertificateRequestPayloads( *this );
        message->addPayloads( payloads_cert_req_r.convertType<Payload>(), false );

        // Includes payload vendor (V)
        if ( this->my_vendor_id.get() ) {
            message->addPayload( auto_ptr<Payload> ( new Payload_VENDOR( this->my_vendor_id->clone() ) ), false );
        }

        // sends the  message
        this->sendMessage( message, "Send: IKE_SA_INIT response" );

        // Store the IKE_SA_INIT response
        this->ike_sa_init_res = this->last_sent_response->clone();

        // updates the state
        this->setState( STATE_IKE_SA_INIT_RES_SENT );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processIkeAuthRequest( Message &message ) {
        if ( this->state == STATE_IKE_SA_INIT_RES_SENT && message.getFirstPayloadByType( Payload::PAYLOAD_AUTH ) != NULL )
            return this->processIkeAuthNoEapRequest( message );

        else if ( this->state == STATE_IKE_SA_INIT_RES_SENT && message.getFirstPayloadByType( Payload::PAYLOAD_AUTH ) == NULL )
            return this->processEapInitRequest( message );

        else if ( this->state == STATE_IKE_AUTH_EAP_INIT_RES_SENT )
            return this->processEapContinueRequest( message );

        else if ( this->state == STATE_IKE_AUTH_EAP_CONT_RES_SENT )
            return this->processEapContinueRequest( message );

        else if ( this->state == STATE_IKE_AUTH_EAP_SUCCESS_RES_SENT )
            return this->processEapFinishRequest( message );

        else if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive IKE_AUTH request] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::INVALID_SYNTAX );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive IKE_AUTH request] state=[" + IKE_SA_STATE_STR( this->state ) + "]. Ommiting request", Payload_NOTIFY::INVALID_SYNTAX, Message::IKE_AUTH );
            return MESSAGE_ACTION_OMIT;
        }
    }

    IkeSa::MESSAGE_ACTION IkeSa::processIkeAuthResponse( Message &message ) {
        // Check state
        if ( this->state == STATE_IKE_AUTH_REQ_SENT )
            return this->processIkeAuthNoEapResponse( message );

        else if ( this->state == STATE_IKE_AUTH_EAP_INIT_REQ_SENT )
            return this->processEapInitResponse( message );

        else if ( this->state == STATE_IKE_AUTH_EAP_CONT_REQ_SENT )
            return this->processEapContinueResponse( message );

        else if ( this->state == STATE_IKE_AUTH_EAP_FINISH_REQ_SENT )
            return this->processEapFinishResponse( message );

        else {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive IKE_AUTH response] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }
    }

    IkeSa::IKE_SA_ACTION IkeSa::createIkeAuthRequest( vector<Payload_CERT_REQ*> received_payloads_cert_req_r ) {
        // Check state
        if ( this->state != STATE_IKE_SA_INIT_REQ_SENT ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start IKE_AUTH request] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }

        // creates the IKE_AUTH request message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::REQUEST );

        // Include all notify request payloads in IKE_AUTH exchange
        NetworkController::addNotifies( *message, *this, this->my_creating_child_sa.get() );

        // includes the identity payload (IDi)
        auto_ptr<Payload_IDi> payload_id_i ( new Payload_IDi ( this->getIkeSaConfiguration().my_id->clone() ) );
        message->addPayload( auto_ptr<Payload> ( payload_id_i ), true );

        // includes the certificate payloads (CERT+)
        AutoVector<Payload_CERT> payloads_cert = this->getIkeSaConfiguration().getAuthenticator().generateCertificatePayloads( *this, received_payloads_cert_req_r );
        message->addPayloads( payloads_cert.convertType<Payload>(), true );

        // includes the certificate request paylaods (CERTREQ+)
        AutoVector<Payload_CERT_REQ> payloads_cert_req_i = this->getIkeSaConfiguration().getAuthenticator().generateCertificateRequestPayloads( *this );
        message->addPayloads( payloads_cert_req_i.convertType<Payload>(), true );

        // includes the authentication payload (AUTH)
        if ( !this->getIkeSaConfiguration().getAuthenticator().initiatorUsesEap() ) {
            auto_ptr<Payload_AUTH> payload_auth = this->getIkeSaConfiguration().getAuthenticator().generateAuthPayload( *this );
            if ( payload_auth.get() == NULL ) {
                Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED", Log::LOG_ERRO, true );
                EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
                return IKE_SA_ACTION_DELETE_IKE_SA;
            }
            message->addPayload( auto_ptr<Payload> ( payload_auth ), true );
        }

        // includes the CHILD_SA negotiation request payloads (SA, TSi, TSr)
        this->createChildSaNegotiationRequest( *message );

        if ( this->getIkeSaConfiguration().getAuthenticator().initiatorUsesEap() ) {
            this->sendMessage( message, "Send: EAP_INIT request" );
            this->setState( STATE_IKE_AUTH_EAP_INIT_REQ_SENT );
        }
        else {
            this->sendMessage( message, "Send: IKE_AUTH request" );
            this->setState( STATE_IKE_AUTH_REQ_SENT );
        }

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processIkeAuthNoEapResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_AUTH response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        Log::writeLockedMessage( this->getLogId(), "DEBUG: processing IKE_AUTH", Log::LOG_INFO, true );

        // Process notifies (N). Exits if an error notify is found.
        NOTIFY_ACTION action = this->processNotifies( message, this->my_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            Log::writeLockedMessage( this->getLogId(), "DEBUG: Notifies error", Log::LOG_ERRO, true );

            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // Process identity payload (IDr)
        Payload_IDr& payload_id_r = ( Payload_IDr& ) message.getUniquePayloadByType( Payload::PAYLOAD_IDr );
        this->my_id = this->getIkeSaConfiguration().my_id->clone();
        this->peer_id = payload_id_r.id->clone();
        if ( !this->getIkeSaConfiguration().checkId( *this->peer_id ) ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: The peer ID is not allowed", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // Process authentication payload (AUTH)
        bool auth_check = this->getIkeSaConfiguration().getAuthenticator().verifyAuthPayload( message, *this );
        if ( !auth_check ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot verify AUTH payload", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // process CHILD_SA negotiation response payloads (SA, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationResponse( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            Log::writeLockedMessage( this->getLogId(), "DEBUG: error processing CHILD_SA", Log::LOG_ERRO, true );

            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // Install the CHILD_SA in the kernel
		Log::writeLockedMessage( this->getLogId(), "DEBUG: Before creating Child_SA", Log::LOG_INFO, true );

        this->createChildSa( this->my_creating_child_sa );

		Log::writeLockedMessage( this->getLogId(), "DEBUG: After creating Child_SA", Log::LOG_INFO, true );

        this->setState( STATE_IKE_SA_ESTABLISHED );

        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_ESTABLISHED, *this ) ) );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processEapInitResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: EAP_INIT response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notifies (N)
        NOTIFY_ACTION action = this->processNotifies( message, this->my_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process identity payload (IDr)
        Payload_IDr& payload_id_r = ( Payload_IDr& ) message.getUniquePayloadByType( Payload::PAYLOAD_IDr );
        this->my_id = this->getIkeSaConfiguration().my_id->clone();
        this->peer_id = payload_id_r.id->clone();
        if ( !this->getIkeSaConfiguration().checkId( *this->peer_id ) ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: The peer ID is not allowed", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // process authentication payload (AUTH)
        bool auth_check = this->getIkeSaConfiguration().getAuthenticator().verifyAuthPayload( message, *this );
        if ( !auth_check ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot verify AUTH payload", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // Process EAP payload (EAP)
        Payload_EAP& payload_eap = ( Payload_EAP& ) message.getUniquePayloadByType( Payload::PAYLOAD_EAP );
        auto_ptr<Payload_EAP> eap_response = this->getIkeSaConfiguration().getAuthenticator().processEapRequest( payload_eap );
        if ( eap_response.get() == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot create EAP CONTINUE request", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // creates the command to send the EAP CONT request
        this->pushCommand( auto_ptr<Command> ( new SendEapContinueReqCommand( eap_response ) ), false );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processIkeAuthNoEapRequest( Message & message ) {

	Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_AUTH request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

	// update policies, in case they have changed
	IpsecController::updatePolicies(false);
        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );
	Proposal *proposal_sa = payload_sa.proposals->front() ;
	Enums::PROTOCOL_ID peer_selected_ipsec_proto = Enums::PROTO_NONE;
	if ( proposal_sa != NULL )
		peer_selected_ipsec_proto = proposal_sa->protocol_id;


        // creates the CHILD_SA
        this->peer_creating_child_sa.reset( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(), peer_selected_ipsec_proto), peer_selected_ipsec_proto, false ) );

        // process notify payloads (N) and vendor payloads (V)
        NOTIFY_ACTION action = this->processNotifies( message, this->peer_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process identity payload (IDi)
        Payload_IDi& payload_id_i = ( Payload_IDi& ) message.getUniquePayloadByType( Payload::PAYLOAD_IDi );
        this->my_id = this->getIkeSaConfiguration().my_id->clone();
        this->peer_id = payload_id_i.id->clone();
        if ( !this->getIkeSaConfiguration().checkId( *this->peer_id ) ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: The peer ID is not allowed", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        Log::writeLockedMessage( this->getLogId(), "***** 7", Log::LOG_ERRO, true );



 	auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();
        BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
        bool mobility = false;
        if (mobility_attr  != NULL )
        	mobility = mobility_attr->value;

	BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
	bool is_ha = false;
	if (is_ha_attr  != NULL )
	is_ha = is_ha_attr->value;

	if (mobility && is_ha){
                Log::writeLockedMessage( this->getLogId(), "***** 8", Log::LOG_ERRO, true );


		if (payload_id_i.id->id_type == Enums::ID_IPV6_ADDR){
			Log::writeLockedMessage( this->getLogId(), "MOBILITY 1", Log::LOG_ERRO, true );

			this->home_address = NetworkController::getSocketAddress(NetworkController::getIpAddress( Enums::ADDR_IPV6 , payload_id_i.id->id_data->clone()) ,500);
			Log::writeLockedMessage( this->getLogId(), "MOBILITY 2", Log::LOG_ERRO, true );
			this->peer_addr = this->home_address->clone();
			Log::writeLockedMessage( this->getLogId(), "MOBILITY 3", Log::LOG_ERRO, true );

		}
		else {
			Log::writeLockedMessage( this->getLogId(), "MOBILITY ERROR: You forgot to configure the HoA as ID in the initiator (IDi)", Log::LOG_ERRO, true );
		    	this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
		    	EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
		    	return MESSAGE_ACTION_DELETE_IKE_SA;
		}
	}

			Log::writeLockedMessage( this->getLogId(), "MOBILITY 4", Log::LOG_ERRO, true );


			Log::writeLockedMessage( this->getLogId(), "MOBILITY 5", Log::LOG_ERRO, true );

        // process certificate request payloads (CERTREQ+)
        vector<Payload*> payloads_cert_req = message.getPayloadsByType( Payload::PAYLOAD_CERT_REQ ) ;
        vector<Payload_CERT_REQ*> payloads_cert_req_i;
        for ( vector<Payload*>::iterator it = payloads_cert_req.begin(); it != payloads_cert_req.end(); it++ )
            payloads_cert_req_i.push_back( ( Payload_CERT_REQ* ) *it );
			Log::writeLockedMessage( this->getLogId(), "MOBILITY 6", Log::LOG_ERRO, true );

        // process authentication payloads (CERT+, AUTH)
        bool auth_check = this->getIkeSaConfiguration().getAuthenticator().verifyAuthPayload( message, *this );
        if ( !auth_check ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
			Log::writeLockedMessage( this->getLogId(), "MOBILITY 7", Log::LOG_ERRO, true );

        // process CHILD_SA negotiation request payloads (SA, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationRequest( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
			Log::writeLockedMessage( this->getLogId(), "MOBILITY 8", Log::LOG_ERRO, true );

        // Create the IKE_AUTH response for the request
        return this->createIkeAuthNoEapResponse( payloads_cert_req_i );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createIkeAuthNoEapResponse( const vector<Payload_CERT_REQ*> payloads_cert_req_i ) {
        // creates the response message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::RESPONSE );

        // include all the notify payloads (N)
        NetworkController::addNotifies( *message, *this, this->peer_creating_child_sa.get() );

        // include identity payload (IDr)
        auto_ptr<Payload_IDr> payload_id_r ( new Payload_IDr ( this->getIkeSaConfiguration().my_id->clone() ) );
        message->addPayload( auto_ptr<Payload> ( payload_id_r ), true );

        // include the certificate payloads (CERT+)
        AutoVector<Payload_CERT> payloads_cert = this->getIkeSaConfiguration().getAuthenticator().generateCertificatePayloads( *this, payloads_cert_req_i );
        message->addPayloads( payloads_cert.convertType<Payload>(), true );

        // include the authentication payload
        auto_ptr<Payload_AUTH> payload_auth = this->getIkeSaConfiguration().authenticator->generateAuthPayload( *this );
        if ( payload_auth.get() == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        message->addPayload( auto_ptr<Payload> ( payload_auth ), true );

        // include the CHILD_SA negotiation response payloads (SA, TSi, TSr)
        this->createChildSaNegotiationResponse( *message );


        // creates the CHILD_SA
        this->createChildSa( this->peer_creating_child_sa );

        this->setState( STATE_IKE_SA_ESTABLISHED );

        // sends the message
        this->sendMessage( message, "Send: IKE_AUTH response" );


        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_ESTABLISHED, *this ) ) );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processEapInitRequest( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: EAP_INIT request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // do not process IKE_SA notifies (N). It will be done after authentication

        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );
	Proposal *proposal_sa = payload_sa.proposals->front() ;

	Enums::PROTOCOL_ID peer_selected_ipsec_proto = Enums::PROTO_NONE;
	if ( proposal_sa != NULL )
		peer_selected_ipsec_proto = proposal_sa->protocol_id;

        // creates the new CHILD_SA
        this->peer_creating_child_sa.reset( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(), peer_selected_ipsec_proto), peer_selected_ipsec_proto, false ) );


        // process the identity payload (IDi)
        Payload_IDi& payload_id_i = ( Payload_IDi& ) message.getUniquePayloadByType( Payload::PAYLOAD_IDi );
        this->my_id = this->getIkeSaConfiguration().my_id->clone();
        this->peer_id = payload_id_i.id->clone();
        if ( !this->getIkeSaConfiguration().checkId( *this->peer_id ) ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: The peer ID is not allowed", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // process the certificate request payloads (CERTREQ+)
        vector<Payload*> payloads_cert_req = message.getPayloadsByType( Payload::PAYLOAD_CERT_REQ ) ;
        vector<Payload_CERT_REQ*> payloads_cert_req_i;
        for ( vector<Payload*>::iterator it = payloads_cert_req.begin(); it != payloads_cert_req.end(); it++ )
            payloads_cert_req_i.push_back( ( Payload_CERT_REQ* ) *it );

        // do not process CHILD_SA negotiation request payloads (SA, TSi, TSr). It will be done after authentication

        // Store a copy of this message to be used at the end of the EAP authentication
        this->eap_init_req = message.clone();

        // Create the IKE_AUTH response for the request
        return this->createEapInitResponse( payloads_cert_req_i );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createEapInitResponse( vector<Payload_CERT_REQ*> payloads_cert_req_i ) {
        // creates the response message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::RESPONSE );

        // include the notify payloads (N)
        NetworkController::addNotifies( *message, *this, this->peer_creating_child_sa.get() );

        // includes the identity payload (IDr)
        auto_ptr<Payload_IDr> payload_id_r ( new Payload_IDr ( this->getIkeSaConfiguration().my_id->clone() ) );
        message->addPayload( auto_ptr<Payload> ( payload_id_r ), true );

        // includes the certificate payloads (CERT+)
        AutoVector<Payload_CERT> payloads_cert = this->getIkeSaConfiguration().getAuthenticator().generateCertificatePayloads( *this, payloads_cert_req_i );
        message->addPayloads( payloads_cert.convertType<Payload>(), true );

        // includes the authentication payload (AUTH)
        auto_ptr<Payload_AUTH> payload_auth = this->getIkeSaConfiguration().getAuthenticator().generateAuthPayload( *this );
        message->addPayload( auto_ptr<Payload> ( payload_auth ), true );

        // includes the intial EAP request payload (EAP)
        auto_ptr<Payload_EAP> eap_initial_request = this->getIkeSaConfiguration().getAuthenticator().generateInitialEapRequest( *this->peer_id );
        if ( eap_initial_request.get() == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot create EAP INIT response", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        message->addPayload( auto_ptr<Payload> ( eap_initial_request ), true );

        // Sends message to the Peer
        this->sendMessage( message, "Send: EAP_INIT response" );

        this->setState( STATE_IKE_AUTH_EAP_INIT_RES_SENT );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::IKE_SA_ACTION IkeSa::createEapContinueRequest( auto_ptr<Payload_EAP> payload_eap ) {
        // Check state
        if ( this->state != STATE_IKE_AUTH_EAP_INIT_REQ_SENT && this->state != STATE_IKE_AUTH_EAP_CONT_REQ_SENT ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start IKE_AUTH_EAP_CONT] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }

        // creates the request message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::REQUEST );

        // include the EAP payload (EAP)
        message->addPayload( auto_ptr<Payload> ( payload_eap ), true );

        // sends the message
        this->sendMessage( message, "Send: EAP_CONT request" );

        this->setState( STATE_IKE_AUTH_EAP_CONT_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processEapContinueResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: EAP_CONT response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process the EAP payload (EAP)
        Payload_EAP& payload_eap = ( Payload_EAP& ) message.getUniquePayloadByType( Payload::PAYLOAD_EAP );
        EapPacket& eap_packet = payload_eap.getEapPacket();

        // If EAP FAILURE
        if ( eap_packet.code == EapPacket::EAP_CODE_FAILURE ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: EAP authentication failed", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // else if EAP SUCCESS
        else if ( eap_packet.code == EapPacket::EAP_CODE_SUCCESS ) {
            this->getIkeSaConfiguration().getAuthenticator().processEapSuccess( payload_eap );
            this->pushCommand( auto_ptr<Command> ( new SendEapFinishReqCommand() ), false );
        }

        // else, if EAP CONT request
        else {
            auto_ptr<Payload_EAP> eap_response = this->getIkeSaConfiguration().getAuthenticator().processEapRequest( payload_eap );
            if ( eap_response.get() == NULL ) {
                Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot create EAP CONTINUE request", Log::LOG_ERRO, true );
                EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
                return MESSAGE_ACTION_DELETE_IKE_SA;
            }
            this->pushCommand( auto_ptr<Command> ( new SendEapContinueReqCommand( eap_response ) ), false );
        }

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processEapContinueRequest( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: EAP_CONT request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process EAP payload (EAP)
        Payload_EAP & payload_eap = ( Payload_EAP& ) message.getUniquePayloadByType( Payload::PAYLOAD_EAP );

        auto_ptr<Payload_EAP> eap_request = this->getIkeSaConfiguration().getAuthenticator().processEapResponse( payload_eap, *this->peer_id );
        if ( eap_request.get() == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot create EAP CONT response", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        return this->createEapContinueResponse( eap_request );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createEapContinueResponse( auto_ptr<Payload_EAP> payload_eap ) {
        // creates the response message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::RESPONSE );

        // includes the EAP payload (EAP)
        EapPacket::EAP_CODE sent_eap_code = payload_eap->getEapPacket().code;
        message->addPayload( auto_ptr<Payload> ( payload_eap ), true );

        // sends the message
        this->sendMessage( message, "Send: EAP_CONT response" );

        // change state
        if ( sent_eap_code == EapPacket::EAP_CODE_SUCCESS ) {
            this->setState( STATE_IKE_AUTH_EAP_SUCCESS_RES_SENT );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( sent_eap_code == EapPacket::EAP_CODE_FAILURE ) {
            this->setState( STATE_CLOSED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else {
            this->setState( STATE_IKE_AUTH_EAP_CONT_RES_SENT );
            return MESSAGE_ACTION_COMMIT;
        }

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::IKE_SA_ACTION IkeSa::createEapFinishRequest() {
        // Check state
        if ( this->state != STATE_IKE_AUTH_EAP_CONT_REQ_SENT ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start IKE_AUTH_EAP_FINISH] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }

        // creates the request message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::REQUEST );

        // include the authentication payload (AUTH)
        auto_ptr<Payload_AUTH> payload_auth = this->getIkeSaConfiguration().getAuthenticator().generateEapAuthPayload( *this );
        message->addPayload( auto_ptr<Payload> ( payload_auth ), true );

        // sends the message
        this->sendMessage( message, "Send: EAP_FINISH request" );

        this->setState( STATE_IKE_AUTH_EAP_FINISH_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processEapFinishResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: EAP_FINISH response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notification payloads (N) and vendor payloads (V)
        NOTIFY_ACTION action = this->processNotifies( message, this->my_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process authentication payload (AUTH)
        bool auth_check = this->getIkeSaConfiguration().getAuthenticator().verifyEapAuthPayload( message, *this );
        if ( !auth_check ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot verify AUTH payload", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // process CHILD_SA negotiation response payloads (SA, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationResponse( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // creates the CHILD_SA physically
        this->createChildSa( this->my_creating_child_sa );

        this->setState( STATE_IKE_SA_ESTABLISHED );

        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_ESTABLISHED, *this ) ) );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processEapFinishRequest( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: EAP_FINISH request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();


        // process notification (N) and vendor (V) payloads (from the EAP_INIT request)
        NOTIFY_ACTION action = this->processNotifies( *this->eap_init_req, this->peer_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

	// get msk key from eap method
	this->getIkeSaConfiguration().getAuthenticator().processFinish();

        // process the authentication payload (AUTH)
        bool auth_check = this->getIkeSaConfiguration().getAuthenticator().verifyEapAuthPayload( message, *this );
        if ( !auth_check ) {
            Log::writeLockedMessage( this->getLogId(), "AUTHENTICATION_FAILED: Cannot veirfy AUTH payload", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_AUTH, Payload_NOTIFY::AUTHENTICATION_FAILED );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // process CHILD_SA negotiation request payloads (SA, TSi, TSr) (from the EAP_INIT request)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationRequest( *this->eap_init_req );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        // Create the IKE_AUTH response for the request
        return this->createEapFinishResponse( );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createEapFinishResponse( ) {
        // creates the response message
        auto_ptr<Message> message = this->createMessage( Message::IKE_AUTH, Message::RESPONSE );

        // include all notifications (N)
        NetworkController::addNotifies( *message, *this, this->peer_creating_child_sa.get() );

        // includes the authentication payload (AUTH)
        auto_ptr<Payload_AUTH> payload_auth = this->getIkeSaConfiguration().getAuthenticator().generateEapAuthPayload( *this );
        message->addPayload( auto_ptr<Payload> ( payload_auth ), true );

        // includes the CHILD_SA negotiation response payloads (SA, TSi, TSr)
        this->createChildSaNegotiationResponse( *message );

        // creates the CHILD_SA physically
        this->createChildSa( this->peer_creating_child_sa );

        // updates the IKE_SA state
        this->setState( STATE_IKE_SA_ESTABLISHED );

        // sends the message
        this->sendMessage( message, "Send: EAP_FINISH response" );

        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_ESTABLISHED, *this ) ) );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processCreateChildSaRequest( Message & message ) {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive CREATE_CHILD_SA request] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::INVALID_SYNTAX );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        // IKE_SA REKEY
        if ( message.getFirstPayloadByType( Payload::PAYLOAD_TSi ) == NULL && message.getFirstPayloadByType( Payload::PAYLOAD_TSr ) == NULL )
            return this->processRekeyIkeSaRequest( message );

        // CHILD SA REKEY
        else if ( message.getFirstNotifyByType( Payload_NOTIFY::REKEY_SA ) != NULL )
            return this->processRekeyChildSaRequest( message );

        // NEW CHILD SA
        else
            return this->processNewChildSaRequest( message );
    }

    IkeSa::MESSAGE_ACTION IkeSa::processCreateChildSaResponse( Message & message ) {
        // Check state
        IkeSa::MESSAGE_ACTION action;

        // NEW CHILD SA
        if ( this->state == STATE_NEW_CHILD_SA_REQ_SENT )
            return this->processNewChildSaResponse( message );

        // REKEY CHILD SA
        else if ( this->state == STATE_REKEY_CHILD_SA_REQ_SENT )
            return this->processRekeyChildSaResponse( message );

        // REKEY REDUNDANT CHILD SA
        else if ( this->state == STATE_REDUNDANT_CHILD_SA )
            return this->processRedundantRekeyChildSaResponse( message );

        // REKEY IKE SA
        else if ( this->state == STATE_REKEY_IKE_SA_REQ_SENT )
            return this->processRekeyIkeSaResponse( message );

        // REKEY REDUNDANT IKE SA
        else if ( this->state == STATE_REDUNDANT_IKE_SA )
            return this->processRedundantRekeyIkeSaResponse( message );

        else {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive CREATE_CHILD_SA response] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }
    }

    IkeSa::IKE_SA_ACTION IkeSa::createNewChildSaRequest( auto_ptr<ChildSaRequest> child_sa_request ) {
        // Check state
	Log::writeLockedMessage( this->getLogId(), "Entrando en createNewChildSaRequest", Log::LOG_WARN, true );

	if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start NEW_CHILD_SA] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
        else if ( this->state == STATE_WAITING_FOR_DELETION || this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            // Omit the command
	Log::writeLockedMessage( this->getLogId(), "Punto 1", Log::LOG_WARN, true );

            return IKE_SA_ACTION_CONTINUE;
        }
        else if ( this->state > STATE_IKE_SA_ESTABLISHED ) {
	    Log::writeLockedMessage( this->getLogId(), "Punto 2", Log::LOG_WARN, true );
            this->pushDeferredCommand( auto_ptr<Command> ( new SendNewChildSaReqCommand( child_sa_request ) ) );
            return IKE_SA_ACTION_CONTINUE;
        }
	Log::writeLockedMessage( this->getLogId(), "Punto 3", Log::LOG_WARN, true );


    auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
        is_ha = is_ha_attr->value;


        BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
        bool mobility = false;
    auto_ptr<SocketAddress> hoa (NULL);
    auto_ptr<SocketAddress> coa (NULL);
        if (mobility_attr  != NULL )
        mobility = mobility_attr->value;

    if (mobility && (child_sa_request->mode == Enums::TUNNEL_MODE)){
        if (is_ha) {
            // creates outbound SA in the kernel
            this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->care_of_address->getIpAddress(),this->my_addr->getIpAddress(),child_sa_request->ipsec_protocol ), child_sa_request ) );

        }
        else {
            // creates outbound SA in the kernel
            this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->care_of_address->getIpAddress(),child_sa_request->ipsec_protocol ), child_sa_request ) );

        }
    }
    else {
        // creates outbound SA in the kernel
        this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(),child_sa_request->ipsec_protocol ), child_sa_request ) );


    }



	Log::writeLockedMessage( this->getLogId(), "Punto 4", Log::LOG_WARN, true );

        // creates the request message
        auto_ptr<Message> message = this->createMessage( Message::CREATE_CHILD_SA, Message::REQUEST );



/*
 	auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
		is_ha = is_ha_attr->value;

        // TODO

	if ( this->mobility ) {

		if (child_sa_request->mode == Enums::TUNNEL_MODE){
			if (is_ha){
				message->dst_addr = this->care_of_address->clone();
				Log::writeLockedMessage( this->getLogId(), "Cambiando el destino del mensaje por la ", Log::LOG_WARN, true );
			}
			else {
				message->src_addr = this->care_of_address->clone();
				Log::writeLockedMessage( this->getLogId(), "Punto 1", Log::LOG_WARN, true );
			}
		}

	}

*/
	Log::writeLockedMessage( this->getLogId(), "Punto 5", Log::LOG_WARN, true );


        // include notify payloads (N)
        NetworkController::addNotifies( *message, *this, this->my_creating_child_sa.get() );

	Log::writeLockedMessage( this->getLogId(), "Punto 6", Log::LOG_WARN, true );


        // include the CHILD_SA negotiation request payloads (SA, [KE], NONCE, TSi, TSr)
        this->createChildSaNegotiationRequest( *message );

	Log::writeLockedMessage( this->getLogId(), "Punto 7", Log::LOG_WARN, true );


        // sends the message
        this->sendMessage( message, "Send: NEW_CHILD_SA request" );

	Log::writeLockedMessage( this->getLogId(), "Punto 8", Log::LOG_WARN, true );


        // updates IKE_SA state
        this->setState( STATE_NEW_CHILD_SA_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processNewChildSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: CREATE_CHILD response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notifications (N)
        NOTIFY_ACTION action = this->processNotifies( message, this->my_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->my_creating_child_sa ) ) );
            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process CHILD_SA negotiation response payloads (SA, [KE], NONCE, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationResponse( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR )
            return MESSAGE_ACTION_OMIT;

        // Install the CHILD_SA in the kernel
        this->createChildSa( this->my_creating_child_sa );

        // updates IKE_SA state
        this->setState( STATE_IKE_SA_ESTABLISHED );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::IKE_SA_ACTION IkeSa::createRekeyChildSaRequest ( uint32_t spi_rekey ) {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start REKEY_CHILD_SA] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
        else if ( this->state == STATE_WAITING_FOR_DELETION || this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            // Omit the command
            return IKE_SA_ACTION_CONTINUE;
        }
        else if ( this->state > STATE_IKE_SA_ESTABLISHED ) {
            this->pushDeferredCommand( auto_ptr<Command> ( new SendRekeyChildSaReqCommand( spi_rekey ) ) );
            return IKE_SA_ACTION_CONTINUE;
        }

        // Obtains the ChildSa to be rekeyed
        ChildSa* rekeyed_child_sa = this->child_sa_collection->getChildSa( spi_rekey );

        // If we don't controls this ChildSa, then omit
        if ( rekeyed_child_sa == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: IKE_SA doesn't control this IPsec SA", Log::LOG_WARN, true );
            return IKE_SA_ACTION_CONTINUE;
        }

        // If the rekeyed CHILD_SA is already being rekeyed, then omit
        if ( rekeyed_child_sa->getState() == ChildSa::CHILD_SA_REKEYING || rekeyed_child_sa->getState() == ChildSa::CHILD_SA_REKEYED ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: IPSec SA=" + rekeyed_child_sa->getId() ->toString() + " already rekeyed or rekeying.", Log::LOG_WARN, true );
            return IKE_SA_ACTION_CONTINUE;
        }

        // If the CHILD_SA is being deleted, then omit
        if ( rekeyed_child_sa->getState() == ChildSa::CHILD_SA_DELETING ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: IPSec SA=" + rekeyed_child_sa->getId() ->toString() + " already deleting.", Log::LOG_WARN, true );
            return IKE_SA_ACTION_CONTINUE;
        }

        // updates the state of the rekeyed SA
        rekeyed_child_sa->setState( ChildSa::CHILD_SA_REKEYING );

        // creates the new SA, with the same attributes than the rekeyed
        this->my_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(), this->my_addr->getIpAddress() ,rekeyed_child_sa->ipsec_protocol), true, *rekeyed_child_sa ) );

        // creates the request message
        auto_ptr<Message> message = this->createMessage( Message::CREATE_CHILD_SA, Message::REQUEST );

        // include all notify request payloads in IKE_AUTH exchange
        NetworkController::addNotifies( *message, *this, this->my_creating_child_sa.get() );

        // include the CHILD_SA negotiation request payloads (SA, [KE], NONCE, TSi, TSr)
        this->createChildSaNegotiationRequest( *message );

        // sends the message
        this->sendMessage( message, "Send: REKEY_CHILD_SA request" );

        this->setState( STATE_REKEY_CHILD_SA_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processRekeyChildSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: REKEY_CHILD_SA response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // Obtain the rekeyed SA
        ChildSa *rekeyed_child_sa = this->child_sa_collection->getChildSa( this->my_creating_child_sa->rekeyed_spi );

        // if IPSEC SPI no longer exist, then it will be deleted by the peer and we must omit the response
        if ( rekeyed_child_sa == NULL ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->my_creating_child_sa ) ) );
            Log::writeLockedMessage( this->getLogId(), "Rekeyed SA no longer exists: Doing nothing", Log::LOG_WARN, true );
            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }

        // process notify payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message,  this->my_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->my_creating_child_sa ) ) );
            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process CHILD_SA negotiation response payloads (SA, [KE], NONCE, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationResponse( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR )
            return MESSAGE_ACTION_OMIT;

        // creates CHILD_SA physically
        this->createRekeyChildSa( this->my_creating_child_sa, *rekeyed_child_sa );

        // send a delete CHILD_SA request
        this->pushCommand( auto_ptr<Command> ( new SendDeleteChildSaReqCommand( rekeyed_child_sa->inbound_spi ) ), true );

        // updates rekeyed CHILD_SA state
        rekeyed_child_sa->setState ( ChildSa::CHILD_SA_REKEYED );

        // updates the IKE_SA state
        this->setState( STATE_IKE_SA_ESTABLISHED );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processRedundantRekeyChildSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: REKEY_CHILD_SA response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // Obtain the rekeyed SA
        ChildSa *rekeyed_child_sa = this->child_sa_collection->getChildSa( this->my_creating_child_sa->rekeyed_spi );

        // if IPSEC SPI no longer exist, then it will be deleted by the peer and we must omit the response
        if ( rekeyed_child_sa == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "Rekeyed SA no longer exists: Doing nothing", Log::LOG_WARN, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->my_creating_child_sa ) ) );
            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }

        // process the notify payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message, this->my_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->my_creating_child_sa ) ) );
            this->createRekeyChildSa( this->peer_creating_child_sa, *rekeyed_child_sa );
            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process CHILD_SA negotiation response payloads (SA, [KE], NONCE, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationResponse( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            return MESSAGE_ACTION_OMIT;
        }

        // find the looser redundant CHILD_SA
        ChildSa & looser_child_sa = ChildSa::hasMinNonce( *this->my_creating_child_sa, *this->peer_creating_child_sa );
        Log::writeLockedMessage( this->getLogId(), "Warning: Found redundant Child SAs. SPI1=" + this->my_creating_child_sa->getId()->toString() + " SPI2=" + this->peer_creating_child_sa->getId()->toString() + " SPI to be deleted=" + looser_child_sa.getId()->toString(), Log::LOG_WARN, true );

        // If loser is ours
        if ( &looser_child_sa == this->my_creating_child_sa.get() ) {
            this->createChildSa( this->my_creating_child_sa );
            this->createRekeyChildSa( this->peer_creating_child_sa, *rekeyed_child_sa );
            this->pushCommand( auto_ptr<Command> ( new SendDeleteChildSaReqCommand( looser_child_sa.inbound_spi ) ), false );
        }

        // if loser is peer's one
        else {
            this->createRekeyChildSa( this->my_creating_child_sa, *rekeyed_child_sa );
            this->createChildSa( this->peer_creating_child_sa );
            this->pushCommand( auto_ptr<Command> ( new SendDeleteChildSaReqCommand( rekeyed_child_sa->inbound_spi ) ), false );
        }

        // updates rekeyed CHILD_SA state
        rekeyed_child_sa->setState ( ChildSa::CHILD_SA_REKEYED );

        // updates IKE_SA state
        this->setState( STATE_IKE_SA_ESTABLISHED );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processNewChildSaRequest( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: NEW_CHILD_SA request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // If we are also rekeying the IKE SA, send NO_ADDITIONAL_SAS
        // Section 5.11.8 IKEv2 clarifications document */
        if ( this->state == STATE_REKEY_IKE_SA_REQ_SENT || this->state == STATE_REDUNDANT_IKE_SA || this->state == STATE_WAITING_FOR_DELETION ) {
            Log::writeLockedMessage( this->getLogId(), "NO_ADDITIONAL_SAS: See section 5.11.8 of IKEv2 clarifications document", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_ADDITIONAL_SAS );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );
	Proposal *proposal_sa = payload_sa.proposals->front() ;

	Enums::PROTOCOL_ID peer_selected_ipsec_proto = Enums::PROTO_NONE;
	if ( proposal_sa != NULL ){
		peer_selected_ipsec_proto = proposal_sa->protocol_id;
        }
        // create the new ChildSa object




    auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
        is_ha = is_ha_attr->value;


        BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
        bool mobility = false;
    auto_ptr<SocketAddress> hoa (NULL);
    auto_ptr<SocketAddress> coa (NULL);
        if (mobility_attr  != NULL )
        mobility = mobility_attr->value;

    Payload_NOTIFY* payload_notify = ( Payload_NOTIFY* ) message.getFirstPayloadByType( Payload::PAYLOAD_NOTIFY );

    if (mobility && (payload_notify == NULL)){ //TODO: asegurarse que sea realmente el notify que indica modo transporte
        if (is_ha) {
            // creates outbound SA in the kernel
            Log::writeLockedMessage( this->getLogId(), "COA ANTES: "+this->care_of_address->toStringTab(0), Log::LOG_ERRO, true );
            this->care_of_address = message.src_addr->clone();
            Log::writeLockedMessage( this->getLogId(), "COA DESPUES: "+this->care_of_address->toStringTab(0), Log::LOG_ERRO, true );
            this->peer_creating_child_sa.reset( new ChildSa( IpsecController::getSpi(this->care_of_address->getIpAddress(),this->my_addr->getIpAddress(),peer_selected_ipsec_proto), peer_selected_ipsec_proto, false ) );

        }
        else {
            // creates outbound SA in the kernel
            Log::writeLockedMessage( this->getLogId(), "COA ANTES: "+this->care_of_address->toStringTab(0), Log::LOG_ERRO, true );
            this->care_of_address = message.dst_addr->clone();
            Log::writeLockedMessage( this->getLogId(), "COA DESPUES: "+this->care_of_address->toStringTab(0), Log::LOG_ERRO, true );
            this->peer_creating_child_sa.reset( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->care_of_address->getIpAddress(),peer_selected_ipsec_proto), peer_selected_ipsec_proto, false ) );

        }
    }
    else {
        // creates outbound SA in the kernel
        this->peer_creating_child_sa.reset( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(),peer_selected_ipsec_proto), peer_selected_ipsec_proto, false ) );


    }



        // process notify payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message, this->peer_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->peer_creating_child_sa ) ) );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process CHILD_SA negotiation request payloads (SA, [KE], NONCE, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationRequest( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->peer_creating_child_sa ) ) );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // Create the NEW CHILD SA responses
        return this->createNewChildSaResponse( );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createNewChildSaResponse( ) {
        // creates the message
        auto_ptr<Message> message = this->createMessage( Message::CREATE_CHILD_SA, Message::RESPONSE );

        // includes all notify (N) payloads
        NetworkController::addNotifies( *message, *this, this->peer_creating_child_sa.get() );

        // includes CHILD_SA negotiation response payloads (SA, [KE], NONCE, TSi, TSr)
        this->createChildSaNegotiationResponse( *message );

        this->createChildSa( this->peer_creating_child_sa );

        // Keeps the same state
        this->setState( this->state );

        // sends the message
        this->sendMessage( message, "Send: New CREATE_CHILD response" );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processRekeyChildSaRequest( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: REKEY_CHILD_SA request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // If we are also rekeying the IKE SA, send NO_ADDITIONAL_SAS
        // Section 5.11.8 IKEv2 clarifications document */
        if ( this->state == STATE_REKEY_IKE_SA_REQ_SENT || this->state == STATE_REDUNDANT_IKE_SA || this->state == STATE_WAITING_FOR_DELETION ) {
            Log::writeLockedMessage( this->getLogId(), "NO_ADDITIONAL_SAS: See section 5.11.8 of IKEv2 clarifications document", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_ADDITIONAL_SAS );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );
	Proposal *proposal_sa = payload_sa.proposals->front() ;

	Enums::PROTOCOL_ID peer_selected_ipsec_proto = Enums::PROTO_NONE;
	if ( proposal_sa != NULL )
		peer_selected_ipsec_proto = proposal_sa->protocol_id;

        // creates the new CHILD SA object
        this->peer_creating_child_sa.reset ( new ChildSa( IpsecController::getSpi(this->peer_addr->getIpAddress(),this->my_addr->getIpAddress(),peer_selected_ipsec_proto), peer_selected_ipsec_proto, false ) );

        // process notification payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message, this->peer_creating_child_sa.get() );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->peer_creating_child_sa ) ) );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        // process CHILD_SA negotiation request payloads (SA, [KE], NONCE, TSi, TSr)
        NEGOTIATION_ACTION negotiation_action = this->processChildSaNegotiationRequest( message );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_FAILED, *this, *this->peer_creating_child_sa ) ) );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // Create the CREATE_CHILD_SA response for the request
        return this->createRekeyChildSaResponse();
    }

    IkeSa::MESSAGE_ACTION IkeSa::createRekeyChildSaResponse( ) {
        // creates the message
        auto_ptr<Message> message = this->createMessage( Message::CREATE_CHILD_SA, Message::RESPONSE );

        // include all the notify (N) payloads
        NetworkController::addNotifies( *message, *this, this->peer_creating_child_sa.get() );

        // include CHILD_SA negotiation response payloads (SA, [KE], NONCE, TSi, TSr)
        this->createChildSaNegotiationResponse( *message );


        // If we are already REKEYING this CHILD_SA, then change state to redundant CHILD_SA
        // Section 5.11.3 IKEv2 clarifications document
        ChildSa* rekeyed_child_sa = this->child_sa_collection->getChildSa ( this->peer_creating_child_sa->rekeyed_spi );
        assert ( rekeyed_child_sa != NULL );

        if ( this->state == STATE_REKEY_CHILD_SA_REQ_SENT && rekeyed_child_sa->getState() == ChildSa::CHILD_SA_REKEYING ) {
            // updates the IKE_SA state
            this->setState( STATE_REDUNDANT_CHILD_SA );

            // Keeps the same CHILD_SA state to avoid forget we are REKEYING
            rekeyed_child_sa->setState( ChildSa::CHILD_SA_REKEYING );
        }
        else {
            // Keeps the same state
            this->setState( this->state );

            rekeyed_child_sa->setState( ChildSa::CHILD_SA_REKEYED );
            this->createRekeyChildSa( this->peer_creating_child_sa,  *rekeyed_child_sa );
        }

        // sends the message
        this->sendMessage( message, "Send: Rekey CREATE_CHILD response" );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::IKE_SA_ACTION IkeSa::createRekeyIkeSaRequest() {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start REKEY_IKE_SA] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
        else if ( this->state == STATE_WAITING_FOR_DELETION || this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            // Omit the command
            return IKE_SA_ACTION_CONTINUE;
        }
        else if ( this->state > STATE_IKE_SA_ESTABLISHED ) {
            this->pushDeferredCommand( auto_ptr<Command> ( new SendRekeyIkeSaReqCommand() ) );
            return IKE_SA_ACTION_CONTINUE;
        }

        // creates the new IKE_SA
        this->my_creating_ike_sa.reset ( new IkeSa( IkeSaController::nextSpi(), true, *this ) );

        // creates the CREATE_CHILD_SA request message
        auto_ptr<Message> message = this->createMessage( Message::CREATE_CHILD_SA, Message::REQUEST );

        // include all the notify (N) payloads in the message
        NetworkController::addNotifies( *message, *this, NULL );

        // include the IKE_SA negotiation payloads (SA, KE, NONCE)
        this->createIkeSaNegotiationRequest( *message, *this->my_creating_ike_sa );

        // sends the message
        this->sendMessage( message, "Send: IKE_SA_REKEY request" );

        // Commit state transition
        this->setState( STATE_REKEY_IKE_SA_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processRekeyIkeSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_SA_REKEY response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notify (N) payloads
        NOTIFY_ACTION action = this->processNotifies( message, NULL );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this->my_creating_ike_sa ) ) );
            this->setState( STATE_WAITING_FOR_DELETION );
            this->pushCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ), true );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT ) {
            return MESSAGE_ACTION_OMIT;
        }

        // process IKE_SA negotiation response payloads (SA, KE, NONCE)
        NEGOTIATION_ACTION negotiation_action = this->processIkeSaNegotiationResponse( message, *this->my_creating_ike_sa );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            return MESSAGE_ACTION_OMIT;
        }

        // Create the rekeyed IKE_SA
        this->createRekeyIkeSa( this->my_creating_ike_sa );

        // Starts the delete exchange for the old IKE_SA
        this->pushCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ), true );

        // changes state
        this->setState( STATE_WAITING_FOR_DELETION );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processRedundantRekeyIkeSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_SA_REKEY response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notify (N) payloads
        NOTIFY_ACTION action = this->processNotifies( message, NULL );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this->my_creating_ike_sa ) ) );
            this->setState( STATE_WAITING_FOR_DELETION );
            this->createRekeyIkeSa( this->peer_creating_ike_sa );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT ) {
            return MESSAGE_ACTION_OMIT;
        }

        // process IKE_SA negotiation response payloads (SA, KE, NONCE)
        NEGOTIATION_ACTION negotiation_action = this->processIkeSaNegotiationResponse( message, *this->my_creating_ike_sa );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            return MESSAGE_ACTION_OMIT;
        }

        // get the looser redundant IKE_SA
        IkeSa & looser_ike_sa = IkeSa::hasMinNonce( *this->my_creating_ike_sa, *this->peer_creating_ike_sa );

        Log::writeLockedMessage( this->getLogId(), "Warning: Redundant IKE SA: SPI reduntant=" + Printable::toHexString( &looser_ike_sa.my_spi, 8 ), Log::LOG_WARN, true );

        // If the redundant to be deleted is ours created, then start both and delete mine
        if ( &looser_ike_sa == this->my_creating_ike_sa.get() ) {
            this->createRekeyIkeSa( this->peer_creating_ike_sa );
            this->my_creating_ike_sa->setState( STATE_WAITING_FOR_DELETION );
            this->my_creating_ike_sa->pushCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ), true );
            IkeSaController::addIkeSa( this->my_creating_ike_sa );
        }

        // if the redundat to be deleted is peers one, then start both and delete old one
        else {
            this->createRekeyIkeSa( this->my_creating_ike_sa );
            this->peer_creating_ike_sa->setState( STATE_WAITING_FOR_DELETION );
            this->pushCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ), true );
            IkeSaController::addIkeSa( this->peer_creating_ike_sa );
        }

        this->setState( STATE_WAITING_FOR_DELETION );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processRekeyIkeSaRequest( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: IKE_SA_REKEY request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // If the state is redundant, no additional REKEY IKE_SA is allowed
        if ( this->state == STATE_REDUNDANT_IKE_SA ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Recv REKEY_IKE_SA request] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::INVALID_SYNTAX );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // If we have CHILD_SAs half-open, then reject the IKE_SA rekey
        // References 5.11.8 IKEv2 clarifications document
        if ( this->state == STATE_REKEY_CHILD_SA_REQ_SENT || this->state == STATE_REDUNDANT_CHILD_SA || this->state == STATE_NEW_CHILD_SA_REQ_SENT || this->state == STATE_DELETE_CHILD_SA_REQ_SENT ) {
            Log::writeLockedMessage( this->getLogId(), "NO_PROPOSAL_CHOSEN: See section 5.11.8 of the IKEv2 clarification document", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_PROPOSAL_CHOSEN );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // If we are DELETING the IKE_SA, then reject the IKE_SA rekey
        // References 5.11.9 IKEv2 clarifications document
        else if ( this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            Log::writeLockedMessage( this->getLogId(), "NO_PROPOSAL_CHOSEN: See section 5.11.9 of the IKEv2 clarification document", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_PROPOSAL_CHOSEN );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // If we are DELETING the IKE_SA, then reject the IKE_SA rekey
        // References 5.11.9 IKEv2 clarifications document
        else if ( this->state == STATE_WAITING_FOR_DELETION ) {
            Log::writeLockedMessage( this->getLogId(), "NO_PROPOSAL_CHOSEN: The IKE_SA has been already rekeyed. Waiting for delete exchange", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_PROPOSAL_CHOSEN );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // creates the new IKE_SA
        this->peer_creating_ike_sa.reset ( new IkeSa( IkeSaController::nextSpi(), false, *this ) );

        // process notification payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message, NULL );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this->peer_creating_ike_sa ) ) );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT ) {
            return MESSAGE_ACTION_OMIT;
        }

        // process IKE_SA negotiation request payloads (SA, KE, NONCE)
        NEGOTIATION_ACTION negotiation_action = this->processIkeSaNegotiationRequest( message, *this->peer_creating_ike_sa );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this->peer_creating_ike_sa ) ) );
            this->setState( this->state );
            return MESSAGE_ACTION_COMMIT;
        }

        // Creates the response to the REKEY_IKE_SA request
        return this->createRekeyIkeSaResponse( );
    }

    IkeSa::MESSAGE_ACTION IkeSa::createRekeyIkeSaResponse( ) {
        // creates the message
        auto_ptr<Message> message = this->createMessage( Message::CREATE_CHILD_SA, Message::RESPONSE );

        // include all the notify request payload (N)
        NetworkController::addNotifies( *message, *this, NULL  );

        // include the IKE_SA negotiation reponse payloads (SA, KE, NONCE)
        this->createIkeSaNegotiationResponse( *message, *this->peer_creating_ike_sa );

        // sends the message
        this->sendMessage( message, "Send: IKE_SA_REKEY response" );

        // If we are also rekeying the IKE SA, change state to REDUNDANT IKE_SA
        if ( this->state == STATE_REKEY_IKE_SA_REQ_SENT ) {
            this->setState( STATE_REDUNDANT_IKE_SA );
        }
        else {
            this->createRekeyIkeSa( this->peer_creating_ike_sa );
            this->setState( STATE_WAITING_FOR_DELETION );
        }

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::createInformationalResponse( AutoVector<Payload> responses ) {
        // Creates Message
        auto_ptr<Message> message = this->createMessage( Message::INFORMATIONAL, Message::RESPONSE );

        // add notify (N) paylaods
        NetworkController::addNotifies( *message, *this, NULL );

        // adds encrypted payloads
        message->addPayloads( responses, true );

        // add configuration response
        NetworkController::createConfigurationResponse( *message, *this );

        // sends the message
        this->sendMessage( message, "Send: INFORMATIONAL response" );

        this->setState( this->state );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::IKE_SA_ACTION IkeSa::createDeleteChildSaRequest( uint32_t spi ) {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start DELETE_CHILD_SA] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
        else if ( this->state == STATE_WAITING_FOR_DELETION || this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            // Omit the command
            return IKE_SA_ACTION_CONTINUE;
        }
        else if ( this->state > STATE_IKE_SA_ESTABLISHED ) {
            this->pushDeferredCommand( auto_ptr<Command> ( new SendDeleteChildSaReqCommand( spi ) ) );
            return IKE_SA_ACTION_CONTINUE;
        }

        // get the child sa to be deleted. if the child sa has been already deleted or it is in deleting state, OMIT deletion
        ChildSa* child_sa = this->child_sa_collection->getChildSa( spi );
        if ( child_sa == NULL || child_sa->getState() == ChildSa::CHILD_SA_DELETING ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: IPSec SA=" + Printable::toHexString( &spi, 4 ) + " already deleted.", Log::LOG_WARN, true );
            return IKE_SA_ACTION_CONTINUE;
        }

        // create the INFORMATIONAL message
        auto_ptr<Message> message = this->createMessage( Message::INFORMATIONAL, Message::REQUEST );

        // include the delete payload (D)
        auto_ptr<Payload_DEL> payload_del ( new Payload_DEL( child_sa->ipsec_protocol ) );
        payload_del->addIpsecSpi( child_sa->inbound_spi );
        message->addPayload ( auto_ptr<Payload> ( payload_del ), true );

        // sends the message
        this->sendMessage( message, "Send: DELETE_CHILD_SA request" );

        // deletes the inbound IPsec SA
        Log::writeLockedMessage( this->getLogId(), "Deleting IPSEC SA=" + child_sa->getId() ->toString(), Log::LOG_IPSC, true );
        IpsecController::deleteIpsecSa( this->peer_addr->getIpAddress(), this->my_addr->getIpAddress(), child_sa->ipsec_protocol, child_sa->inbound_spi );

        // updates IKE_SA and CHILD_SA states
        this->setState( STATE_DELETE_CHILD_SA_REQ_SENT );
        child_sa->setState( ChildSa::CHILD_SA_DELETING );

        return IKE_SA_ACTION_CONTINUE;
    }


    IkeSa::IKE_SA_ACTION IkeSa::createDeleteIkeSaRequest() {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start DELETE_IKE_SA] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
        else if ( this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            // Omit the command
            return IKE_SA_ACTION_CONTINUE;
        }
        else if ( this->state != STATE_IKE_SA_ESTABLISHED && this->state != STATE_WAITING_FOR_DELETION ) {
            this->pushDeferredCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ) );
            return IKE_SA_ACTION_CONTINUE;
        }

        // create the INFORMATIONAL message
        auto_ptr<Message> message = this->createMessage( Message::INFORMATIONAL, Message::REQUEST );

        // creates the Payload_DEL
        auto_ptr<Payload_DEL> payload_del ( new Payload_DEL( Enums::PROTO_IKE ) );
        message->addPayload( auto_ptr<Payload> ( payload_del ), true );

        // sends the message
        this->sendMessage( message, "Send: DELETE_IKE_SA request" );

        // updates IKE_SA
        this->setState( STATE_DELETE_IKE_SA_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::IKE_SA_ACTION IkeSa::createGenericInformationalRequest( AutoVector< Payload > payloads ) {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Start generic INFORMATIONAL] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
        else if ( this->state == STATE_WAITING_FOR_DELETION || this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            // Omit the command
            return IKE_SA_ACTION_CONTINUE;
        }
        else if ( this->state > STATE_IKE_SA_ESTABLISHED ) {
            this->pushDeferredCommand( auto_ptr<Command> ( new SendInformationalReqCommand( payloads ) ) );
            return IKE_SA_ACTION_CONTINUE;
        }

        // create the INFORMATIONAL message
        auto_ptr<Message> message = this->createMessage( Message::INFORMATIONAL, Message::REQUEST );

        // adds the payloads
        message->addPayloads( payloads, true );

        // sends the message
        this->sendMessage( message, "Send: INFORMATIONAL request" );

        this->setState( STATE_GENERIC_INFORMATIONAL_REQ_SENT );

        return IKE_SA_ACTION_CONTINUE;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processInformationalRequest( Message & message ) {
        // Check state
        if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive INFORMATIONAL request] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::INFORMATIONAL, Payload_NOTIFY::INVALID_SYNTAX );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }

        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: INFORMATIONAL request", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notification payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message, NULL );
        if ( action == NOTIFY_ACTION_ERROR ) {
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return MESSAGE_ACTION_DELETE_IKE_SA;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        AutoVector<Payload> response;

        // process delete payloads (D)
        vector<Payload*> payloads_delete = message.getPayloadsByType( Payload::PAYLOAD_DEL );

        AutoVector<Payload> temp = this->processDeleteRequest( payloads_delete );
        for ( uint16_t i = 0; i < temp->size(); i++ )
            response->push_back( temp[ i ] );
        temp->clear();

        // process configuration payloads (CP)
        NetworkController::processConfigurationRequest( message, *this );

        return this->createInformationalResponse( response );
    }


    IkeSa::MESSAGE_ACTION IkeSa::processInformationalResponse( Message & message ) {
        if ( this->state == STATE_DELETE_IKE_SA_REQ_SENT ) {
            return this->processDeleteIkeSaResponse( message );
        }
        else if ( this->state == STATE_DELETE_CHILD_SA_REQ_SENT ) {
            return this->processDeleteChildSaResponse( message );
        }
        else if ( this->state == STATE_GENERIC_INFORMATIONAL_REQ_SENT ) {
            return this->processGenericInformationalResponse( message );
        }
        else {
            Log::writeLockedMessage( this->getLogId(), "Transition error: event=[Receive INFORMATIONAL response] state=[" + IKE_SA_STATE_STR( this->state ) + "]", Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }
    }

    IkeSa::MESSAGE_ACTION IkeSa::processGenericInformationalResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: INFORMATIONAL response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process notification payloads (N)
        NOTIFY_ACTION action = this->processNotifies( message, NULL );
        if ( action == NOTIFY_ACTION_ERROR ) {
            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }
        else if ( action == NOTIFY_ACTION_OMIT )
            return MESSAGE_ACTION_OMIT;

        vector<Payload*> payloads_config = message.getPayloadsByType( Payload::PAYLOAD_CONF );
        this->processConfigResponse( payloads_config );

        this->setState( STATE_IKE_SA_ESTABLISHED );

        return MESSAGE_ACTION_COMMIT;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processDeleteIkeSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: DELETE_IKE_SA response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process the unique delete payload
        Payload_DEL& payload_del = ( Payload_DEL& ) message.getUniquePayloadByType( Payload::PAYLOAD_DEL );

        // check the protocol of the delete payload
        if ( payload_del.protocol_id != Enums::PROTO_IKE ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: Invalid DELETE_IKE_SA reponse received. Protocol mismatch received=[" + Enums::PROTOCOL_ID_STR( payload_del.protocol_id ) + "]", Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }

        this->setState( STATE_CLOSED );

        // if all is OK, delete the IKE_SA
        return MESSAGE_ACTION_DELETE_IKE_SA;
    }

    IkeSa::MESSAGE_ACTION IkeSa::processDeleteChildSaResponse( Message & message ) {
        Log::acquire();
        Log::writeMessage( this->getLogId(), "Recv: DELETE_CHILD_SA response", Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message.toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // process the unique delete payload
        Payload_DEL* payload_del = ( Payload_DEL* ) message.getFirstPayloadByType( Payload::PAYLOAD_DEL );

        if ( payload_del == NULL ) {
            // check for half-closed CHILD_SAs
            if ( this->child_sa_collection->hasHalfClosedChildSas() ) {
                Log::writeLockedMessage( this->getLogId(), "Warning: Peer doesn't delete all the CHILD SAs. Closing IKE_SA\n", Log::LOG_WARN, true );
                this->pushCommand( auto_ptr<Command> ( new SendDeleteIkeSaReqCommand() ), false );
            }

            this->setState( STATE_IKE_SA_ESTABLISHED );
            return MESSAGE_ACTION_COMMIT;
        }

        // check the SPI count == 1
        if ( payload_del->ipsec_spi_values.size() != 1 ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: Invalid DELETE_CHILD_SA reponse received. More than one SPI count=[" + intToString( payload_del->ipsec_spi_values.size() ) + "]", Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }

        uint32_t spi = payload_del->ipsec_spi_values.front();

        // Gets the Child_SA that controls this IPSec_SA spi
        ChildSa* child_sa = this->child_sa_collection->getChildSa( spi );

        // If the child sa doesn't exist
        if ( child_sa == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: Invalid DELETE_CHILD_SA response. IKE_SA doesn't control indicated spi. SPI=" + Printable::toHexString( &spi, 4 ), Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }

        if ( child_sa->getState() != ChildSa::CHILD_SA_DELETING ) {
            Log::writeLockedMessage( this->getLogId(), "Warning: Invalid DELETE_CHILD_SA response. CHILD_SA not in deleting state. SPI=" + child_sa->getId()->toString( ), Log::LOG_WARN, true );
            return MESSAGE_ACTION_OMIT;
        }

        uint16_t count = this->child_sa_collection->size() - 1;
        Log::writeLockedMessage( this->getLogId(), "Deleting Child SA: Count=[" + intToString( count ) + "]", Log::LOG_INFO, true );
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_DELETED, *this, *child_sa, &count ) ) );
        this->child_sa_collection->deleteChildSa( child_sa->inbound_spi );

        // Deletes de outbound IPSec_SA
        IpsecController::deleteIpsecSa( this->my_addr->getIpAddress(), this->peer_addr->getIpAddress(), child_sa->ipsec_protocol, spi );

        // commit state
        this->setState( STATE_IKE_SA_ESTABLISHED );

        return MESSAGE_ACTION_COMMIT;
    }

    AutoVector< Payload > IkeSa::processDeleteRequest( vector< Payload * > payloads_delete ) {
        AutoVector<Payload> result;

        this->processDeleteIkeSaRequest( payloads_delete, result );

        this->processDeleteChildSaRequest( payloads_delete, result );

        return result;
    }

    void IkeSa::processDeleteIkeSaRequest( vector< Payload * > payloads_delete, AutoVector<Payload> & result ) {
        //AutoVector<Payload> result;

        // TODO: Mejorar para poder enviar invalid syntax s hay más de uno



        // For each Payload_DEL received do
        for ( uint16_t i = 0; i < payloads_delete.size();i++ ) {
            Payload_DEL& received_payload_del = ( Payload_DEL & ) * payloads_delete[ i ];
            auto_ptr<Payload_DEL> response_payload_del ( new Payload_DEL( received_payload_del.protocol_id ) );

            // If deleting an IKE_SA,
            if ( received_payload_del.protocol_id == Enums::PROTO_IKE ) {

                // If IKE_SA was rekeyig, then start the waiting IKE_SA since aany REKEY_IKE_SA response will be received
                if ( this->state == STATE_REDUNDANT_IKE_SA )
                    this->createRekeyIkeSa( this->peer_creating_ike_sa );

                this->pushCommand( auto_ptr<Command> ( new ExitIkeSaCommand() ), true );

                // Insert the payload del in the result
                result->push_back( response_payload_del.release() );

                //return result;
            }
        }
    }

    void IkeSa::processDeleteChildSaRequest( vector< Payload * > payloads_delete, AutoVector<Payload> & result ) {
        //AutoVector<Payload> result;

        // If we are also rekeying the IKE SA, return no DELETE payloads for AH nor ESP
        // Section 5.11.8 IKEv2 clarifications document */
        if ( this->state == STATE_REKEY_IKE_SA_REQ_SENT ) {
            // Result MUST contain no payloads for AH nor ESP
            //return result;
            return;
        }

        // For each Payload_DEL received do
        for ( uint16_t i = 0; i < payloads_delete.size();i++ ) {
            Payload_DEL& received_payload_del = ( Payload_DEL & ) * payloads_delete[ i ];
            auto_ptr<Payload_DEL> response_payload_del ( new Payload_DEL( received_payload_del.protocol_id ) );

            // If deleting an IPSec_SA
            if ( received_payload_del.protocol_id == Enums::PROTO_AH || received_payload_del.protocol_id == Enums::PROTO_ESP ) {
                // For each SPI in this Payload_DEL
                for ( uint16_t j = 0; j < received_payload_del.ipsec_spi_values.size(); j++ ) {
                    uint32_t spi = received_payload_del.ipsec_spi_values[ j ];

                    // Gets the Child_SA that controls this IPSec_SA spi
                    ChildSa* child_sa = this->child_sa_collection->getChildSa( spi );

                    // If this IKE_SA doesn't controlls the CHILD_SA, then omit
                    if ( child_sa == NULL ) {
                        Log::writeLockedMessage( this->getLogId(), "Warning: Invalid delete in request. Omitting", Log::LOG_WARN, true );
                        continue;
                    }

                    // If the CHILD_SA doesn't match the payload_del protocol
                    if ( child_sa->ipsec_protocol != received_payload_del.protocol_id ) {
                        Log::writeLockedMessage( this->getLogId(), "Warning: Invalid child_sa protocol. Omitting", Log::LOG_WARN, true );
                        continue;
                    }

                    // If we are deleting a REKEYING CHILD_SA and there are a redundant, start the peer creating CHILD_SA and forget about our rekeying
                    if ( this->state == STATE_REDUNDANT_CHILD_SA && child_sa->getState() == ChildSa::CHILD_SA_REKEYING ) {
                        this->createRekeyChildSa( this->peer_creating_child_sa, *child_sa );
                        this->retransmition_alarm->disable();
                        this->setState( STATE_IKE_SA_ESTABLISHED );
                    }

                    // Deletes the outbound IPSec_SA
                    Log::writeLockedMessage( this->getLogId(), "Deleting IPSEC SA=" + child_sa->getId() ->toString(), Log::LOG_IPSC, true );
                    IpsecController::deleteIpsecSa( this->my_addr->getIpAddress(), this->peer_addr->getIpAddress(), child_sa->ipsec_protocol, spi );

                    // If we are not already deleting this CHILD_SA, then add a delete payload for the inbound spi to the response
                    // Section 5.11.1 IKEV2 clarifications document
                    if ( this->state != STATE_DELETE_CHILD_SA_REQ_SENT || child_sa->getState() != ChildSa::CHILD_SA_DELETING ) {
                        // Adds the inbound SPI in the response Payload_DEL
                        response_payload_del->addIpsecSpi( child_sa->inbound_spi );

                        // Deletes the inbound IPSec_SA
                        IpsecController::deleteIpsecSa( this->peer_addr->getIpAddress(), this->my_addr->getIpAddress(), child_sa->ipsec_protocol, child_sa->inbound_spi );
                    }

                    uint16_t count =  this->child_sa_collection->size() - 1;
                    Log::writeLockedMessage( this->getLogId(), "Deleting Child SA: Count=[" + intToString( count ) + "]", Log::LOG_INFO, true );
                    EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_DELETED, *this, *child_sa, &count ) ) ) ;
                    this->child_sa_collection->deleteChildSa( child_sa->inbound_spi );
                }

                // Inser the payload del in the result if there is any SPI value
                // Section 5.11.1 IKEV2 clarifications document
                if ( response_payload_del->ipsec_spi_values.size() > 0 )
                    result->push_back( response_payload_del.release() );
            }
        }

        // return result;
    }

    AutoVector<Payload> IkeSa::processConfigRequest( vector< Payload * > payloads_config ) {
        AutoVector<Payload> result;
        return result;
    }

    void IkeSa::processConfigResponse( vector< Payload * > payloads_config ) {
        // NOT YET
    }

    void IkeSa::sendNotifyResponse( Message::EXCHANGE_TYPE exchange_type, Payload_NOTIFY::NOTIFY_TYPE notify_type ) {
        auto_ptr<Payload_NOTIFY> payload_notify ( new Payload_NOTIFY( notify_type, Enums::PROTO_NONE, auto_ptr<ByteArray> ( NULL ) , auto_ptr<ByteArray> ( NULL ) ) );
        this->sendNotifyResponse( exchange_type, payload_notify );
    }

    void IkeSa::sendNotifyResponse( Message::EXCHANGE_TYPE exchange_type, auto_ptr<Payload_NOTIFY> notify ) {
        auto_ptr<Message> message = this->createMessage( exchange_type, Message::RESPONSE );

        Payload_NOTIFY::NOTIFY_TYPE notification_type = notify->notification_type;
        bool is_error = notify->isError();

        // Adds unencrypted payload if exchange type is IKE_SA_INIT
        if ( exchange_type == Message::IKE_SA_INIT )
            message->addPayload( auto_ptr<Payload> ( notify ), false );
        // Else adds encrypted payload
        else
            message->addPayload( auto_ptr<Payload> ( notify ), true );

        // sends the message
        if ( is_error )
            this->sendMessage( message, "Send: Error NOTIFY type=[" + Payload_NOTIFY::NOTIFY_TYPE_STR( notification_type ) + "]" );
        else
            this->sendMessage( message, "Send: Status NOTIFY type=[" + Payload_NOTIFY::NOTIFY_TYPE_STR( notification_type ) + "]" );
    }

    IkeSa::IKE_SA_ACTION IkeSa::processMessage( Message & message ) {


        // If Peer SPI is invalid or has a invalid message ID, omits message
        if ( !this->checkPeerIkeSpi( message ) || !this->checkMessageId( message ) )
            return IKE_SA_ACTION_CONTINUE;

        // Check if the received message is an INVALID_IKE_SPI notify
        if ( message.getFirstNotifyByType( Payload_NOTIFY::INVALID_IKE_SPI ) != NULL ) {
            // Log response and omit it
            Log::writeLockedMessage( this->getLogId(), "Peer sent INVALID_IKE_SPI message", Log::LOG_WARN, true );
            return IKE_SA_ACTION_CONTINUE;
        }

        // If integrity check fails, then omits message and log event
        if ( !message.checkIntegrity( this->receive_cipher.get() ) ) {
            Log::writeLockedMessage( this->getLogId(), "Integrity check failed", Log::LOG_ERRO, true );
            return IKE_SA_ACTION_CONTINUE;
        }

        // If this message is a request and has already been transmitted, retransmit last response and return
        if ( message.message_type == Message::REQUEST && ( message.message_id == this->peer_message_id - 1 ) ) {
            // Send previous response
            this->retransmitLastResponse();
            return IKE_SA_ACTION_CONTINUE;
        }

        // Generetes the payloads objects
        try {
            message.decryptPayloadSK( this->receive_cipher.get() );
        }
        catch ( UnknownPayloadException & ex ) {
            Log::writeLockedMessage( this->getLogId(), ex.what() , Log::LOG_ERRO, true );
            if ( message.message_type == Message::RESPONSE ) {
                // At this point is needed that peer_spi is initialized
                this->peer_spi = message.spi_i;
                auto_ptr<ByteBuffer> byte_buffer ( new ByteBuffer( 1 ) );
                byte_buffer->writeInt8( ex.payload_type );
                this->sendNotifyResponse( message.exchange_type, auto_ptr<Payload_NOTIFY> ( new Payload_NOTIFY( Payload_NOTIFY::UNSUPPORTED_CRITICAL_PAYLOAD, Enums::PROTO_NONE, auto_ptr<ByteArray> ( NULL ), auto_ptr<ByteArray> ( byte_buffer ) ) ) );
            }

            if ( message.exchange_type == Message::IKE_SA_INIT || message.exchange_type == Message::IKE_AUTH ) {
                EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
                return IKE_SA_ACTION_DELETE_IKE_SA;
            }
            return IKE_SA_ACTION_CONTINUE;
        }
        catch ( ParsingException & ex ) {
            Log::writeLockedMessage( this->getLogId(), ex.what() , Log::LOG_ERRO, true );
            if ( message.message_type == Message::REQUEST )
                this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );

            if ( message.exchange_type == Message::IKE_SA_INIT || message.exchange_type == Message::IKE_AUTH ) {
                EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
                return IKE_SA_ACTION_DELETE_IKE_SA;
            }
            return IKE_SA_ACTION_CONTINUE;
        }

        // Process received message
        try {

	    IkeSa::MESSAGE_ACTION action = MESSAGE_ACTION_COMMIT;


            if ( this->mobility ) {
                Payload_NOTIFY* payload_notify = ( Payload_NOTIFY* ) message.getFirstPayloadByType( Payload::PAYLOAD_NOTIFY );

                auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

                /*auto_ptr<SocketAddress> hoa (NULL);
                StringAttribute* string_attr = general_conf->attributemap->getAttribute<StringAttribute>( "home_address" );
                if (string_attr!=NULL ){
                    auto_ptr<IpAddress> addr = NetworkController::getIpAddress( string_attr->value );
                    hoa = NetworkController::getSocketAddress(addr,500) ;
                }*/


                if (( message.exchange_type == Message::CREATE_CHILD_SA ) && (payload_notify!=NULL) ){
                    Log::writeLockedMessage( "NetworkController", "Mobility: Changing CoA to HoA in received message in a second chance.", Log::LOG_WARN, true );
                    if ( ! this->is_ha ){
                        Log::writeLockedMessage( "NetworkController", "Mobility: Let's Changing CoA to HoA in MR .", Log::LOG_WARN, true );
                        this->care_of_address = message.dst_addr->clone();
                        Log::writeLockedMessage( "NetworkController", "Mobility: ------- .", Log::LOG_WARN, true );

                        message.dst_addr = this->home_address->clone();
                        Log::writeLockedMessage( "NetworkController", "Mobility: CoA to HoA in MR  changed.", Log::LOG_WARN, true );
                    }
                    else {
                        Log::writeLockedMessage( "NetworkController", "Mobility: Let's Changing CoA to HoA in HA .", Log::LOG_WARN, true );
                        this->care_of_address = message.src_addr->clone();
                        Log::writeLockedMessage( "NetworkController", "Mobility: ------- .", Log::LOG_WARN, true );
                        Log::writeLockedMessage( "NetworkController", "CoA: ." + this->care_of_address->toStringTab(0), Log::LOG_WARN, true );

                        message.src_addr = this->home_address->clone();
                        Log::writeLockedMessage( "NetworkController", "Mobility: CoA to HoA in MR  changed.", Log::LOG_WARN, true );
                        Log::writeLockedMessage( "NetworkController", "Source: ." + message.src_addr->toStringTab(0), Log::LOG_WARN, true );
                    }
                }
                else {
                   Log::writeLockedMessage( "NetworkController", "Mobility: NOT CHANGING CoA for HoA in a second chance.", Log::LOG_WARN, true );
                }
            }


            // IKE_SA_INIT request
            if ( message.exchange_type == Message::IKE_SA_INIT && message.message_type == Message::REQUEST )
                action = this->processIkeSaInitRequest( message );

            // IKE_SA_INIT response
            else if ( message.exchange_type == Message::IKE_SA_INIT && message.message_type == Message::RESPONSE )
                action = this->processIkeSaInitResponse( message );

            // IKE_AUTH request
            else if ( message.exchange_type == Message::IKE_AUTH && message.message_type == Message::REQUEST )
                action = this->processIkeAuthRequest( message );

            // IKE_AUTH response
            else if ( message.exchange_type == Message::IKE_AUTH && message.message_type == Message::RESPONSE )
                action = this->processIkeAuthResponse( message );

            // CREATE_CHILD_SA request
            else if ( message.exchange_type == Message::CREATE_CHILD_SA && message.message_type == Message::REQUEST )
                action = this->processCreateChildSaRequest( message );

            // CREATE_CHILD_SA response
            else if ( message.exchange_type == Message::CREATE_CHILD_SA && message.message_type == Message::RESPONSE )
                action = this->processCreateChildSaResponse( message );

            // INFORMATIONAL request
            else if ( message.exchange_type == Message::INFORMATIONAL && message.message_type == Message::REQUEST )
                action = this->processInformationalRequest( message );

            // INFORMATIONAL  response
            else if ( message.exchange_type == Message::INFORMATIONAL && message.message_type == Message::RESPONSE )
                action = this->processInformationalResponse( message );

            // If ACTION is OMIT, then return immediately
            if ( action == MESSAGE_ACTION_OMIT )
                return IKE_SA_ACTION_CONTINUE;

            // else if action is close IKE_SA, then return and deletes the IKE_SA
            else if ( action == MESSAGE_ACTION_DELETE_IKE_SA )
        		return IKE_SA_ACTION_DELETE_IKE_SA;
            // IF ACTION  is CONTINUE, then do the following

            // If we are initiators
            if ( message.message_type == Message::RESPONSE ) {
                // Disables retransmit alarm
                this->retransmition_alarm->disable();

                // Increments message id
                this->my_message_id++;

                // Reset timeout retries
                this->remaining_timeout_retries = this->getIkeSaConfiguration().ike_max_exchange_retransmitions;
            }
            // If we are responders
            else
                this->peer_message_id++;
            return IKE_SA_ACTION_CONTINUE;
        }
        catch ( Exception & ex ) {
            // If message is a response, then
            if ( message.message_type == Message::RESPONSE ) {
                Log::writeLockedMessage( this->getLogId(), "GENERIC FAILURE: " + string( ex.what() ), Log::LOG_ERRO, true );
            }
            // Else if message is a request, then
            else if ( message.message_type == Message::REQUEST ) {
                Log::writeLockedMessage( this->getLogId(), "GENERIC FAILURE: " + string( ex.what() ), Log::LOG_ERRO, true );
                this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
            }

            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }
    }

    IkeSa::IKE_SA_ACTION IkeSa::retransmitLastRequest() {
        // If timeout retries count is exceded, finalices IkeSa execution
        if ( this->remaining_timeout_retries == 0 ) {
            Log::writeLockedMessage( this->getLogId(), "Timeout retries exceeded", Log::LOG_ERRO, true );
            EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
            return IKE_SA_ACTION_DELETE_IKE_SA;
        }

        // Decrease remaining timeout retries
        this->remaining_timeout_retries--;

        // Retransmit request
        NetworkController::sendMessage( *this->last_sent_request, this->send_cipher.get() );

        // Resets retransmition alarm
        this->retransmition_alarm->setTime( this->retransmition_alarm->getTotalTime() + this->getIkeSaConfiguration().retransmition_factor * 1000 );
        this->retransmition_alarm->reset();

        Log::writeLockedMessage( this->getLogId(), "Retr: Last request. Next retransmition in=[" + intToString( this->retransmition_alarm->getTotalTime() / 1000 ) + "] seconds", Log::LOG_INFO, true );

        return IKE_SA_ACTION_CONTINUE;
    }

    void IkeSa::retransmitLastResponse() {
        // Retransmit last response
        NetworkController::sendMessage( *this->last_sent_response, this->send_cipher.get() );

        Log::writeLockedMessage( this->getLogId(), "Retr: Last response", Log::LOG_INFO, true );
    }

    void IkeSa::inheritIkeSaStatus( IkeSa & other ) {
        AutoLock other_auto_lock( *other.mutex );
        AutoLock this_auto_lock( *this->mutex );

        // Inherits all child sas
        this->child_sa_collection = other.child_sa_collection;
        other.child_sa_collection.reset ( new ChildSaCollection() );

        Log::acquire();
        Log::writeMessage( this->getLogId(), "Inherit Child SAs from SPI=" + Printable::toHexString( &other.my_spi, 8 ), Log::LOG_INFO, true );
        Log::writeMessage( this->getLogId(), this->child_sa_collection->toStringTab( 1 ), Log::LOG_INFO, false );
        Log::release();

        // Inherits the attributeMap //
        this->attributemap->inherit( *other.attributemap );

        // Inherits the CHILD_SA related commands (NEW, REKEY or DELETE) //
        // From the Command Queue
        deque<Command*>::iterator it_command_queue = other.command_queue.begin();
        while ( it_command_queue != other.command_queue.end() ) {
            Command * command = *it_command_queue;
            if ( command->isInheritable() ) {
                Log::writeMessage( this->getLogId(), "Inherit Command from SPI=" + Printable::toHexString( &other.my_spi, 8 ) + " Command=[" + command->getCommandName() + "]", Log::LOG_INFO, true );
                it_command_queue = other.command_queue.erase( it_command_queue );
                this->command_queue.push_back( command );
            }
            else
                it_command_queue++;
        }

        // From the Deferred Command Queue
        deque<Command*>::iterator it_deferred_command_queue = other.deferred_queue.begin();
        while ( it_deferred_command_queue != other.deferred_queue.end() ) {
            Command * command = *it_deferred_command_queue;
            if ( command->isInheritable() ) {
                Log::writeMessage( this->getLogId(), "Inherit Deferred Command from SPI=" + Printable::toHexString( &other.my_spi, 8 ) + " Command=[" + command->getCommandName() + "]", Log::LOG_INFO, true );
                it_deferred_command_queue = other.deferred_queue.erase( it_deferred_command_queue );
                this->deferred_queue.push_back( command );
            }
            else
                it_deferred_command_queue++;
        }
    }

    void IkeSa::notifyAlarm( Alarm& alarm ) {
        auto_ptr<Command> command ( new AlarmCommand( alarm ) );

        // This is the only one method (apart pushCommand) that can be executed from another thread.
        // So it needs to use the IkeSaController method to add the command
        IkeSaController::pushCommandByIkeSaSpi( this->my_spi, command, false );
    }

    IkeSa::IKE_SA_ACTION IkeSa::processAlarm( Alarm& alarm ) {
        Log::writeLockedMessage( this->getLogId(), "AlarmController notifies IKE_SA: Alarm id=" + alarm.getLogId(), Log::LOG_ALRM, true );

        // If notification is from halfopen alarm, then if IKE_SA is not created close IKE_SA
        if ( &alarm == this->halfopen_alarm.get() ) {
            if ( this->state < STATE_IKE_SA_ESTABLISHED ) {
                Log::writeLockedMessage( this->getLogId(), "IKE SA creation timeout", Log::LOG_INFO, true );
                EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_FAILED, *this ) ) );
                return IKE_SA_ACTION_DELETE_IKE_SA;
            }
            return IKE_SA_ACTION_CONTINUE;
        }

        // If notification is from retransmition alarm, then retransmit last request
        else if ( &alarm == this->retransmition_alarm.get() ) {
            return this->retransmitLastRequest();
        }

        // If notification is from IDLE alarm, then start Dead Peer Detection
        else if ( &alarm == this->idle_ike_sa_alarm.get() ) {
            if ( this->state >= STATE_IKE_SA_ESTABLISHED ) {
                Log::writeLockedMessage( this->getLogId(), "Starting Dead Peer Detection", Log::LOG_INFO, true );
                AutoVector<Payload> payloads;
                return this->createGenericInformationalRequest( payloads );
            }
            return IKE_SA_ACTION_CONTINUE;
        }

        // If notification is from REKEY alarm, then
        else if ( &alarm == this->rekey_ike_sa_alarm.get() ) {
            auto_ptr<Command> command ( new SendRekeyIkeSaReqCommand() );
            this->pushCommand( command, false );
            return IKE_SA_ACTION_CONTINUE;
        }

        return IKE_SA_ACTION_CONTINUE;
    }

    bool IkeSa::hasMoreCommands() {
        AutoLock auto_lock( *this->mutex );

        if ( this->command_queue.size() > 0 || ( this->state == STATE_IKE_SA_ESTABLISHED && !this->deferred_queue.empty() ) )
            return true;
        else
            return false;
    }

    void IkeSa::createChildSaNegotiationRequest( Message & message ) {
        // updates the CHILD_SA configuration
        this->my_creating_child_sa->setChildSaConfiguration( this->getChildSaConfiguration().clone() );
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_CREATED, *this, *this->my_creating_child_sa ) ) );

        // includes the configuration payload (CP) (if needed)
        NetworkController::createConfigurationRequest( message, *this );

        // includes the security association payload (SA)
        // set spi value into proposal

	//uint32_t spi = IpsecController::getSpi(this->my_addr->getIpAddress(),this->peer_addr->getIpAddress(),this->my_creating_child_sa->ipsec_protocol);
        //this->my_creating_child_sa->inbound_spi = spi;
        //this->my_creating_child_sa->getProposal().setIpsecSpi( spi );
        this->my_creating_child_sa->getProposal().setIpsecSpi( this->my_creating_child_sa->inbound_spi );

        // deletes the DH transforms from the CHILD_SA proposal when IKE_AUTH, because they are not needed)
        if ( message.exchange_type == Message::IKE_AUTH )
            this->my_creating_child_sa->getProposal().deleteTransformsByType( Enums::D_H );
        message.addPayload( auto_ptr<Payload> ( new Payload_SA( this->my_creating_child_sa->getProposal().clone() ) ), true );

        // If this is a CREATE_CHILD_SA message
        if ( message.exchange_type == Message::CREATE_CHILD_SA ) {
            // includes the nonce payload (NONCE)
            auto_ptr<Payload_NONCE> payload_nonce ( new Payload_NONCE() );
            this->my_creating_child_sa->my_nonce = payload_nonce->getNonceValue().clone();
            message.addPayload( auto_ptr<Payload> ( payload_nonce ), true );

            // includes the key exchange payload (KE)
            Transform* dh_transform = this->my_creating_child_sa->getProposal().getFirstTransformByType( Enums::D_H );
            if ( dh_transform != NULL ) {
                this->my_creating_child_sa->pfs_dh = CryptoController::getDiffieHellman( ( Enums::DH_ID ) dh_transform->id );
                message.addPayload( auto_ptr<Payload> ( new Payload_KE( *this->my_creating_child_sa->pfs_dh ) ), true );
            }
        }

        // if this is a IKE_AUTH message
        else if ( message.exchange_type == Message::IKE_AUTH ) {
            this->my_creating_child_sa->my_nonce = this->my_nonce->clone();
        }

        // includes the initiator traffic selector payload (TSi)
        message.addPayload( auto_ptr<Payload> ( new Payload_TSi ( *this->my_creating_child_sa->my_traffic_selector ) ), true );

        // includes the responder traffic selector payload (TSr)
        message.addPayload( auto_ptr<Payload> ( new Payload_TSr ( *this->my_creating_child_sa->peer_traffic_selector ) ), true );
    }

    IkeSa::NEGOTIATION_ACTION IkeSa::processChildSaNegotiationResponse( Message& message ) {
        // process configuration payload (CP)
        NEGOTIATION_ACTION negotiation_action = NetworkController::processConfigurationResponse( message, *this );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR )
            return negotiation_action;

        // process security association payload (SA)
        // check if responder choose only one of the proposals
        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );
        if ( payload_sa.proposals->size() != 1 ) {
            Log::writeLockedMessage( this->my_creating_child_sa->getLogId(), "IPSEC Proposal error: RESPONDER CHOOSE INVALID NUMBER OF IPSEC PROPOSALS:" + intToString( payload_sa.proposals->size() ), Log::LOG_ERRO, true );
            return NEGOTIATION_ACTION_ERROR;
        }

        // checks if response proposal is valid
        auto_ptr<Proposal> best_proposal = CryptoController::chooseProposal( payload_sa, this->my_creating_child_sa->getProposal() );
        if ( best_proposal.get() == NULL ) {
            Log::writeLockedMessage( this->my_creating_child_sa->getLogId(), "IPSEC Proposal error: RESPONDER CHOOSE INVALID IPSEC PROPOSAL", Log::LOG_ERRO, true );
            return NEGOTIATION_ACTION_ERROR;
        }

        // updates the CHILD_SA proposal
        this->my_creating_child_sa->setProposal( payload_sa.proposals[ 0 ] ->clone() );

        // updates the peer spi
        this->my_creating_child_sa->outbound_spi = this->my_creating_child_sa->getProposal().getIpsecSpi();

        // if message is CREATE_CHILD_SA
        if ( message.exchange_type == Message::CREATE_CHILD_SA ) {
            // process the nonce payload (Nr)
            Payload_NONCE& payload_nonce = ( Payload_NONCE& ) message.getUniquePayloadByType( Payload::PAYLOAD_NONCE );
            this->my_creating_child_sa->peer_nonce = payload_nonce.getNonceValue().clone();

            // process key exchange payload (KE)
            Payload_KE * payload_ke = ( Payload_KE* ) message.getFirstPayloadByType( Payload::PAYLOAD_KE );
            if ( this->my_creating_child_sa->pfs_dh.get() ) {
                // If Payload KE doesn't exists
                if ( payload_ke == NULL ) {
                    Log::writeLockedMessage( this->my_creating_child_sa->getLogId(), "PFS PAYLOAD_KE required", Log::LOG_ERRO, true );
                    return NEGOTIATION_ACTION_ERROR;
                }

                // checks the Payload_KE groups
                if ( payload_ke->group != this->my_creating_child_sa->pfs_dh->group_id ) {
                    Log::writeLockedMessage( this->my_creating_child_sa->getLogId(), "PFS DH groups differs", Log::LOG_ERRO, true );
                    return NEGOTIATION_ACTION_ERROR;
                }

                // Generate shared secret (DH)
                this->my_creating_child_sa->pfs_dh->generateSharedSecret( payload_ke->getPublicKey() );

                // Prints in log the DH shared secret
                Log::acquire();
                Log::writeMessage( this->getLogId(), "New shared secret (PFS)", Log::LOG_CRYP, true );
                Log::writeMessage( this->getLogId(), this->my_creating_child_sa->pfs_dh->getSharedSecret().toStringTab( 1 ), Log::LOG_CRYP, false );
                Log::release();
            }
        }

        // if message is IKE_AUTH
        else if ( message.exchange_type == Message::IKE_AUTH ) {
            this->my_creating_child_sa->peer_nonce = this->peer_nonce->clone();
        }

        // process traffic selector payloads (TSi and TSr)
        Payload_TSi& payload_ts_i = ( Payload_TSi& ) message.getUniquePayloadByType( Payload::PAYLOAD_TSi );
        Payload_TSr& payload_ts_r = ( Payload_TSr& ) message.getUniquePayloadByType( Payload::PAYLOAD_TSr );

        // Checks if peer did a valid narrowing
        if ( !IpsecController::checkNarrowPayloadTS( payload_ts_i, payload_ts_r, *this->my_creating_child_sa ) ) {
            Log::writeLockedMessage( this->my_creating_child_sa->getLogId(), "INVALID TRAFFIC SELECTOR NARROWING", Log::LOG_ERRO, true );
            return NEGOTIATION_ACTION_ERROR;
        }

        // Generate CHILD_SA keying material
        ByteArray * shared_secret = ( this->my_creating_child_sa->pfs_dh.get() ) ? &this->my_creating_child_sa->pfs_dh->getSharedSecret() : NULL;
        this->my_creating_child_sa->keyring = CryptoController::getKeyRing( this->my_creating_child_sa->getProposal(), *this->prf );

        this->my_creating_child_sa->keyring->generateChildSaKeys( *this->my_creating_child_sa->my_nonce, *this->my_creating_child_sa->peer_nonce, *this->key_ring->sk_d, shared_secret );

        Log::acquire();
        Log::writeMessage( this->getLogId(), "New IPSEC keying material", Log::LOG_CRYP, true );
        Log::writeMessage( this->getLogId(), this->my_creating_child_sa->keyring->toStringTab( 1 ), Log::LOG_CRYP, false );
        Log::release();

        return NEGOTIATION_ACTION_CONTINUE;
    }

    IkeSa::NEGOTIATION_ACTION IkeSa::processChildSaNegotiationRequest( Message & message ) {
	// update policies, in case they have changed

	IpsecController::updatePolicies(true);

	// updates the CHILD_SA configuration
        this->peer_creating_child_sa->setChildSaConfiguration( this->getChildSaConfiguration().clone() );
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventChildSa( BusEventChildSa::CHILD_SA_CREATED, *this, *this->peer_creating_child_sa ) ) );


	// process Notify Payloads to see if transport mode is used


	auto_ptr<GeneralConfiguration> general_conf = Configuration::getInstance().getGeneralConfiguration();

    	BoolAttribute* mobility_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "mobility" );
    	bool mobility = false;

        BoolAttribute* is_ha_attr = general_conf->attributemap->getAttribute<BoolAttribute>( "is_ha" );
        bool is_ha = false;
        if (is_ha_attr  != NULL )
		is_ha = is_ha_attr->value;


	auto_ptr<SocketAddress>	coa (NULL);
        if (mobility_attr  != NULL )
		mobility = mobility_attr->value;

    	if ( mobility ) {

       		/*StringAttribute* string_attr = general_conf->attributemap->getAttribute<StringAttribute>( "home_address" );
		if (string_attr!=NULL ){
	    		auto_ptr<IpAddress> addr (NetworkController::getIpAddress(string_attr->value ));
			hoa = NetworkController::getSocketAddress (addr,500);
		*/
	                Payload_NOTIFY* payload_notify = ( Payload_NOTIFY* ) message.getFirstPayloadByType( Payload::PAYLOAD_NOTIFY );


			if (!((message.exchange_type == Message::CREATE_CHILD_SA)&&(payload_notify==NULL))){
				Log::writeLockedMessage( "NetworkController", "Mobility: Changing CoA to HoA in received message.", Log::LOG_WARN, true );
				Log::writeLockedMessage( "NetworkController", "Mobility: TRANSPORT MODE.", Log::LOG_WARN, true );
				if ( is_ha ){
				       coa = message.dst_addr->clone();
				       message.dst_addr = this->home_address->clone();			                        }
				else {
				       coa = message.src_addr->clone();
				       message.src_addr = this->home_address->clone();
	                        }
			}
		/*}*/
	}








        // process configuration payload (CP)
        IkeSa::NEGOTIATION_ACTION negotiation_action = NetworkController::processConfigurationRequest( message, *this );
        if ( negotiation_action == NEGOTIATION_ACTION_ERROR ) {
            Log::writeLockedMessage( this->peer_creating_child_sa->getLogId(), "INTERNAL ADDRESS FAILURE", Log::LOG_ERRO, true  );
            this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INTERNAL_ADDRESS_FAILURE );
            return negotiation_action;
        }

        // process security association payload (SA)
        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );

        if ( message.exchange_type == Message::IKE_AUTH )
	            this->peer_creating_child_sa->getProposal().deleteTransformsByType( Enums::D_H );

        // Generate the intersection between my ipsec proposal and the initiator ipsec proposal
        auto_ptr<Proposal> best_proposal = CryptoController::chooseProposal( payload_sa, this->peer_creating_child_sa->getProposal() );

        // If no proposal chosen, then send notify payloads
        if ( best_proposal.get() == NULL ) {
            Log::writeLockedMessage( this->peer_creating_child_sa->getLogId(), "NO PROPOSAL CHOSEN(IPSEC)", Log::LOG_ERRO, true  );
            this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::NO_PROPOSAL_CHOSEN );
            return NEGOTIATION_ACTION_ERROR;
        }

        // Sets the new proposal for this IPSEC SAS
        this->peer_creating_child_sa->setProposal( best_proposal );

        // sets the CHILD_SA ipsec protocol
        this->peer_creating_child_sa->ipsec_protocol = this->peer_creating_child_sa->getProposal().protocol_id;

        // updates peer spi
        this->peer_creating_child_sa->outbound_spi = this->peer_creating_child_sa->getProposal().getIpsecSpi();

        // if CREATE_CHILD_SA message
        if ( message.exchange_type == Message::CREATE_CHILD_SA ) {
            // process nonce payload (Ni)
            Payload_NONCE& payload_nonce = ( Payload_NONCE& ) message.getUniquePayloadByType( Payload::PAYLOAD_NONCE );
            this->peer_creating_child_sa->peer_nonce = payload_nonce.getNonceValue().clone();

            // process key exchange payload (KE)
            Payload_KE * payload_ke = ( Payload_KE* ) message.getFirstPayloadByType( Payload::PAYLOAD_KE );
            Transform *dh_transform = this->peer_creating_child_sa->getProposal().getFirstTransformByType( Enums::D_H );
            if ( dh_transform != NULL ) {
                if ( payload_ke == NULL ) {
                    Log::writeLockedMessage( this->peer_creating_child_sa->getLogId(), "Proposal error: DH specified but no PAYLOAD_KE present", Log::LOG_ERRO, true );
                    this->sendNotifyResponse( Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_PROPOSAL_CHOSEN );
                    return NEGOTIATION_ACTION_ERROR;
                }

                // If responder selected DH group and initiator proposed DH group aren't the same, then send a notification payload
                if ( payload_ke->group != dh_transform->id ) {
                    auto_ptr<ByteBuffer> temp ( new ByteBuffer( 2 ) );
                    temp->writeInt16( dh_transform->id );
                    auto_ptr<Payload_NOTIFY> notify ( new Payload_NOTIFY( Payload_NOTIFY::INVALID_KE_PAYLOAD, Enums::PROTO_NONE, auto_ptr<ByteArray> ( NULL ), auto_ptr<ByteArray> ( temp ) ) );
                    Log::writeLockedMessage( this->peer_creating_child_sa->getLogId(), "INVALID KE PAYLOAD(PFS)", Log::LOG_ERRO, true );
                    this->sendNotifyResponse( Message::CREATE_CHILD_SA, notify );
                    return NEGOTIATION_ACTION_ERROR;
                }

                // creates a new DH object
                this->peer_creating_child_sa->pfs_dh = CryptoController::getDiffieHellman( ( Enums::DH_ID ) dh_transform->id );

                // Generate shared secret (DH)
                this->peer_creating_child_sa->pfs_dh->generateSharedSecret( payload_ke->getPublicKey() );

                // Print in log the shared secret
                Log::acquire();
                Log::writeMessage( this->getLogId(), "New shared secret(PFS)", Log::LOG_CRYP, true );
                Log::writeMessage( this->getLogId(), this->peer_creating_child_sa->pfs_dh->getSharedSecret().toStringTab( 1 ), Log::LOG_CRYP, false );
                Log::release();
            }
        }

        // if IKE_AUTH message
        else {
            this->peer_creating_child_sa->peer_nonce = this->peer_nonce->clone();
        }

        // process traffic selector payloads (TSi and TSr)
        Payload_TSi& payload_ts_i = ( Payload_TSi& ) message.getUniquePayloadByType( Payload::PAYLOAD_TSi );
        Payload_TSr& payload_ts_r = ( Payload_TSr& ) message.getUniquePayloadByType( Payload::PAYLOAD_TSr );

        // Check if suggested traffic selectors are acceptables and select the narrowest
        bool result = IpsecController::narrowPayloadTS( payload_ts_i, payload_ts_r, *this, *this->peer_creating_child_sa );
        if ( !result ) {
            Log::writeLockedMessage( this->peer_creating_child_sa->getLogId(), "TS_UNACCEPTABLE", Log::LOG_ERRO, true );
            this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::TS_UNACCEPTABLE );
            return NEGOTIATION_ACTION_ERROR;
        }

        return NEGOTIATION_ACTION_CONTINUE;
    }

    void IkeSa::createChildSaNegotiationResponse( Message & message ) {
        assert ( message.exchange_type == Message::IKE_AUTH || message.exchange_type == Message::CREATE_CHILD_SA );

        // include the configuration payload (CP)
        NetworkController::createConfigurationResponse( message, *this );

        // include the security association payload (SA)
        //uint32_t spi = IpsecController::getSpi(this->ike_sa_src_addr, this->ike_sa_dst_addr,this->peer_creating_child_sa->ipsec_protocol );
        //this->peer_creating_child_sa->inbound_spi = spi;
        //this->peer_creating_child_sa->getProposal().setIpsecSpi( spi );
        this->peer_creating_child_sa->getProposal().setIpsecSpi( this->peer_creating_child_sa->inbound_spi );
        message.addPayload( auto_ptr<Payload> ( new Payload_SA( this->peer_creating_child_sa->getProposal().clone() ) ), true );

        // if CREATE_CHILD_SA message
        if ( message.exchange_type == Message::CREATE_CHILD_SA ) {
            // includes the nonce payload (Nr)
            auto_ptr<Payload_NONCE> payload_nonce ( new Payload_NONCE() );
            this->peer_creating_child_sa->my_nonce = payload_nonce->getNonceValue().clone();
            message.addPayload( auto_ptr<Payload> ( payload_nonce ), true );

            // creates the payload KE (if needed)
            if ( this->peer_creating_child_sa->pfs_dh.get() ) {
                message.addPayload( auto_ptr<Payload> ( new Payload_KE( *this->peer_creating_child_sa->pfs_dh ) ), true );
            }
        }

        // if IKE_AUTH message
        else {
            this->peer_creating_child_sa->my_nonce = this->my_nonce->clone();
        }

        // includes the traffic selector payloads (TSi and TSr)
        message.addPayload( auto_ptr<Payload> ( new Payload_TSi( *this->peer_creating_child_sa->peer_traffic_selector ) ), true ) ;
        message.addPayload( auto_ptr<Payload> ( new Payload_TSr( *this->peer_creating_child_sa->my_traffic_selector ) ), true ) ;

        // Generate CHILD_SA keying material
        ByteArray * shared_secret = ( this->peer_creating_child_sa->pfs_dh.get() ) ? &this->peer_creating_child_sa->pfs_dh->getSharedSecret() : NULL;
        this->peer_creating_child_sa->keyring = CryptoController::getKeyRing( this->peer_creating_child_sa->getProposal(), *this->prf );

        this->peer_creating_child_sa->keyring->generateChildSaKeys( *this->peer_creating_child_sa->peer_nonce, *this->peer_creating_child_sa->my_nonce, *this->key_ring->sk_d, shared_secret );

        Log::acquire();
        Log::writeMessage( this->getLogId(), "New IPSEC keying material", Log::LOG_CRYP, true );
        Log::writeMessage( this->getLogId(), this->peer_creating_child_sa->keyring->toStringTab( 1 ), Log::LOG_CRYP, false );
        Log::release();
    }

    void IkeSa::createIkeSaNegotiationRequest( Message & message, IkeSa & ike_sa ) {
        bool crypted = ( message.exchange_type == Message::CREATE_CHILD_SA );

        // includes security association payload (SA)
        if ( message.exchange_type == Message::CREATE_CHILD_SA )
            ike_sa.getProposal().setIkeSpi( ike_sa.my_spi );
        message.addPayload( auto_ptr<Payload> ( new Payload_SA( ike_sa.getProposal().clone() ) ), crypted );

        // includes key exchange payload (KE)
        Transform *transform = ike_sa.getProposal().getFirstTransformByType( Enums::D_H );
        assert ( transform != NULL );
        ike_sa.dh = CryptoController::getDiffieHellman( ( Enums::DH_ID ) transform->id );
        message.addPayload( auto_ptr<Payload> ( new Payload_KE( *ike_sa.dh ) ), crypted );

        // includes the nonce payload (Ni)
        auto_ptr<Payload_NONCE> payload_nonce ( new Payload_NONCE() );
        ike_sa.my_nonce = payload_nonce->getNonceValue().clone();
        message.addPayload( auto_ptr<Payload> ( payload_nonce ), crypted );
    }

    IkeSa::NEGOTIATION_ACTION IkeSa::processIkeSaNegotiationResponse( Message & message, IkeSa & ike_sa ) {
        // process security association payload (SA)
        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );
        if ( payload_sa.proposals->size() != 1 ) {
            Log::writeLockedMessage( this->getLogId(), "Proposal error: RESPONDER CHOOSE INVALID NUMBER OF IKE PROPOSALS", Log::LOG_ERRO, true );
            return NEGOTIATION_ACTION_ERROR;
        }

        // Checks if response proposal is valid (subset of initiator proposals)
        auto_ptr<Proposal> best_proposal = CryptoController::chooseProposal( payload_sa, ike_sa.getProposal() );
        if ( best_proposal.get() == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "Proposal error: RESPONDER CHOOSE INVALID IKE PROPOSAL", Log::LOG_ERRO, true );
            return NEGOTIATION_ACTION_ERROR;
        }

        // save the best proposal as proposal
        ike_sa.setProposal( payload_sa.proposals[ 0 ] ->clone() );

        // Assigns peer spi
        // if message is IKE_SA_INIT
        if ( message.exchange_type == Message::IKE_SA_INIT )
            ike_sa.peer_spi = message.spi_r;

        // if message is REKEY IKE_SA
        else if ( message.exchange_type == Message::CREATE_CHILD_SA )
            ike_sa.peer_spi = ike_sa.getProposal().getIkeSpi();

        // process key exchange payload (KE)
        Payload_KE& payload_ke = ( Payload_KE& ) message.getUniquePayloadByType( Payload::PAYLOAD_KE );
        // Check if DH groups match
        if ( payload_ke.group != ike_sa.dh->group_id ) {
            Log::writeLockedMessage( this->getLogId(), "Invalid DH group received: Received a not proposed DH group", Log::LOG_ERRO, true );
            return NEGOTIATION_ACTION_ERROR;
        }

        // Generate shared secret (DH)
        ike_sa.dh->generateSharedSecret( payload_ke.getPublicKey() );

        // Prints in log the DH shared secret
        Log::acquire();
        Log::writeMessage( ike_sa.getLogId(), "New shared secret", Log::LOG_CRYP, true );
        Log::writeMessage( ike_sa.getLogId(), ike_sa.dh->getSharedSecret().toStringTab( 1 ), Log::LOG_CRYP, false );
        Log::release();

        // process nonce payload (Nr)
        Payload_NONCE& payload_nonce = ( Payload_NONCE& ) message.getUniquePayloadByType( Payload::PAYLOAD_NONCE );
        // Copies Peer Payload_NONCE for future keying material generation
        ike_sa.peer_nonce = payload_nonce.getNonceValue().clone();

        // Creates KeyRing
        ike_sa.prf = CryptoController::getPseudoRandomFunction( *ike_sa.getProposal().getFirstTransformByType( Enums::PRF ) );
        ike_sa.key_ring = CryptoController::getKeyRing( ike_sa.getProposal(), *ike_sa.prf );

        // Generates keying material
        // If IKE_SA_INIT exchange
        if ( message.exchange_type == Message::IKE_SA_INIT )
            ike_sa.key_ring->generateIkeSaKeys( *ike_sa.my_nonce, *ike_sa.peer_nonce, ike_sa.my_spi, ike_sa.peer_spi, ike_sa.dh->getSharedSecret(), NULL );

        // if REKEY IKE_SA exchange
        else if ( message.exchange_type == Message::CREATE_CHILD_SA )
            ike_sa.key_ring->generateIkeSaKeys( *ike_sa.my_nonce, *ike_sa.peer_nonce, ike_sa.my_spi, ike_sa.peer_spi, ike_sa.dh->getSharedSecret(), this->key_ring->sk_d.get() );

        Log::acquire();
        Log::writeMessage( ike_sa.getLogId(), "New IKE keying material", Log::LOG_CRYP , true );
        Log::writeMessage( ike_sa.getLogId(), ike_sa.key_ring->toStringTab( 1 ), Log::LOG_CRYP, false );
        Log::release();

        // Creates ciphers & PRF
        ike_sa.send_cipher = CryptoController::getCipher( ike_sa.getProposal(), ike_sa.key_ring->sk_ei->clone(), ike_sa.key_ring->sk_ai->clone() );
        ike_sa.receive_cipher = CryptoController::getCipher( ike_sa.getProposal(), ike_sa.key_ring->sk_er->clone(), ike_sa.key_ring->sk_ar->clone() );

        return NEGOTIATION_ACTION_CONTINUE;
    }

    IkeSa::NEGOTIATION_ACTION IkeSa::processIkeSaNegotiationRequest( Message & message, IkeSa & ike_sa ) {
        // process security association payload (SA)
        Payload_SA& payload_sa = ( Payload_SA& ) message.getUniquePayloadByType( Payload::PAYLOAD_SA );

        // Choose proposal
        auto_ptr<Proposal> best_proposal = CryptoController::chooseProposal( payload_sa, ike_sa.getProposal() );

        // If no proposal chosen, then send notification
        if ( best_proposal.get() == NULL ) {
            Log::writeLockedMessage( this->getLogId(), "NO PROPOSAL CHOSEN(IKE)", Log::LOG_ERRO, true );
            this->sendNotifyResponse( message.exchange_type, Payload_NOTIFY::NO_PROPOSAL_CHOSEN );
            return NEGOTIATION_ACTION_ERROR;
        }

        // stores the best proposal
        ike_sa.setProposal( best_proposal );

        // if message is REKEY IKE_SA, assigns peer spi
        if ( message.exchange_type == Message::CREATE_CHILD_SA )
            ike_sa.peer_spi = ike_sa.getProposal().getIkeSpi();

        // process key exchange payload (KE)
        Payload_KE& payload_ke = ( Payload_KE& ) message.getUniquePayloadByType( Payload::PAYLOAD_KE );

        // Get desired DH group
        Transform* transform = ike_sa.getProposal().getFirstTransformByType( Enums::D_H );
        assert ( transform != NULL );

        // If responder selected DH group and initiator proposed DH group aren't the same, then send a notification payload
        if ( payload_ke.group != transform->id ) {
            auto_ptr<ByteBuffer> temp ( new ByteBuffer( 2 ) );
            temp->writeInt16( transform->id );
            auto_ptr<Payload_NOTIFY> notify ( new Payload_NOTIFY( Payload_NOTIFY::INVALID_KE_PAYLOAD, Enums::PROTO_NONE, auto_ptr<ByteArray> ( NULL ), auto_ptr<ByteArray> ( temp ) ) );
            Log::writeLockedMessage( ike_sa.getLogId(), "Received wrong KE payload. Sending INVALID KE PAYLOAD notification", Log::LOG_ERRO, true );
            this->sendNotifyResponse( Message::IKE_SA_INIT, notify );
            return NEGOTIATION_ACTION_ERROR;
        }

        // Creates the DH object
        ike_sa.dh = CryptoController::getDiffieHellman( ( Enums::DH_ID ) transform->id );

        // Generate shared secret
        ike_sa.dh->generateSharedSecret( payload_ke.getPublicKey() );

        // Prints in log the DH shared secret
        Log::acquire();
        Log::writeMessage( ike_sa.getLogId(), "New shared secret", Log::LOG_CRYP, true );
        Log::writeMessage( ike_sa.getLogId(), ike_sa.dh->getSharedSecret().toStringTab( 1 ), Log::LOG_CRYP, false );
        Log::release();

        // process nonce payload (Ni)
        Payload_NONCE& payload_nonce = ( Payload_NONCE& ) message.getUniquePayloadByType( Payload::PAYLOAD_NONCE );
        ike_sa.peer_nonce = payload_nonce.getNonceValue().clone();

        return NEGOTIATION_ACTION_CONTINUE;
    }

    void IkeSa::createIkeSaNegotiationResponse( Message & message, IkeSa & ike_sa ) {
        assert ( message.exchange_type == Message::IKE_SA_INIT || message.exchange_type == Message::CREATE_CHILD_SA );
        bool crypted = ( message.exchange_type == Message::CREATE_CHILD_SA );

        // includes the security association payload (SA)
        if ( message.exchange_type == Message::CREATE_CHILD_SA )
            ike_sa.getProposal().setIkeSpi( ike_sa.my_spi );
        message.addPayload( auto_ptr<Payload> ( new Payload_SA( ike_sa.getProposal().clone() ) ), crypted );

        // includes the key exchange payload (KE)
        message.addPayload( auto_ptr<Payload> ( new Payload_KE ( *ike_sa.dh ) ), crypted );

        // includes the nonce payload (Nr)
        auto_ptr<Payload_NONCE> payload_nonce ( new Payload_NONCE() );
        ike_sa.my_nonce = payload_nonce->getNonceValue().clone();
        message.addPayload( auto_ptr<Payload> ( payload_nonce ), crypted );

        // Creates the new KEYRING
        ike_sa.prf = CryptoController::getPseudoRandomFunction( *ike_sa.getProposal().getFirstTransformByType( Enums::PRF ) );
        ike_sa.key_ring = CryptoController::getKeyRing( ike_sa.getProposal(), *ike_sa.prf );

        // If IKE_SA_INIT exchange
        if ( message.exchange_type == Message::IKE_SA_INIT )
            ike_sa.key_ring->generateIkeSaKeys( *ike_sa.peer_nonce, *ike_sa.my_nonce, ike_sa.peer_spi, ike_sa.my_spi, ike_sa.dh->getSharedSecret(), NULL );

        // if REKEY IKE_SA exchange
        else if ( message.exchange_type == Message::CREATE_CHILD_SA )
            ike_sa.key_ring->generateIkeSaKeys( *ike_sa.peer_nonce, *ike_sa.my_nonce, ike_sa.peer_spi, ike_sa.my_spi, ike_sa.dh->getSharedSecret(), this->key_ring->sk_d.get() );

        Log::acquire();
        Log::writeMessage( ike_sa.getLogId(), "New IKE keying material", Log::LOG_CRYP, true );
        Log::writeMessage( ike_sa.getLogId(), ike_sa.key_ring->toStringTab( 1 ), Log::LOG_CRYP, false );
        Log::release();

        // Creates ciphers & PRF
        ike_sa.send_cipher = CryptoController::getCipher( ike_sa.getProposal(), ike_sa.key_ring->sk_er->clone(), ike_sa.key_ring->sk_ar->clone() );
        ike_sa.receive_cipher = CryptoController::getCipher( ike_sa.getProposal(), ike_sa.key_ring->sk_ei->clone(), ike_sa.key_ring->sk_ai->clone() );
    }

    void IkeSa::createRekeyIkeSa( auto_ptr<IkeSa>  new_ike_sa ) {
        new_ike_sa->inheritIkeSaStatus( *this );
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_ESTABLISHED, *new_ike_sa.get() ) ) );
        EventBus::getInstance().sendBusEvent( auto_ptr<BusEvent> ( new BusEventIkeSa( BusEventIkeSa::IKE_SA_REKEYED, *this, new_ike_sa.get() ) ) );
        new_ike_sa->rekey_ike_sa_alarm->reset();
        new_ike_sa->idle_ike_sa_alarm->reset();
        IkeSaController::addIkeSa( new_ike_sa );
    }

    void IkeSa::sendMessage( auto_ptr< Message > message, string text ) {
        // write a log message
        Log::acquire();
        Log::writeMessage( this->getLogId(), text, Log::LOG_MESG, true );
        Log::writeMessage( this->getLogId(), message->toStringTab( 1 ), Log::LOG_MESG, false );
        Log::release();

        // Sends message to the Peer
        NetworkController::sendMessage( *message, this->send_cipher.get() );

        // Activates retransmition alarm (if request)
        if ( message->message_type == Message::REQUEST ) {
            this->retransmition_alarm->reset();

            // Sets this message as the last sent request
            this->last_sent_request = message;
        }

        else if ( message->message_type == Message::RESPONSE ) {
            // sets this message as the last sent reponse
            this->last_sent_response = message;
        }

        else
            assert( "Unknown message type" || 0 );
    }

    bool IkeSa::controlsChildSa( uint32_t spi ) {
        AutoLock auto_lock( *this->mutex );
        return this->child_sa_collection->hasChildSa( spi );
    }

    string IkeSa::getLogId() {
        return "IKE_SA=" + Printable::toHexString( &this->my_spi, 8 );
    }
}


