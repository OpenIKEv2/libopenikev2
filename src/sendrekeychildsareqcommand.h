/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDREKEYCHILDSAREQCOMMAND_H
#define OPENIKEV2SENDREKEYCHILDSAREQCOMMAND_H

#include "command.h"

namespace openikev2 {

    /**
        This class represents a Send Rekey CHILD_SA request Command
        @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendRekeyChildSaReqCommand : public Command {
            /****************************** ATTRIBUTES ******************************/
        protected:
            uint32_t rekeyed_spi;               /**< CHILD_SA SPI to be deleted */

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new SendRekeyChildSaReqCommand
             * @param rekeyed_spi CHILD_SA SPI to be deleted
             */
            SendRekeyChildSaReqCommand( uint32_t rekeyed_spi );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendRekeyChildSaReqCommand();
    };

}

#endif
