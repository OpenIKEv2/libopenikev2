/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2IDTEMPLATE_H
#define OPENIKEV2IDTEMPLATE_H

#include "id.h"

namespace openikev2 {

    /**
    This abstract class represents a template to match IDs
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class IdTemplate : public Printable {
            /****************************** METHODS ******************************/
        public:
            /**
             * Checks if the ID matches with this template
             * @param id ID to be checked
             * @return TRUE if the ID matches. FALSE otherwise
             */
            virtual bool match( const ID& id ) const = 0;

            /**
             * Clones this IdTemplate object
             * @return A new IdTemplate object
             */
            virtual auto_ptr<IdTemplate> clone() const = 0;

            virtual string toStringTab( uint8_t tabs ) const = 0;

            virtual ~IdTemplate();

    };

}

#endif
