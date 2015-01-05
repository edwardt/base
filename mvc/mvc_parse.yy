/**
 * @file mvc_parse.y
 *                       
 * @brief Bison specification for MVC parser.
 */

/*
 * -*- part 1: prologue -*-
 */
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "2.5"
%defines
%define namespace "mvc::yy"
%define parser_class_name "Parser"

%code top {
}

/* code required for YYSTYPE and YYLTYPE; will be included in the
   bison-generated file epl_parse.hpp */
%code requires {
  #include <string>
  #include <list>
  #include <vector>
  #include "mvc_exp.h"
  #include "mvc_stm.h"
  #include "mvc_module.h"
  #include "mvc_util.h"

  namespace mvc {
    class ParserDriver;
  }
}

/* parsing context */
%parse-param { mvc::ParserDriver& driver }
%lex-param { mvc::ParserDriver& driver }

/* code to be included in the bison-generated file mvc_parse.cc */
%code {
  #include "mvc_parser_driver.h"
}

%locations
%initial-action {
  // initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.getFile();
};

%union {
  /* scalars */
  int ival;
  bool bval;
  char cval;
  std::string *strval;

  /* expressions */
  std::list<mvc::Exp *> *explval;
  mvc::Exp *expval;

  /* statements */
  std::list<mvc::Stm *> *stmlval;
  mvc::Stm *stmval;

  /* module */
  mvc::Module *modval;
}

/* declarations */
%token MVC_TOK_END        0  "end of file"
%token MVC_TOK_LBRACE     "{"
%token MVC_TOK_RBRACE     "}"
%token MVC_TOK_LPAREN     "("
%token MVC_TOK_RPAREN     ")"
%token MVC_TOK_LBRACK     "["
%token MVC_TOK_RBRACK     "]"
%token MVC_TOK_COMMA      ","
%token MVC_TOK_COLON      ":"
%token MVC_TOK_SEMICOLON  ";"
%token MVC_TOK_DOT        "."
%token MVC_TOK_ASSIGN     "="

%token MVC_TOK_PLUS       "+"
%token MVC_TOK_MINUS      "-"
%token MVC_TOK_TIMES      "*"
%token MVC_TOK_DIV        "/"
%token MVC_TOK_LT         "<"
%token MVC_TOK_LE         "<="
%token MVC_TOK_GT         ">"
%token MVC_TOK_GE         ">="
%token MVC_TOK_EQ         "=="
%token MVC_TOK_NE         "!="
%token MVC_TOK_AND        "&&"
%token MVC_TOK_OR         "||"

%token MVC_TOK_ID
%token MVC_TOK_STRING
%token MVC_TOK_NUMBER

%token MVC_TOK_TIME
%token MVC_TOK_MILLISECONDS
%token MVC_TOK_SECONDS
%token MVC_TOK_MINUTES
%token MVC_TOK_HOURS
%token MVC_TOK_DAYS
%token MVC_TOK_CREATE

%token MVC_TOK_MODULE
%token MVC_TOK_EVENT
%token MVC_TOK_PROCESS
%token MVC_TOK_FUNCTION
%token MVC_TOK_VAR
%token MVC_TOK_IF
%token MVC_TOK_ELSE
%token MVC_TOK_WHILE
%token MVC_TOK_FOR
%token MVC_TOK_TRIGGER
%token MVC_TOK_RETURN
%token MVC_TOK_CONTINUE
%token MVC_TOK_BREAK

/* operator precedence */
%left MVC_TOK_LE MVC_TOK_LT MVC_TOK_GT MVC_TOK_GE MVC_TOK_EQ MVC_TOK_NE
%left MVC_TOK_AND MVC_TOK_OR
%left MVC_TOK_PLUS MVC_TOK_MINUS
%left MVC_TOK_TIMES MVC_TOK_DIV
%left MVC_TOK_UMINUS
%left MVC_TOK_DOT
%left MVC_TOK_LBRACK

/* for handling dangling-else */
%nonassoc MVC_TOK_RPAREN
%nonassoc MVC_TOK_ELSE

/*
%prec MVC_TOK_DAYS
%prec MVC_TOK_HOURS
%prec MVC_TOK_MINUTES
%prec MVC_TOK_SECONDS
%prec MVC_TOK_MILLISECONDS
*/

/*                      
%destructor { free ($$) } <*>
*/

/*
 * -*- part 2: grammar rules -*-
 */
%%
%start module;

module:
    MVC_TOK_MODULE identifier MVC_TOK_LBRACE one_or_more_stmdefs 
    MVC_TOK_RBRACE MVC_TOK_SEMICOLON
    {
      Module *module = driver.getModule();
      SymbolExp *name = static_cast<SymbolExp *>($<expval>2);
      module->setName(name);
      $<modval>$ = module;
    }
  | stmdef
    {
      Module *module = driver.getModule();
      ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *name = ef->createSymbol("unnamed");
      module->setName(name);
      module->addStm($<stmval>1);
      $<modval>$ = module;
    }
  ;

one_or_more_stmdefs:
    stmdef
    {
      Module *module = driver.getModule();
      module->addStm($<stmval>1);
      $<modval>$ = module;
    }
  | stmdef one_or_more_stmdefs
    {
      $<modval>2->addStm($<stmval>1);
      $<modval>$ = $<modval>2;
    }
  ;

/*
 * define statements
 */
stmdef:
    stm_eventdef 
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_vardef 
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_funcdef
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_procdef
    {
      $<stmval>$ = $<stmval>1;
    }
  ;

/*
 * statements
 */
stm:
    stm_block
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_if
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_while
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_for
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_ifelse
    {
      $<stmval>$ = $<stmval>1;
      }
  | stm_assign
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_trigger
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_return
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_continue
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_break
    {
      $<stmval>$ = $<stmval>1;
    }
  ;

/*
 * event definition
 */
stm_eventdef:
    MVC_TOK_EVENT identifier MVC_TOK_SEMICOLON
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createEventdef(static_cast<SymbolExp *>($<expval>2));
    }
  ;

/*
 * variable definition
 */
stm_vardef:
    MVC_TOK_VAR identifier MVC_TOK_SEMICOLON
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createVardef(static_cast<SymbolExp *>($<expval>2));
    }
  ;

/*
 * function definition
 */
stm_funcdef:
    MVC_TOK_FUNCTION identifier param_list stm_block MVC_TOK_SEMICOLON
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::FuncdefStm *fundef = NULL;
      fundef = sf->createFuncdef(static_cast<SymbolExp *>($<expval>3),
                                 $<explval>4,static_cast<Stm *>($<stmval>5));
      $<stmval>$ = fundef;
    }
  ;

param_list:
    MVC_TOK_LPAREN one_or_more_params MVC_TOK_RPAREN
    {
      $<explval>$ = $<explval>2;
    }
  ;

one_or_more_params:
    identifier
    {
      std::list<mvc::Exp *> *exps = new std::list<mvc::Exp *>;
      SymbolExp *name = static_cast<SymbolExp *>($<expval>1);
      exps->push_back(name);
      $<explval>$ = exps;
    }
  | identifier MVC_TOK_COMMA one_or_more_params
    {
      SymbolExp *name = static_cast<SymbolExp *>($<expval>3);
      $<explval>3->push_back(name);
      $<explval>$ = $<explval>3;
    }
  ;

/*
 * process definition
 */
stm_procdef:
    MVC_TOK_PROCESS identifier event_list stm_block MVC_TOK_SEMICOLON
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::ProcdefStm *procdef = NULL;
      procdef = sf->createProcdef(static_cast<SymbolExp *>($<expval>2),
                                  $<explval>3,
                                  static_cast<BlockStm *>($<stmval>4));
      delete $<explval>3;
      $<stmval>$ = procdef;
    }
  ;

event_list:
    MVC_TOK_LPAREN one_or_more_events MVC_TOK_RPAREN
    {
      $<explval>$ = $<explval>2;
    }
  ;

one_or_more_events:
    identifier
    {
      std::list<mvc::Exp *> *exps = new std::list<mvc::Exp *>;
      exps->push_back($<expval>1);
      $<explval>$ = exps;
    }
  | identifier one_or_more_events
    {
      $<explval>2->push_back($<expval>1);
      $<explval>$ = $<explval>2;
    }
  ;

/*
 * block statement
 */
stm_block:
    MVC_TOK_LBRACE one_or_more_blkstms MVC_TOK_RBRACE
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::BlockStm *blkstm = sf->createBlock();
      std::list<Stm *>& blkbody = blkstm->getBody();
      blkbody.assign($<stmlval>2->begin(), $<stmlval>2->end());
      delete $<stmlval>2;
      $<stmval>$ = blkstm;
    }
  ; 

one_or_more_blkstms:
    stm
    {
      std::list<mvc::Stm *> *stms = new std::list<mvc::Stm *>;
      stms->push_front($<stmval>1);
      $<stmlval>$ = stms;
    }
  | stm_vardef
    {
      std::list<mvc::Stm *> *stms = new std::list<mvc::Stm *>;
      stms->push_front($<stmval>1);
      $<stmlval>$ = stms;
    }
  | stm one_or_more_blkstms
    {
      $<stmlval>2->push_front($<stmval>1);
      $<stmlval>$ = $<stmlval>2;
    }
  | stm_vardef one_or_more_blkstms
    {
      $<stmlval>2->push_front($<stmval>1);
      $<stmlval>$ = $<stmlval>2;
    }
  ;

/*
 * if statement
 */
stm_if:
    MVC_TOK_IF MVC_TOK_LPAREN exp MVC_TOK_RPAREN stm
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createIf($<expval>3, $<stmval>5);
    }
  ;

/*
 * ifelse statement
 */
stm_ifelse:
    MVC_TOK_IF MVC_TOK_LPAREN exp MVC_TOK_RPAREN stm MVC_TOK_ELSE stm
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createIfElse($<expval>3, $<stmval>5, $<stmval>7);
    }
  ;

/*
 * while statement
 */
stm_while:
    MVC_TOK_WHILE MVC_TOK_LPAREN exp MVC_TOK_RPAREN stm
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createWhile($<expval>3, $<stmval>5);
    }
  ;

/*
 * for statement
 */
stm_for:
    MVC_TOK_FOR MVC_TOK_LPAREN assign_opt MVC_TOK_SEMICOLON 
    exp_opt MVC_TOK_SEMICOLON assign_opt MVC_TOK_RPAREN stm
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createFor($<stmval>3, $<expval>5, $<stmval>7,$<stmval>9);
    }
  ;

assign_opt:
    /* empty */
    {
      $<stmval>$ = NULL;
    }
  | assign
    {
      $<stmval>$ = $<stmval>1;
    }
  ;

/*
 * assign statement
 */
stm_assign:
    assign MVC_TOK_SEMICOLON
    {
      $<stmval>$ = $<stmval>1;
    }
  ;

assign:
    exp MVC_TOK_ASSIGN exp 
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createAssign($<expval>1, $<expval>3);
    }
  ;

/*
 * trigger statement
 */
stm_trigger:
    MVC_TOK_TRIGGER exp MVC_TOK_SEMICOLON
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createTrigger();
    }
  ;

/*
 * return statement
 */
stm_return:
    MVC_TOK_RETURN exp_opt MVC_TOK_SEMICOLON
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createReturn($<expval>2);
    }
  ;

/*
 * continue statement
 */
stm_continue:
    MVC_TOK_CONTINUE MVC_TOK_SEMICOLON
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createContinue();
    }
  ;

/*
 * break statement
 */
stm_break:
    MVC_TOK_BREAK exp MVC_TOK_SEMICOLON
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      $<stmval>$ = sf->createBreak();
    }
  ;

/*
 * Expressions
 */
exp:
    exp_varref
    {
      $<expval>$ = $<expval>1;
    }
  | exp_number 
    {
      $<expval>$ = $<expval>1;
    }
  | exp_string
    {
      $<expval>$ = $<expval>1;
    }  
  | exp_unary
    {
      $<expval>$ = $<expval>1;
    }
  | exp_binary
    {
      $<expval>$ = $<expval>1;
    }
  | MVC_TOK_LPAREN exp MVC_TOK_RPAREN
    {
      $<expval>$ = $<expval>2;
    }
  ;

exp_opt:
    /* empty */
    {
      $<expval>$ = NULL;
    }
  | exp
    {
      $<expval>$ = $<expval>1;
    }
  ;

exp_varref:
    identifier
    {
      $<expval>$ = $<expval>1;
    }
  | exp MVC_TOK_DOT identifier %prec MVC_TOK_DOT
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>($<expval>3);
      const std::string& name = symbol->getName();
      $<expval>$ = ef->createFieldref($<expval>1, name);
    }
  | exp MVC_TOK_LBRACK exp MVC_TOK_RBRACK
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      $<expval>$ = ef->createArrayref($<expval>1, $<expval>3);
    }
  ;

exp_number:
    number
    {
      $<expval>$ = $<expval>1;
    }
 ;

exp_string:
    string
    {
      $<expval>$ = $<expval>1;
    }
 ;

exp_unary:
    MVC_TOK_MINUS exp %prec MVC_TOK_UMINUS
    {
      $<expval>$ = new UnaryExp(mvc::UOT_MINUS, $<expval>2);
    }
  ;

exp_binary:
    exp MVC_TOK_PLUS exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_ADD, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_MINUS exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_SUB, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_TIMES exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_MUL, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_DIV exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_DIV, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_EQ exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_EQ, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_LT exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_LT, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_LE exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_LE, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_GT exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_GT, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_GE exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_GE, $<expval>1, $<expval>3);
    }
  | exp MVC_TOK_AND exp
    {
      $<expval>$ = new BinaryExp(mvc::BOT_AND, $<expval>1, $<expval>3);
    }
  ;
  
identifier:
    MVC_TOK_ID
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = ef->createSymbol(*$<strval>1);
      delete $<strval>1;
      $<expval>$ = symbol;
    }
  ;

number:
    MVC_TOK_NUMBER
    {
      $<expval>$ = new mvc::IntegerExp($<ival>1);
    }
  ;

string:
    MVC_TOK_STRING
    {
      $<expval>$ = new mvc::StringExp(*$<strval>1);
      delete $<strval>1;
    }
  ;

/*
 * -*- part 3: epilogue -*-
 */
%%
/* error member function */
void mvc::yy::Parser::error(const mvc::yy::Parser::location_type& l,
                            const std::string& m)
{
  driver.error(l, m);
}
