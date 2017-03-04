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
#include "boolattribute.h"
#include "utils.h"

namespace openikev2 {

    BoolAttribute::BoolAttribute( bool value ) {
        this->value = value;
    }

    BoolAttribute::~BoolAttribute() {}

    auto_ptr<Attribute> BoolAttribute::cloneAttribute() const {
        return auto_ptr<Attribute> ( new BoolAttribute( value ) );
    }

    string BoolAttribute::toStringTab( uint8_t tabs ) const {
        return boolToString( value ) + " [BOOL]";
    }
}
