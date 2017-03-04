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
#ifndef NOTIFYCONTROLLER_H
#define NOTIFYCONTROLLER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ikesa.h"
#include "childsa.h"
#include "payload_notify.h"

namespace openikev2 {

    /**
        This class represents a Notify Controller.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController {

            /****************************** METHODS ******************************/
        public:
            /**
             * Process a received NOTIFY Payload
             * @param notify Received NOTIFY Payload
             * @param message Message containig the notification
             * @param ike_sa IKE_SA that receives the notification
             * @param child_sa Related CHILD_SA (NULL if no CHILD_SA is related)
             * @return The action to be performed after payload processing.
             */
            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa ) = 0;

            /**
             * Adds new NOTIFY payload
             * @param message Message to include the notification
             * @param ike_sa IKE_SA that sends the notification
             * @param child_sa Related CHILD_SA (NULL if no CHILD_SA is related)
             */
            virtual void addNotify( Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController();
    };
}
#endif
