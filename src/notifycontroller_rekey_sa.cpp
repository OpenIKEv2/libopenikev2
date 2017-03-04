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
#include "notifycontroller_rekey_sa.h"
#include "log.h"

namespace openikev2 {

    NotifyController_REKEY_SA::NotifyController_REKEY_SA() {}

    NotifyController_REKEY_SA::~NotifyController_REKEY_SA() {}

    IkeSa::NOTIFY_ACTION NotifyController_REKEY_SA::processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        assert( notify.notification_type == Payload_NOTIFY::REKEY_SA );

        if ( message.exchange_type != Message::CREATE_CHILD_SA || child_sa == NULL ) {
            Log::writeLockedMessage( ike_sa.getLogId(), "REKEY_SA notify in unspected message", Log::LOG_WARN, true );
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        assert ( child_sa != NULL );

        // Updates the CHILD_SA rekeyed SA
        ByteBuffer temp ( *notify.spi_value );
        uint32_t rekeyed_spi = temp.readInt32();
        child_sa->rekeyed_spi = rekeyed_spi;
        ChildSa* rekeyed_child_sa = ike_sa.child_sa_collection->getChildSa( rekeyed_spi );

        // If CHILD_SA doesn't exist or we are also deleting this CHILD_SA
        // Section 5.11.5 IKEv2 clarifications document */
        if ( rekeyed_child_sa == NULL || ( ike_sa.getState() == IkeSa::STATE_DELETE_CHILD_SA_REQ_SENT && rekeyed_child_sa->getState() == ChildSa::CHILD_SA_DELETING ) ) {
            Log::writeLockedMessage(ike_sa.getLogId(), "NO PROPOSAL CHOSEN: Rekeying an Unknown IPSEC SPI", Log::LOG_ERRO, true );
            ike_sa.sendNotifyResponse(Message::CREATE_CHILD_SA, Payload_NOTIFY::NO_PROPOSAL_CHOSEN);
            return IkeSa::NOTIFY_ACTION_ERROR;
        }

        return IkeSa::NOTIFY_ACTION_CONTINUE;
    }

    void NotifyController_REKEY_SA::addNotify( Message & message, IkeSa & ike_sa, ChildSa * child_sa ) {
        if ( message.exchange_type != Message::CREATE_CHILD_SA || child_sa == NULL )
            return;

        // If the CHILD_SA is rekeying another one,
        if ( child_sa->rekeyed_spi != 0 ) {
            ChildSa* rekeyed_child_sa = ike_sa.child_sa_collection->getChildSa( child_sa->rekeyed_spi );
            assert ( rekeyed_child_sa != NULL );

            auto_ptr<Payload_NOTIFY> notify ( new Payload_NOTIFY( Payload_NOTIFY::REKEY_SA, rekeyed_child_sa->ipsec_protocol, rekeyed_child_sa->getId(), auto_ptr<ByteArray>( NULL ) ) );
            message.addPayloadNotify( notify, true );
        }
    }
}





