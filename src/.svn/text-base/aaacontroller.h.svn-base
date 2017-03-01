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
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alejandro_perez@dif.um.es>
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
