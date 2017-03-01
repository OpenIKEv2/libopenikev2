/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
*                                                                      *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Lesser General Public            *
*   License as published by the Free Software Foundation; either          *
*   version 2.1 of the License, or (at your option) any later version.    *
*                                                                      *
*   This library is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
*   Lesser General Public License for more details.                       *
*                                                                      *
*   You should have received a copy of the GNU Lesser General Public      *
*   License along with this library; if not, write to the Free Software   *
*   Foundation, Inc., 51 Franklin St, Fifth Floor,                        *
*   Boston, MA  02110-1301  USA                                           *
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
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj.fernandez@dif.um.es, alejandro_perez@dif.um.es>
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
