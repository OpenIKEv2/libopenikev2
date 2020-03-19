/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
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
