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
#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "networkcontrollerimpl.h"
#include "payload_conf.h"
#include "ikesa.h"
#include "payload_eap.h"


namespace openikev2 {

    /**
        This class allows to perform network operations using several concrete implementations
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alejandro_perez@dif.um.es, pedroj.fernandez@dif.um.es>
    */

    class NetworkController {

            /****************************** ATTRIBUTES ******************************/
        protected:
            static NetworkControllerImpl* implementation;       /**< Instance of the implementation of Network Controller */

            /****************************** METHODS ******************************/
        public:
            /**
             * Establish the NetworkController concrete implementation
             * @param impl concrete implementation
             */
            static void setImplementation( NetworkControllerImpl* impl );

            /**
             * Creates a new IpAddress, setting its family and the address data
             * @param family Address family
             * @param data Address data
             * @return The new IpAddress object
             */
            static auto_ptr<IpAddress> getIpAddress( Enums::ADDR_FAMILY, auto_ptr<ByteArray> data );
            static auto_ptr<IpAddress> getIpAddress( string address );
	    static auto_ptr<SocketAddress> getSocketAddress( string address, int port );
	    static auto_ptr<SocketAddress> getSocketAddress( auto_ptr<IpAddress> address, int port );
            static void refreshInterfaces();

            static IpAddress * getCurrentCoA( );
	    static IpAddress * getHoAbyCoA( const IpAddress& current_coa );

            /**
             * Creates the Configuration request payload
             * @param ike_sa IkeSa
             * @param ike_sa IkeSa
             * @return The Payload_CONF to be included in the request. NULL if no configuration will be requested.
             */
            static void createConfigurationRequest( Message& message, IkeSa& ike_sa );

            /**
             * Process the Configuration response payload
             * @param ike_sa IkeSa
             * @param child_sa Child SA
             * @param configuration_response Received Payload_CONF response
             */
            static IkeSa::NEGOTIATION_ACTION processConfigurationResponse( Message& message, IkeSa& ike_sa );
            
            /**
            * Processes a Configuration request payload
            * @param ike_sa IkeSa
            * @param child_sa ChildSa
            * @param configuration_request Received Payload_CONF request
            * @return The Payload_CONF to be included in the response. NULL if some error was found.
            */
            static IkeSa::NEGOTIATION_ACTION processConfigurationRequest( Message& message, IkeSa& ike_sa );

            /**
             * Creates the Configuration response
             * @param ike_sa IkeSa
             * @param child_sa ChildSa
             * @param configuration_request Received Payload_CONF request
             * @return The Payload_CONF to be included in the response. NULL if some error was found.
             */
            static void createConfigurationResponse( Message& message, IkeSa& ike_sa );

            /**
             * Adds notification requets to the message
             * @param message Message to add the notify request payloads
             * @param ike_sa IKE SA Controller
             * @param child_sa CHILD_SA related with the notification (NULL if there is no related CHILD_SA)
             */
            static void addNotifies( Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            /**
            * Gets the adecuated NotifyController for such notification type
            * @param type Notification type
            * @return NotifyController object or NULL if not found
            */
            static NotifyController* getNotifyController( uint16_t type );

            /**
             * Registers a NotifyController object for determinated notification type
             * @param type Notification type
             * @param notify_controller NotifyController object to be registered.
             */
            static void registerNotifyController( uint16_t type, auto_ptr<NotifyController> notify_controller );

            /**
             * Sends a Message to the peer thru the socket
             * @param message Message to be sent
             * @param cipher Cipher to be used
             */
            static void sendMessage( Message &message, Cipher* cipher );

            /**
             * Adds a new source address to receive from
             * @param new_src_address New source address
             */
            static void addSrcAddress( auto_ptr<IpAddress> new_src_address );

            /**
             * Removes a source address to receive from
             * @param src_address Source address to be removed
             */
            static void removeSrcAddress( const IpAddress& src_address );

            /**
             * Deletes the instance of the network controller implementation and set it to NULL
             */
            static void close();
    };
};
#endif
