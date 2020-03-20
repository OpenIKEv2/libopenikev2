/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "sendikeauthreqcommand.h"

namespace openikev2 {

    SendIkeAuthReqCommand::SendIkeAuthReqCommand( AutoVector<Payload_CERT_REQ> payloads_cert_req )
            : Command( false ) {
        this->payloads_cert_req = payloads_cert_req;
    }

    SendIkeAuthReqCommand::~SendIkeAuthReqCommand() {}

    IkeSa::IKE_SA_ACTION SendIkeAuthReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createIkeAuthRequest( this->payloads_cert_req.get() );
    }

    string SendIkeAuthReqCommand::getCommandName() const {
        return "SEND_IKE_AUTH_REQ";
    }

}
