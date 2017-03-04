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
#include "notifycontroller_cookie.h"
#include "cryptocontroller.h"
#include "eventbus.h"
#include "log.h"

namespace openikev2 {

    NotifyController_COOKIE::NotifyController_COOKIE() {}

    NotifyController_COOKIE::~NotifyController_COOKIE() {}

    IkeSa::NOTIFY_ACTION NotifyController_COOKIE::processNotify( Payload_NOTIFY & notify, Message & message, IkeSa & ike_sa, ChildSa * child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::COOKIE );

        // Checks if exchange type is IKE_SA_INIT req, else ignore return error
        if ( message.exchange_type != Message::IKE_SA_INIT ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "COOKIE notify payload in wrong exchange", Log::LOG_ERRO, true );
            if ( message.message_type == Message::REQUEST )
                ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // Check notify field correction
        if ( notify.protocol_id > Enums::PROTO_IKE || notify.spi_value.get() != NULL || notify.notification_data.get() == NULL ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "INVALID SYNTAX in COOKIE notify.", Log::LOG_ERRO, true );
            if ( message.message_type == Message::REQUEST )
                ike_sa.sendNotifyResponse( message.exchange_type, Payload_NOTIFY::INVALID_SYNTAX );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        // If cookie is contained in a REQUEST, then check the cookie
        if ( message.message_type == Message::REQUEST ) {
            auto_ptr<Payload_NOTIFY> expected_cookie = CryptoController::generateCookie( message );

            // If the cookie value is valid, return CONTINUE
            if ( *notify.notification_data == *expected_cookie->notification_data ) {
                return IkeSa::NOTIFY_ACTION_CONTINUE;
            }

            // If the cookie value is invalid, send the correct cookie and return ERROR
            else {
                Log::writeLockedMessage( ike_sa.getLogId(), "Cookie received but not valid. Sending correct cookie", Log::LOG_WARN, true );
                ike_sa.sendNotifyResponse( Message::IKE_SA_INIT,  expected_cookie );
                return IkeSa::NOTIFY_ACTION_ERROR;
            }
        }

        // If cookie is contained in a RESPONSE, then includes the cookie in the next request and retransmit
        else {
            // Checks if cookie was already sent, replace old one with the new one
            auto_ptr<Payload_NOTIFY> new_notify ( new Payload_NOTIFY( notify ) );
            if ( ike_sa.last_sent_request->getFirstNotifyByType( Payload_NOTIFY::COOKIE ) != NULL ) {
                ike_sa.last_sent_request->replaceFirstNotifyByType( Payload_NOTIFY::COOKIE, new_notify );
            }
            else {
                ike_sa.last_sent_request->addPayloadNotify( new_notify, false );
            }

            // Updates the IKE_SA_INT message
            ike_sa.remaining_timeout_retries++;
            ike_sa.retransmitLastRequest();

            // Update the IKE_SA_INIT request message
            ike_sa.ike_sa_init_req = ike_sa.last_sent_request->clone();

            Log::writeLockedMessage( ike_sa.getLogId(), "Peer sends a cookie: Retransmiting last request with the cookie.", Log::LOG_WARN, true );
            return IkeSa::NOTIFY_ACTION_OMIT;
        }

        assert ("code not reachable" && 0);
        return IkeSa::NOTIFY_ACTION_CONTINUE;
    }
}


