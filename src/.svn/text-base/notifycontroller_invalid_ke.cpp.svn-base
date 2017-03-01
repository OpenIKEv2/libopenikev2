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
#include "notifycontroller_invalid_ke.h"
#include "cryptocontroller.h"
#include "payload_ke.h"
#include "log.h"


namespace openikev2 {

    NotifyController_INVALID_KE::NotifyController_INVALID_KE() {}

    NotifyController_INVALID_KE::~NotifyController_INVALID_KE() {}

    IkeSa::NOTIFY_ACTION NotifyController_INVALID_KE::processNotify( Payload_NOTIFY & notify, Message & message, IkeSa & ike_sa, ChildSa * child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::INVALID_KE_PAYLOAD );

        // If is a request
        if ( message.message_type == Message::REQUEST ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID_KE_PAYLOAD notify in request.", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // If exchange is invalid
        if ( message.exchange_type != Message::IKE_SA_INIT && message.exchange_type != Message::CREATE_CHILD_SA ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID_KE_PAYLOAD notify in wrong exchange.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() != NULL || notify.notification_data.get() == NULL || notify.notification_data->size() != 2 ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in INVALID_KE_PAYLOAD notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // In the IKE_SA_INIT case
        if ( message.exchange_type == Message::IKE_SA_INIT ) {
            // Store into "group" responder desired group
            ByteBuffer temp( *notify.notification_data );
            uint16_t group = temp.readInt16();

            Log::writeLockedMessage( ike_sa.getLogId(), "Peer prefers a different DH group=[" + intToString( group ) + "]", Log::LOG_WARN, true );

            // Checks if selected groups is different to the current (to avoid retransmitions issues)
            if ( ike_sa.dh->group_id == group ) {
                Log::writeLockedMessage( ike_sa.getLogId(), "Received retransmition of an old response. Omitting", Log::LOG_ERRO, true );
                return IkeSa::NOTIFY_ACTION_OMIT;
            }

            // If desired group isn't in the original initiator proposal, then error
            Transform dh_transform( Enums::D_H, group );
            if ( !ike_sa.getIkeSaConfiguration().getProposal().hasTransform( dh_transform ) ) {
                Log::writeLockedMessage( ike_sa.getLogId(), "INVALID_KE_PAYLOAD contains a non proposed DH group=[" + intToString( group ) + "]", Log::LOG_ERRO, true );
                return IkeSa::NOTIFY_ACTION_ERROR;
            }

            // Creates the new DH object and the new KE payload
            ike_sa.dh = CryptoController::getDiffieHellman( ( Enums::DH_ID ) group );
            auto_ptr<Payload> new_payload_ke ( new Payload_KE( *ike_sa.dh ) );
            ike_sa.last_sent_request->replaceFirstPayloadByType( Payload::PAYLOAD_KE, new_payload_ke );

            // To avoid TIMEOUT RETRIES EXCEEDED when INVALID_KE_PAYLOAD is received
            ike_sa.remaining_timeout_retries++;
            ike_sa.retransmitLastRequest();

            // Update the IKE_SA_INIT request message
            ike_sa.ike_sa_init_req = ike_sa.last_sent_request->clone();

            return IkeSa::NOTIFY_ACTION_OMIT;
        }

        // In the CHILD SA Creation/Rekeying and in IKE_SA Rekeying
        else if ( message.exchange_type == Message::CREATE_CHILD_SA ) {
            DiffieHellman * old_dh = NULL;
            Proposal* proposal = NULL;

            // If CHILD_SA related
            if ( child_sa != NULL ) {
                old_dh = child_sa->pfs_dh.get();
                proposal = &child_sa->getProposal();
            }

            // else if IKE_SA related
            else {
                // Check if there is another redundant IKE SA. If so, return ERROR
                if ( ike_sa.getState() == IkeSa::STATE_REDUNDANT_IKE_SA ) {
                    return IkeSa::NOTIFY_ACTION_ERROR;
                }

                old_dh = ike_sa.my_creating_ike_sa->dh.get();
                proposal = &ike_sa.my_creating_ike_sa->getIkeSaConfiguration().getProposal();
            }

            assert( old_dh != NULL );
            assert( proposal != NULL );

            // Store into "group" responders desired group
            ByteBuffer temp( *notify.notification_data );
            uint16_t group = temp.readInt16();

            Log::writeLockedMessage( ike_sa.getLogId(), "Peer preffers different DH group=[" + intToString( group ) + "]", Log::LOG_WARN, true );

            // Checks if selected groups is different to the current (to avoid retransmitions issues)
            if ( old_dh->group_id == group ) {
                Log::writeLockedMessage( ike_sa.getLogId(), "Received retransmition of an old response.", Log::LOG_ERRO, true );
                return IkeSa::NOTIFY_ACTION_OMIT;
            }

            // If desired group isn't in the original initiator proposal, then error
            Transform dh_transform( Enums::D_H, group );
            if ( !proposal->hasTransform( dh_transform ) ) {
                Log::writeLockedMessage( ike_sa.getLogId(), "INVALID_KE_PAYLOAD contains a non proposed DH group=[" + intToString( group ) + "]", Log::LOG_ERRO, true );
                return IkeSa::NOTIFY_ACTION_ERROR;
            }

            auto_ptr<DiffieHellman> new_dh = CryptoController::getDiffieHellman( ( Enums::DH_ID ) group );

            auto_ptr<Payload> new_payload_ke ( new Payload_KE( *new_dh ) );
            ike_sa.last_sent_request->replaceFirstPayloadByType( Payload::PAYLOAD_KE, new_payload_ke );

            ike_sa.last_sent_request->message_id++;
            ike_sa.my_message_id++;

            ike_sa.remaining_timeout_retries++;
            ike_sa.retransmitLastRequest();

            // If is a child_sa creation
            if ( child_sa != NULL ) {
                child_sa->pfs_dh = new_dh;
            }

            // If is a ike_sa rekey
            else {
                ike_sa.my_creating_ike_sa->dh = new_dh;
            }
            
            return IkeSa::NOTIFY_ACTION_OMIT;
        }
        else
            assert( 0 );

        return IkeSa::NOTIFY_ACTION_ERROR;
    }

}
