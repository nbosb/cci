//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __GS_CCI_PARAM_BASE_H__
#define __GS_CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

#include "cci.h"
#include "greencontrol/gcnf/apis/gs_param/gs_param.h"

namespace cci {

  template <typename T>
  class gs_cci_param_t
  : public cci_param<T>
  , public gs::gs_param<T>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param_t<T> my_type;
    
  public:

    using cci_param<T>::operator=;

    explicit gs_cci_param_t(const std::string& n, 
                            const val_type &val, 
                            const bool force_top_level_name = false,
                            const bool register_at_db = true)
    : gs::gs_param<val_type>(n, val, NULL, force_top_level_name, register_at_db) {
    }

    explicit gs_cci_param_t(const std::string& n, 
                            const bool force_top_level_name = false,
                            const bool register_at_db = true)
    : gs::gs_param<val_type>(n, std::string(""), NULL, force_top_level_name, register_at_db) {
    }
    
    ~gs_cci_param_t() {
    }
    
    /*void init(const val_type& default_val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    void init() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }*/
    
    const std::string& get_name() const {
      return gs::gs_param<T>::getName();
    }
   
    const bool is_destructing() const {
      return gs::gs_param<T>::is_destucting();
    }

    void set(const val_type& val) {
      gs::gs_param<T>::setValue(val);
    }
    
    const val_type& get() const {
      return gs::gs_param<T>::getValue();
    }
    
    bool get_string(std::string& retvalue) {
      retvalue = gs::gs_param<T>::getString();
      return true;
    }
    
    boost::shared_ptr<callb_adapt_b> register_callback(boost::shared_ptr< callb_adapt_b> callb) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return callb;
    }
    
    void unregister_all_callbacks(void* observer) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    bool unregister_param_callback(callb_adapt_b* callb)  {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }
    
    bool has_callbacks() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
    }
        
  };

} // namespace cci

#endif