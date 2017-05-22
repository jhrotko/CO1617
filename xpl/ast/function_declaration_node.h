// $Id: function_declaration_node.h,v 1.5 2017/04/21 14:53:36 ist181423 Exp $ -*- c++ -*-
#ifndef __XPL_FUNCTION_DECLARATION_NODE_H__
#define __XPL_FUNCTION_DECLARATION_NODE_H__

#include "declaration_node.h"
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <string>

namespace xpl {

  /**
   * Class for describing function nodes.
   */
  class function_declaration_node: public xpl::declaration_node {
  	cdk::sequence_node *_arguments;
    cdk::expression_node *_return;
    bool _is_procedure;
	

  public:
    inline function_declaration_node(int lineno, std::string *name, std::string *qualifier,
    					basic_type *type, cdk::sequence_node *arguments, cdk::expression_node *returnV, 
              bool is_procedure) :

        xpl::declaration_node(lineno, name, qualifier, type, returnV), _arguments(arguments),
        _is_procedure(is_procedure) {}
  
  public:
    inline std::string *name() {
      return xpl::declaration_node::name();
    }

  public:
    inline std::string *qualifier() {
      return xpl::declaration_node::qualifier();
    }

  public:
    inline basic_type *type() {
      return xpl::declaration_node::type();
    }

  public:
  	inline cdk::sequence_node *arguments() {
  		return _arguments;
  	}

  public:
    inline cdk::expression_node *returnF() {
      return _return;
    }

  public:
    inline bool is_procedure() {
      return _is_procedure;
    }


    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // xpl

#endif
