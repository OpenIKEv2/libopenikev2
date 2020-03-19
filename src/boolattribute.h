/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef BOOLATTRIBUTE_H
#define BOOLATTRIBUTE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "attribute.h"

namespace openikev2 {

    /**
        This class represents a boolean attribute that can be stored in an AttributeMap
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class BoolAttribute : public Attribute {

            /****************************** ATTRIBUTES ******************************/
        public:
            bool value;                     /**< Value of the attribute */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new BoolAttribute setting its value
             * @param value Bool value
             */
            BoolAttribute( bool value );

            virtual auto_ptr<Attribute> cloneAttribute() const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~BoolAttribute();
    };
};
#endif
