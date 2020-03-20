/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDEAPCONTINUEREQCOMMAND_H
#define OPENIKEV2SENDEAPCONTINUEREQCOMMAND_H

#include "command.h"
#include "payload_eap.h"

namespace openikev2 {

    /**
        This class represents a Send EAP_CONTINUE request Command
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendEapContinueReqCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<Payload_EAP> payload_eap;          /**< Payload_EAP containing the EAP response */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new SendEapContinueReqCommand
             * @param payload_eap Payload_EAP containing the EAP response
             */
            SendEapContinueReqCommand( auto_ptr<Payload_EAP> payload_eap );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendEapContinueReqCommand();
    };

}

#endif
