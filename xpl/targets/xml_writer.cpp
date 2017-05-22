#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void xpl::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++){
    //std::cout << node->node(i)->label() << std::endl;
    node->node(i)->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  process_literal(node, lvl);
}

void xpl::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  process_literal(node, lvl);
}

void xpl::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  process_literal(node, lvl);
}
//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_unary_expression(cdk::unary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}
void xpl::xml_writer::do_not_node(cdk::not_node * const node, int lvl) {
  do_unary_expression(node, lvl);
}

//---------------------------------------------------------------------------

inline void xpl::xml_writer::do_binary_expression(cdk::binary_expression_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_and_node(cdk::and_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}
void xpl::xml_writer::do_or_node(cdk::or_node * const node, int lvl) {
  do_binary_expression(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->name() << "</" << node->label() << ">" << std::endl;
}

void xpl::xml_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);

  node->lvalue()->accept(this, lvl);
  reset_new_symbol();

  node->rvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

/*void xpl::xml_writer::do_program_node(xpl::program_node * const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 4);
  closeTag(node, lvl);
}*/

//---------------------------------------------------------------------------

void xpl::xml_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  //node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void xpl::xml_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  openTag(node, lvl);
  openTag("f_definition:name", lvl + 2);
  os() << std::string(lvl + 4, ' ') << "name:" << *node->name()<< std::endl;
  closeTag("f_definition:name", lvl + 2);
  
  if(node->qualifier()->compare("") != 0) {
    openTag("f_definition:qualifier", lvl + 2);
    os() << std::string(lvl + 4, ' ') << "qualifier:" << *node->qualifier() << std::endl;
    closeTag("f_definition:qualifier", lvl + 2);
  }
  if(node->type() != nullptr) {
    openTag("f_definition:type", lvl + 2);
    os() << std::string(lvl + 4, ' ') << "type:" << node->type()->name() << std::endl;
    closeTag("f_definition:type", lvl + 2);
  }
    _symtab.push();


  if(node->arguments() != nullptr) {
    openTag("f_definition:args", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("f_definition:args", lvl + 2);
  }

  if(node->returnV() != nullptr) {
    openTag("f_definition:return", lvl + 2);
    node->returnV()->accept(this, lvl + 4);
    closeTag("f_definition:return", lvl + 2);
  }

  if(node->block() != nullptr) {
    openTag("f_definition:block", lvl + 2);

    node->block()->accept(this, lvl + 4);

    closeTag("f_definition:block", lvl + 2);
  }

  closeTag(node, lvl);
  _symtab.pop();


}

void xpl::xml_writer::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("f_declaration:name", lvl + 2);
  os() << std::string(lvl + 4, ' ') << "name:" << *node->name()<< std::endl;
  closeTag("f_declaration:name", lvl + 2);
  if(node->qualifier()->compare("") != 0) {
    openTag("f_declaration:qualifier", lvl + 2);
    os() << std::string(lvl + 4, ' ') << "qualifier:" << *node->qualifier() << std::endl;
    closeTag("f_declaration:qualifier", lvl + 2);    
  }
  if(node->type() != nullptr) {
    openTag("f_declaration:type", lvl + 2);
    os() << std::string(lvl + 4, ' ') << "type:" << node->type()->name() << std::endl;
    closeTag("f_declaration:type", lvl + 2);    
  }
  _symtab.push();
  if(node->arguments() != nullptr) {
    openTag("f_declaration:arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("f_declaration:arguments", lvl + 2);    
  }
    _symtab.pop();


  closeTag(node, lvl);
}


void xpl::xml_writer::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("f_call:name", lvl + 2);
  os() << std::string(lvl + 4, ' ') << "callme:" << *node->name() << std::endl;
  closeTag("f_call:name", lvl + 2);
  if(node->arguments() != nullptr) {
      _symtab.push();
    openTag("f_call:arguments", lvl + 2);
    node->arguments()->accept(this, lvl + 4);
    closeTag("f_call:arguments", lvl + 2);
    _symtab.pop();

  }
  closeTag(node, lvl);
}

void xpl::xml_writer::do_index_node(xpl::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("index:base", lvl + 2);
  node->base()->accept(this, lvl + 4);
  closeTag("index:base", lvl + 2);
  openTag("index:offset", lvl + 2);
  node->offset()->accept(this, lvl + 4);
  closeTag("index:offset", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_sweep_minus_node(xpl::sweep_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("sweep-:lval", lvl + 2);
  node->leftValue()->accept(this, lvl + 4);
  closeTag("sweep-:lval", lvl + 2);
  openTag("sweep-:Initial value", lvl + 2);
  node->initialValue()->accept(this, lvl + 4);
  closeTag("sweep-:Initial value", lvl + 2);
  openTag("sweep-:condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("sweep-:condition", lvl + 2);
  openTag("sweep-:increment", lvl + 2);
  node->increment()->accept(this, lvl + 4);
  closeTag("sweep-:increment", lvl + 2);
  openTag("sweep-:instruction", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("sweep-:instruction", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_sweep_plus_node(xpl::sweep_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("sweep+:lval", lvl + 2);
  node->leftValue()->accept(this, lvl + 4);
  closeTag("sweep+:lval", lvl + 2);
  openTag("sweep+:Initial value", lvl + 2);
  node->initialValue()->accept(this, lvl + 4);
  closeTag("sweep+:Initial value", lvl + 2);
  openTag("sweep+:condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("sweep+:condition", lvl + 2);
  openTag("sweep+:increment", lvl + 2);
  node->increment()->accept(this, lvl + 4);
  closeTag("sweep+:increment", lvl + 2);
  openTag("sweep+:instruction", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("sweep+:instruction", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_next_node(xpl::next_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_return_node(xpl::return_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_null_node(xpl::null_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_address_node(xpl::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("declaration:type", lvl + 2);
  os() << std::string(lvl + 4, ' ') << "type:" << node->type()->name() << std::endl;
  closeTag("declaration:type", lvl + 2);
  openTag("declaration:name", lvl + 2);
  os() << std::string(lvl + 4, ' ') << "name:" << *node->name() << std::endl;
  closeTag("declaration:name", lvl + 2);
  if(node->qualifier()->compare("") != 0) {
    openTag("declaration:qualifier", lvl + 2);
    os() << std::string(lvl + 4, ' ') << "qualifier:" << node->qualifier() << std::endl;
    closeTag("declaration:qualifier", lvl + 2);
  }
  if(node->expression() != nullptr) {
    openTag("declaration:expression", lvl + 2);
    node->expression()->accept(this, lvl + 4);  
    closeTag("declaration:expression", lvl + 2);  
  }
  closeTag(node, lvl);
}

void xpl::xml_writer::do_identidy_node(xpl::identidy_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("identidy", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("identidy", lvl + 2);
  closeTag(node, lvl);
}

void xpl::xml_writer::do_block_node(xpl::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _symtab.push();
  openTag(node, lvl);

  if(node->declarations() != nullptr ) {
    openTag("block:declaration", lvl + 2);
    node->declarations()->accept(this, lvl + 4);
    closeTag("block:declaration", lvl + 2);
  }

  if(node->instr() != nullptr) {
    openTag("block:instructions", lvl + 2);
    node->instr()->accept(this, lvl + 4);
    closeTag("block:instructions", lvl + 2);
  }

  closeTag(node, lvl);
  _symtab.pop();

}

void xpl::xml_writer::do_malloc_node(xpl::malloc_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  openTag(node, lvl);
  openTag("malloc", lvl + 2);
  node->argument()->accept(this, lvl + 4);
  closeTag("malloc", lvl + 2);
  closeTag(node, lvl);
}

