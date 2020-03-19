/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "transformattribute.h"
#include "autovector.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents a transform (cryptographic algorithm), used in a cryptographic suite negociation
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class Transform : public Printable {

            /****************************** ATTRIBUTES ******************************/
        public:
            uint16_t id;                                    /**< ID of the transform (DES, 3DES, ...) */
            Enums::TRANSFORM_TYPE type;                     /**< Type of the transform (ENCR, PRF, ...) */
            AutoVector<TransformAttribute> attributes;      /**< Transform attributes collection */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Transform, setting its type and its id.
             * @param type Type of the transform (ENCR, PRF, ...)
             * @param id ID of the transform (DES, 3DES, ...)
             */
            Transform( Enums::TRANSFORM_TYPE type, uint16_t id );

            /**
             * Creates a new Transform, setting its type, its id and adding a TransformAttribute of type ATTR_KEY_LEN
             * @param type Type of the transform (ENCR, PRF, ...)
             * @param id ID of the transform (DES, 3DES, ...)
             * @param keylen Value of the ATTR_KEY_LEN attribute
             */
            Transform( Enums::TRANSFORM_TYPE type, uint16_t id, uint16_t keylen );

            /**
             * Creates a new Transform based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "transform length" field
             */
            static auto_ptr<Transform> parse( ByteBuffer& byte_buffer );

            /**
             * Creates a new Transform clonning this
             * @return New cloned Transform
             */
            auto_ptr<Transform> clone() const;

            /**
             * Adds a TransformAttribute to the Transform.
             * @param attribute TransformAttribute to be added.
             */
            void addAttribute( auto_ptr<TransformAttribute> attribute );

            /**
             * Indicates if the Transform has the indicate attribute
             * @param transform_attribute Searched TransformAttribute
             * @return TRUE if the Transform has the indicated attribute. FALSE otherwise
             */
            bool hasAttribute( const TransformAttribute& transform_attribute ) const;

            /**
             * Appends the binary representation of the Transform in the ByteBuffer
             * @param byte_buffer ByteBuffer where append the binary representation
             */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            /**
             * Compares if this Transform object is equal to other
             * @param other The other Transform
             * @return TRUE if both are equals, FALSE otherwise
             */
            virtual bool operator==( const Transform &other ) const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~Transform();

    };
}
#endif
