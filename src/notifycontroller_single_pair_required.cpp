/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_single_pair_required.h"
#include "log.h"

namespace openikev2 {

    NotifyController_SINGLE_PAIR_REQUIRED::NotifyController_SINGLE_PAIR_REQUIRED() {}

    NotifyController_SINGLE_PAIR_REQUIRED::~NotifyController_SINGLE_PAIR_REQUIRED() {}

    IkeSa::NOTIFY_ACTION NotifyController_SINGLE_PAIR_REQUIRED::processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::SINGLE_PAIR_REQUIRED );
        assert( child_sa != NULL );

        // If is a request
        if ( message.message_type == Message::REQUEST ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "SINGLE_PAIR_REQUIRED notify in a request.", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // If exchange is invalid
        if ( message.exchange_type != Message::IKE_AUTH && message.exchange_type != Message::CREATE_CHILD_SA ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "SINGLE_PAIR_REQUIRED notify in a wrong exchange.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() || notify.notification_data.get() ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in SINGLE_PAIR_REQUIRED notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        Log::writeLockedMessage( ike_sa.getLogId(), "Peer require single pair TS.", Log::LOG_ERRO, true );
        return IkeSa::NOTIFY_ACTION_ERROR;
    }

}
