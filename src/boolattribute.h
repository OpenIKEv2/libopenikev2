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
