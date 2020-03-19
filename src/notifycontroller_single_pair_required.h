/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NOTIFYCONTROLLER_SINGLE_PAIR_REQUIRED_H
#define NOTIFYCONTROLLER_SINGLE_PAIR_REQUIRED_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an SINGLE_PAIR_REQUIRED notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController_SINGLE_PAIR_REQUIRED : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            NotifyController_SINGLE_PAIR_REQUIRED();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_SINGLE_PAIR_REQUIRED();
    };
};
#endif
