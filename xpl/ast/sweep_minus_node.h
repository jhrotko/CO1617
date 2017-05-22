// $Id: sweep_minus_node.h,v 1.3 2017/04/21 00:10:53 ist181423 Exp $
#ifndef __XPL_SWEEPMINUSNODE_H__
#define __XPL_SWEEPMINUSNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>
#include <cdk/ast/lvalue_node.h>

namespace xpl {

  /**
   * Class for describing sweet minus node.
   */
  class sweep_minus_node: public cdk::basic_node {
   cdk::lvalue_node *_leftValue;
   cdk::expression_node *_initialValue, *_condition, *_increment; 
   cdk::basic_node *_block; 

  public:
    inline sweep_minus_node(int lineno, cdk::lvalue_node *leftValue, 
                          cdk::expression_node *initialValue, cdk::expression_node *condition,
                          cdk::expression_node *increment, cdk::basic_node *block) :

        cdk::basic_node(lineno), _leftValue(leftValue), _initialValue(initialValue), 
        _condition(condition), _increment(increment), _block(block) {
    }

  public:
    inline cdk::lvalue_node *leftValue() {
      return _leftValue;
    }

    inline cdk::expression_node *initialValue() {
      return _initialValue;
    }
    
    inline cdk::expression_node *condition() {
      return _condition;
    }

    inline cdk::expression_node *increment() {
      return _increment;
    }

    inline cdk::basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sweep_minus_node(this, level);
    }

  };

} // xpl

#endif
