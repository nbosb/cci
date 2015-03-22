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
 * @file      parameter_owner.h
 * @brief     This file defines the sc_module which is the owner class of a
 *            cci- parameter
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      10th May, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_OWNER_H_
#define EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_OWNER_H_

#include <cci>
#include "xreport.hpp"

/// This module declares an integer parameter and also illustrates usage of CCI
/// infrastructure in locking/unlocking of the parameter
SC_MODULE(ex13_parameter_owner) {
 public:
  SC_CTOR(ex13_parameter_owner)
     : int_param("mutable_int_param", 0) {
    XREPORT("[OWNER C_TOR] : Parameter locking status : "
            << int_param.is_locked() << "\tValue : " << int_param.get());

    /// SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  /// This process demonstrates various locking/unlocking APIs
  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[OWNER] : Locking the parameter without password");
      int_param.lock();
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get());

      /// Illustrating usage of try and catch mechanism in order to catch the
      /// exception raised when trying to set a value to the parameter if it
      /// is already locked
      try {
        XREPORT("[OWNER] : Try to set parameter value to 1 from 0");
        int_param = 1;
      } catch (sc_core::sc_report e) {
        XREPORT("[OWNER] : " << name()
                << "\tFailed to set parameter value to 1 from 0.");
        XREPORT("[OWNER] : Caught Exception : " << e.what());
      }

      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Unlocking parameter (locked earlier) without"
              " password");
      int_param.unlock();
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get());
      key = NULL;
      XREPORT("[OWNER] : Directly lock the unlocked parameter with password");
      int_param.lock(&key);

      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get());

      /// This 'set' API llustrating assigning new value to an already locked
      /// parameter without unlocking it
      XREPORT("[OWNER] : Overwrite value of the (locked!) parameter to 4"
              " without unlocking");
      int_param.set(4, &key);
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get());

      wait(30.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cnf::cci_param<int> int_param;

  // Arbitrary password to lock the parameter with
  void* key;
};
// ex13_parameter_owner

#endif  // EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_OWNER_H_