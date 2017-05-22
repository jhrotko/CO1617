// $Id: index_node.h,v 1.2 2017/03/23 16:04:24 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_INDEXNODE_H__
#define __XPL_INDEXNODE_H__

#include <cdk/ast/lvalue_node.h>
#include <cdk/ast/expression_node.h>

namespace xpl {

  /**
   * Class for describing expression nodes.
   */
  class index_node: public cdk::lvalue_node {
    cdk::expression_node *_base, *_offset;

  public:
    inline index_node(int lineno, cdk::expression_node *base, cdk::expression_node *offset) :

        cdk::lvalue_node(lineno), _base(base), _offset(offset) {
    }

  public:
  	inline cdk::expression_node *base(){
  		return _base;
  	}

  public:
  	inline cdk::expression_node *offset() {
  		return _offset;
  	}

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };

} // xpl

#endif
