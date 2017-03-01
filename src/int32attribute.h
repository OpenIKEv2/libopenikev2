/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#ifndef INT32ATTRIBUTE_H
#define INT32ATTRIBUTE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "attribute.h"

namespace openikev2 {

    /**
        This class represents a 32bit integer attribute that can be stored in an AttributeMap
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj.fernandez@dif.um.es, alejandro_perez@dif.um.es>
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
