/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Lesser General Public            *
*   License as published by the Free Software Foundation; either          *
*   version 2.1 of the License, or (at your option) any later version.    *
*                                                                         *
*   This library is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
*   Lesser General Public License for more details.                       *
*                                                                         *
*   You should have received a copy of the GNU Lesser General Public      *
*   License along with this library; if not, write to the Free Software   *
*   Foundation, Inc., 51 Franklin St, Fifth Floor,                        *
*   Boston, MA  02110-1301  USA                                           *
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
