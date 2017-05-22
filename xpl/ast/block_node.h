// $Id: block_node.h,v 1.3 2017/04/21 00:10:53 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_BLOCKNODE_H__
#define __XPL_BLOCKNODE_H__

#include <cdk/ast/basic_node.h>

namespace xpl {

  /**
   * Class for describing block nodes.
   */
  class block_node: public cdk::basic_node {
  cdk::sequence_node *_declarations;
  cdk::sequence_node *_instr;

  public:
    inline block_node(int lineno, cdk::sequence_node *declarations, cdk::sequence_node *instr) :
        cdk::basic_node(lineno), _declarations(declarations), _instr(instr) {
    }

  public:
    inline cdk::sequence_node *declarations() {
      return _declarations;
    }

  public:
    inline cdk::sequence_node *instr() {
      return _instr;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_block_node(this, level);
    }

  };

} // xpl

#endif