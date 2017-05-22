%option c++ yyclass="xpl_scanner" outfile="xpl_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
/* $Id: xpl_scanner.l,v 1.13 2017/05/22 15:40:20 ist181423 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "xpl_scanner.h"
#include "xpl_parser.tab.h"

// don't change this
//FIX operadores
#define yyerror LexerError

std::string str;

%}
HEX [0-9A-Fa-f][0-9A-Fa-f]*
%x X_STRING X_COMMENT X_COMMENTS X_IGNORE
%%


">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 

"while"                return tWHILE; 
"if"                   return tIF; 
"elsif"			   	   return tELSEIF;
"else"                 return tELSE; 
"sweep"				   return tSWEEP;

"next"				   return tNEXT;
"stop"				   return tSTOP;
"return"			   return tRETURN;

"!!" 				   return tPRINTLN;
"!" 				   return tPRINT;

"null" 				   return tNULL;
"int"				   return tINT;
"real"				   return tREAL;
"string"			   return tRSTRING;
"public" 			   return tPUBLIC;
"use"				   return tUSE;
"procedure"			   return tPROCEDURE;


[A-Za-z_][A-Za-z0-9]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

[+-]?[[1-9][0-9]*]+[eE][-][[:digit:]]+			         yylval.i = strtol(yytext, nullptr, 10); if( errno == ERANGE ) yyerror("Out of range."); return tDOUBLE;
[+-]?"."[[:digit:]]+								     yylval.i = strtol(yytext, nullptr, 10); if( errno == ERANGE ) yyerror("Out of range."); return tDOUBLE;
[+-]?[[:digit:]]+"."[[:digit:]]*([eE][+-]?[[:digit:]]+)? yylval.i = strtol(yytext, nullptr, 10); if( errno == ERANGE ) yyerror("Out of range."); return tDOUBLE;
0|[1-9]+[0-9]*        				    	    	 	 yylval.i = strtol(yytext, nullptr, 10); if( errno == ERANGE ) yyerror("Out of range."); return tINTEGER;
0x[[:xdigit:]]+											 yylval.i = strtol(yytext, nullptr, 16); if( errno == ERANGE ) yyerror("Out of range."); return tINTEGER;


"/*"            	   yy_push_state(X_COMMENT);
<X_COMMENT>"/*" 	   yy_push_state(X_COMMENT);
<X_COMMENT>"*/" 	   yy_pop_state();
<X_COMMENT>.|\n        ;

"//"				   yy_push_state(X_COMMENTS);
<X_COMMENTS>\n 		   yy_pop_state();
<X_COMMENTS>. 		   ;			


\"                  	  yy_push_state(X_STRING); 
<X_STRING>\"			  yylval.s = new std::string(str); str.clear(); yy_pop_state(); return tSTRING;
<X_STRING>\\{HEX}		  str += strtol(yytext+1, nullptr, 16);
<X_STRING>[1-9][0-9]*	  str += yytext ;
<X_STRING>[A-Za-z_][A-Za-z0-9]* str += yytext ;
<X_STRING>\\n 			  str += '\n';
<X_STRING>\\r 			  str += '\r';
<X_STRING>\\t 			  str += '\t';
<X_STRING>\\\" 			  str += '\"';
<X_STRING>\\\\			  str += '\\';
<X_STRING>"\0"[[:xdigit:]] str += yytext + 1;
<X_STRING>"\0" 			  yy_push_state(X_IGNORE); yylval.s = new std::string(str); str.clear(); return tSTRING; 			  
<X_STRING>. 			  str += yytext; //std::cout << yytext << std::endl;
<X_STRING>\n              yyerror("newline in string");
<X_IGNORE>\" 			  yy_pop_state();
<X_IGNORE>\\\"			  ;
<X_IGNORE>. 			  ;
<X_IGNORE>\n              yyerror("newline in string");

[-()<>=+*/%;{}&|,~?!@:\[\]]       return *yytext;

[ \t\n\r]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int xpl_scanner::yywrap() { return 1; }