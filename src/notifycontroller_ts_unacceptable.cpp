/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_ts_unacceptable.h"
#include "log.h"

namespace openikev2 {

    NotifyController_TS_UNACCEPTABLE::NotifyController_TS_UNACCEPTABLE() {}

    NotifyController_TS_UNACCEPTABLE::~NotifyController_TS_UNACCEPTABLE() {}

    IkeSa::NOTIFY_ACTION NotifyController_TS_UNACCEPTABLE::processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::TS_UNACCEPTABLE );
        assert( child_sa != NULL );

        // If is a request
        if ( message.message_type == Message::REQUEST ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "TS_UNACCEPTABLE notify in a request.", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse( message.exchange_type, auto_ptr<Payload_NOTIFY> ( new Payload_NOTIFY( Payload_NOTIFY::INVALID_SYNTAX, Enums::PROTO_NONE ) ) );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // If exchange is invalid
        if ( message.exchange_type != Message::IKE_AUTH && message.exchange_type != Message::CREATE_CHILD_SA ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "TS_UNACCEPTABLE notify in a wrong exchange.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() || notify.notification_data.get() ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in TS_UNACCEPTABLE notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        Log::writeLockedMessage( ike_sa.getLogId(), "Peer doesn't accept traffic selector proposal.", Log::LOG_ERRO, true );
        return IkeSa::NOTIFY_ACTION_ERROR;
    }
}
