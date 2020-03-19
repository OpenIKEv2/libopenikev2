/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendeapfinishreqcommand.h"

namespace openikev2 {

    SendEapFinishReqCommand::SendEapFinishReqCommand()
            : Command( false ) {}

    SendEapFinishReqCommand::~SendEapFinishReqCommand() {}


    IkeSa::IKE_SA_ACTION SendEapFinishReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createEapFinishRequest();
    }

    string SendEapFinishReqCommand::getCommandName() const {
        return "SEND_EAP_FINISH_REQ";
    }
}
