// $Id: identidy_node.h,v 1.1 2017/03/23 16:04:24 ist181423 Exp $
#ifndef __XPL_IDENTIDYNODE_H__
#define __XPL_IDENTIDYNODE_H__

#include <cdk/ast/unary_expression_node.h>
#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing ('+') operator.
   */
  class identidy_node: public cdk::unary_expression_node {

  public:
    inline identidy_node(int lineno, cdk::expression_node *expression) :
        cdk::unary_expression_node(lineno, expression) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identidy_node(this, level);
    }

  };

} // xpl

#endif
