/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NOTIFYCONTROLLER_TS_UNACCEPTABLE_H
#define NOTIFYCONTROLLER_TS_UNACCEPTABLE_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an TS_UNACCEPTABLE notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController_TS_UNACCEPTABLE : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            NotifyController_TS_UNACCEPTABLE();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_TS_UNACCEPTABLE();
    };
};
#endif
