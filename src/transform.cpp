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
#include "transform.h"
#include "utils.h"
#include "exception.h"

namespace openikev2 {

    Transform::Transform( Enums::TRANSFORM_TYPE type, uint16_t id ) {
        this->type = type;
        this->id = id;
    }

    Transform::Transform( Enums::TRANSFORM_TYPE type, uint16_t id, uint16_t keylen ) {
        this->type = type;
        this->id = id;
        this->addAttribute( auto_ptr<TransformAttribute> ( new TransformAttribute ( Enums::ATTR_KEY_LEN, keylen ) ) );
    }

    auto_ptr<Transform> Transform::parse( ByteBuffer& byte_buffer ) {
        // Pointer to the beginning of the transform
        uint8_t * transform_begin = byte_buffer.getReadPosition() - 2;

        // reads the transform size
        uint32_t transform_size = byte_buffer.readInt16();

        // Size must be at least size of fixed header
        if ( transform_size < 8 )
            throw ParsingException( "Transform length cannot be < 8 bytes." );

        // Reads the transform type from buffer
        Enums::TRANSFORM_TYPE type = ( Enums::TRANSFORM_TYPE ) byte_buffer.readInt8();

        // reads RESERVED byte
        byte_buffer.readInt8();

        // Reads the transform id from buffer
        uint16_t id = byte_buffer.readInt16();

        // creates the transform
        auto_ptr<Transform> result ( new Transform ( type, id ) );

        // If current position < intial position + size, then there are more attributes
        while ( byte_buffer.getReadPosition() < transform_begin + transform_size ) {
            // parse the transform attribute
            auto_ptr<TransformAttribute> attribute = TransformAttribute::parse( byte_buffer );

            // adds the attribute to the collection
            result->addAttribute( attribute );
        }

        // check size
        if ( byte_buffer.getReadPosition() != transform_begin + transform_size )
            throw ParsingException( "Indicated transform size differs with its real size." );

        return result;
    }

    Transform::~Transform() { }

    auto_ptr<Transform> Transform::clone() const {
        auto_ptr<Transform> result ( new Transform( this->type, this->id ) );

        // Clones all the attributes
        for ( vector<TransformAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ )
            result->addAttribute( ( *it ) ->clone() );

        return result;
    }

    void Transform::addAttribute( auto_ptr<TransformAttribute> attribute ) {
        assert ( attribute.get() != NULL );

        attributes->push_back( attribute.release() );
    }

    void Transform::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // Pointer to the beginning of the transform
        uint8_t * transform_begin = byte_buffer.getWritePosition();

        // writes a dummy value for the transform length
        byte_buffer.writeInt16( 0 );

        // Writes transform type into buffer
        byte_buffer.writeInt8( this->type );

        // Writes RESERVED field
        byte_buffer.writeInt8( 0 );

        // Writes transform id into buffer
        byte_buffer.writeInt16( this->id );

        // Inserts all attributes into buffer
        for ( vector<TransformAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ ) {
            ( *it ) ->getBinaryRepresentation( byte_buffer );
        }

        // pointer to the end of the transform
        uint8_t* transform_end = byte_buffer.getWritePosition();

        // writes the real transform length value
        byte_buffer.setWritePosition( transform_begin );
        byte_buffer.writeInt16( transform_end - transform_begin + 2 );
        byte_buffer.setWritePosition( transform_end );
    }

    string Transform::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<TRANSFORM> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "type=" << Enums::TRANSFORM_TYPE_STR( this->type ) + "\n";

        switch ( this->type ) {
            case Enums::ENCR:
                oss << Printable::generateTabs( tabs + 1 ) << "id=" << Enums::ENCR_ID_STR( ( Enums::ENCR_ID ) this->id ) << "\n";
                break;
            case Enums::PRF:
                oss << Printable::generateTabs( tabs + 1 ) << "id=" << Enums::PRF_ID_STR( ( Enums::PRF_ID ) this->id ) << "\n";
                break;
            case Enums::INTEG:
                oss << Printable::generateTabs( tabs + 1 ) << "id=" << Enums::INTEG_ID_STR( ( Enums::INTEG_ID ) this->id ) << "\n";
                break;
            case Enums::D_H:
                oss << Printable::generateTabs( tabs + 1 ) << "id=" << Enums::DH_ID_STR( ( Enums::DH_ID ) this->id ) << "\n";
                break;
            case Enums::ESN:
                oss << Printable::generateTabs( tabs + 1 ) << "id=" << Enums::ESN_ID_STR( ( Enums::ESN_ID ) this->id ) << "\n";
                break;
            default:
                oss << Printable::generateTabs( tabs + 1 ) << "id=" << this->id << "\n";
                break;
        }

        // prints all attributes
        for ( vector<TransformAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ )
            oss << ( *it ) ->toStringTab( tabs + 1 );

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    bool Transform::hasAttribute( const TransformAttribute & transform_attribute ) const {
        for ( vector<TransformAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ ) {
            TransformAttribute* current_attribute = ( *it );
            if ( *current_attribute == transform_attribute )
                return true;
        }

        return false;
    }

    bool Transform::operator ==( const Transform & other ) const {
        if ( this->type != other.type )
            return false;

        if ( this->id != other.id )
            return false;

        for ( vector<TransformAttribute*>::const_iterator it = this->attributes->begin(); it != this->attributes->end(); it++ ) {
            TransformAttribute* current_attribute = ( *it );
            if ( !other.hasAttribute( *current_attribute ) )
                return false;

        }
        return true;
    }
}





