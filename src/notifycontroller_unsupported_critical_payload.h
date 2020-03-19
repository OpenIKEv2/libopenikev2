/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NOTIFYCONTROLLER_UNSUPPORTED_CRITICAL_PAYLOAD_H
#define NOTIFYCONTROLLER_UNSUPPORTED_CRITICAL_PAYLOAD_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an UNSUPPORTED_CRITICAL_PAYLOAD notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_UNSUPPORTED_CRITICAL_PAYLOAD();
    };
};
#endif
