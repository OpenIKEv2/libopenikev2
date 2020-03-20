/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2CHILDSACOLLECTION_H
#define OPENIKEV2CHILDSACOLLECTION_H

#include <map>
#include "childsa.h"
#include "mutex.h"
#include "autolock.h"

using namespace std;

namespace openikev2 {

    /**
     This class represent a CHILD SA collection
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class ChildSaCollection : public Printable {
            /****************************** ATTRIBUTES ******************************/
        protected:
            map<uint32_t, ChildSa*> child_sa_collection_inbound;            /**< Child SA collection (indexed by inbound SPI) */
            map<uint32_t, ChildSa*> child_sa_collection_outbound;           /**< Child SA collection (indexed by outbound SPI) */
            auto_ptr<Mutex> mutex;                                          /**< Mutex to protect accesses */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates an empty ChildSa collection
             */
            ChildSaCollection();

            /**
             * Adds a ChildSa object to the collection
             * @param child_sa ChildSa object to be added
             */
            virtual void addChildSa( auto_ptr<ChildSa> child_sa );

            /**
             * Gets the ChildSa object owning the indicated SPI value
             * @param spi SPI value
             * @return The ChildSa object. NULL if the SPI value is not found.
             */
            virtual ChildSa* getChildSa( uint32_t spi );

            /**
             * Returns the first ChildSa object of the collection
             * @return The first ChildSa object of the collection
             */
            virtual ChildSa* getFirstChildSa();

            /**
             * Removes the ChildSa object with the indicated SPI value
             * @param spi SPI value
             */
            virtual void deleteChildSa( uint32_t spi );

            /**
             * Indicates if the ChildSaCollection has some ChildSa object with the indicated SPI
             * @param spi SPI value
             * @return TRUE if any ChildSa is found. FALSE otherwise
             */
            virtual bool hasChildSa( uint32_t spi );

            /**
             * Indicated if this collection has half-closed ChildSa
             * @return TRUE if there is any half-closed ChildSa. FALSE otherwise
             */
            virtual bool hasHalfClosedChildSas() const;

            /**
             * Indicates the number of ChildSa objects in the collection
             * @return The number of ChildSa objects in the collection
             */
            virtual uint32_t size();

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~ChildSaCollection();
    };
}

#endif
