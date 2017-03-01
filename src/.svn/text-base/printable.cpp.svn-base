/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj.fernandez@dif.um.es                 *
*   Alejandro Perez Mendez     alejandro_perez@dif.um.es                  *
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
#include "printable.h"

#include <iomanip>

namespace openikev2 {

    unsigned char Printable::hextable[ 16 ] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    void Printable::fillHexValue( unsigned char* destination, unsigned char value, unsigned char terminator ) {
        destination[ 0 ] = hextable[ value >> 4 ];
        destination[ 1 ] = hextable[ value & 0x0F ];
        destination[ 2 ] = terminator;
        destination[ 3 ] = '\0';
    }

    string Printable::toString() const {
        return this->toStringTab( 0 );
    }

    string Printable::toHexString( void *pointer, uint16_t size ) {
        return Printable::toHexString( pointer, size, 16, 0 );
    }

    string Printable::toHexString( void *pointer, uint16_t size, uint16_t line_size_max, uint16_t tabs ) {
        string result;
        uint8_t *buffer = ( uint8_t* ) pointer;

        if ( size == 0 )
            return "[]";

        if ( size > line_size_max )
            result += "\n" + Printable::generateTabs( ++tabs);

        result += "[";

        char temp[4];
        for ( uint16_t i = 0; i < size - 1; i++ ) {
            Printable::fillHexValue( ( unsigned char* ) temp, buffer[i], ':' );
            result += temp;
            if ( ( ( i + 1 ) % line_size_max ) == 0 )
                result += "\n" + Printable::generateTabs( tabs ) + " ";
        }

        Printable::fillHexValue( ( unsigned char* ) temp, buffer[size - 1], ']' );
        result += temp;

        return result;
    }

    string Printable::generateTabs( uint16_t num_tabs ) {
        return string( num_tabs * TAB_SIZE, ' ' );
    }
}
