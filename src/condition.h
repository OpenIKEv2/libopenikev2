/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
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
#ifndef CONDITION_H
#define CONDITION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mutex.h"

namespace openikev2 {

    /**
        This abstract class represents a Condition, used to synchronize Threads.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Condition : public Mutex {

            /****************************** METHODS ******************************/
        public:
            /**
             *    Wait until notify() is executed
             */
            virtual void wait() = 0;

            /**
             *  Wake up one of the Threads waiting on this Condition
             */
            virtual void notify() = 0;

            /**
             *    Locks the internal Mutex.
             */
            virtual void acquire() = 0;

            /**
             *    Unlocks the internal Mutex.
             */
            virtual void release() = 0;

            virtual ~Condition();
    };

};
#endif
