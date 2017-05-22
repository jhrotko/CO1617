// $Id: address_node.h,v 1.2 2017/03/23 16:04:24 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_ADDRESSNODE_H__
#define __XPL_ADDRESSNODE_H__

#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing address nodes.
   */
  class address_node: public cdk::unary_expression_node {

  public:
    inline address_node(int lineno, cdk::lvalue_node *lvalue) :
        cdk::unary_expression_node(lineno, lvalue) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_address_node(this, level);
    }

  };

} // xpl

#endif