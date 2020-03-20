/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/

#ifndef UTILES_H
#define UTILES_H

#include "printable.h"
#include "bytearray.h"
#include <assert.h>

namespace openikev2 {
    string intToString( uint32_t integer );
    string intToString( int32_t integer );
    string intToString( uint64_t integer );
    string intToString( int64_t integer );
    string floatToString( float floating );
    string boolToString( bool boolean );
}

#endif
