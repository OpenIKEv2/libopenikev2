/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Alejandro Perez Mendez     alex@um.es                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
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
#ifndef OPENIKEV2VECTOROFPOINTER_H
#define OPENIKEV2VECTOROFPOINTER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include <vector>

using namespace std;

namespace openikev2 {

    template <typename _Tp1>
    struct AutoVectorRef {
        vector<_Tp1*> internal_vector;
        explicit AutoVectorRef( vector<_Tp1*> __p ) throw() : internal_vector( __p ) {};
    };

    /**
        This class wraps an STL vector of pointers, deleting all the internal objects in it destructor
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    template <typename T> class AutoVector {
            typedef T* element_type;
            typedef vector<element_type> collection;

            /****************************** ATTRIBUTES ******************************/
        protected:
            collection internal_vector;     /**< Internal vector */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new empty AutoVector
             */
            AutoVector( ) throw() {};

            /**
             * Creates a new AutoVector initializated with the indicated vector of pointers
             * @param v Vector of pointers
             */
            explicit AutoVector( collection v ) throw() : internal_vector( v ) {};

            /**
             * Crates a new AutoVector and obtainst the content from another one
             * @param other Another AutoVector
             */
            AutoVector( AutoVector< T > &other ) throw() : internal_vector( other.release() ) {};

            /**
            * Assingns other AutoVector to this one
            * @param other Another AutoVector
            */
            AutoVector<T>& operator=( AutoVector<T>& other ) throw() {
                this->reset( other.release() );
                return *this;
            }

            virtual ~AutoVector() {
                this->deleteAll();
            }

            /**
            * Obtains a reference to the internal vector of pointers
            * @return Reference to the internal vector of pointers
            */
            collection& get() {
                return this->internal_vector;
            }

            /**
            * Obtains a const reference to the internal vector of pointers
            * @return Reference to the internal vector of pointers
            */
            const collection& get() const {
                return this->internal_vector;
            }

            /**
            * Obtains the internal vector of pointers and releases the AutoVector
            * @return Reference to a vector of pointers
            */
            collection release() {
                collection temp = this->internal_vector;
                this->internal_vector.clear();
                return temp;
            }

            /**
            * Changes the internal vector of pointers
            * @return Vector of pointers
            */
            void reset( collection v ) {
                this->deleteAll();
                this->internal_vector = v;
            }

            collection* operator->() throw() {
                return & this->internal_vector;
            }

            void clear() {
                this->deleteAll();
                this->internal_vector.clear();
            }

            const collection* operator->() const throw() {
                return & this->internal_vector;
            }

            element_type& operator[] ( int i ) {
                return this->internal_vector[ i ];
            }

            AutoVector( AutoVectorRef<T> __ref ) throw() : internal_vector ( __ref.internal_vector ) {}

            AutoVector& operator=( AutoVectorRef<T> __ref ) throw() {
                this->deleteAll();
                this->internal_vector = __ref.internal_vector;
                return *this;
            }

            template <typename _Tp1> operator AutoVectorRef< _Tp1 >() throw() {
                return AutoVectorRef<_Tp1>( this->release() );
            }

            template <typename _Tp1> operator AutoVector<_Tp1>() throw() {
                return AutoVector<_Tp1>( this->release() );
            }

            void deleteAll() {
                typename collection::iterator it;
                for ( it = this->internal_vector.begin(); it != this->internal_vector.end(); it++ )
                    delete ( *it );
            }

            template<typename T2> AutoVector<T2> convertType() {
                AutoVector<T2> result;
                typename collection::iterator it;

                for ( it = this->internal_vector.begin(); it != this->internal_vector.end(); it++ )
                    result->push_back( ( T2* ) *it );

                this->internal_vector.clear();

                return result;
            }

    };

}

#endif
