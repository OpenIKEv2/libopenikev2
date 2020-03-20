/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#include "utils.h"
#include <stdio.h>

namespace openikev2 {

//**** PRINTING UTILS *************************************************
    string intToString( uint32_t integer ) {
        string str;
        char temp[20];
        snprintf( temp, 20, "%u", integer );
        str += temp;
        return str;
    }

    string intToString( int32_t integer ) {
        string str;
        char temp[20];
        snprintf( temp, 20, "%d", integer );
        str += temp;
        return str;
    }

    string intToString( uint64_t integer ) {
        string str;
        char temp[20];
        snprintf( temp, 20, "%lu", integer );
        str += temp;
        return str;
    }

    string intToString( int64_t integer ) {
        string str;
        char temp[20];
        snprintf( temp, 20, "%li", integer );
        str += temp;
        return str;
    }

    string boolToString( bool boolean ) {
        string str;
        if ( boolean )
            return "YES";
        return "NO";
    }

    string floatToString( float floating ) {
        string str;
        char temp[200];
        snprintf( temp, 200, "%f", floating );
        str += temp;
        return str;
    }

}


//*********************************************************************








