/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "notifycontroller_no_additional_sas.h"
#include "log.h"

namespace openikev2 {

    NotifyController_NO_ADDITIONAL_SAS::NotifyController_NO_ADDITIONAL_SAS() {}

    NotifyController_NO_ADDITIONAL_SAS::~NotifyController_NO_ADDITIONAL_SAS() {}

    IkeSa::NOTIFY_ACTION NotifyController_NO_ADDITIONAL_SAS::processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::NO_ADDITIONAL_SAS );
        assert( child_sa != NULL );

        // If is a request
        if ( message.message_type == Message::REQUEST ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "NO_ADDITIONAL_SAS notify in a request.", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // If exchange is invalid
        if ( message.exchange_type != Message::CREATE_CHILD_SA ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "NO_ADDITIONAL_SAS notify in a wrong exchange.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() != NULL || notify.notification_data.get() != NULL ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in NO_ADDITIONAL_SAS notify.", Log::LOG_ERRO, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        Log::writeLockedMessage( ike_sa.getLogId(), "Peer does not accept additional SAs.", Log::LOG_ERRO, true );
        return IkeSa::NOTIFY_ACTION_ERROR;
    }

}
