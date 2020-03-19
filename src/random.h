/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef RANDOM_H
#define RANDOM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bytearray.h"

using namespace std;

namespace openikev2 {

    /**
        This abstract class represents a random number generator.
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Random {

            /****************************** METHODS ******************************/
        public:
            /**
             * Get a new ByteArray filled with random bytes
             * @param size Size of the ByteArray to be returned
             * @return A new randomly filled ByteArray
             */
            virtual auto_ptr<ByteArray> getRandomBytes( uint32_t size ) = 0;

            /**
             * Generate a 32 bits random number between min and max (both included)
             * @param min Minimun number to be generated.
             * @param max Maximun number to be generated.
             * @return Random number generated.
             */
            virtual uint32_t getRandomInt32( uint32_t min, uint32_t max ) = 0;

            /**
             * Generate a 64 bits random number between min and max
             * @param min Minimun number to be generated.
             * @param max Maximun number to be generated.
             * @return Random number generated.
             */
            virtual uint64_t getRandomInt64( uint64_t min, uint64_t max ) = 0;

            virtual ~Random();
    };
}
#endif
