/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef OPENIKEV2EXITIKESACOMMAND_H
#define OPENIKEV2EXITIKESACOMMAND_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "command.h"

namespace openikev2 {

    /**
        This class represents an Exit IKE_SA Command
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ExitIkeSaCommand : public Command {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new ExitIkeSaCommand
             */
            ExitIkeSaCommand();

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );

            virtual string getCommandName() const;

            virtual ~ExitIkeSaCommand();
    };

}

#endif
