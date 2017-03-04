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
#ifndef OPENIKEV2IKESACONTROLLERIMPL_H
#define OPENIKEV2IKESACONTROLLERIMPL_H

#include "ikesa.h"

namespace openikev2 {

    /**
     This abstract class represents an IkeSaController, that manages all the IkeSa objects
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class IkeSaControllerImpl {

            /****************************** METHODS ******************************/
        public:
            /**
            * Increments the half-opened IKE SAs counter.
            */
            virtual void incHalfOpenCounter() = 0;

            /**
            * Decrements the half-opened IKE SAs counter.
            */
            virtual void decHalfOpenCounter() = 0;

            /**
             * Indicates the the cookie mechanism must be used with the current number of half-opened IKE_SA
             * @return TRUE if cookie mechanism must be used. FALSE otherwise
             */
            virtual bool useCookies() = 0;

            /**
             * Gets the next SPI value
             * @return SPI value
             */
            virtual uint64_t nextSpi() = 0;

            /**
             * Adds an IkeSa to the collection
             * @param ike_sa IkeSa to be added
             */
            virtual void addIkeSa( auto_ptr<IkeSa> ike_sa ) = 0;

            /**
            * Request the creation of an CHILD_SA. If exists a suittable IKE_SA between the indicated addresses, then uses it. Otherwise, a new IKE_SA
            * will be created.
            * @param ike_sa_src_addr Source address for the IKE_SA
            * @param ike_sa_dst_addr Destination address for the IKE_SA
            * @param mode IPsec mode
            * @param ipsec_protocol IPsec protocol
            * @param my_traffic_selector My traffic selector
            * @param peer_traffic_selector Peer traffic selector
            */
            virtual void requestChildSa( IpAddress& ike_sa_src_addr, IpAddress& ike_sa_dst_addr, auto_ptr<ChildSaRequest> child_sa_request) = 0;

            virtual void requestChildSaMobility( IpAddress& ike_sa_src_addr, IpAddress& ike_sa_dst_addr, auto_ptr<ChildSaRequest> child_sa_request, IpAddress& ike_sa_coa_addr, bool is_ha) = 0;


            /**
             * Push a Command into an IkeSa by IKE SPI value
             * @param spi IkeSa SPI value
             * @param command The Command to be pushed
             * @param priority The priority. TRUE if it must be inserted in the front and FALSE if it must be inserted in the back
             * @return TRUE if the IKE_SA with the indicated SPI exists. FALSE otherwise
             */
            virtual bool pushCommandByIkeSaSpi( uint64_t spi, auto_ptr<Command> command, bool priority ) = 0;

            virtual IkeSa* getIkeSaByIkeSaSpi( uint64_t spi ) = 0;


            /**
             * Push a command into an IKE_SA by CHILD_SA SPI value
             * @param spi CHILD_SA SPI value
             * @param command The Command to be pushed
             * @param priority The priority. TRUE if it must be inserted in the front and FALSE if it must be inserted in the back
             * @return TRUE if the IKE_SA with the indicated CHILD_SA SPI exists. FALSE otherwise
             */
            virtual bool pushCommandByChildSaSpi( uint32_t spi, auto_ptr<Command> command, bool priority ) = 0;

            virtual ~IkeSaControllerImpl();
    };

}

#endif
