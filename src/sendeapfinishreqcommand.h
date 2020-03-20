/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDEAPFINISHREQCOMMAND_H
#define OPENIKEV2SENDEAPFINISHREQCOMMAND_H

#include "command.h"

namespace openikev2 {

    /**
        This class represents a Send EAP_FINISH request Command
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendEapFinishReqCommand : public Command {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new SendEapFinishReqCommand
             */
            SendEapFinishReqCommand();

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendEapFinishReqCommand();
    };

}

#endif
