/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

/**
 *  @file     parameter_configurator.h
 *  @brief    This file declares and defines configurator and implements the
 *            locking and unlocking mechnanisms on the owner parameter
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     10th May, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_CONFIGURATOR_H_

#include <cci>
#include <cassert>
#include "xreport.hpp"

/**
 *  @class  ex13_parameter_configurator
 *  @brief  This module lists and defines the locking/unlocking mechanism by a
 *          parameter-setter (configurator)
 */
SC_MODULE(ex13_parameter_configurator) {
 public:
  /**
   *  @fn     ex13_parameter_configurator
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex13_parameter_configurator) {
    // Getting handle of the default broker for the class/sc_module
    myConfigBroker =
        &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    // Assert if broker handle returned is NULL
    assert(myConfigBroker != NULL && "Broker handle returned is NULL");

    if (myConfigBroker->param_exists("param_owner.mutable_int_param")) {
      // Getting handle for the integer parameter of onwer module
      // by the configurator
      int_param_ptr = myConfigBroker->get_param("param_owner.mutable_int_param");

      assert(int_param_ptr != NULL && "Base parameter handle returned NULL");
    } else {
      XREPORT("[CFGR C_TOR] : int_param not found");
    }

    // Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
}

  /**
   *  @fn     void run_mutable_cfgr(void)
   *  @brief  This function illustrates various locking and unlocking mechanisms
   *          of a parameter using 'lock()', 'lock(&password)', 'unlock()',
   *          'unlock(&password)' APIs. 'is_locked() is used to query the locking status
   *  @return void
   */
  void run_mutable_cfgr(void) {
    while (1) {
      wait(2.0, sc_core::SC_NS);

      // Before configurator tries to assign a new value to the parameter, it
      // is always recommended to check the locking status of the parameter
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      // 'locked' API returns TRUE when a parameter is in LOCKED STATE and
      // FALSE when in UNLOCKED STATE
      if (int_param_ptr->is_locked()) {
        XREPORT("[CFGR] :  Cannot assign new value to the parameter as it"
                " is already locked!!");
      } else {
        int_param_ptr->json_deserialize("2");
      }

      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());
      XREPORT("[CFGR] : Parameter is not locked!! Assign a new value '3'"
              " to it");
      int_param_ptr->json_deserialize("3");
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      // Demonstrating 'lock' API to lock a parameter without a password
      XREPORT("[CFGR] : Lock parameter without password");
      int_param_ptr->lock();
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      // Demonstrating 'unlock' API to lock a parameter without a password
      XREPORT("[CFGR] : Unlock parameter without password");
      int_param_ptr->unlock();
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      // Demonstrating 'lock' API to lock a parameter with a password
      // 'Lock' API returns TRUE if parameter is locked successfully and
      // FALSE if it could not be locked
      psswd = NULL;
      XREPORT("[CFGR] : Lock with password");

      if (int_param_ptr->lock(&psswd)) {
        XREPORT("[CFGR] : Parameter locked with password (password not known"
                " to OWNER)");
      } else {
        XREPORT("[CFGR] : Could not lock the parameter.");
      }

      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      try {
        XREPORT("[CFGR] : Set parameter value to '4'");
        int_param_ptr->json_deserialize("4");
      } catch (sc_core::sc_report exception) {
        XREPORT("[CFGR] : Caught Exception : " << exception.what());
      }

      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      // 'Unlock' API returns TRUE if parameter is unlocked successfully and
      // FALSE if it could not be unlocked. Demonstrating unlocking with the
      // right password
      XREPORT("[CFGR] : Now, unlock with the correct password");
      int_param_ptr->unlock(&psswd);
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_ptr->is_locked() << "\tValue : "
              << int_param_ptr->json_serialize());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cnf::cci_cnf_broker_if* myConfigBroker;  ///< CCI configuration broker interface instance
  cci::cnf::cci_base_param* int_param_ptr;  ///< CCI base parameter handle to access the actual owner's parameter

  char* paramName;  ///< The parameter name

  // Some passwords local to the configurator
  void* psswd;  ///< local password
  void* newpsswd; ///< local password
};
// ex13_parameter_configurator

#endif  // EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_CONFIGURATOR_H_
