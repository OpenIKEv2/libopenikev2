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




