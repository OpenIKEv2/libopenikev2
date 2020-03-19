/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_unsupported_critical_payload.h"
#include "eventbus.h"
#include "buseventikesa.h"
#include "log.h"

namespace openikev2 {

    NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD::NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD() {}

    NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD::~NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD() {}

    IkeSa::NOTIFY_ACTION NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD::processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::UNSUPPORTED_CRITICAL_PAYLOAD );

        // If is a request
        if ( message.message_type == Message::REQUEST ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "UNSUPPORTED_CRITICAL_PAYLOAD notify in a request.", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse( message.exchange_type, auto_ptr<Payload_NOTIFY> ( new Payload_NOTIFY( Payload_NOTIFY::INVALID_SYNTAX, Enums::PROTO_NONE, auto_ptr<ByteArray>( NULL ), auto_ptr<ByteArray>( NULL ) ) ) );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() || notify.notification_data.get() ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in UNSUPPORTED_CRITICAL_PAYLOAD notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        Log::writeLockedMessage( ike_sa.getLogId(), "Unsupported critical payload notification received.", Log::LOG_ERRO, true );
        return IkeSa::NOTIFY_ACTION_ERROR;
    }
}

