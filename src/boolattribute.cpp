/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
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
