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
#include "networkcontrollerimpl.h"

#include "ikesacontroller.h"

#include "notifycontroller_cookie.h"
#include "notifycontroller_invalid_ke.h"
#include "notifycontroller_no_proposal_chosen.h"
#include "notifycontroller_invalid_syntax.h"
#include "notifycontroller_unsupported_critical_payload.h"
#include "notifycontroller_use_transport_mode.h"
#include "notifycontroller_authentication_failed.h"
#include "notifycontroller_internal_address_failure.h"
#include "notifycontroller_ts_unacceptable.h"
#include "notifycontroller_single_pair_required.h"
#include "notifycontroller_no_additional_sas.h"
#include "notifycontroller_rekey_sa.h"
#include "notifycontroller_http_cert_lookup_supported.h"
#include "exception.h"
#include "autolock.h"
#include "log.h"

namespace openikev2 {

    NetworkControllerImpl::NetworkControllerImpl() {
        // Register the standard notify controllers
        this->registerNotifyController( Payload_NOTIFY::COOKIE, auto_ptr<NotifyController> ( new NotifyController_COOKIE() ) );
        this->registerNotifyController( Payload_NOTIFY::INVALID_KE_PAYLOAD, auto_ptr<NotifyController> ( new NotifyController_INVALID_KE() ) );
        this->registerNotifyController( Payload_NOTIFY::NO_PROPOSAL_CHOSEN, auto_ptr<NotifyController> ( new NotifyController_NO_PROPOSAL_CHOSEN() ) );
        this->registerNotifyController( Payload_NOTIFY::UNSUPPORTED_CRITICAL_PAYLOAD, auto_ptr<NotifyController> ( new NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD() ) );
        this->registerNotifyController( Payload_NOTIFY::INVALID_SYNTAX, auto_ptr<NotifyController> ( new NotifyController_INVALID_SYNTAX() ) );
        this->registerNotifyController( Payload_NOTIFY::USE_TRANSPORT_MODE, auto_ptr<NotifyController> ( new NotifyController_USE_TRANSPORT_MODE() ) );
        this->registerNotifyController( Payload_NOTIFY::AUTHENTICATION_FAILED, auto_ptr<NotifyController> ( new NotifyController_AUTHENTICATION_FAILED() ) );
        this->registerNotifyController( Payload_NOTIFY::INTERNAL_ADDRESS_FAILURE, auto_ptr<NotifyController> ( new NotifyController_INTERNAL_ADDRESS_FAILURE() ) );
        this->registerNotifyController( Payload_NOTIFY::SINGLE_PAIR_REQUIRED, auto_ptr<NotifyController> ( new NotifyController_SINGLE_PAIR_REQUIRED() ) );
        this->registerNotifyController( Payload_NOTIFY::NO_ADDITIONAL_SAS, auto_ptr<NotifyController> ( new NotifyController_NO_ADDITIONAL_SAS() ) );
        this->registerNotifyController( Payload_NOTIFY::TS_UNACCEPTABLE, auto_ptr<NotifyController> ( new NotifyController_TS_UNACCEPTABLE() ) );
        this->registerNotifyController( Payload_NOTIFY::REKEY_SA, auto_ptr<NotifyController> ( new NotifyController_REKEY_SA() ) );
        this->registerNotifyController( Payload_NOTIFY::HTTP_CERT_LOOKUP_SUPPORTED, auto_ptr<NotifyController> ( new NotifyController_HTTP_CERT_LOOKUP_SUPPORTED() ) );
    }

    NetworkControllerImpl::~NetworkControllerImpl() {
        for ( map<uint16_t, NotifyController*>::iterator it = this->notify_controllers.begin(); it != this->notify_controllers.end(); it++ )
            delete it->second;
    }

    NotifyController * NetworkControllerImpl::getNotifyController( uint16_t type ) {
        map<uint16_t, NotifyController*>::iterator it = this->notify_controllers.find( type );
        if ( it == this->notify_controllers.end() )
            return NULL;

        return ( *it ).second;
    }

    void NetworkControllerImpl::registerNotifyController( uint16_t type, auto_ptr<NotifyController> notify_controller ) {
        map<uint16_t, NotifyController*>::iterator it = this->notify_controllers.find( type );
        if ( it != this->notify_controllers.end() )
            this->notify_controllers.erase( it );

        this->notify_controllers.insert( pair<uint16_t, NotifyController*>( type, notify_controller.release() ) );
    }

    void NetworkControllerImpl::addNotifies( Message& message, IkeSa& ike_sa, ChildSa* child_sa ) {
        // for all the notify controllers, adds the request to the message
        for ( map<uint16_t, NotifyController*>::iterator it = this->notify_controllers.begin(); it != this->notify_controllers.end(); it++ ){
            ( *it ).second->addNotify( message, ike_sa, child_sa );
        }
    }
}

