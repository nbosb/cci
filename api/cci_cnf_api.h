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


#ifndef __CCI_CNF_API_H__
#define __CCI_CNF_API_H__


#include <boost/shared_ptr.hpp>

#include "cci_param.h"
#include "cci_callbacks.h"


namespace cci {


  // forward declaration 
  class cci_param_base;
    
  template <class T> 
  class cci_param;

  
  /// CCI configuration API interface.
  class cci_cnf_api
  {
    
  public:
    
    // Destructor
    virtual ~cci_cnf_api() { }

    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////   Access Parameters and Values   /////////////////////// //

    
    /// Set a parameter's init value. 
    /**
     * The init value has priority to the initial value set by the owner!
     *
     * @param parname Full hierarchical parameter name.
     * @param value   String representation of the init value the parameter has to be set to.
     * @return        Success of the setting.
     */
    virtual bool set_init_value(const std::string &parname, const std::string &value) = 0;
    
    /// Get a parameter's value (string representation). Independent of the implicit or explicit status.
    /**
     * @param parname  Full hierarchical name of the parameter whose value should be returned.
     * @return  Value of the parameter, converted to the user-chosen type
     */
    virtual const std::string get_string(const std::string &parname) = 0;

    /// Get a parameter pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    virtual cci_param_base* get_param(const std::string &parname) = 0;
    
    /// Checks whether a parameter exists (implicit or explicit).
    /**
     * @param parname  Full hierarchical parameter name.
     * @return Whether the parameter <parname> exists in the ConfigPlugin.
     */
    virtual bool exists_param(const std::string &parname) = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // ////////////////   Get Parameter List   //////////////////////////// //

    
    /// Return a list of all parameters existing (TODO implicit and explicit?) in the registry.
    /**
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector<std::string> get_param_list() = 0;
    // also see optional functions!

    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////   Callback Handling   //////////////////////////// //
    
    
    /// Registers an observer callback function (with the signature of callback_func_ptr).
    /**
     * Same as @see cci::cci_param_base::register_callback but additional
     * - create param callbacks
     * - callbacks to not yet existing params
     * - (optionally) callbacks for regex names
     * 
     * This will forward callback registration to the parameter object.
     *
     * Usage example:
     * \code
     * class MyIP_Class
     * : public sc_core::sc_module
     * {
     * public:
     *   // some code [...]
     *   
     *   cci_param<int> my_param;
     *
     *   MyIP_Class(sc_core::sc_module_name name)
     *    : sc_core::sc_module(name),
     *      my_param("my_param", 10) 
     *   { //...
     *   }
     *
     *   // Example code to register callback function
     *   void main_action() {
     *     // some code, parameters etc...
     *
     *     my_param.register_callback(cci::post_write,  this, MyIP_Class::config_callback);
     *     //   equal to
     *     m_api.register_callback(cci::post_write   , my_param.get_name(), this, MyIP_Class::config_callback);
     *
     *     my_param.register_callback(cci::destroy_param, this, MyIP_Class::config_callback);
     *     //   equal to
     *     m_api.register_callback(cci::destroy_param, my_param.get_name(), this, MyIP_Class::config_callback);
     *
     *     m_api.register_callback(cci::create_param , "*"                , this, MyIP_Class::config_callback);
     *  OPTIONAL:
     *     m_api.register_callback(cci::post_write, "*.my_param"  , this, MyIP_Class::config_callback);
     *     m_api.register_callback(cci::post_write, "MyIP_Class.*", this, MyIP_Class::config_callback);
     *   }
     *
     *   // Callback function with default signature.
     *   void config_callback(cci_param_base& changed_param) {
     *     // some action
     *   }
     * };
     * \endcode
     *
     * @param type        Type of the callback.
     * @param parname     Parameter name or pattern the callback should be applied to.
     * @param observer    Pointer to the observing object (the one being called).
     * @param callb_func  Function pointer to the function being called.
     * @return            boost shared pointer to the callback adapter (e.g. to be used for unregister calls).
     */
    template <typename T>
    boost::shared_ptr<callb_adapt_b> register_callback(const callback_types type, const std::string& parname, T* observer, void (T::*callb_func_ptr)(cci_param_base& changed_param)) {
      // call the pure virtual function, independent from template T
      return register_callback(parname, boost::shared_ptr< callb_adapt_b>(new callb_adapt<T>(observer, callb_func_ptr, get_param(parname))));
    }
    
    /// Function handling the callback (without template)
    virtual boost::shared_ptr< callb_adapt_b> register_callback(const std::string& parname, boost::shared_ptr< callb_adapt_b> callb) = 0;

    
    /// Unregisters all callbacks (within all existing parameters) for the specified observer object (e.g. sc_module). 
    /**
     * @param observer   Pointer to the observer module who did register parameter callbacks.
     */
    virtual void unregister_all_callbacks(void* observer) = 0;
    
    // TODO: If we want to provide create_param callbacks for initial values 
    //       being set for not yet existing parameters (implicit parameters),
    //       we will need another register_callback()-function with a different 
    //       callback function signature getting <stringName,stringValue>-pair
    //       instead of cci_param_base.
    
    /// Unregisters the callback and (default) deletes the callback adapter.
    /**
     * Unregisters (only) the given callback.
     *
     * The callback adapter is NOT deleted because this should be done by the
     * surrounding shared pointer!
     *
     * This may be used by a user module which stored the shared pointer to a
     * specific callback or by the destructor of the param callback adapter.
     *
     * @param callb  Parameter callback adapter
     * @return       If the callback adapter existed in this parameter.
     */
    virtual bool unregisterParamCallback(callb_adapt_b* callb) = 0;
    
    /// Returns if the parameter has registered callbacks
    virtual bool has_callbacks(std::string& parname) = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Registry Functions   ///////////////////////////// //

    
  //protected:
  //  friend class cci_param_base;
    
    /// Add a parameter to the registry.
    /** 
     * Note: addPar (and all related methods) must not call any of the 
     *       pure virtual functions in cci_param_base because this method is 
     *       called by the cci_param_base constructor.
     *
     * @param par Parameter (including name and value).
     * @return Success of the adding.
     */
    virtual bool add_param(cci_param_base* par) = 0;
    
    /// Remove a parameter from the registry. Called by the parameter destructor.
    /**
     * Checks if destruction flag is set (so this may only be called by the
     * parameter destructor).
     *
     * @param par Parameter pointer.
     * @return Success of remove.
     */
    virtual bool remove_param(cci_param_base* par) = 0;
    

  public:
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Optional functions   ///////////////////////////// //

    
    /// Set an alias to a parameter name
    /**
     * TODO: Guideline for other functions, e.g.:
     * - get_param_list will NOT return any alias names
     * - all parameter access functions will take an alias
     *
     * @param orig_parname    Full hierarchical name of the original parameter (or another alias).
     * @param alias_parname   Full hierarchical (full user chosen) alias name.
     */
    virtual void set_alias(std::string& orig_parname, std::string& alias_parname) = 0;
        
    /// Convenience function to get a typed parameter pointer.
    /**
     * @param   parname   Full hierarchical parameter name.
     * @return  Pointer to the parameter object (NULL if not existing).
     */ 
    template<class T>
    cci_param<T>* get_cci_param(const std::string &parname) {
      return dynamic_cast<cci_param<T>*>(get_param(parname));
    };
    
    /// Return a list of all parameters (TODO implicit and explicit?) matching the pattern
    /**
     * TODO:
     * e.g.
     * - parameters of the specified module, pattern = module_name, e.g. "mymod.mysubmod";
     * - pattern = module_name+'.*' to include the parameters of the childs, e.g. "mymod.mysubmod.*"
     * - pattern = '*'.param_name to get all parameters with the name, e.g. "*.cache_size"
     * - pattern = '*'.hierarchical.param_name to get all parameters with the name, e.g. "*.mysubmod.cache_size"
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with full hierarchical parameter names.
     */
    virtual const std::vector<std::string> get_param_list(const std::string& pattern) = 0;
    
    /// Return a pointer list of all (explicit) parameters matching the pattern
    /**
     * pattern @see get_param_list
     *
     * @param pattern Specifies the parameters to be returned.
     * @return Vector with parameter base object pointers.
     */
    virtual const std::vector<cci_param_base*> get_params(const std::string& pattern = "") = 0;

  };

      
} // end namespace cci

#endif