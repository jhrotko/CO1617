// $Id: function_definition_node.h,v 1.7 2017/05/13 01:44:55 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_FUNCTION_DEF_NODE_H__
#define __XPL_FUNCTION_DEF_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <string>
#include "block_node.h"

namespace xpl {

  /**
   * Class for describing function definition nodes.
   */
  class function_definition_node: public cdk::basic_node {
  	std::string *_name, *_qualifier;
    basic_type *_type;
  	cdk::sequence_node *_arguments;
    xpl::block_node *_block;
    cdk::expression_node *_return;
    bool _is_procedure;
	

  public:
    inline function_definition_node(int lineno, std::string *name, std::string *qualifier, basic_type *type,
    					cdk::sequence_node *arguments, xpl::block_node *block,
    					cdk::expression_node *returnV, bool is_procedure) :

        cdk::basic_node(lineno), _name(name), _qualifier(qualifier), _type(type), _arguments(arguments), 
                      _block(block), _return(returnV), _is_procedure(is_procedure) {}

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
  	inline cdk::sequence_node *arguments() {
  		return _arguments;
  	}
  public:
    inline xpl::block_node *block() {
      return _block;
    }

  public:
    inline cdk::expression_node *returnV() {
      return _return;
    }

  public:
  	inline bool is_procedure() {
  		return _is_procedure;
  	}

  public:
    inline void setReturnV(cdk::expression_node *node) {
      _return = node;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // xpl

#endif
