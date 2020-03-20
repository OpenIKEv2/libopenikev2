/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
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
