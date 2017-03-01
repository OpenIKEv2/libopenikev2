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
