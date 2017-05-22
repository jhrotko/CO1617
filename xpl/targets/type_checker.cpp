#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void xpl::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_STRING));
}

void xpl::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != basic_type::TYPE_INT && node->argument()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in argument of unary expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

void xpl::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void xpl::type_checker::do_not_node(cdk::not_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE && node->left()->type()->name() != basic_type::TYPE_POINTER )
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_DOUBLE && node->right()->type()->name() != basic_type::TYPE_POINTER )
    throw std::string("wrong type in right argument of binary expression");

  // in XPL, expressions are always int
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

inline void xpl::type_checker::processBinaryExpression2(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_DOUBLE)
    throw std::string("wrong type in right argument of binary expression");

  // in XPL, expressions are always int
  if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->type(new basic_type(4, basic_type::TYPE_INT));
  } else {
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }
  
}

void xpl::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_POINTER) {
      if(node->right()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(4, basic_type::TYPE_INT));
      }
    } else if(node->right()->type()->name() == basic_type::TYPE_POINTER) {
      if(node->left()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(4, basic_type::TYPE_INT));
      }
    } else if( node->left()->type()->name() == basic_type::TYPE_INT) {
      if( node->right()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(4, basic_type::TYPE_INT));
      }
    } else if( node->left()->type()->name() == basic_type::TYPE_INT) {
      if( node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      }
    } else if( node->left()->type()->name() == basic_type::TYPE_DOUBLE) {
      if( node->right()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      }
    }
}
void xpl::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl + 2);
  if (node->left()->type()->name() != basic_type::TYPE_INT && node->left()->type()->name() != basic_type::TYPE_DOUBLE && node->left()->type()->name() != basic_type::TYPE_POINTER )
    throw std::string("wrong type in left argument of binary expression");

  node->right()->accept(this, lvl + 2);
  if (node->right()->type()->name() != basic_type::TYPE_INT && node->right()->type()->name() != basic_type::TYPE_DOUBLE && node->right()->type()->name() != basic_type::TYPE_POINTER )
    throw std::string("wrong type in right argument of binary expression");
  else if(node->right()->type()->name() == basic_type::TYPE_POINTER)
    throw std::string("Cannot subtract something to a pointer");
  else if( node->left()->type()->name() == basic_type::TYPE_POINTER && !(node->right()->type()->name() == basic_type::TYPE_POINTER || node->right()->type()->name() == basic_type::TYPE_INT))
    throw std::string("pointer type can only be subtracted by another pointer or integer");
  
  if(node->left()->type()->name() == basic_type::TYPE_POINTER) {
      if(node->right()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(4, basic_type::TYPE_INT));
      }
    } else if( node->left()->type()->name() == basic_type::TYPE_INT) {
      if( node->right()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(4, basic_type::TYPE_INT));
      }
    } else if( node->left()->type()->name() == basic_type::TYPE_INT) {
      if( node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      }
    } else if( node->left()->type()->name() == basic_type::TYPE_DOUBLE) {
      if( node->right()->type()->name() == basic_type::TYPE_INT) {
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
      }
    } 
}
void xpl::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression2(node, lvl);
}
void xpl::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression2(node, lvl);
}
void xpl::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_and_node(cdk::and_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void xpl::type_checker::do_or_node(cdk::or_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
//---------------------------------------------------------------------------

void xpl::type_checker::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(id);
  if (symbol != nullptr) {
    node->type(symbol->type());
  }
  else {
    throw id;
  }
}

void xpl::type_checker::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_UNSPEC;
  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void xpl::type_checker::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  try {

    node->lvalue()->accept(this, lvl);

  } catch (const std::string &id) {
    std::shared_ptr<xpl::symbol> symbol = std::make_shared<xpl::symbol>(new basic_type(4, basic_type::TYPE_INT), id, 0);
    _symtab.insert(id, symbol);
    _parent->set_new_symbol(symbol);  // advise parent that a symbol has been inserted
    node->lvalue()->accept(this, lvl);  //DAVID: bah!
  }
  node->rvalue()->accept(this, lvl);


  if(node->rvalue()->label() == "malloc_node") {
    node->rvalue()->type()->_subtype = new basic_type(node->lvalue()->type()->subtype()->size(), node->lvalue()->type()->subtype()->name());
  }

  if(node->rvalue()->type()->name() == basic_type::TYPE_POINTER && node->lvalue()->type()->name() == basic_type::TYPE_POINTER) {
    if(node->lvalue()->type()->subtype()->name() != node->rvalue()->type()->subtype()->name()) {
      if(node->lvalue()->type()->subtype()->name() == basic_type::TYPE_DOUBLE) {
        if(!(node->rvalue()->type()->subtype()->name() == basic_type::TYPE_INT || node->rvalue()->type()->subtype()->name() == basic_type::TYPE_DOUBLE))
          throw std::string("wrong type argument pointer of assignment expression.");
      
      }else 
        throw std::string("wrong type argument pointer of assignment expression");
    }
  } else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE){
    if (!(node->rvalue()->type()->name() == basic_type::TYPE_INT || node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE))
      throw std::string("wrong type in assignment expression");
  
  } else if (node->lvalue()->type()->name() == basic_type::TYPE_INT){
    if (node->rvalue()->type()->name() != basic_type::TYPE_INT )
      throw std::string("wrong type in assignment expression");

 } else if(node->lvalue()->type()->name() == basic_type::TYPE_STRING) {
    if(node->rvalue()->type()->name() != basic_type::TYPE_STRING)
      throw std::string("wrong type in assignment expression");
 }  else
    throw std::string("wrong type in assignment.");

  // in XPL, expressions are always int
  node->type(new basic_type(node->rvalue()->type()->size(), node->rvalue()->type()->name()));
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

void xpl::type_checker::do_print_node(xpl::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_read_node(xpl::read_node * const node, int lvl) {
  //nothing to check
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_while_node(xpl::while_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

//---------------------------------------------------------------------------

void xpl::type_checker::do_if_node(xpl::if_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void xpl::type_checker::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  node->condition()->accept(this, lvl + 4);
}

void xpl::type_checker::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  const std::string &name = *(node->name());
  const std::string &qualifier = *(node->qualifier());
  std::vector<basic_type*> types;
 
  if(node->arguments() != nullptr) {
    for(size_t i = 0; i < node->arguments()->size(); i++) {
      xpl::declaration_node *arg = (xpl::declaration_node*) node->arguments()->node(i);
      types.push_back(arg->type());
    }
  }std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), types, name, qualifier, false);
  _symtab.insert(name, std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), types, name, qualifier, false));
}

void xpl::type_checker::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  const std::string &name = *(node->name());
  const std::string &qualifier = *(node->qualifier());
  std::vector<basic_type*> types;
  
  if(node->arguments() != nullptr) {
    for(size_t i = 0; i < node->arguments()->size(); i++) {
      xpl::declaration_node *arg = (xpl::declaration_node*) node->arguments()->node(i);
      types.push_back(arg->type());
    }
  } 

  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(name);
  
  if(symbol != nullptr) {//if the fuction was declared, need to check parameters
    if(symbol->defined()) {
      throw std::string("function: " + name + " is already defined");
    } else {

      /*if(node->qualifier()->compare(symbol->qualifier()) != 0)
        throw std::string("Function " + name + " has invalid qualifier.");*/

      if(symbol->argsType().size() != types.size()) {
        throw std::string("Incorrect number of arguments.");
      } else {
        for(size_t i = 0; i < types.size(); i++) {
          if(types.at(i)->name() != symbol->argsType().at(i)->name())
            throw std::string("Different types between definiton and declaration arguments in function " + name);
        }
      }
      if(node->type() != nullptr && symbol->type() != nullptr) {
        if( node->type()->name() != symbol->type()->name()) {
          throw std::string("Different return types from declaration and definition of " + name + " fucntion.");
        
        } else if( node->type()->name() == basic_type::TYPE_INT) {
          node->setReturnV(new cdk::integer_node(node->lineno(), 0));

        } else if( node->type()->name() == basic_type::TYPE_POINTER) {
          node->setReturnV(new xpl::null_node(node->lineno()));
        }
      }

      if( node->returnV() != nullptr) {
        node->returnV()->accept(this, lvl);
      
        if( node->returnV()->type()->name() != symbol->type()->name()) {
          throw std::string("Return Value different from return type.");
        }
      }
      symbol->define(true);
    }

  } else { //else need to add to symbol table
    if(node->returnV() != nullptr) {
      node->returnV()->accept(this, lvl);

      if(node->type() != nullptr) {
        if(!(node->returnV()->type()->name() == basic_type::TYPE_INT && node->type()->name() == basic_type::TYPE_DOUBLE)){
          if(node->returnV()->type()->name() != node->type()->name())
            throw std::string("return type and return value are not consistent in " + name);
        }
      }

    } else if (node->type() != nullptr){
      if( node->type()->name() == basic_type::TYPE_INT) {
        node->setReturnV(new cdk::integer_node(node->lineno(), 0));

      } else if( node->type()->name() == basic_type::TYPE_POINTER) {
        node->setReturnV(new xpl::null_node(node->lineno()));
      }
    }

    std::shared_ptr<xpl::symbol> function = std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), types, name, qualifier, true);
    if (function->type()->name() == basic_type::TYPE_POINTER)
      function->type()->_subtype = new basic_type(node->type()->subtype()->size(), node->type()->subtype()->name());
    _symtab.insert(name, function);
  } 
  node->block()->accept(this, lvl);
}

void xpl::type_checker::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  const std::string &name = *(node->name());
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(name);

  if(symbol == nullptr) {
    throw std::string("function does not exist.");

  /*} else if(!symbol->defined()) {
    throw "function is not defined yet.";*/

  } else {
    std::vector<basic_type*> types;
    if(symbol->type()->name() == basic_type::TYPE_POINTER) {
      basic_type *t = new basic_type(symbol->type()->size(), symbol->type()->name());
      t->_subtype = new basic_type(symbol->type()->subtype()->size(), symbol->type()->subtype()->name());
      node->type(t);
    } else {
      node->type(new basic_type(symbol->type()->size(), symbol->type()->name()));  
    }

    
    if(node->arguments() != nullptr) {

      for(size_t i = 0; i < node->arguments()->size(); i++) {
        cdk::expression_node *arg = (cdk::expression_node*) node->arguments()->node(i);
        arg->accept(this, lvl);
        types.push_back(arg->type());
      }


      if(types.size() != node->arguments()->size()) {
        throw std::string("function call has not enough arguments.");
      } else {
        for(size_t i = 0; i < types.size(); i++) {
          if(node->arguments()->node(i) != nullptr) {
            basic_type *type = types.at(i);
            if(symbol->argsType().at(i)->name() == basic_type::TYPE_DOUBLE && type->name() == basic_type::TYPE_INT){
              
            }
            else if (type->name() == basic_type::TYPE_POINTER && symbol->argsType().at(i)->name() == basic_type::TYPE_POINTER) {
              if(symbol->argsType().at(i)->subtype()->name() == basic_type::TYPE_DOUBLE) {
                if(type->name() == basic_type::TYPE_DOUBLE || type->name() == basic_type::TYPE_INT){

              }
                }
            } else if(type->name() == symbol->argsType().at(i)->name()) {
            }
            else
              throw std::string("Different argument types between function invocation and declaration/definition " + name);
          }
        }
      }
    }
  }
}

void xpl::type_checker::do_index_node(xpl::index_node * const node, int lvl) {
  node->base()->accept(this, lvl);
  node->offset()->accept(this, lvl);

  if(node->base()->type()->name() == basic_type::TYPE_POINTER) {
    if(node->offset()->type()->name() == basic_type::TYPE_INT) {
      node->type(new basic_type(node->base()->type()->subtype()->size(), node->base()->type()->subtype()->name()));

    } else if(node->offset()->type()->name() == basic_type::TYPE_UNSPEC) {
      node->offset()->type(new basic_type(4, basic_type::TYPE_INT));
      node->type(new basic_type(node->base()->type()->subtype()->size(), node->base()->type()->subtype()->name()));

    }
  } else if(node->base()->type()->name() == basic_type::TYPE_UNSPEC ) {
      node->base()->type(new basic_type(4, basic_type::TYPE_INT));
      node->type(new basic_type(node->offset()->type()->subtype()->size(),node->offset()->type()->subtype()->name()));
  } else {
    throw std::string("invalid types at index_node");
  }
}

void xpl::type_checker::do_sweep_minus_node(xpl::sweep_minus_node * const node, int lvl) {
  node->leftValue()->accept(this, lvl);
  node->initialValue()->accept(this, lvl);
  if(node->leftValue()->type()->name() != node->initialValue()->type()->name())
    throw std::string("cannot initialize value in sweep-");

  node->condition()->accept(this, lvl);
  node->increment()->accept(this, lvl);
  node->block()->accept(this, lvl);
}

void xpl::type_checker::do_sweep_plus_node(xpl::sweep_plus_node * const node, int lvl) {
  node->leftValue()->accept(this, lvl);
  node->initialValue()->accept(this, lvl);
  if(node->leftValue()->type()->name() != node->initialValue()->type()->name())
    throw std::string("cannot initialize value in sweep+");

  node->condition()->accept(this, lvl);
  node->increment()->accept(this, lvl);
  node->block()->accept(this, lvl);

}

void xpl::type_checker::do_address_node(xpl::address_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
  node->type()->_subtype = new basic_type(node->argument()->type()->size(), node->argument()->type()->name());
}

void xpl::type_checker::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  const std::string &name = *(node->name());
  const std::string &qualifier = *(node->qualifier());
  std::vector<basic_type*> types;

  std::shared_ptr<xpl::symbol> symbol = _symtab.find_local(name);

  if(symbol == nullptr) {
    if(node->expression() != nullptr) {
      node->expression()->accept(this, lvl);
      if(node->type()->name() == node->expression()->type()->name()){
        if(node->type()->name() == basic_type::TYPE_DOUBLE) {
          if(!(node->expression()->type()->name() != basic_type::TYPE_DOUBLE || node->expression()->type()->name() != basic_type::TYPE_INT))
            throw std::string("invalid type on declaration");
        }
        else if(node->type()->name() == basic_type::TYPE_POINTER) {
          if(node->expression()->type()->subtype() == nullptr) { //null_node
            node->expression()->type()->_subtype = new basic_type(node->type()->subtype()->size(), node->type()->subtype()->name());   
          } else {
            if( node->type()->subtype()->name() == basic_type::TYPE_DOUBLE){
              if(!(node->expression()->type()->subtype()->name() == basic_type::TYPE_INT || node->expression()->type()->subtype()->name() == basic_type::TYPE_DOUBLE))
                throw std::string("invalid subtype on declaration");
            } else {
                throw std::string("invalid type in declaration");
            }
          }
        }
      }
      std::shared_ptr<xpl::symbol> var = std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), types, name, qualifier, true);
      
      if(var->type()->name() == basic_type::TYPE_POINTER) {
        var->type()->_subtype = new basic_type(node->type()->subtype()->size(), node->type()->subtype()->name());
      }
      
      _symtab.insert(name, var);

    } else if(node->expression() == nullptr) {
      std::shared_ptr<xpl::symbol> var = std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), types, name, qualifier, false);
      
      if(var->type()->name() == basic_type::TYPE_POINTER) {
        var->type()->_subtype = new basic_type(node->type()->subtype()->size(), node->type()->subtype()->name());
      }

      _symtab.insert(name, var);
    }

  } else {
      throw std::string("Variable already defined.");
  }
}

void xpl::type_checker::do_identidy_node(xpl::identidy_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

void xpl::type_checker::do_block_node(xpl::block_node * const node, int lvl) {
  //nothing to check
}

void xpl::type_checker::do_malloc_node(xpl::malloc_node * const node, int lvl) {
  node->argument()->accept(this, lvl);

  if(node->argument()->type()->name() != basic_type::TYPE_INT) {
    throw std::string("wrong expression type at malloc_node");
  }
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void xpl::type_checker::do_stop_node(xpl::stop_node * const node, int lvl) {
  //nothing to check
}

void xpl::type_checker::do_next_node(xpl::next_node * const node, int lvl) {
  //nothing to check
}

void xpl::type_checker::do_return_node(xpl::return_node * const node, int lvl) {
  //nothing to check
}

void xpl::type_checker::do_null_node(xpl::null_node * const node, int lvl) {
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}