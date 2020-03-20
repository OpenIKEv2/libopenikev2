/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDIKESAINITREQCOMMAND_H
#define OPENIKEV2SENDIKESAINITREQCOMMAND_H

#include "command.h"

namespace openikev2 {

    /**
        This class represents an Send IKE_SA_INIT request Command
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendIkeSaInitReqCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ChildSaRequest> child_sa_request;        /**< New CHILD_SA to be created */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new SendIkeSaInitReqCommand
             */
            SendIkeSaInitReqCommand( auto_ptr<ChildSaRequest> child_sa_request );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendIkeSaInitReqCommand();

    };

}

#endif
