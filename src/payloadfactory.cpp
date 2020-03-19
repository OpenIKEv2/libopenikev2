/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payloadfactory.h"

#include "payload_auth.h"
#include "payload_idi.h"
#include "payload_idr.h"
#include "payload_ke.h"
#include "payload_nonce.h"
#include "payload_sa.h"
#include "payload_notify.h"
#include "payload_tsi.h"
#include "payload_tsr.h"
#include "payload_del.h"
#include "payload_conf.h"
#include "payload_eap.h"
#include "payload_cert.h"
#include "payload_cert_req.h"
#include "payload_vendor.h"
#include "payload_sk.h"

namespace openikev2 {

    auto_ptr<Payload> PayloadFactory::createPayload( Payload::PAYLOAD_TYPE type, ByteBuffer& byte_buffer ) {
        switch ( type ) {
            case Payload::PAYLOAD_AUTH:
                return auto_ptr<Payload> ( Payload_AUTH::parse( byte_buffer ) );
            case Payload::PAYLOAD_NONCE:
                return auto_ptr<Payload> ( Payload_NONCE::parse( byte_buffer ) );
            case Payload::PAYLOAD_KE:
                return auto_ptr<Payload> ( Payload_KE::parse( byte_buffer ) );
            case Payload::PAYLOAD_IDi:
                return auto_ptr<Payload> ( Payload_IDi::parse( byte_buffer ) );
            case Payload::PAYLOAD_IDr:
                return auto_ptr<Payload> ( Payload_IDr::parse( byte_buffer ) );
            case Payload::PAYLOAD_TSi:
                return auto_ptr<Payload> ( Payload_TSi::parse( byte_buffer ) );
            case Payload::PAYLOAD_TSr:
                return auto_ptr<Payload> ( Payload_TSr::parse( byte_buffer ) );
            case Payload::PAYLOAD_SA:
                return auto_ptr<Payload> ( Payload_SA::parse( byte_buffer ) );
            case Payload::PAYLOAD_DEL:
                return auto_ptr<Payload> ( Payload_DEL::parse( byte_buffer ) );
            case Payload::PAYLOAD_NOTIFY:
                return auto_ptr<Payload> ( Payload_NOTIFY::parse( byte_buffer ) );
            case Payload::PAYLOAD_CONF:
                return auto_ptr<Payload> ( Payload_CONF::parse( byte_buffer ) );
            case Payload::PAYLOAD_EAP:
                return auto_ptr<Payload> ( Payload_EAP::parse( byte_buffer ) );
            case Payload::PAYLOAD_CERT:
                return auto_ptr<Payload> ( Payload_CERT::parse( byte_buffer ) );
            case Payload::PAYLOAD_CERT_REQ:
                return auto_ptr<Payload> ( Payload_CERT_REQ::parse( byte_buffer ) );
            case Payload::PAYLOAD_VENDOR:
                return auto_ptr<Payload> ( Payload_VENDOR::parse( byte_buffer ) );
            case Payload::PAYLOAD_SK:
                return auto_ptr<Payload> ( Payload_SK::parse( byte_buffer ) );
            default:
                return auto_ptr<Payload> ( NULL );
        }
    }
}


