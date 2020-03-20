/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef ATTRIBUTEMAP_H
#define ATTRIBUTEMAP_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "printable.h"
#include "attribute.h"

#include <map>

using namespace std;

namespace openikev2 {

    /**
        This class represents an attribute map that allows to extend dynamically the number of attributes of a class (integers, strings, booleans...)
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class AttributeMap : public Printable {

            /****************************** ATTRIBUTES ******************************/
        protected:
            map<string, Attribute*> attribute_map;                      /**<  Attribute map */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new empty attribute map
             */
            AttributeMap();

            /**
             * Adds an attribute to the AttributeMap
             * @param key Name of the attribute to be added
             * @param attribute Attribute to be added
             */
            virtual void addAttribute( string key, auto_ptr<Attribute> attribute );

            /**
             * Gets an Attribute by its name
             * @param key Name of the attribute to be obtained
             * @return The attribute with such name. NULL if the attribute was not found
             */
            template<typename T> T* getAttribute( string key ) {
                map<string, Attribute*>::iterator it = this->attribute_map.find( key );
                if ( it == this->attribute_map.end() )
                    return NULL;
                else
                    return dynamic_cast<T*> ( ( *it ).second );
            }


            /**
             * Checks if the specified Attribute exists
             * @param key Attribute name
             */
            bool existsAttribute( string key );


            /**
             * Deletes the Attribute with the indicated name
             * @param key Attribute name
             */
            virtual void deleteAttribute( string key );

            /**
             * Inherits all the non-common attributes from other AttributeMap and deletes them from it
             * @param other Other AttributeMap
             */
            virtual void inherit( AttributeMap& other );

            /**
             * Creates a new AttributeMap exactly equals to this one, with the same attributes inside.
             * @return The new AttributeMap created
             */
            virtual auto_ptr<AttributeMap> clone() const;

            virtual string toStringTab( uint8_t tabs ) const;

            virtual ~AttributeMap();
    };

};
#endif
