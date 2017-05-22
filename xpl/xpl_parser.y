%{
// $Id: xpl_parser.y,v 1.16 2017/05/18 02:14:31 ist181423 Exp $
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE       compiler->scanner()->lineno()
#define yylex()    compiler->scanner()->scan()
#define yyerror(s) compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler

//-- don't change *any* of these --- END!
%}

%union {
  int                   i;  /* integer value */
  double 				d;  /*real value*/
  std::string          *s;  /* symbol name or string literal */
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;
  basic_type		   *basic;
  xpl::block_node 	   *block;
  xpl::declaration_node *declaration;	
};

%token <i> tINTEGER
%token <s> tIDENTIFIER tSTRING
%token <d> tDOUBLE
%token <basic> tINT tREAL tRSTRING 
%token tWHILE tIF tELSEIF tELSE tNULL tPRINTLN tPRINT
%token tNEXT tSTOP tRETURN tSWEEP tPUBLIC tPROCEDURE tUSE 

%nonassoc tUNARY
%nonassoc tIFX
%nonassoc tELSEIF
%nonassoc tELSE

%right '='
%left '|'
%left '&'
%nonassoc '~'
%left tEQ tNE 
%left tGE tLE '>' '<'
%left '+' '-' 
%left '*' '/' '%'
%left '(' ')' '[' ']' 

%type <sequence> instr file varlist args arg
%type <expression> expr literal 
%type <s> strings
%type <lvalue> lval
%type <basic> type
%type <sequence> callargs callarg
%type <block> block_func
%type <node> elseif ifs iter stmt declr func func_def func_declr program var 
%type <declaration> varnoqual

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program : file 		{ compiler->ast($1);}
		|			{ compiler->ast(nullptr);}
		;

file: declr 		{ $$ = new cdk::sequence_node(LINE, $1); }
	| file declr 	{ $$ = new cdk::sequence_node(LINE, $2, $1); }			
	;

declr: func      	    { $$ = $1; }
	 | var ';'    		{ $$ = $1; }
	 ;

func: func_declr  	 { $$ = $1; }
	| func_def 		 { $$ = $1; }
	;


func_def: tPUBLIC type tIDENTIFIER '(' args ')' '=' literal '{' block_func '}'  { $$ = new xpl::function_definition_node(LINE, $3, new std::string("public"), $2, $5, $10, $8, 0); }
		| type tIDENTIFIER '(' args ')' '=' literal '{' block_func '}' 		  	{ $$ = new xpl::function_definition_node(LINE, $2, new std::string(""), $1, $4, $9, $7, 0); }
		| tPUBLIC tPROCEDURE tIDENTIFIER '(' args ')' '{' block_func '}'        { $$ = new xpl::function_definition_node(LINE, $3, new std::string("public"), new basic_type(0, basic_type::TYPE_VOID), $5, $8, nullptr, 1); } 
		| tPUBLIC type tIDENTIFIER '(' args ')' '{' block_func '}'				{ $$ = new xpl::function_definition_node(LINE, $3, new std::string("public"), $2, $5, $8, nullptr, 0); } 
		| tPROCEDURE tIDENTIFIER '(' args ')' '{' block_func '}' 			  	{ $$ = new xpl::function_definition_node(LINE, $2, new std::string(""), new basic_type(0, basic_type::TYPE_VOID), $4, $7, nullptr, 1); }
		| type tIDENTIFIER '(' args ')' '{' block_func '}' 						{ $$ = new xpl::function_definition_node(LINE, $2, new std::string(""), $1, $4, $7, nullptr, 1); }
		;

func_declr: type tIDENTIFIER '(' args ')'								{ $$ = new xpl::function_declaration_node(LINE, $2, new std::string(""), $1, $4, nullptr, 0); }
		  | tPUBLIC type tIDENTIFIER '(' args ')' 						{ $$ = new xpl::function_declaration_node(LINE, $3, new std::string("public"), $2, $5, nullptr, 0); }
		  | tPUBLIC tPROCEDURE tIDENTIFIER '(' args ')'  		  		{ $$ = new xpl::function_declaration_node(LINE, $3, new std::string("public"),new basic_type(0, basic_type::TYPE_VOID), $5, nullptr, 1);}
		  | tUSE tPROCEDURE tIDENTIFIER '(' args ')'  		  			{ $$ = new xpl::function_declaration_node(LINE, $3, new std::string("use"), new basic_type(0, basic_type::TYPE_VOID), $5, nullptr, 1);}
		  | tPROCEDURE tIDENTIFIER '(' args ')' 					  	{ $$ = new xpl::function_declaration_node(LINE, $2, new std::string(""), new basic_type(0, basic_type::TYPE_VOID), $4, nullptr, 1); }
		  | tUSE type tIDENTIFIER '(' args ')'							{ $$ = new xpl::function_declaration_node(LINE, $3, new std::string("use"), $2, $5, nullptr, 0); }
		  ;

varlist: varnoqual ';'				{ $$ = new cdk::sequence_node(LINE, $1);}
		 | varlist varnoqual ';'	{ $$ = new cdk::sequence_node(LINE, $2, $1);}
		 ;

var:  varnoqual							 { $$ = $1;}
	| tPUBLIC type tIDENTIFIER '=' expr  { $$ = new xpl::declaration_node(LINE, $3, new std::string("public"), $2, $5);}
	| tPUBLIC type tIDENTIFIER 		 	 { $$ = new xpl::declaration_node(LINE, $3, new std::string("public"), $2, nullptr); }
	| tUSE type tIDENTIFIER 		 	 { $$ = new xpl::declaration_node(LINE, $3, new std::string("use"), $2, nullptr); }
	;

varnoqual: type tIDENTIFIER				{ $$ = new xpl::declaration_node(LINE, $2,  new std::string(""), $1, nullptr); }
		|  type tIDENTIFIER '=' expr 	{ $$ = new xpl::declaration_node(LINE, $2, new std::string(""), $1, $4); }
		;

args: 			{ $$ = nullptr; }
	| arg 		{ $$ = $1; }
	;

arg:  var 					{ $$ = new cdk::sequence_node(LINE, $1); }
    | arg ',' var 			{ $$ = new cdk::sequence_node(LINE, $3, $1); }
	;

type: tRSTRING			{ $$ = new basic_type(1, basic_type::TYPE_STRING); }
	| tINT 				{ $$ = new basic_type(4, basic_type::TYPE_INT); }
	| tREAL 			{ $$ = new basic_type(8, basic_type::TYPE_DOUBLE); }
	| '[' type ']'		{ basic_type *bsc = new basic_type(4, basic_type::TYPE_POINTER);
						  basic_type *bcs1 = $2;
						  bsc->_subtype = bcs1;
						  $$ = bsc; }
	;

block_func: varlist  instr	{ $$ = new xpl::block_node(LINE, $1, $2); }
		  | varlist			{ $$ = new xpl::block_node(LINE, $1, nullptr); }
		  | instr 				{ $$ = new xpl::block_node(LINE, nullptr, $1);}
		  | 					{ $$ = new xpl::block_node(LINE, nullptr, nullptr); }
		  ;
		  
instr : stmt      	   { $$ = new cdk::sequence_node(LINE, $1); }
      | instr stmt 	   { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

stmt : expr ';'       				{ $$ = new xpl::evaluation_node(LINE, $1); }
     | expr tPRINTLN  				{ $$ = new xpl::print_node(LINE, $1,1); }
     | expr tPRINT		  			{ $$ = new xpl::print_node(LINE, $1,0); }
     | ifs			  				{ $$ = $1; } 
     | iter 						{ $$ = $1; }
     | '{' block_func '}' 			{ $$ = $2; }
     | tNEXT 						{ $$ = new xpl::next_node(LINE); }
     | tRETURN 						{ $$ = new xpl::stop_node(LINE); }
     | tSTOP 						{ $$ = new xpl::stop_node(LINE); }
     ;
    

ifs: tIF elseif 	{ $$ = $2; }
   ;

elseif: '(' expr ')' stmt tELSEIF elseif  			{ $$ = new xpl::if_else_node(LINE, $2, $4, $6); } 
   	  | '(' expr ')' stmt tELSE stmt 				{ $$ = new xpl::if_else_node(LINE, $2, $4, $6 ); }
   	  |	'(' expr ')' stmt 				%prec tIFX	{ $$ = new xpl::if_node(LINE, $2, $4); }
   	  ;

iter: tWHILE '(' expr ')' stmt 										{ $$ = new xpl::while_node(LINE, $3, $5); }
	| tSWEEP '+' '(' lval ':' expr ':' expr ':' expr ')' stmt		{ $$ = new xpl::sweep_plus_node(LINE, $4, $6, $8, $10, $12); }
	| tSWEEP '+' '(' lval ':' expr ':' expr ')' stmt 				{ $$ = new xpl::sweep_plus_node(LINE, $4, $6, $8, new cdk::integer_node(LINE, 1), $10); }
	| tSWEEP '-' '(' lval ':' expr ':' expr ':' expr ')' stmt		{ $$ = new xpl::sweep_minus_node(LINE, $4, $6, $8, $10, $12); }
	| tSWEEP '-' '(' lval ':' expr ':' expr ')' stmt				{ $$ = new xpl::sweep_minus_node(LINE, $4, $6, $8, new cdk::integer_node(LINE, -1), $10); }
	;  

expr : literal 				   { $$ = $1; }
	 | '@' 					   { $$ = new xpl::read_node(LINE); }
     | '(' expr ')'            { $$ = $2; }
     | '[' expr ']'			   { $$ = new xpl::malloc_node(LINE, $2); }
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); }
     | '+' expr %prec tUNARY   { $$ = new xpl::identidy_node(LINE, $2); }
     | '~' expr 			   { $$ = new cdk::not_node(LINE, $2); }
     | lval '?' %prec tUNARY   { $$ = new xpl::address_node(LINE, $1); }
     | expr '+' expr           { $$ = new cdk::add_node(LINE, $1, $3); }
     | expr '-' expr           { $$ = new cdk::sub_node(LINE, $1, $3); }
     | expr '*' expr           { $$ = new cdk::mul_node(LINE, $1, $3); }
     | expr '/' expr           { $$ = new cdk::div_node(LINE, $1, $3); }
     | expr '%' expr           { $$ = new cdk::mod_node(LINE, $1, $3); } 
     | expr '&' expr 		   { $$ = new cdk::and_node(LINE, $1, $3); }
     | expr '|' expr 		   { $$ = new cdk::or_node(LINE, $1, $3); }
     | expr '<' expr           { $$ = new cdk::lt_node(LINE, $1, $3); }
     | expr '>' expr           { $$ = new cdk::gt_node(LINE, $1, $3); }
     | expr tGE expr           { $$ = new cdk::ge_node(LINE, $1, $3); }
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); }
     | expr tNE expr           { $$ = new cdk::ne_node(LINE, $1, $3); }
     | expr tEQ expr           { $$ = new cdk::eq_node(LINE, $1, $3); }
     | lval                    { $$ = new cdk::rvalue_node(LINE, $1); } 
     | tIDENTIFIER '(' callargs ')'	{ $$ = new xpl::function_call_node(LINE, $1, $3); }
     | lval '=' expr 			{ $$ = new cdk::assignment_node(LINE, $1, $3); }
     ;


callargs: callarg			{ $$ = $1; }
		| 					{ $$ = new cdk::sequence_node(LINE); }
		;

callarg:  expr 				{ $$ = new cdk::sequence_node(LINE, $1); }
		| callarg ',' expr { $$ = new cdk::sequence_node(LINE, $3, $1); }
		;
	  
literal: tINTEGER		{ $$ = new cdk::integer_node(LINE, $1); }
	   | strings		{ $$ = new cdk::string_node(LINE, $1); }
	   | tDOUBLE 		{ $$ = new cdk::double_node(LINE, $1); }
	   | tNULL 			{ $$ = new xpl::null_node(LINE); }
	   ;

strings: tSTRING 			{ $$ = $1; }
	   | strings tSTRING 	{ *$1 += *$2; delete($2); $$ = $1; } 
	   ;


lval : tIDENTIFIER       { $$ = new cdk::identifier_node(LINE, $1); }
	 | expr '[' expr ']' { $$ = new xpl::index_node(LINE, $1, $3); }
     ; 
%%