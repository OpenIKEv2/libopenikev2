/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
*   Alejandro Perez Mendez     alex@um.es                                 *
*                                                                         *
*   This software may be modified and distributed under the terms         *
*   of the Apache license.  See the LICENSE file for details.             *
***************************************************************************/
#ifndef PEERCONFIGURATION_H
#define PEERCONFIGURATION_H

#include "ikesaconfiguration.h"
#include "childsaconfiguration.h"
#include "printable.h"
#include "autovector.h"
#include "networkprefix.h"

namespace openikev2 {

    /**
        This class represents a per-peer configuration
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class PeerConfiguration : public Printable {
            friend class Configuration;

            /****************************** ATTRIBUTES ******************************/
        protected:
            AutoVector<NetworkPrefix> network_prefixes;             /**< Collection of network prefixes having this configuration */
            Enums::ROLE_ID role;                                    /**< Configuration role. Only for initiators, responders or both. */
            auto_ptr<IkeSaConfiguration> ike_sa_configuration;      /**< IKE SA configuration */
            auto_ptr<ChildSaConfiguration> child_sa_configuration;  /**< CHILD SA configuration */

            /****************************** METHODS ******************************/
        public:
            /**
             * Creates a new Peer configuration with default values
             */
            PeerConfiguration();

            /**
             * Obtains the IkeSaConfiguration for this PeerConfiguration
             * @return The IkeSaConfiguration for this PeerConfiguration
             */
            virtual IkeSaConfiguration& getIkeSaConfiguration() const;

            /**
             * Obtains the ChildSaConfiguration for this PeerConfiguration
             * @return The ChildSaConfiguration for this PeerConfiguration
             */
            virtual ChildSaConfiguration& getChildSaConfiguration() const;

            /**
             * Sets the IkeSaConfiguration
             * @param  ike_sa_configuration IkeSaConfiguration
             */
            virtual void setIkeSaConfiguration( auto_ptr<IkeSaConfiguration> ike_sa_configuration);

            /**
             * Sets the ChildSaConfiguration
             * @param  ike_sa_configuration ChildSaConfiguration
             */
            virtual void setChildSaConfiguration( auto_ptr<ChildSaConfiguration> child_sa_configuration);

            /**
             * Sets the role
             * @param role Role
             */
            virtual void setRole(Enums::ROLE_ID role);

            /**
             * Gets the role
             * @return The Role
             */
            virtual Enums::ROLE_ID getRole() const;

            /**
             * Add a network prefix to the configuration
             * @param network_prefix NetworkPrefix to be added
             */
            virtual void addNetworkPrefix( auto_ptr<NetworkPrefix> network_prefix );

            /**
             * Remove a peer network prefix from the configuration
             * @param network_prefix NetworkPrefix to be deleted
             */
            virtual void deleteNetworkPrefix( const NetworkPrefix& network_prefix );

            /**
             * Indicates if this Peer Configuration contains an IP address
             * @param ip_address IP address
             * @return TRUE if has the IP Address. FALSE otherwise
             */
            virtual bool hasIpAddress( const IpAddress& ip_address ) const ;

            /**
             * Indicates if this peer configuration contains an Network Prefix
             * @param network_prefix Network Prefix
             * @return TRUE if has the Network Prefix. FALSE otherwise.
             */
            virtual bool hasNetworkPrefix (const NetworkPrefix& network_prefix) const;

            /**
             * Obtains the number of registered network prefixes
             * @return The nunber of registered network prefixes
             */
            virtual uint16_t getNetworkPrefixCount() const;

            /**
             * Returns a new PeerConfiguration object as a clone of this
             * @return A clone of this
             */
            virtual auto_ptr<PeerConfiguration> clone() const;

            virtual string toStringTab( uint8_t tabs ) const ;

            virtual ~PeerConfiguration();
    };
}
#endif
