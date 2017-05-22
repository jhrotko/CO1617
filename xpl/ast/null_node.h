// $Id: null_node.h,v 1.4 2017/04/13 19:52:18 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_NULLNODE_H__
#define __XPL_NULLNODE_H__

#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing null nodes.
   */
  class null_node: public cdk::expression_node {

  public:
    inline null_node(int lineno) :
        cdk::expression_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_null_node(this, level);
    }

  };

} // xpl

#endif