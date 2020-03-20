/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef DEBUGIMPL_H
#define DEBUGIMPL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "log.h"

using namespace std;

namespace openikev2 {
    /**
        This class represents a Log concrete implementation.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class LogImpl {
        public:
            /**
             * Writes a log message to the log file.
             * @param who Module writting the message
             * @param message Log message to be writed
             * @param type Type of log message (Log::LOG_INFO, D_THRD, ...)
             * @param main_info Indicates if date must be writed
             */
            virtual void writeMessage( string who, string message, uint16_t type, bool main_info ) = 0;

            virtual ~LogImpl();
    };

};
#endif
