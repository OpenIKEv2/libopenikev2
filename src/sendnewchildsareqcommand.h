/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDNEWCHILDSAREQCOMMAND_H
#define OPENIKEV2SENDNEWCHILDSAREQCOMMAND_H

#include "command.h"
#include "childsarequest.h"

namespace openikev2 {

    /**
        This class represents a Send New CHILD_SA request Command.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendNewChildSaReqCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            auto_ptr<ChildSaRequest> child_sa_request;        /**< New CHILD_SA to be created */

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new SendNewChildSaReqCommand
             * @param new_child_sa CHILD_SA to be created
             */
            SendNewChildSaReqCommand( auto_ptr<ChildSaRequest> child_sa_request );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendNewChildSaReqCommand();
    };

}

#endif
