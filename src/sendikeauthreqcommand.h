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
#ifndef OPENIKEV2SENDIKEAUTHREQCOMMAND_H
#define OPENIKEV2SENDIKEAUTHREQCOMMAND_H

#include "command.h"
#include "payload_cert_req.h"

namespace openikev2 {

    /**
        This class represents a Send IKE_AUTH request Command.
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class SendIkeAuthReqCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            AutoVector<Payload_CERT_REQ> payloads_cert_req;    /**< Payload_CERT_REQ collection to be included in the AUTH request */

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new SendIkeAuthReqCommand.
             * @param payloads_cert_req Payload_CERT_REQ collection to be included in the AUTH request. They will be cloned.
             */
            SendIkeAuthReqCommand( AutoVector<Payload_CERT_REQ> payloads_cert_req );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );

            virtual string getCommandName() const;

            virtual ~SendIkeAuthReqCommand();

    };

}

#endif
