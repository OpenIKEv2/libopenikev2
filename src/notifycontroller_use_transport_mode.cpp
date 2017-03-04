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
#include "notifycontroller_use_transport_mode.h"
#include "log.h"

namespace openikev2 {

    NotifyController_USE_TRANSPORT_MODE::NotifyController_USE_TRANSPORT_MODE() {}

    NotifyController_USE_TRANSPORT_MODE::~NotifyController_USE_TRANSPORT_MODE() {}

    IkeSa::NOTIFY_ACTION NotifyController_USE_TRANSPORT_MODE::processNotify ( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert ( notify.notification_type == Payload_NOTIFY::USE_TRANSPORT_MODE );
        assert ( child_sa != NULL );

        // It can be in request and response messages

        // Only can appear in AUTH and CREATE_CHILD exchanges
        if ( message.exchange_type != Message::IKE_AUTH && message.exchange_type != Message::CREATE_CHILD_SA ) {
            Log::writeLockedMessage ( ike_sa.getLogId(), "USE_TRANSPORT_MODE notify in invalid exchage.", Log::LOG_ERRO, true );
            if ( message.message_type == Message::REQUEST )
                ike_sa.sendNotifyResponse ( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() || notify.notification_data.get() ) {
            Log::writeLockedMessage ( ike_sa.getLogId(), "INVALID SYNTAX in USE_TRANSPORT_MODE notify.", Log::LOG_ERRO, true );
            if ( message.message_type == Message::REQUEST )
                ike_sa.sendNotifyResponse ( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        child_sa->mode = Enums::TRANSPORT_MODE;

        Log::writeLockedMessage ( ike_sa.getLogId(), "Transport mode requested by the peer.", Log::LOG_INFO, true );
        return IkeSa::NOTIFY_ACTION_CONTINUE;
    }

    void NotifyController_USE_TRANSPORT_MODE::addNotify ( Message & message, IkeSa & ike_sa, ChildSa * child_sa ) {
        if ( message.exchange_type != Message::IKE_AUTH && message.exchange_type != Message::CREATE_CHILD_SA )
            return;

        // If not realated with any CHILD_SA
        if ( child_sa == NULL )
            return;

        if ( child_sa->mode == Enums::TRANSPORT_MODE )
            message.addPayloadNotify ( auto_ptr<Payload_NOTIFY> ( new Payload_NOTIFY ( Payload_NOTIFY::USE_TRANSPORT_MODE, Enums::PROTO_NONE ) ), true );
    }
}
