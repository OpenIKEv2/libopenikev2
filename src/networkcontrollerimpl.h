/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef NETWORKCONTROLLERIMPL_H
#define NETWORKCONTROLLERIMPL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <map>

#include "ipaddress.h"
#include "payload_conf.h"
#include "childsa.h"
#include "ikesa.h"
#include "message.h"
#include "notifycontroller.h"

using namespace std;

namespace openikev2 {

    /**
        This abstract class represents a NetworkController concrete implementation
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class NetworkControllerImpl {

            /****************************** ATRIBUTES ******************************/
        protected:
            map<uint16_t, NotifyController*> notify_controllers;    /**< NotifyController collection */

            /****************************** METHODS ******************************/
        protected:
            NetworkControllerImpl();

        public:
            /**
             * Creates a new IpAddress, setting its family and the address data
             * @param family Address family
             * @param data Address data
             * @return The new IpAddress object
             */
            virtual auto_ptr<IpAddress> getIpAddress( Enums::ADDR_FAMILY family, auto_ptr<ByteArray> data) = 0;
            virtual auto_ptr<IpAddress> getIpAddress( string address ) = 0;
	    virtual auto_ptr<SocketAddress> getSocketAddress( string address, int port ) = 0;
	    virtual auto_ptr<SocketAddress> getSocketAddress( auto_ptr<IpAddress> address, int port ) = 0;
            virtual void refreshInterfaces() = 0;

            virtual IpAddress* getCurrentCoA( ) = 0;
            virtual IpAddress* getHoAbyCoA( const IpAddress& current_coa ) = 0;

            virtual void createConfigurationRequest( Message& message, IkeSa& ike_sa ) = 0;

            virtual IkeSa::NEGOTIATION_ACTION processConfigurationResponse( Message& message, IkeSa& ike_sa ) = 0;

            virtual IkeSa::NEGOTIATION_ACTION processConfigurationRequest( Message& message, IkeSa& ike_sa ) = 0;

            virtual void createConfigurationResponse( Message& message, IkeSa& ike_sa ) = 0;

            /**
             * Adds notification requets to the message
             * @param message Message to add the notify request payloads
             * @param ike_sa IKE SA Controller
             * @param child_sa CHILD_SA related with the notification (NULL if there is no related CHILD_SA)
             */
            virtual void addNotifies( Message& message, IkeSa& ike_sa, ChildSa* child_sa );

            /**
             * Gets the adecuated NotifyController for such notification type
             * @param type Notification type
             * @return NotifyController object or NULL if not found
             */
            virtual NotifyController* getNotifyController( uint16_t type );

            /**
             * Registers a NotifyController object for determinated notification type
             * @param type Notification type
             * @param notify_controller NotifyController object to be registered.
             */
            virtual void registerNotifyController( uint16_t type, auto_ptr<NotifyController> notify_controller );

            /**
             * Sends a Message to the peer thru the socket
             * @param message Message to be sent
             * @param cipher Cipher to be used
             */
            virtual void sendMessage( Message &message, Cipher* cipher ) = 0;

            /**
             * Adds a new source address to receive from
             * @param new_src_address New source address
             */
            virtual void addSrcAddress( auto_ptr<IpAddress> new_src_address ) = 0;

            /**
             * Removes a source address to receive from
             * @param src_address Source address to be removed
             */
            virtual void removeSrcAddress( const IpAddress& src_address ) = 0;


            virtual ~NetworkControllerImpl();
    };
}

#endif
