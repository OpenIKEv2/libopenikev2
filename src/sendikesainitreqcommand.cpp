/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendikesainitreqcommand.h"

namespace openikev2 {

    SendIkeSaInitReqCommand::SendIkeSaInitReqCommand(auto_ptr<ChildSaRequest> child_sa_request)
            : Command( false ) {
            this->child_sa_request = child_sa_request;
            }

    SendIkeSaInitReqCommand::~SendIkeSaInitReqCommand() {}

    IkeSa::IKE_SA_ACTION SendIkeSaInitReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createIkeSaInitRequest( this->child_sa_request );
    }

    string SendIkeSaInitReqCommand::getCommandName() const {
        return "SEND_IKE_SA_INIT_REQ";
    }

}
