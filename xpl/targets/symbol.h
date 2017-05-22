#ifndef __XPL_SEMANTICS_SYMBOL_H__
#define __XPL_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace xpl {

    class symbol {
      basic_type *_type;
      std::string _name;
      long _value; // hack!
      std::vector<basic_type*> _argumentTypes;
      std::string _qualifier;
      bool _isDefined;
      bool _isGlobal;
      int _offset;

    public:
      inline symbol(basic_type *returnType, std::vector<basic_type*> types, const std::string &name, const std::string &qualifier, bool defined) :
        _type(returnType), _argumentTypes(types), _name(name), _qualifier(qualifier), _isDefined(defined) {
      }

      inline symbol(basic_type *type, const std::string &name, long value) :
        _type(type), _name(name), _value(value) {
      }

      inline symbol(basic_type *type, const std::string &name, int offset) :
        _type(type), _name(name), _offset(offset) {
      }
      
      virtual ~symbol() {
        delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }

      inline const std::string &name() const {
        return _name;
      }
      inline long value() const {
        return _value;
      }
      inline long value(long v) {
        return _value = v;
      }

      inline bool defined() {
        return _isDefined;
      }

      inline bool isGlobal() {
        return _isGlobal;
      }

      inline void isGlobal(bool global) {
        _isGlobal = global;
      }

      inline std::vector<basic_type*> argsType() {
        return _argumentTypes;
      }

      inline const std::string &qualifier() {
        return _qualifier;
      }

      inline void define(bool d) {
        _isDefined = true;
      }

      inline void offset(int off) {
        _offset = off;
      }

      inline int offset() {
        return _offset;
      }
    };

} // xpl

#endif
