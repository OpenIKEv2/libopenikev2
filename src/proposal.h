/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PROPOSAL_H
#define PROPOSAL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "transform.h"

using namespace std;

namespace openikev2 {

    /**
        This class represents a Proposal, used in a cryptographic suite negociation
        @author Pedro J. Fernandez Ruiz, Alejandro Perez Mendez <pedroj@um.es, alex@um.es>
    */
    class Proposal : public Printable {

            /****************************** ATTRIBUTES ******************************/
        public:
            AutoVector<Transform> transforms;       /**< Transform collection */
            auto_ptr<ByteArray> spi;                /**< SPI value */
            Enums::PROTOCOL_ID protocol_id;         /**< Protocol id */
            uint8_t proposal_number;                /**< Number of the proposal */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Proposal. The SPI will be 0 length until it was assigned
             * @param protocol_id Protocol id
             */
            Proposal( Enums::PROTOCOL_ID protocol_id );

            /**
             * Creates a new Proposal
             * @param protocol_id Protocol id
             * @param spi SPI value
             */
            Proposal( Enums::PROTOCOL_ID protocol_id, auto_ptr<ByteArray> spi );

            /**
             * Creates a new Proposal based on its binary representation.
             * @param byte_buffer Buffer with its read pointer at the "proposal length" field
             */
            static auto_ptr<Proposal> parse( ByteBuffer& byte_buffer );

            /**
             * Creates a new Proposal clonning this one
             * @return New clonned protocol
             */
            virtual auto_ptr<Proposal> clone() const;

            /**
             * Adds a Transform to the Proposal object.
             * @param transform Transform to be added.
             */
            virtual void addTransform( auto_ptr<Transform> transform );

            /**
             * Gets the first Transform with indicated type from the Proposal.
             * @param type Transform type.
             * @return Transform with indicated type. NULL if it isn't found.
             */
            virtual Transform* getFirstTransformByType( Enums::TRANSFORM_TYPE type ) const;

            /**
             * Indicates if the Proposal has the indicated Transform
             * @param transform Searched transform
             * @return TRUE if the Proposal has the Transform. FALSE otherwise
             */
            virtual bool hasTransform( const Transform& transform ) const;

            /**
             * Indicates if the Proposals has the same transform types than other
             * @param other Other proposal
             * @return TRUE if both have the same transform types. FALSE otherwise
             */
            virtual bool hasTheSameTransformTypes( const Proposal& other ) const;

            /**
             * Detele all the transforms with the specified type
             * @param type Transform type.
             */
            virtual void deleteTransformsByType( Enums::TRANSFORM_TYPE type );

            /**
             * Sets the SPI value when the protocol is IKE
             * @param spi SPI value for the IKE protocol (in network order)
             */
            virtual void setIkeSpi( uint64_t spi );

            /**
             * Sets the SPI value when the protocol is AH or ESP
             * @param spi SPI value for the IPsec protocol (in network order)
             */
            virtual void setIpsecSpi( uint32_t spi );

            /**
             * Gets the SPI value when the protocol is IKE
             * @return SPI value (in network order)
             */
            virtual uint64_t getIkeSpi();

            /**
             * Gets the SPI value when the protocol is AH or ESP
             * @return SPI value (in network order)
             */
            virtual uint32_t getIpsecSpi();

            /**
             * Calculates the intersection between two proposals.
             * The order will be the same as in the proposal1
             * The protocol_id and the SPI will be the same of proposal1.
             * @param proposal1 One Proposal
             * @param proposal2 Other Proposal
             * @return The insertection Proposal
             */
            static auto_ptr<Proposal> intersection( const Proposal& proposal1, const Proposal& proposal2 );

            /**
             * Appends the binary representation of the Proposal in the ByteBuffer
             * @param byte_buffer ByteBuffer where append the binary representation
             */
            virtual void getBinaryRepresentation( ByteBuffer& byte_buffer ) const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~Proposal();
    };
}
#endif
