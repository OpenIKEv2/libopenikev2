/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "exitikesacommand.h"

namespace openikev2 {

    ExitIkeSaCommand::ExitIkeSaCommand()
            : Command( false ) {}

    ExitIkeSaCommand::~ExitIkeSaCommand() {}

    IkeSa::IKE_SA_ACTION ExitIkeSaCommand::executeCommand( IkeSa& ike_sa ) {
        if ( ike_sa.getState() != IkeSa::STATE_CLOSED )
            ike_sa.setState( IkeSa::STATE_CLOSED );
        return IkeSa::IKE_SA_ACTION_DELETE_IKE_SA;
    }

    string ExitIkeSaCommand::getCommandName() const {
        return "EXIT_IKE_SA";
    }

}
