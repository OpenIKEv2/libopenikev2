/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendrekeychildsareqcommand.h"

namespace openikev2 {

    SendRekeyChildSaReqCommand::SendRekeyChildSaReqCommand( uint32_t rekeyed_spi )
            : Command( true ) {
        this->rekeyed_spi = rekeyed_spi;
    }

    SendRekeyChildSaReqCommand::~SendRekeyChildSaReqCommand() {}

    IkeSa::IKE_SA_ACTION SendRekeyChildSaReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createRekeyChildSaRequest( this->rekeyed_spi );
    }

    string SendRekeyChildSaReqCommand::getCommandName() const {
        return "SEND_REKEY_CHILD_SA_REQ";
    }

}
