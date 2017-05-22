// $Id: malloc_node.h,v 1.2 2017/05/09 17:06:42 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_MALLOCNODE_H__
#define __XPL_MALLOCNODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace xpl {

  /**
   * Class for describing malloc nodes; to reserve memory
   */
  class malloc_node: public cdk::unary_expression_node {

  public:
    inline malloc_node(int lineno, cdk::expression_node *toMemory) :
    cdk::unary_expression_node(lineno, toMemory) {}


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_malloc_node(this, level);
    }

  };

} // xpl

#endif