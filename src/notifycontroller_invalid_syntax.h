/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef NOTIFYCONTROLLER_INVALID_SYNTAX_H
#define NOTIFYCONTROLLER_INVALID_SYNTAX_H

#include "notifycontroller.h"

namespace openikev2 {

    /**
        This class represents an INVALID SYNTAX notify controller
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class NotifyController_INVALID_SYNTAX : public NotifyController {

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new NotifyController_INVALID_SYNTAX
             */
            NotifyController_INVALID_SYNTAX();

            virtual IkeSa::NOTIFY_ACTION processNotify( Payload_NOTIFY& notify, Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            virtual ~NotifyController_INVALID_SYNTAX();
    };

};
#endif
