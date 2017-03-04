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
#ifndef ID_H
#define ID_H

#include "ipaddress.h"
#include "printable.h"
#include "enums.h"

namespace openikev2 {

    /**
        This class represents a peer identity
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */

    class ID : public Printable {
            /****************************** ATTRIBUTES ******************************/
        public:
            Enums::ID_TYPE id_type;         /**< Type of identification data. */
            auto_ptr<ByteArray> id_data;    /**< Identification data */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new ID
             * @param type Identification type
             * @param id_data Identification data
             */
            ID( Enums::ID_TYPE type, auto_ptr<ByteArray> id_data );

            /**
             * Creates a new ID based on an IP address
             * @param ip_address IP Address
             */
            ID( IpAddress& ip_address );

            /**
             * Creates a new ID based on a string (to be used with all the textual ID types)
             * @param type ID type
             * @param id_data string containing identification data
             */
            ID( Enums::ID_TYPE type, string id_data );

            /**
             * Creates a new ID cloning another one
             * @param other Other ID
             */
            ID( const ID& other );

            /**
             * Indicates if this ID is equals to another
             * @param other Other ID
             * @return TRUE if both are equals. FALSE otherwise.
             */
            virtual bool operator==( const ID& other ) const;

            /**
             * Creates a clone of this ID
             * @return A new ID object
             */
            virtual auto_ptr<ID> clone() const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~ID();
    };
};
#endif
