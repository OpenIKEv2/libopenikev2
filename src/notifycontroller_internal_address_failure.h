/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NOTIFYCONTROLLER_INTERNAL_ADDRESS_FAILURE_H
#define NOTIFYCONTROLLER_INTERNAL_ADDRESS_FAILURE_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an INTERNAL_ADDRESS_FAILURE notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController_INTERNAL_ADDRESS_FAILURE : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new NotifyController_INTERNAL_ADDRESS_FAILURE
             */
            NotifyController_INTERNAL_ADDRESS_FAILURE();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_INTERNAL_ADDRESS_FAILURE();
    };
};
#endif
