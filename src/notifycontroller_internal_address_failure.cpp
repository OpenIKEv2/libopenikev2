/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_internal_address_failure.h"
#include "eventbus.h"
#include "buseventikesa.h"
#include "log.h"

namespace openikev2 {

    NotifyController_INTERNAL_ADDRESS_FAILURE::NotifyController_INTERNAL_ADDRESS_FAILURE() {}

    NotifyController_INTERNAL_ADDRESS_FAILURE::~NotifyController_INTERNAL_ADDRESS_FAILURE() {}

    IkeSa::NOTIFY_ACTION NotifyController_INTERNAL_ADDRESS_FAILURE::processNotify( Payload_NOTIFY & notify, Message & message, IkeSa & ike_sa, ChildSa * child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::INTERNAL_ADDRESS_FAILURE );

        // Check message type
        if ( ! (message.exchange_type != Message::IKE_AUTH && message.message_type == Message::RESPONSE) ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INTERNAL_ADDRESS_FAILURE notify in wrong message", Log::LOG_ERRO, true );
            if (message.message_type == Message::REQUEST)
                ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() != NULL || notify.notification_data.get() != NULL ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in INTERNAL_ADDRESS_FAILURE notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        Log::writeLockedMessage( ike_sa.getLogId(), "Peer failed assigning internal address.", Log::LOG_ERRO, true );
        return IkeSa::NOTIFY_ACTION_ERROR;
    }
}
