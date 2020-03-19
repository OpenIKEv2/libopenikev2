/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#include "transformattribute.h"
#include "utils.h"

namespace openikev2 {

    TransformAttribute::TransformAttribute( Enums::ATTRIBUTE_TYPE type, uint16_t TVvalue ) {
        this->isTV = true;
        this->type = type;
        this->TVvalue = TVvalue;
    }

    TransformAttribute::TransformAttribute( Enums::ATTRIBUTE_TYPE type, auto_ptr<ByteArray> TLVvalue ) {
        this->isTV = false;
        this->type = type;
        this->TLVvalue = TLVvalue;
        this->TVvalue = 0;
    }

    auto_ptr<TransformAttribute> TransformAttribute::parse( ByteBuffer& byte_buffer ) {
        // reads the first 2 bytes
        uint16_t temp = byte_buffer.readInt16();

        // get the attribute format bit
        bool isTV = temp >> 15;

        // gets the attribute type
        Enums::ATTRIBUTE_TYPE type = ( Enums::ATTRIBUTE_TYPE ) ( temp & 0x7FFF );

        // if TV format
        auto_ptr<TransformAttribute> result;
        if ( isTV ) {
            uint16_t TVvalue = byte_buffer.readInt16();
            result.reset( new TransformAttribute( type, TVvalue ) );
        }
        // Else, format is TLV
        else {
            uint16_t length = byte_buffer.readInt16();
            auto_ptr<ByteArray> TLVvalue = byte_buffer.readByteArray( length );
            result.reset( new TransformAttribute( type, TLVvalue ) );
        }

        return result;
    }

    TransformAttribute::~TransformAttribute() {}

    auto_ptr< TransformAttribute > TransformAttribute::clone( ) const {
        auto_ptr<TransformAttribute> result;

        if ( this->isTV )
            result.reset( new TransformAttribute( this->type, this->TVvalue ) );
        else
            result.reset ( new TransformAttribute( this->type, this->TLVvalue->clone() ) );

        return result;
    }

    void TransformAttribute::getBinaryRepresentation( ByteBuffer& byte_buffer ) const {
        // writes attribute format and type
        byte_buffer.writeInt16( this->isTV << 15 | this->type );

        // if TV format
        if ( this->isTV ) {
            byte_buffer.writeInt16( this->TVvalue );
        }
        // else, if TLV format
        else {
            byte_buffer.writeInt16( this->TLVvalue->size() );
            byte_buffer.writeByteArray( *this->TLVvalue );
        }
    }

    string TransformAttribute::toStringTab( uint8_t tabs ) const {
        ostringstream oss;

        oss << Printable::generateTabs( tabs ) << "<TRANSFORM_ATTRIBUTE> {\n";

        oss << Printable::generateTabs( tabs + 1 ) << "isTV=" << this->isTV << "\n";

        oss << Printable::generateTabs( tabs + 1 ) << "type=" << Enums::ATTRIBUTE_TYPE_STR( this->type ) << "\n";

        if ( this->isTV )
            oss << Printable::generateTabs( tabs + 1 ) << "TVvalue=" << this->TVvalue << "\n";
        else
            oss << Printable::generateTabs( tabs + 1 ) << "TLVvalue=" << this->TLVvalue->toStringTab( tabs + 1 ) << "\n";

        oss << Printable::generateTabs( tabs ) << "}\n";

        return oss.str();
    }

    bool TransformAttribute::operator ==( const TransformAttribute & other ) const {
        if ( this->isTV != other.isTV )
            return false;

        if ( this->isTV ) {
            if ( this->TVvalue != other.TVvalue )
                return false;
        }
        else {
            if ( !( *this->TLVvalue == *other.TLVvalue ) )
                return false;
        }

        return true;
    }
}



