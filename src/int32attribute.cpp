/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "int32attribute.h"

#include "utils.h"

namespace openikev2 {

    Int32Attribute::Int32Attribute( int32_t value ) {
        this->value = value;
    }

    Int32Attribute::~Int32Attribute() {}


    auto_ptr<Attribute> Int32Attribute::cloneAttribute() const {
        return auto_ptr<Attribute> ( new Int32Attribute( this->value ) );
    }

    string Int32Attribute::toStringTab( uint8_t tabs ) const {
        return intToString( value ) + " [INT32]";
    }
}
