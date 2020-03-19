/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NOTIFYCONTROLLER_REKEY_SA_H
#define NOTIFYCONTROLLER_REKEY_SA_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an REKEY_SA notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController_REKEY_SA : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            NotifyController_REKEY_SA();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual void addNotify( Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_REKEY_SA();
    };
};
#endif
