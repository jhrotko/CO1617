// $Id: return_node.h,v 1.2 2017/03/23 16:04:24 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_RETURNNODE_H__
#define __XPL_RETURNNODE_H__

#include <cdk/ast/basic_node.h>

namespace xpl {

  /**
   * Class for describing retrurn nodes.
   */
  class return_node: public cdk::basic_node {

  public:
    inline return_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_return_node(this, level);
    }

  };

} // xpl

#endif
