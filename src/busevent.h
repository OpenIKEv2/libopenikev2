/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef BUSEVENT_H
#define BUSEVENT_H

#include "stdint.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;

namespace openikev2 {

    /**
        This abstract class represents an generic bus event.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class BusEvent {

            /****************************** ENUMS ******************************/
        public:
            /** Event type */
            enum EVENT_TYPE {
                IKE_SA_EVENT,        /**< IKE SA Bus Event */
                CHILD_SA_EVENT,      /**< CHILD SA Bus Event */
                CORE_EVENT,          /**< Core Bus Event */
            };

            /****************************** METHODS ******************************/
        public:
            EVENT_TYPE type;        /**< Type of the Bus Event */

            virtual ~BusEvent();
    };

};

#endif
