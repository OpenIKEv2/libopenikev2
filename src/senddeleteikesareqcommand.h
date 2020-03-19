/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef OPENIKEV2SENDDELETEIKESAREQCOMMAND_H
#define OPENIKEV2SENDDELETEIKESAREQCOMMAND_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "command.h"

namespace openikev2 {

    /**
        This class represents a Send DELETE IKE_SA request Command
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendDeleteIkeSaReqCommand : public Command {

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new SendDeleteIkeSaReqCommand
             */
            SendDeleteIkeSaReqCommand();

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendDeleteIkeSaReqCommand();
    };

}

#endif
