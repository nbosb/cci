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
 *  @brief    This file takes the references of the owner's parameters using
 *           cci_base_param and sets values to the owner's parameters at various
 *           timing points 
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     9th June, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAMETER_CONFIGURATOR_H_

#include <cci>
#include <cassert>
#include <vector>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex11_parameter_configurator
 *  @brief  The configurator module searches for the owner parameters using the
 *          get_param API. Implementation using 'get_param_list' API is ignored though
 *          the parameters are being searched as per the string pattern provided.
 */
SC_MODULE(ex11_parameter_configurator) {
 public:
  /**
   *  @fn     ex11_parameter_configurator()
   *  @brief  The class constructor
   */
  SC_CTOR(ex11_parameter_configurator) {
    // Get handle of the broker responsible for the class/module
    myCfgrBrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    // Report if handle returned is NULL
    assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");

    // Hierarchical names for the cci_parameters of the owner modules
    std::string cfgr_param_str1 = "top_mod.param_owner1.clk_freq_Hz";
    std::string cfgr_param_str2 = "top_mod.param_owner2.clock_speed_Hz";

    // Check for the existence of 'clk_freq_Hz' cci_parameter of owner module 1
    if (myCfgrBrokerIF->param_exists(cfgr_param_str1)) {
      cfgr_param_ptr1 = myCfgrBrokerIF->get_param(cfgr_param_str1);

      assert(cfgr_param_ptr1 != NULL
             && "Configuration parameter returned is NULL");

      XREPORT("[CFGR C_TOR] : Parameter Name : " << cfgr_param_ptr1->get_name()
              << "\tParameter Value : " << cfgr_param_ptr1->json_serialize());
    } else {
      XREPORT("[CFGR C_TOR] : Parameter " << cfgr_param_str1
              << "\tdoesn't exists in top_module");
    }

    // Check for the existence of 'clock_speed_Hz' cci_parameter of owner 2
    if (myCfgrBrokerIF->param_exists(cfgr_param_str2)) {
      cfgr_param_ptr2 = myCfgrBrokerIF->get_param(cfgr_param_str2);

      assert(cfgr_param_ptr2 != NULL
             && "Configuration parameter returned is NULL");

      XREPORT("[CFGR C_TOR] : Parameter Name : " << cfgr_param_ptr2->get_name()
              << "\tParameter Value : " << cfgr_param_ptr2->json_serialize());
    } else {
      XREPORT("[CFGR C_TOR] : Parameter " << cfgr_param_str1
              << "\tdoesn't exists in top_module");
    }

    // Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_cfgr);
  }

  /**
   *  @fn     void before_end_of_elaboration(void)
   *  @brief  The value of cci_parameter of owner(1) is modified and the updated
   *          values of owners are read
   *  @return void
   */
  void before_end_of_elaboration(void) {
    // Change the value of the cci_parameter 'clk_freq_Hz' of OWNER (1)
    // to '5000' (Hz)
    if (cfgr_param_ptr1 != NULL) {
      XREPORT("[CFGR within beoe] Within the BEOE phase");
      XREPORT("[CFGR within beoe] : Changing the 'clk_freq_Hz' of OWNER (1)"
              " to 5000 (Hz).");
      cfgr_param_ptr1->json_deserialize("5000");
    }

    XREPORT("[CFGR within beoe] : Parameter Name : "
            << cfgr_param_ptr1->get_name() << "\tParameter Value : "
            << cfgr_param_ptr1->json_serialize());

    XREPORT("[CFGR within beoe] : Parameter Name : "
            << cfgr_param_ptr2->get_name() << "\tParameter Value : "
            << cfgr_param_ptr2->json_serialize());
  }

  /**
   *  @fn     void run_cfgr(void)
   *  @brief  The value of the cci_parameter of owner(2) is modified and the
   *          updated values of both owners are read
   *  @return void
   */
  void run_cfgr(void) {
    while (1) {
      // Change the value of the cci_parameter 'clock_speed_Hz' of
      // OWNER (2) to '12000' (Hz)
      if (cfgr_param_ptr2 != NULL) {
        XREPORT("@ " << sc_core::sc_time_stamp());
        XREPORT("[CFGR] : Changing the 'clock_speed_Hz' of OWNER (2)"
                " to 12000 (Hz).");
        cfgr_param_ptr2->json_deserialize("12000");
      }

      XREPORT("[CFGR] : Parameter Name : " << cfgr_param_ptr1->get_name()
              << "\tParameter Value : " << cfgr_param_ptr1->json_serialize());

      XREPORT("[CFGR] : Parameter Name : " << cfgr_param_ptr2->get_name()
              << "\tParameter Value : " << cfgr_param_ptr2->json_serialize());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;  ///< Declaring a CCI configuration broker interface instance

  // CCI base parameters
  cci::cnf::cci_base_param* cfgr_param_ptr1;  ///< CCI base parameter
  cci::cnf::cci_base_param* cfgr_param_ptr2;  ///< CCI base parameter
};
// ex11_parameter_configurator

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAMETER_CONFIGURATOR_H_

