/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "closeikesacommand.h"

namespace openikev2 {

    CloseIkeSaCommand::CloseIkeSaCommand()
            : Command( false ) {}

    CloseIkeSaCommand::~CloseIkeSaCommand() {}

    IkeSa::IKE_SA_ACTION CloseIkeSaCommand::executeCommand( IkeSa& ike_sa ) {
        return ike_sa.close();
    }

    string CloseIkeSaCommand::getCommandName() const {
        return "CLOSE_ALL_SAS";
    }

}
