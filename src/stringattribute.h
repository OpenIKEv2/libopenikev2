/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef STRINGATTRIBUTE_H
#define STRINGATTRIBUTE_H

#include "attribute.h"

namespace openikev2 {

    /**
        This class represents a string attribute that can be stored in an AttributeMap
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class StringAttribute : public Attribute {
            /****************************** ATTRIBUTES ******************************/
        public:
            string value;                   /**< Attribute value */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new StringAttribute setting its value
             * @param value String value
             */
            StringAttribute( string value );

            virtual auto_ptr<Attribute> cloneAttribute() const ;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~StringAttribute();

    };
};
#endif
