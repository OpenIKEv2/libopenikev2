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
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
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
