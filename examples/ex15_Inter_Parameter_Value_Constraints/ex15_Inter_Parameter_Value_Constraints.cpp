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
 *  @file      main.cpp
 *  @brief     This file defines the 'sc_main' function
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      10th June, 2011 (Friday)
 */

#include "ex15_processor.h"
#include "ex15_configurator.h"

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  The testbench function that instantiates the processor and configurator modules
 *  @param  sc_argc The number of input arguments
 *  @param  sc_argv The list of input arguments
 *  @return An integer representing the execution status
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  SC_REPORT_INFO("sc_main", "[MAIN] : In this example, the following is"
                 " condition is verified");
  SC_REPORT_INFO("sc_main", "[MAIN] : \t\tx <= 2^n - 1");
  SC_REPORT_INFO("sc_main", "[MAIN] : where, 'x' : value of 'mem_block_size'"
                 " (Memory Block size), and");
  SC_REPORT_INFO("sc_main", "[MAIN] :        'n' : total number of address"
                 " lines - 'curr_addr_lines'");

  // Instantiation of the owner and configurator sc_modules
  ex15_processor processor("processor");
  ex15_configurator param_cfgr("param_cfgr");

  // Start and run the simulation till 50 nanoseconds
  sc_core::sc_start(15.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
