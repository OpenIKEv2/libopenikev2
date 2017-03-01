/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                 *
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
#ifndef OPENIKEV2AAACONTROLLERIMPL_H
#define OPENIKEV2AAACONTROLLERIMPL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eappacket.h"

namespace openikev2 {
    class AAASender;
    /**
        This class contains the AAAController implementation.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class AAAControllerImpl {
            /****************************** METHODS ******************************/
        public:
            /**
             * Sends a AAA message to the AAA server using the proper protocol.
             * @param eap_msg Eap message to be sent
             * @param sender Sender that waits for a AAA response.
             */
            virtual void AAA_send( AAASender& sender ) = 0;

            virtual ~AAAControllerImpl();
    };

}

#endif
