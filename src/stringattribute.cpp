/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "stringattribute.h"

namespace openikev2 {

    StringAttribute::StringAttribute( string value ) {
        this->value = value;
    }

    StringAttribute::~StringAttribute() {}

    auto_ptr<Attribute> StringAttribute::cloneAttribute() const {
        return auto_ptr<Attribute> ( new StringAttribute( this->value ) );
    }

    string StringAttribute::toStringTab( uint8_t tabs ) const {
        return value + " [STR]";
    }
}
