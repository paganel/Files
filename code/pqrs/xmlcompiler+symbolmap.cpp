#include "pqrs/xmlcompiler.hpp"

namespace pqrs {
  xmlcompiler::symbolmap::symbolmap(void)
  {
    clear();
  }

  void
  xmlcompiler::symbolmap::clear(void)
  {
    symbolmap_.clear();
    add("ConfigIndex", "VK__AUTOINDEX__BEGIN__", 0);
  }

  boost::optional<uint32_t>
  xmlcompiler::symbolmap::get(const std::string& name)
  {
    auto it = symbolmap_.find(name);
    if (it == symbolmap_.end()) {
      return boost::none;
    }

    return it->second;
  }

  boost::optional<uint32_t>
  xmlcompiler::symbolmap::get(const std::string& type, const std::string& name)
  {
    return get(type + "::" + name);
  }

  bool
  xmlcompiler::symbolmap::add(const std::string& type, const std::string& name, uint32_t value)
  {
    auto n = type + "::" + name;
    if (get(n)) {
      return false;
    }

    symbolmap_[n] = value;
    return true;
  }

  bool
  xmlcompiler::symbolmap::add(const std::string& type, const std::string& name)
  {
    auto n = type + "::VK__AUTOINDEX__BEGIN__";
    auto v = get(n);
    if (! v) {
      throw xmlcompiler_logic_error("VK__AUTOINDEX__BEGIN__ is not found.");
      return false;
    }
    symbolmap_[n] = *v + 1;
    return add(type, name, *v);
  }

  // ============================================================
  bool
  xmlcompiler::reload_symbolmap_(void)
  {
    symbolmap_.clear();

    const char* xmlfilepath = "/Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources/symbolmap.xml";

    boost::property_tree::ptree pt;
    if (! pqrs::xmlcompiler::read_xml_(xmlfilepath, pt, true)) {
      return false;
    }

    return traverse_symbolmap_(pt);
  }

  bool
  xmlcompiler::traverse_symbolmap_(const boost::property_tree::ptree& pt)
  {
    for (auto it : pt) {
      if (it.first != "item") {
        traverse_symbolmap_(it.second);
      } else {
        auto value = pqrs::string::to_uint32_t(it.second.get<std::string>("<xmlattr>.value"));
        if (! value) {
          throw xmlcompiler_runtime_error("Invalid value in symbolmap.xml.");
        }
        if (! symbolmap_.add(it.second.get<std::string>("<xmlattr>.type"),
                             it.second.get<std::string>("<xmlattr>.name"),
                             *value)) {
          throw xmlcompiler_runtime_error("Failed to symbolmap_.add.");
        }
      }
    }
    return true;
  }
}