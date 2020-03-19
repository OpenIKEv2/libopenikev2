/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef AAACONTROLLER_H
#define AAACONTROLLER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "aaacontrollerimpl.h"
#include "eappacket.h"
#include "aaasender.h"

namespace openikev2 {

    /**
        This class represents an AAA Manager, that controls the AAA communication.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class AAAController {
            /****************************** ATTRIBUTES ******************************/
        protected:
            static AAAControllerImpl* implementation;     /**< Implementation of the AlarmController */

            /****************************** METHODS ******************************/
        public:
            /**
             * Establish the AAAController implementation
             * @param implementation AAAController implementation to be used
             */
            static void setImplementation( AAAControllerImpl* implementation );

            /**
             * Adds the Alarm to the Alarm collection
             * @param alarm Alarm to be added
             */
            static void AAA_send( AAASender& sender );

    };
}
#endif
