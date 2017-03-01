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
