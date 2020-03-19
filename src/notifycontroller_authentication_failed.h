/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NOTIFYCONTROLLER_AUTHENTICATION_FAILED_H
#define NOTIFYCONTROLLER_AUTHENTICATION_FAILED_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an AUTHENTICATION_FAILED notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
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
