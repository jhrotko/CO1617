// $Id: stop_node.h,v 1.2 2017/03/23 16:04:24 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_STOPNODE_H__
#define __XPL_STOPNODE_H__

#include <cdk/ast/basic_node.h>

namespace xpl {

  /**
   * Class for describing stop nodes.
   */
  class stop_node: public cdk::basic_node {

  public:
    inline stop_node(int lineno) :
        cdk::basic_node(lineno) {
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_stop_node(this, level);
    }

  };

} // xpl

#endif
