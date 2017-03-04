/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
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
