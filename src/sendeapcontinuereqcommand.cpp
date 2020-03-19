/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendeapcontinuereqcommand.h"

namespace openikev2 {

    SendEapContinueReqCommand::SendEapContinueReqCommand( auto_ptr<Payload_EAP> payload_eap )
            : Command( false ) {
        this->payload_eap = payload_eap;
    }

    SendEapContinueReqCommand::~SendEapContinueReqCommand() {}

    IkeSa::IKE_SA_ACTION SendEapContinueReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createEapContinueRequest( this->payload_eap );
    }

    string SendEapContinueReqCommand::getCommandName() const {
        return "SEND_EAP_CONTINUE_REQ";
    }

}
