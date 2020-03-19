/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef BUSOBSERVER_H
#define BUSOBSERVER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "busevent.h"

namespace openikev2 {

    /**
        This abstract class represents a Bus Observer.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class BusObserver {

            /****************************** METHODS ******************************/
        public:
            /**
             * Method to receive Bus Events
             * @param event Bus Event
             */
            virtual void notifyBusEvent( const BusEvent& event ) = 0;

            virtual ~BusObserver();
    };
};
#endif
