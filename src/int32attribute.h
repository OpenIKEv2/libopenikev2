/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef INT32ATTRIBUTE_H
#define INT32ATTRIBUTE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "attribute.h"

namespace openikev2 {

    /**
        This class represents a 32bit integer attribute that can be stored in an AttributeMap
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Int32Attribute : public Attribute {

            /****************************** ATTRIBUTES ******************************/
        public:
            int32_t value;                      /**< Attribute value */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Int32Attribute setting its value
             * @param value Attribute value
             */
            Int32Attribute( int32_t value );

            virtual auto_ptr<Attribute> cloneAttribute() const ;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~Int32Attribute();
    };
};
#endif
