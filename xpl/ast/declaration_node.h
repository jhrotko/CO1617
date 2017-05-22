// $Id: declaration_node.h,v 1.5 2017/04/21 00:10:53 ist181423 Exp $
#ifndef __XPL_DECLARATIONNODE_H__
#define __XPL_DECLARATIONNODE_H__

#include <string>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing declaration nodes.
   */
  class declaration_node: public cdk::basic_node {
    std::string *_name, *_qualifier;
    basic_type *_type; 
    cdk::expression_node *_expression;

  public:
    inline declaration_node(int lineno, std::string *name,
            std::string *qualifier, basic_type *type, 
            cdk::expression_node *expression) :
        cdk::basic_node(lineno), _name(name), _qualifier(qualifier), 
        _type(type), _expression(expression) {
    }

  public:
    inline std::string *name() {
      return _name;
    }

  public:
    inline std::string *qualifier() {
      return _qualifier;
    }

  public:
    inline basic_type *type() {
      return _type;
    }

    public:
    inline cdk::expression_node *expression() {
      return _expression;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_declaration_node(this, level);
    }

  };

} // xpl

#endif
