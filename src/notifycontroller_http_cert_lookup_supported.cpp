/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_http_cert_lookup_supported.h"
#include "log.h"

namespace openikev2 {

    NotifyController_HTTP_CERT_LOOKUP_SUPPORTED::NotifyController_HTTP_CERT_LOOKUP_SUPPORTED() : NotifyController() {}

    NotifyController_HTTP_CERT_LOOKUP_SUPPORTED::~NotifyController_HTTP_CERT_LOOKUP_SUPPORTED() {}

    IkeSa::NOTIFY_ACTION NotifyController_HTTP_CERT_LOOKUP_SUPPORTED::processNotify( Payload_NOTIFY & notify, Message & message, IkeSa & ike_sa, ChildSa * child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::HTTP_CERT_LOOKUP_SUPPORTED );

        // If is not an IKE_SA_INIT response or an IKE_AUTH request
        if ( !( message.exchange_type == Message::IKE_SA_INIT && message.message_type == Message::RESPONSE ) && !( message.exchange_type == Message::IKE_AUTH && message.message_type == Message::REQUEST ) ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "Received a HTTP_CERT_LOOKUP_SUPPORTED notify in an invalid message type.", Log::LOG_ERRO, true );

            // If is a request
            if ( message.message_type == Message::REQUEST )
                ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() != NULL || notify.notification_data.get() != NULL ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in HTTP_CERT_LOOKUP_SUPPORTED notify.", Log::LOG_ERRO, true );
            if ( message.message_type == Message::REQUEST )
                ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        Log::writeLockedMessage( ike_sa.getLogId(), "Peer supports HTTP CERT LOOKUP.", Log::LOG_WARN, true );

        ike_sa.peer_supports_hash_url = true;

        return IkeSa::NOTIFY_ACTION_CONTINUE;
    }
}

