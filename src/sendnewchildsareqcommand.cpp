/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendnewchildsareqcommand.h"

namespace openikev2 {

    SendNewChildSaReqCommand::SendNewChildSaReqCommand( auto_ptr<ChildSaRequest> child_sa_request )
            : Command( true ) {
        this->child_sa_request = child_sa_request;
    }

    SendNewChildSaReqCommand::~SendNewChildSaReqCommand() {}

    IkeSa::IKE_SA_ACTION SendNewChildSaReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createNewChildSaRequest( this->child_sa_request );
    }

    string SendNewChildSaReqCommand::getCommandName() const {
        return "SEND_NEW_CHILD_SA_REQ";
    }

}

