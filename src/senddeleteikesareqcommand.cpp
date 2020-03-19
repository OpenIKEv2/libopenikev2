/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "senddeleteikesareqcommand.h"

namespace openikev2 {

    SendDeleteIkeSaReqCommand::SendDeleteIkeSaReqCommand()
            : Command( false ) {}

    SendDeleteIkeSaReqCommand::~SendDeleteIkeSaReqCommand() {}

    IkeSa::IKE_SA_ACTION SendDeleteIkeSaReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createDeleteIkeSaRequest();
    }

    string SendDeleteIkeSaReqCommand::getCommandName() const {
        return "SEND_DELETE_IKE_SA_REQ";
    }

}
