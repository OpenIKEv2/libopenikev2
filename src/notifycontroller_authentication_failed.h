/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#ifndef NOTIFYCONTROLLER_AUTHENTICATION_FAILED_H
#define NOTIFYCONTROLLER_AUTHENTICATION_FAILED_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an AUTHENTICATION_FAILED notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj.fernandez@dif.um.es, alejandro_perez@dif.um.es>
    */
    class NotifyController_AUTHENTICATION_FAILED : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new NotifyController_AUTHENTICATION_FAILED
             */
            NotifyController_AUTHENTICATION_FAILED();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_AUTHENTICATION_FAILED();
    };
};
#endif
