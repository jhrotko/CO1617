#include <string>
#include <sstream>
#include <vector>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated
#include "targets/local_size.h"



//---------------------------------------------------------------------------
std::vector<int> *nextStack = new std::vector<int>();
std::vector<int> *stopStack = new std::vector<int>();
std::vector<std::string> _externList;

std::string stringDeclaration;
std::string _functionContext;
int _label;
std::string varName;
bool isGlobal = true;
bool isDeclaration = false;
bool isDouble;
int _offset;

//---------------------------------------------------------------------------

void xpl::postfix_writer::compareTo(cdk::expression_node *left, cdk::expression_node *right, int lvl) {
  //ASSERT_SAFE_EXPRESSIONS;

  left->accept(this, lvl);
  if(left->type()->name() == basic_type::TYPE_INT){
    if(right->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.I2D();
  }
  right->accept(this, lvl);
  if(left->type()->name() == basic_type::TYPE_DOUBLE){
    if(right->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();    
  }

  if(left->type()->name() == basic_type::TYPE_DOUBLE || right->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.DCMP();
    _pf.INT(0);
  } 
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if(isGlobal) {
    _pf.CONST(node->value());
    
  } else {
    if(isDouble)
      _pf.I2D();
    else 
    _pf.INT(node->value());
  }
}

void xpl::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl;

  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl = ++_lbl)); // give the string a name
  _label = lbl;
  _pf.STR(node->value()); 

  if (!isGlobal){
      _pf.TEXT(); // return to the TEXT segment
      _pf.ADDR(mklbl(lbl));
  }
}

void xpl::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  int lbl;

  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl = ++_lbl)); // give the string a name
  _label = lbl;
  _pf.DOUBLE(node->value()); 

  if (!isGlobal){
      _pf.TEXT(); // return to the TEXT segment
      _pf.ADDR(mklbl(lbl));
  } 

}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) { //FIXME: atençao ao zero
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if(node->type()->name() == basic_type::TYPE_INT)
    _pf.NEG(); // 2-complement
  else if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DNEG();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    
    _pf.DADD();
  } else if( node->left()->type()->name() == basic_type::TYPE_DOUBLE  && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl); 
    
    _pf.I2D();
    _pf.DADD();
  }else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->right()->accept(this, lvl);
    node->left()->accept(this, lvl);
    
    _pf.I2D();
    _pf.DADD();
  } else if (node->type()->name() == basic_type::TYPE_INT){
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);

    _pf.ADD();
  }
}
void xpl::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    
    _pf.DSUB();
  } else if( node->left()->type()->name() == basic_type::TYPE_DOUBLE  && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl); 
    
    _pf.I2D();
    _pf.DSUB();
  }else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->right()->accept(this, lvl);
    node->left()->accept(this, lvl);
    
    _pf.I2D();
    _pf.DSUB();
  } else if (node->type()->name() == basic_type::TYPE_INT){
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);

    _pf.SUB();
  } 
}
void xpl::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    
    _pf.DMUL();
  } else if( node->left()->type()->name() == basic_type::TYPE_DOUBLE  && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl); 
    
    _pf.I2D();
    _pf.DMUL();
  }else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->right()->accept(this, lvl);
    node->left()->accept(this, lvl);
    
    _pf.I2D();
    _pf.DMUL();
  } else if (node->type()->name() == basic_type::TYPE_INT){
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);

    _pf.MUL();
  }
}
void xpl::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    
    _pf.DDIV();
  } else if( node->left()->type()->name() == basic_type::TYPE_DOUBLE  && node->right()->type()->name() == basic_type::TYPE_INT) {
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl); 
    
    _pf.I2D();
    _pf.DDIV();
  }else if (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) {
    node->right()->accept(this, lvl);
    node->left()->accept(this, lvl);
    
    _pf.I2D();
    _pf.DDIV();
  } else if (node->type()->name() == basic_type::TYPE_INT){
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);

    _pf.DIV();
  }
}
void xpl::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void xpl::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  compareTo(node->left(), node->right(), lvl);
  _pf.LT();
}
void xpl::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  compareTo(node->left(), node->right(), lvl);
  _pf.LE();
}
void xpl::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  compareTo(node->left(), node->right(), lvl);
  _pf.GE();
}
void xpl::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  compareTo(node->left(), node->right(), lvl);
  _pf.GT();
}
void xpl::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void xpl::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

void xpl::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.NOT();
}

void xpl::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int end;

  node->left()->accept(this, lvl);
  _pf.JZ(mklbl(end = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(end));

}

void xpl::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int end;

  node->left()->accept(this, lvl);
  _pf.DUP();
  _pf.JNZ(mklbl(end = ++_lbl));
  node->right()->accept(this, lvl);
  _pf.OR();

  _pf.ALIGN();
  _pf.LABEL(mklbl(end));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_identifier_node(cdk::identifier_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
    std::shared_ptr<xpl::symbol> symbol = _symtab.find(node->name());

    if(symbol == nullptr) throw "Variable not found : postfix_writer";
    
    if(symbol->isGlobal())
      _pf.ADDR(node->name());
    else
      _pf.LOCAL(symbol->offset());
  
}

void xpl::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->lvalue()->accept(this, lvl);
  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DLOAD();
  else
    _pf.LOAD(); // depends on type size
}

void xpl::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if(node->type()->name() == basic_type::TYPE_DOUBLE){
    node->rvalue()->accept(this, lvl); // determine the new value
    if(node->rvalue()->type()->name() == basic_type::TYPE_INT)
      _pf.I2D();
    _pf.DDUP();
    node->lvalue()->accept(this, lvl);
    _pf.DSTORE();
  } else if( node->type()->name() == basic_type::TYPE_INT) {
    node->rvalue()->accept(this, lvl);
    _pf.DUP();
    node->lvalue()->accept(this, lvl);
    _pf.STORE();
  }
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_evaluation_node(xpl::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->type()->name() == basic_type::TYPE_INT || node->argument()->type()->name() == basic_type::TYPE_POINTER 
      || node->argument()->type()->name() == basic_type::TYPE_STRING ) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.TRASH(8); // delete the evaluated value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_VOID) {
    _pf.TRASH(0);
  }

  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

void xpl::postfix_writer::do_print_node(xpl::print_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  } else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  } else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8);
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  
  if(node->newline())
    _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_read_node(xpl::read_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _pf.CALL("readi");
  _pf.PUSH();
  _pf.STORE();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_while_node(xpl::while_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl, lbl2;
  _pf.LABEL(mklbl(lbl = ++_lbl));
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  nextStack->push_back(lbl);
  stopStack->push_back(lbl);
  node->block()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl));
  _pf.LABEL(mklbl(lbl2));
  nextStack->pop_back();
  stopStack->pop_back();
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_node(xpl::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void xpl::postfix_writer::do_if_else_node(xpl::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
  node->elseblock()->accept(this, lvl + 2);
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl = lbl2));
}

void xpl::postfix_writer::do_function_definition_node(xpl::function_definition_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  _functionContext = *(node->name());
  isGlobal = false;
  std::string name = *(node->name());
  node->accept(_localSize, lvl);

  _pf.TEXT();
  _pf.ALIGN();

  if(node->name()->compare("xpl") == 0) {
    _pf.GLOBAL("_main", _pf.FUNC());
    _pf.LABEL("_main");
    _pf.EXTERN("readi");
    _pf.EXTERN("readd");
    _pf.EXTERN("printi");
    _pf.EXTERN("prints");
    _pf.EXTERN("printd");
    _pf.EXTERN("println");
    _pf.EXTERN("argc");
    _pf.EXTERN("argv");
    _pf.EXTERN("envp");

    std::shared_ptr<xpl::symbol> symbol = _symtab.find(name);
    if(symbol != nullptr) {
      for(std::string funcName : _externList) {
        if(funcName.compare(name) == 0) {
          _pf.EXTERN(name);
        }
      }
    }
  } else {
    _pf.GLOBAL(name, _pf.FUNC());
    _pf.LABEL(name);
  }

  _pf.ENTER(_localSize->getLocalSize());

  if(node->returnV() != nullptr) {
    node->returnV()->accept(this, lvl);
    _pf.LOCAL(-4);
    if(node->returnV()->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.DSTORE();
    else
      _pf.STORE();

  }

  _offset = 8;
  _symtab.push();
  if(node->arguments() != nullptr)
    node->arguments()->accept(this, lvl);
  
  //set offset for local variables
  _offset = 0;
  if(node->block() != nullptr)
    node->block()->accept(this, lvl);
  _symtab.pop();
  
  _pf.POP();
  _pf.LEAVE();
  _pf.RET();

  isGlobal = true;
}

void xpl::postfix_writer::do_function_declaration_node(xpl::function_declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(*(node->name()));
  if(symbol != nullptr){
    if(!symbol->defined()) {
      _externList.push_back(*(node->name()));
    }
  }
}

void xpl::postfix_writer::do_function_call_node(xpl::function_call_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int trash = 0;

  if(node->arguments() != nullptr) {

    for(size_t i = 0; i < node->arguments()->size(); i++) {
      node->arguments()->node(node->arguments()->size()-i)->accept(this, lvl);
      xpl::declaration_node *dcl = (xpl::declaration_node*) node->arguments()->node(i);
      trash += dcl->type()->size();
    }    //extern?
  }

  _pf.CALL(*(node->name()));
  _pf.TRASH(trash);

  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DPUSH();
  else 
    _pf.PUSH();
  
}

void xpl::postfix_writer::do_sweep_minus_node(xpl::sweep_minus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int endSweep = ++_lbl;
  int inc = ++_lbl;
  int test= ++_lbl;

  cdk::assignment_node *assignment = new cdk::assignment_node(node->lineno(), node->leftValue(), node->initialValue());
  xpl::evaluation_node(node->lineno(), assignment).accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(mklbl(test));

  cdk::ge_node *test_node = new cdk::ge_node(node->lineno(), new cdk::rvalue_node(node->lineno(), node->leftValue()),node->condition());
  test_node->accept(this, lvl);
  _pf.JZ(mklbl(endSweep));
  
  nextStack->push_back(inc);
  stopStack->push_back(inc);
  

  node->block()->accept(this, lvl);
  
  _pf.ALIGN();
  _pf.LABEL(mklbl(inc));
  cdk::add_node *add = new cdk::add_node(node->lineno(), new cdk::rvalue_node(node->lineno(), node->leftValue()), node->increment()) ;
  cdk::assignment_node *assignment2 = new cdk::assignment_node(node->lineno(), node->leftValue(), add);
  xpl::evaluation_node(node->lineno(), assignment2).accept(this, lvl);

  _pf.JMP(mklbl(test));

  _pf.ALIGN();
  _pf.LABEL(mklbl(endSweep)); 

  nextStack->pop_back();
  stopStack->pop_back();
}
void xpl::postfix_writer::do_sweep_plus_node(xpl::sweep_plus_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int endSweep = ++_lbl;
  int inc = ++_lbl;
  int test= ++_lbl;

  cdk::assignment_node *assignment = new cdk::assignment_node(node->lineno(), node->leftValue(), node->initialValue());
  xpl::evaluation_node(node->lineno(), assignment).accept(this, lvl);

  _pf.ALIGN();
  _pf.LABEL(mklbl(test));

  cdk::le_node *test_node = new cdk::le_node(node->lineno(), new cdk::rvalue_node(node->lineno(), node->leftValue()),node->condition());
  test_node->accept(this, lvl);
  _pf.JZ(mklbl(endSweep));
  
  nextStack->push_back(inc);
  stopStack->push_back(inc);
  

  node->block()->accept(this, lvl);
  
  _pf.ALIGN();
  _pf.LABEL(mklbl(inc));
  cdk::add_node *add = new cdk::add_node(node->lineno(), new cdk::rvalue_node(node->lineno(), node->leftValue()), node->increment()) ;
  cdk::assignment_node *assignment2 = new cdk::assignment_node(node->lineno(), node->leftValue(), add);
  xpl::evaluation_node(node->lineno(), assignment2).accept(this, lvl);

  _pf.JMP(mklbl(test));

  _pf.ALIGN();
  _pf.LABEL(mklbl(endSweep)); 

  nextStack->pop_back();
  stopStack->pop_back();
}
void xpl::postfix_writer::do_declaration_node(xpl::declaration_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  if(node->expression() == nullptr) {

    if(isGlobal){
      _pf.BSS();
      _pf.ALIGN();
      _pf.GLOBAL(*(node->name()), _pf.OBJ()); //public
      std::shared_ptr<xpl::symbol> symbol = _symtab.find(*(node->name()));
      symbol->isGlobal(true);
      _pf.LABEL(*(node->name()));
      _pf.BYTE(node->type()->size());
    }
  } else {
    node->expression()->type()->name() == basic_type::TYPE_DOUBLE ? isDouble = true : isDouble = false;
    if(node->expression()->type()->name() == basic_type::TYPE_STRING || node->expression()->type()->name() == basic_type::TYPE_DOUBLE) {
      node->expression()->accept(this, lvl);
    }

    if(isGlobal) {
      _pf.DATA();
      _pf.ALIGN();
      _pf.GLOBAL(*(node->name()), _pf.OBJ());
      _pf.LABEL(*(node->name()));
      isDeclaration = true;
      if(!(node->expression()->type()->name() == basic_type::TYPE_STRING || node->expression()->type()->name() == basic_type::TYPE_DOUBLE))
        node->expression()->accept(this, lvl);

      std::shared_ptr<xpl::symbol> symbol = _symtab.find(*(node->name()));

      symbol->isGlobal(true);
    } else {

      _offset -= node->type()->size();
      node->expression()->accept(this, lvl);
      std::shared_ptr<xpl::symbol> symbol = _symtab.find(*(node->name()));

      symbol->offset(_offset);
      //symbol = std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), *(node->name()), _offset); 
      _pf.LOCAL(_offset);
      if(isDouble)
        _pf.DSTORE();
      else
        _pf.STORE();
    }


    if(node->expression()->type()->name() == basic_type::TYPE_STRING || node->expression()->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.ID(mklbl(_label));
    
  }

// positivo argumentos else decalraçoes
  if(!isGlobal) {
    std::shared_ptr<xpl::symbol> symbol = _symtab.find(*(node->name()));
    if(symbol != nullptr) {
      symbol->isGlobal(false);
      if(_offset > 0) {
        _offset += node->type()->size();
      } else {
        _offset -= node->type()->size();
      }
      symbol = std::make_shared<xpl::symbol>(new basic_type(node->type()->size(), node->type()->name()), *(node->name()), _offset);
    }
  } else {

  }
}

void xpl::postfix_writer::do_identidy_node(xpl::identidy_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
}

void xpl::postfix_writer::do_block_node(xpl::block_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _symtab.push();
  if(node->declarations() != nullptr)
    node->declarations()->accept(this, lvl);

  node->instr()->accept(this, lvl);
  _symtab.pop();
}

void xpl::postfix_writer::do_malloc_node(xpl::malloc_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->argument()->accept(this, lvl);
  node->argument()->type()->size();
  _pf.MUL();
  _pf.ALLOC();
  _pf.SP();
}

void xpl::postfix_writer::do_index_node(xpl::index_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->base()->accept(this, lvl);
  _pf.INT(node->base()->type()->subtype()->size());
  node->offset()->accept(this, lvl);
  _pf.MUL();
  _pf.ADD();
}

void xpl::postfix_writer::do_address_node(xpl::address_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
}

void xpl::postfix_writer::do_null_node(xpl::null_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _pf.INT(0);
}
void xpl::postfix_writer::do_stop_node(xpl::stop_node * const node, int lvl) {
  _pf.JMP(mklbl(stopStack->back()));
}
void xpl::postfix_writer::do_next_node(xpl::next_node * const node, int lvl) {
  _pf.JMP(mklbl(nextStack->back()));
}
void xpl::postfix_writer::do_return_node(xpl::return_node * const node, int lvl) {
  basic_type *type;
  std::shared_ptr<xpl::symbol> symbol = _symtab.find(_functionContext);
  
  if(symbol != nullptr) {
    type = symbol->type();
    if(type->name() == basic_type::TYPE_DOUBLE)
      _pf.DPOP();
    else 
      _pf.POP();
  }
  _pf.LEAVE();
  _pf.RET();
}
