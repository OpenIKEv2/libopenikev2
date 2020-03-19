/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include <memory>

namespace openikev2 {

    /**
        This abstract class represents an object that can be stored into an AttributeMap
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Attribute : public Printable {

            /****************************** METHODS ******************************/
        public:
            /**
             * Clones the Attribute
             * @return A new object identical to this
             */
            virtual auto_ptr<Attribute> cloneAttribute() const = 0;

            virtual string toStringTab( uint8_t tabs ) const = 0;

            virtual ~Attribute();
    };
}
#endif
