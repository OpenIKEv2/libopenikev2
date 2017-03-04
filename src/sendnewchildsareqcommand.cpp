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

