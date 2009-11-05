//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

#ifndef __globals_h__
#define __globals_h__

// Must be included BEFORE SystemC because SystemC 2.1
// has a buggy boost implementation included!!!!
#include <boost/tokenizer.hpp> // for parameter array!

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
  #define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include <systemc>


//#define GC_VERBOSE
//#define GCNF_VERBOSE

#define SHOW_REGRESSION_TEST_RESULTS_ON

#ifdef SHOW_REGRESSION_TEST_RESULTS_ON
# define REGR_TEST(string) { std::cout <<"------------------------------------------------------------------------------------------"<<std::endl<<"    *** Regression test result: ***"<< std::endl<<"   @" << sc_core::sc_time_stamp() << " /" << sc_core::sc_delta_count() << " ("<<this->name() << "): " << string <<std::endl<<"------------------------------------------------------------------------------------------"<< std::endl; }
#else
# define REGR_TEST(string)
#endif


#endif