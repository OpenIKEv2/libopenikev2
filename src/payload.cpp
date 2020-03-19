/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "payload.h"
#include "utils.h"

namespace openikev2 {
    Payload::Payload( Payload::PAYLOAD_TYPE type, bool critical ) {
        this->type = type;
        this->critical = critical;
    }

    Payload::~Payload() {}

    string Payload::PAYLOAD_TYPE_STR( Payload::PAYLOAD_TYPE type ) {
        switch ( type ) {
            case Payload::PAYLOAD_AUTH:
                return "PAYLOAD_AUTH";
            case Payload::PAYLOAD_CERT:
                return "PAYLOAD_CERT";
            case Payload::PAYLOAD_CERT_REQ:
                return "PAYLOAD_CERT_REQ";
            case Payload::PAYLOAD_CONF:
                return "PAYLOAD_CONF";
            case Payload::PAYLOAD_DEL:
                return "PAYLOAD_DEL";
            case Payload::PAYLOAD_EAP:
                return "PAYLOAD_EAP";
            case Payload::PAYLOAD_IDi:
                return "PAYLOAD_IDi";
            case Payload::PAYLOAD_IDr:
                return "PAYLOAD_IDr";
            case Payload::PAYLOAD_KE:
                return "PAYLOAD_KE";
            case Payload::PAYLOAD_NONCE:
                return "PAYLOAD_NONCE";
            case Payload::PAYLOAD_NONE:
                return "PAYLOAD_NONE";
            case Payload::PAYLOAD_NOTIFY:
                return "PAYLOAD_NOTIFY";
            case Payload::PAYLOAD_SA:
                return "PAYLOAD_SA";
            case Payload::PAYLOAD_SK:
                return "PAYLOAD_SK";
            case Payload::PAYLOAD_TSi:
                return "PAYLOAD_TSi";
            case Payload::PAYLOAD_TSr:
                return "PAYLOAD_TSr";
            case Payload::PAYLOAD_VENDOR:
                return "PAYLOAD_VENDOR";
            default:
                return intToString( type );
        }
    }
}
