/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "senddeletechildsareqcommand.h"

namespace openikev2 {

    SendDeleteChildSaReqCommand::SendDeleteChildSaReqCommand( uint32_t deleted_spi )
            : Command( true ) {
        this->deleted_spi = deleted_spi;
    }

    SendDeleteChildSaReqCommand::~SendDeleteChildSaReqCommand() {}

    IkeSa::IKE_SA_ACTION SendDeleteChildSaReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createDeleteChildSaRequest( this->deleted_spi );
    }

    string SendDeleteChildSaReqCommand::getCommandName() const {
        return "SEND_DELETE_CHILD_SA_REQ";
    }

}
