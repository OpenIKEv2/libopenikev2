/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "messagereceivedcommand.h"

namespace openikev2 {

    MessageReceivedCommand::MessageReceivedCommand( auto_ptr< Message > message )
            : Command( false ) {
        this->message = message;
    }

    IkeSa::IKE_SA_ACTION MessageReceivedCommand::executeCommand( IkeSa & ike_sa ) {
        return ike_sa.processMessage( *this->message );
    }

    string MessageReceivedCommand::getCommandName( ) const {
        return "MESSAGE_RECEIVED";
    }

    MessageReceivedCommand::~MessageReceivedCommand() {}}




