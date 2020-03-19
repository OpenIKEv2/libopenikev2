/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "sendinformationalreqcommand.h"

namespace openikev2 {

    SendInformationalReqCommand::SendInformationalReqCommand()
            : Command( false ) {}

    SendInformationalReqCommand::SendInformationalReqCommand( AutoVector< Payload > payloads )
            : Command( false ) {
        this->payloads = payloads;
    }

    SendInformationalReqCommand::~SendInformationalReqCommand() { }

    void SendInformationalReqCommand::addPayload( auto_ptr< Payload > payload ) {
        this->payloads->push_back( payload.release() );
    }

    IkeSa::IKE_SA_ACTION SendInformationalReqCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.createGenericInformationalRequest( this->payloads );
    }

    string SendInformationalReqCommand::getCommandName() const {
        return "SEND_INFORMATIONAL_REQ";
    }

}

