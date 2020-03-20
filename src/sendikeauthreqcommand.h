/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDIKEAUTHREQCOMMAND_H
#define OPENIKEV2SENDIKEAUTHREQCOMMAND_H

#include "command.h"
#include "payload_cert_req.h"

namespace openikev2 {

    /**
        This class represents a Send IKE_AUTH request Command.
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
