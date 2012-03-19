#ifndef PQRS_XMLCOMPILER_HPP
#define PQRS_XMLCOMPILER_HPP

#include <vector>
#include <tr1/memory>
#include <tr1/unordered_map>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include "pqrs/string.hpp"

namespace pqrs {
  class xmlcompiler {
  public:
    xmlcompiler(void);

    bool reload(void);

    class symbolmap_keycode {
    public:
      void clear(void);

      boost::optional<unsigned int> get(const std::string& name);
      boost::optional<unsigned int> get(const std::string& type, const std::string& name);

      // Call append("KeyCode", "RETURN", 36) to register "KeyCode::RETURN = 36".
      bool append(const std::string& type, const std::string& name, unsigned int value);
      bool append(const std::string& type, const std::string& name);

    private:
      std::tr1::unordered_map<std::string, unsigned int> symbolmap_;
    };

    class appdef {
    public:
      const std::string& get_name(void) const { return name_; }
      void set_name(const std::string& v) { name_ = v; }
      void add_rule_equal(const std::string& v);
      void add_rule_prefix(const std::string& v);
      bool is_rules_matched(const std::string& identifier);

    private:
      std::string name_;
      std::vector<std::string> rules_equal_;
      std::vector<std::string> rules_prefix_;
    };

  private:
    bool read_xml_(const char* xmlfilepath, boost::property_tree::ptree& pt, bool with_replacement);

    void normalize_identifier(std::string& identifier);

    bool reload_replacementdef_(void);
    void traverse_replacementdef_(const boost::property_tree::ptree& pt);

    bool reload_appdef_(void);
    void traverse_appdef_(const boost::property_tree::ptree& pt);

    std::string errormessage_;
    symbolmap_keycode symbolmap_keycode_;
    pqrs::string::replacement replacement_;

    std::vector<std::tr1::shared_ptr<appdef> > app_;
  };
}

#endif