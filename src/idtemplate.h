/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
 *   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
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
#ifndef OPENIKEV2IDTEMPLATE_H
#define OPENIKEV2IDTEMPLATE_H

#include "id.h"

namespace openikev2 {

    /**
    This abstract class represents a template to match IDs
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
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
