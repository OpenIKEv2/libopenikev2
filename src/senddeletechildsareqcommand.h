/***************************************************************************
 *   Copyright (C) 2005 by                                                 *
 *   Alejandro Perez Mendez     alex@um.es                                 *
 *   Pedro J. Fernandez Ruiz    pedroj@um.es                               *
 *                                                                         *
 *   This software may be modified and distributed under the terms         *
 *   of the Apache license.  See the LICENSE file for details.             *
 ***************************************************************************/
#ifndef OPENIKEV2SENDDELETECHILDSAREQCOMMAND_H
#define OPENIKEV2SENDDELETECHILDSAREQCOMMAND_H

#include "command.h"

namespace openikev2 {

    /**
        This class represents a Send DELETE CHILD_SA request Command
     @author Alejandro Perez Mendez, Pedro J. Fernandez Ruiz <alex@um.es, pedroj@um.es>
    */
    class SendDeleteChildSaReqCommand : public Command {

            /****************************** ATTRIBUTES ******************************/
        protected:
            uint32_t deleted_spi;                 /**< SPI of the CHILD_SA to be deleted */

            /****************************** METHODS ******************************/
        public:
            /**
             * Create a new SendDeleteChildSaReqCommand
             * @param deleted_spi SPI of the CHILD_SA to be deleted
             */
            SendDeleteChildSaReqCommand( uint32_t deleted_spi );

            virtual IkeSa::IKE_SA_ACTION executeCommand( IkeSa& ike_sa );
            virtual string getCommandName() const;

            virtual ~SendDeleteChildSaReqCommand();
    };

}

#endif
