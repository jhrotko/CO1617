#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/local_size.h"
#include "ast/all.h"  // all.h is automatically generated


int xpl::local_size::getLocalSize(){
  return _size;
}

void xpl::local_size::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void xpl::local_size::do_integer_node(cdk::integer_node * const node, int lvl) {

}

void xpl::local_size::do_string_node(cdk::string_node * const node, int lvl) {

}

void xpl::local_size::do_double_node(cdk::double_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void xpl::local_size::do_neg_node(cdk::neg_node * const node, int lvl) { //FIXME: atençao ao zero

}

//---------------------------------------------------------------------------

void xpl::local_size::do_add_node(cdk::add_node * const node, int lvl) {

}
void xpl::local_size::do_sub_node(cdk::sub_node * const node, int lvl) {

}
void xpl::local_size::do_mul_node(cdk::mul_node * const node, int lvl) {

}
void xpl::local_size::do_div_node(cdk::div_node * const node, int lvl) {

}
void xpl::local_size::do_mod_node(cdk::mod_node * const node, int lvl) {

}
void xpl::local_size::do_lt_node(cdk::lt_node * const node, int lvl) {

}
void xpl::local_size::do_le_node(cdk::le_node * const node, int lvl) {

}
void xpl::local_size::do_ge_node(cdk::ge_node * const node, int lvl) {

}
void xpl::local_size::do_gt_node(cdk::gt_node * const node, int lvl) {

}
void xpl::local_size::do_ne_node(cdk::ne_node * const node, int lvl) {

}
void xpl::local_size::do_eq_node(cdk::eq_node * const node, int lvl) {

}

void xpl::local_size::do_not_node(cdk::not_node * const node, int lvl) {

}

void xpl::local_size::do_and_node(cdk::and_node * const node, int lvl) {

}

void xpl::local_size::do_or_node(cdk::or_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void xpl::local_size::do_identifier_node(cdk::identifier_node * const node, int lvl) {

}
void xpl::local_size::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {

}
void xpl::local_size::do_assignment_node(cdk::assignment_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void xpl::local_size::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {

}

void xpl::local_size::do_print_node(xpl::print_node * const node, int lvl) {
 
}

//---------------------------------------------------------------------------

void xpl::local_size::do_read_node(xpl::read_node * const node, int lvl) {

}

//---------------------------------------------------------------------------

void xpl::local_size::do_while_node(xpl::while_node * const node, int lvl) {
  node->condition()->accept(this,lvl);
  node->block()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void xpl::local_size::do_if_node(xpl::if_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}

//---------------------------------------------------------------------------

void xpl::local_size::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  node->thenblock()->accept(this, lvl);
  node->elseblock()->accept(this, lvl);
}
void xpl::local_size::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  _size = 0;
  _size += node->type()->size();
  node->block()->accept(this, lvl);

}
void xpl::local_size::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {

}
void xpl::local_size::do_function_call_node(xpl::function_call_node * const node, int lvl) {

}
void xpl::local_size::do_sweep_minus_node(xpl::sweep_minus_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}
void xpl::local_size::do_sweep_plus_node(xpl::sweep_plus_node * const node, int lvl) {
  node->block()->accept(this, lvl);
}
void xpl::local_size::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  _size += node->type()->size();
}
void xpl::local_size::do_block_node(xpl::block_node * const node, int lvl) {
  if(node->declarations() != nullptr)
    node->declarations()->accept(this, lvl);
  if(node->instr() != nullptr)
    node->instr()->accept(this, lvl);
}
void xpl::local_size::do_identidy_node(xpl::identidy_node * const node, int lvl) {

}
void xpl::local_size::do_malloc_node(xpl::malloc_node * const node, int lvl) {

}
void xpl::local_size::do_index_node(xpl::index_node * const node, int lvl) {

}
void xpl::local_size::do_address_node(xpl::address_node * const node, int lvl) {

}
void xpl::local_size::do_null_node(xpl::null_node * const node, int lvl) {
  
}
void xpl::local_size::do_stop_node(xpl::stop_node * const node, int lvl) {

}
void xpl::local_size::do_next_node(xpl::next_node * const node, int lvl) {

}
void xpl::local_size::do_return_node(xpl::return_node * const node, int lvl) {

}
