/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

namespace openikev2 {

    /**
        This \b interface represents a semaphore, used to synchronize threads.
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Semaphore {
        public:
            /**
             * Thread will wait until semaphore has non-zero count
             */
            virtual void wait() = 0;

            /**
             * Increments the semaphore count
             */
            virtual void post() = 0;

            virtual ~Semaphore();


    };
};
#endif
