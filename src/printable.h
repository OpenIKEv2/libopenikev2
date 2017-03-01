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
#ifndef PRINTABLE_H
#define PRINTABLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <sstream>

#define TAB_SIZE    4

using namespace std;

namespace openikev2 {

    /**
        This abstract class defines a class wich is able to represent itself in a textual form
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */
    class Printable {
        protected:
            static unsigned char hextable[ 16 ];        /**< Table to speedup transtalion between deciman and hexadecimal digits */

        protected:
            /**
            * This method writes the hexadecimal value of a byte into destination, using the indicated terminator
            * @param destination
            * @param value
            * @param terminator
            */
            inline static void fillHexValue( unsigned char* destination, unsigned char value, unsigned char terminator );

        public:
            /**
             * Construct a textual hexadecimal representation of raw array.
             * @param pointer Pointer to the raw array
             * @param size Size of the raw array.
             * @return Textual hexadecimal representation of the raw array.
             */
            static string toHexString( void *pointer, uint16_t size );

            /**
             *  Construct a pretty textual hexadecimal representation of a raw array.
             * @param pointer Pointer to a raw array
             * @param size Size of the raw array.
             * @param line_size_max Maximun size of each printed line.
             * @param tabs Number of tabs before each line
             * @return Textual hexadecimal representation of the raw array.
             */
            static string toHexString( void *pointer, uint16_t size, uint16_t line_size_max, uint16_t tabs );

            /**
             * Generate a string with the indicated number of tabs
             * @param num_tabs Number of tabs to be generated
             * @return The new string
             */
            static string generateTabs( uint16_t num_tabs );

            /**
            * Construct a textual representation of the object.
            * This method is equal to toStringTab(0).
            * @return Textual represetation of the object.
            */
            virtual string toString() const ;

            /**
             * Construct a textual representation of the object, preceded of a num of tabs of size TAB_SIZE.
             * @param tabs Num of tabs preceding the textual representation.
             * @return Textual tabulated represetation of the object.
             */
            virtual string toStringTab( uint8_t tabs ) const = 0;

    };
};
#endif
