/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
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
#ifndef OPENIKEV2AUTOLOCK_H
#define OPENIKEV2AUTOLOCK_H

#include <iostream>
#include <cstdlib>
#include <sstream>

#include "mutex.h"

namespace openikev2 {

    /**
        This class automatically acquires a Mutex and release it when it goes out of scope (if not released before)
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class AutoLock {

            /****************************** ATTRIBUTES ******************************/
        private:
            Mutex& mutex;           /**< Mutex to be managed */
            bool must_release;      /**< Indicates if the mutex must be relased when the AutoLock goes out of scope */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new AutoLock to manage the indicated Mutex
             * @param managed_mutex Mutex to be managed
             */
            AutoLock( Mutex& managed_mutex );

            /**
             * Releases the managed Mutex before the AutoLock destruction
             */
            void release();

            ~AutoLock();
    };
}

#endif
