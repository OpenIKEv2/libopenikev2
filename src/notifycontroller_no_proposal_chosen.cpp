/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_no_proposal_chosen.h"
#include "log.h"

namespace openikev2 {

    NotifyController_NO_PROPOSAL_CHOSEN::NotifyController_NO_PROPOSAL_CHOSEN() {}

    NotifyController_NO_PROPOSAL_CHOSEN::~NotifyController_NO_PROPOSAL_CHOSEN() {}

    IkeSa::NOTIFY_ACTION NotifyController_NO_PROPOSAL_CHOSEN::processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::NO_PROPOSAL_CHOSEN );

        // If is a request
        if ( message.message_type == Message::REQUEST ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "NO_PROPOSAL_CHOSEN notify in a request.", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // If exchange is invalid
        if ( message.exchange_type == Message::INFORMATIONAL ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "NO_PROPOSAL_CHOSEN notify in wrong exchange.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() != NULL || notify.notification_data.get() != NULL ){
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in NO_PROPOSAL_CHOSEN notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        if ( message.exchange_type == Message::IKE_SA_INIT || (message.exchange_type == Message::CREATE_CHILD_SA && child_sa == NULL) ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "Peer doesn't choose any IKE_SA proposal.", Log::LOG_ERRO, true );
        }
        else if ( message.exchange_type == Message::IKE_AUTH || (message.exchange_type == Message::CREATE_CHILD_SA && child_sa != NULL)  ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "Peer doesn't choose any CHILD_SA proposal.", Log::LOG_ERRO, true );
        }
        else{
            assert (0);
        }
        return IkeSa::NOTIFY_ACTION_ERROR;
    }
}
