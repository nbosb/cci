/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

/**
 * @file     parameter_configurator.h
 * @brief    This header declares and defines configurator 
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * @date     18th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_CONFIGURATOR_H_

#include <cci>
#include <cassert>

#include "xreport.hpp"

/// This class either reads or sets values to the parameter-owner's
/// parameters initialization
SC_MODULE(ex17_parameter_configurator) {
 public:
  SC_CTOR(ex17_parameter_configurator) {
    /// Get reference of the responsible broker for the module
    myCfgrBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    assert(myCfgrBrokerIF != NULL && "Parameter-Setter handle is NULL");

    // Get handle of the 'int_param' cci-parameter
    if (myCfgrBrokerIF->param_exists("param_owner.mutable_int_param")) {
      XREPORT("[CFGR C_TOR] : Integer parameter exists");

      int_param_ptr = myCfgrBrokerIF->get_param("param_owner.mutable_int_param");

      assert(int_param_ptr != NULL && "Parameter Handle is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : Integer parameter doesn't exists.");
    }

    // Get handle of the 'float_param' cci-parameter
    if (myCfgrBrokerIF->param_exists("param_owner.mutable_float_param")) {
      XREPORT("[CFGR C_TOR] : Float parameter exists");

      float_param_ptr = myCfgrBrokerIF->get_param("param_owner.mutable_float_param");

      assert(float_param_ptr != NULL && "Parameter Handle is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : Float parameter doesn't exists.");
    }

    // Get handle of the 'string_param' cci-parameter
    if (myCfgrBrokerIF->param_exists("param_owner.mutable_string_param")) {
      XREPORT("[CFGR C_TOR] : String parameter exists");

      str_param_ptr = myCfgrBrokerIF->get_param("param_owner.mutable_string_param");

      assert(str_param_ptr != NULL && "Parameter Handle is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : String parameter doesn't exists.");
    }

    /// Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
  }

  /// SC_THREAD implementation
  void run_mutable_cfgr(void) {
    while (1) {
      /// Set value to the 'integer' parameter of the owner module
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Set integer parameter value to '20'"
              " using cci_base_parameter");
      int_param_ptr->json_deserialize("20");

      /// Set value to the 'string' parameter of the owner module
      XREPORT("[CFGR] : Set string  parameter value to 'configure'"
              " using cci_base_parameter");
      str_param_ptr->json_deserialize("\"configure\"");

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  /// Declare a configuration broker
  cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

  /// Declare cci_base_param for each of the owner's cci-parameters
  cci::cnf::cci_base_param* int_param_ptr;
  cci::cnf::cci_base_param* float_param_ptr;
  cci::cnf::cci_base_param* str_param_ptr;
};
/// ex17_parameter_configurator

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_CONFIGURATOR_H_