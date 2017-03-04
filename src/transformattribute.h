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
#ifndef TRANSFORMATTRIBUTE_H
#define TRANSFORMATTRIBUTE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "enums.h"
#include "bytebuffer.h"
#include "bytearray.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents a transform attribute, used in a cryptographic suite negociation.
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class TransformAttribute : public Printable {

            /****************************** ATTRIBUTES ******************************/
        public:
            auto_ptr<ByteArray> TLVvalue;      /**< Attribute value (when using TLV format) */
            uint16_t TVvalue;                   /**< Attribute value (when using TV format) */
            bool isTV;                          /**< Attribute format (TLV or TV). */
            Enums::ATTRIBUTE_TYPE type;         /**< Attribute type */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new TransformAttribute, in TV format
             * @param type Attribute type.
             * @param TVvalue Attribute value.
             */
            TransformAttribute( Enums::ATTRIBUTE_TYPE type, uint16_t TVvalue );

            /**
             * Creates a new TransformAttribute, in TLV format
             * @param type Attribute type.
             * @param TLVvalue Attribute value.
             */
            TransformAttribute( Enums::ATTRIBUTE_TYPE type, auto_ptr<ByteArray> TLVvalue );

            /**
             * Creates a new TransformAttribute based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the begin of a TransformAttribute
             */
            static auto_ptr<TransformAttribute> parse( ByteBuffer& byte_buffer );

            /**
             * Appends the binary representation of the transform in the ByteBuffer
             * @param byte_buffer ByteBuffer where append the binary representation
             */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            /**
             * Clones this TransformAttribute
             * @return A new TransformAttribute
             */

            virtual auto_ptr<TransformAttribute> clone() const;

            /**
             * Compares if this TransformAttribute object is equal to other
             * @param other The other TransformAttribute
             * @return TRUE if both are equals, FALSE otherwise
             */
            virtual bool operator==( const TransformAttribute &other ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~TransformAttribute();

    };
};
#endif
