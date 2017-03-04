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
#ifndef OPENIKEV2SENDINFORMATIONALREQCOMMAND_H
#define OPENIKEV2SENDINFORMATIONALREQCOMMAND_H

#include "command.h"

#include "autovector.h"

namespace openikev2 {

    /**
        This class represents a Send INFORMATIONAL request Command
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendInformationalReqCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            AutoVector<Payload> payloads;       /**< Payloads to be included in the INFORMATIONAL message */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new SendInformationalReqCommand
             */
            SendInformationalReqCommand();

            /**
             * Creates a new SendInformationalReqCommand
             */
            SendInformationalReqCommand( AutoVector<Payload> payloads );

            /**
             * Adds a payload to the INFORMATIONAL message
             * @param payload Payload to be included in the INFORMATIONAL message
             */
            void addPayload( auto_ptr<Payload> payload );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendInformationalReqCommand();
    };

}

#endif
