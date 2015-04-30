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
 *  @file    parameter_owner.h
 *  @brief   This header instantiates various CCI parameters with default values
 *  @author  Girish Verma, CircuitSutra Technologies    <girish@circuitsutra.com>
 *           P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 *  @date    31st August, 2011 (Wednesday)
 */

#ifndef   EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_
#define   EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_

#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <cci>
#include <systemc.h>
#include <cci_cnf/cci_shared_ptr.h>  // HACK; a proper publicly shareable CCI wrapper needed
#include <greencontrol/config.h>
#include <string>
#include <iostream>

/**
 *  @class  route_table_ut
 *  @brief  Route_table_ut structure defines three fields of a typical router
 *          source_addr, dest_addr, and index
 */
struct route_table_ut {
 public:
  /**
   *  @fn     route_table_ut()
   *  @brief  The class constructor
   *  @return void
   */
  route_table_ut()
      : s_address(0x0),  // Source Address
        d_address(0x0),  // Destination Address
        index(0x0) {}    // Index

  /**
   *  @fn     route_table_ut(int saddr, int daddr, int idx)
   *  @brief  Overloaded class constructor
   *  @param  saddr The source address
   *  @param  daddr The destination address
   *  @param  idx The index
   *  @return void
   */
  route_table_ut(int saddr, int daddr, int idx)
      : s_address(saddr),  // Source Address
        d_address(daddr),  // Destination Address
        index(idx) {}      // Index

 public:
  int s_address;  ///< Source address field
  int d_address;  ///< Destination address field
  int index;  ///< Index field
};

namespace cci { namespace cnf {
// add support for cci_value and JSON (de)serialization
template<>
struct cci_value_traits< route_table_ut >
{
  typedef route_table_ut type;
  static bool pack( cci_value::reference dst, type const & src )
  {
    dst.set_map()
      .push_entry( "s_address", src.s_address )
      .push_entry( "d_address", src.d_address )
      .push_entry( "index"    , src.index );
    return true;
  }
  static bool unpack( type & dst, cci_value::const_reference src )
  {
    if( !src.is_map() )
      return false;

    cci_value::const_map_reference m = src.get_map();
    if( m.has_entry("s_address")
               && m["s_address"].try_get( dst.s_address )
     && m.has_entry("d_address")
               && m["d_address"].try_get( dst.d_address )
     && m.has_entry("index")
               && m["index"].try_get( dst.index ) )
       return true;

    return false;
  }
};
} /* namespace cnf */ } /* namespace cci */

// Overload stream insertion operator of C++
std::ostream& operator <<(std::ostream& os, const route_table_ut& ud)
{
  cci::cnf::cci_value udv(ud);
  return os << udv;
}

// Overload stream extraction operator of C++
/** @todo This operator is currently required by the "GreenControl tool"
 *        implementation.  Its serialisation is not yet based on JSON but
 *        uses plain stream extraction to convert a value from a string.
 */
std::istream& operator >>(std::istream& is, route_table_ut& ud)
{
  cci::cnf::cci_value udv;
  is >> udv;
  sc_assert( is && "stream extraction of cci_value failed");
  bool check = udv.try_get(ud);
  sc_assert( check && "stream extraction of route_table_ut failed");
  return is;
}

//!< Provide parameter vendor implementation support for user-defined data type
typedef gs::cnf::gs_param<route_table_ut> user_data_type;


// ----------------------------------------------------------------------------
// copied from gs_cci_symbols.cpp

#include "cci_params.h"

namespace cci { namespace cnf {
#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, MUTABILITY_TYPE) \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam,                   const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const PTYPE& val, const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template  cci::cnf::cci_param_impl_if<PTYPE, MUTABILITY_TYPE>*  create_cci_param (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par, const std::string &nam, const char* val,  const bool is_top_level_name, cci_cnf_broker_if* broker_accessor); \
  template                                                  void  init_cci_param   (cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE> *owner_par); \
  template                                                  void  destroy_cci_param(cci::cnf::cci_param<PTYPE, MUTABILITY_TYPE>* param    );

#define CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(PTYPE) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::mutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::immutable_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::elaboration_time_param) \
  CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM(PTYPE, cci::cnf::other_param)


CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS(route_table_ut);

#undef CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS
#undef CCI_IMPL_CREATE_SYMBOLS_FOR_FUNCTIONS_T_TM

} /* namespace cnf */ } /* namespace cci */

// ----------------------------------------------------------------------------

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_USER_DATATYPE_H_
