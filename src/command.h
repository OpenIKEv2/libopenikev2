/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
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
#ifndef COMMAND_H
#define COMMAND_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "enums.h"
#include "ikesa.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents various sorts of Command for the IkeSa objects
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Command {
            /****************************** ATTRIBUTES ******************************/
        protected:
            bool is_inheritable;                    /**< Indicates if the command will be inherited on IKE_SA rekeyings */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Commmand
             * @param is_inheritable If the Commnad must be inherited in the IKE_SA rekeyings
             */
            Command( bool is_inheritable );

            /**
             * Execute the commmand
             * @param ike_sa IkeSa receiving the action
             * @return The action to be performed after Command execution
             */
            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa ) = 0;

            /**
             * Get the command name
             * @return The commad name
             */
            virtual string getCommandName() const = 0;

            /**
             * Indicates if the command will be inherited on IKE_SA rekeyings
             * @return TRUE if the command is inheritable. FALSE otherwise
             */
            virtual bool isInheritable() const;

            virtual ~Command();
    };
}


#endif
