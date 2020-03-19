/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendrekeyikesareqcommand.h"

namespace openikev2 {

    SendRekeyIkeSaReqCommand::SendRekeyIkeSaReqCommand()
            : Command( false ) {}

    SendRekeyIkeSaReqCommand::~SendRekeyIkeSaReqCommand() {}

    IkeSa::IKE_SA_ACTION SendRekeyIkeSaReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createRekeyIkeSaRequest();
    }

    string SendRekeyIkeSaReqCommand::getCommandName() const {
        return "SEND_REKEY_IKE_SA_REQ";
    }

}
