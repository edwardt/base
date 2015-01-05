/**
 * @file epl_parse.y
 * @brief Bison specification for EPL parser
 *
 * @author cjeong
 */

/*
 * -*- part 1: prologue -*-
 */
%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "2.5"
%defines
%define namespace "mv::epl::yy"
%define parser_class_name "Parser"

%code top {
}

/* code required for YYSTYPE and YYLTYPE; will be included in the
   bison-generated file epl_parse.hpp */
%code requires {
  #include <string>
  #include <list>
  #include <vector>
  #include "mv/epl/epl_exp.h"
  #include "mv/epl/epl_stm.h"
  #include "mv/epl/epl_datatype.h"
  #include "mv/epl/epl_module.h"
  #include "mv/epl/epl_util.h"

  namespace mv { namespace epl {
    class ParserDriver;
  }}
}

/* parsing context */
%parse-param { mv::epl::ParserDriver& driver }
%lex-param { mv::epl::ParserDriver& driver }

/* code to be included in the bison-generated file epl_parse.cpp */
%code {
  #include "mv/epl/epl_parser_driver.h"
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
  std::list<mv::epl::Exp *> *explval;
  mv::epl::Exp *expval;

  /* datatypes */
  mv::epl::Datatype *typval;
  std::pair<std::string, mv::epl::Datatype *> *ntypval;
  std::list<std::pair<std::string, mv::epl::Datatype *> *> *ntyplval;

  /* statements */
  std::list<mv::epl::Stm *> *stmlval;
  mv::epl::Stm *stmval;

  /* module */
  mv::epl::Module *modval;
}

/* declarations */
%token EPL_TOK_END        0  "end of file"
%token EPL_TOK_LBRACE     "{"
%token EPL_TOK_RBRACE     "}"
%token EPL_TOK_LPAREN     "("
%token EPL_TOK_RPAREN     ")"
%token EPL_TOK_LBRACK     "["
%token EPL_TOK_RBRACK     "]"
%token EPL_TOK_COMMA      ","
%token EPL_TOK_COLON      ":"
%token EPL_TOK_SEMICOLON  ";"
%token EPL_TOK_DOT        "."
%token EPL_TOK_ASSIGN     "="

%token EPL_TOK_PLUS       "+"
%token EPL_TOK_MINUS      "-"
%token EPL_TOK_TIMES      "*"
%token EPL_TOK_DIV        "/"
%token EPL_TOK_LT         "<"
%token EPL_TOK_LE         "<="
%token EPL_TOK_GT         ">"
%token EPL_TOK_GE         ">="
%token EPL_TOK_EQ         "=="
%token EPL_TOK_NE         "!="
%token EPL_TOK_AND        "&&"
%token EPL_TOK_OR         "||"

%token EPL_TOK_ID
%token EPL_TOK_STRING
%token EPL_TOK_NUMBER

%token EPL_TOK_SELECT
%token EPL_TOK_FROM
%token EPL_TOK_WHERE
%token EPL_TOK_AS
%token EPL_TOK_RETAIN
%token EPL_TOK_EVENTS
%token EPL_TOK_TIME
%token EPL_TOK_MILLISECONDS
%token EPL_TOK_SECONDS
%token EPL_TOK_MINUTES
%token EPL_TOK_HOURS
%token EPL_TOK_DAYS
%token EPL_TOK_CREATE
%token EPL_TOK_SCHEMA
%token EPL_TOK_WINDOW

%token EPL_TOK_MODULE
%token EPL_TOK_EVENT
%token EPL_TOK_PROCESS
%token EPL_TOK_FUNCTION
%token EPL_TOK_TYPE
%token EPL_TOK_VAR
%token EPL_TOK_ARRAY
%token EPL_TOK_STRUCT
%token EPL_TOK_OF
%token EPL_TOK_VOID
%token EPL_TOK_BOOL
%token EPL_TOK_CHAR
%token EPL_TOK_SHORT
%token EPL_TOK_INT
%token EPL_TOK_LONG
%token EPL_TOK_FLOAT
%token EPL_TOK_DOUBLE
%token EPL_TOK_STRINGT
%token EPL_TOK_IF
%token EPL_TOK_ELSE
%token EPL_TOK_WHILE
%token EPL_TOK_FOR
%token EPL_TOK_TRIGGER
%token EPL_TOK_RETURN
%token EPL_TOK_CONTINUE
%token EPL_TOK_BREAK

/* operator precedence */
%left EPL_TOK_LE EPL_TOK_LT EPL_TOK_GT EPL_TOK_GE EPL_TOK_EQ EPL_TOK_NE
%left EPL_TOK_AND EPL_TOK_OR
%left EPL_TOK_PLUS EPL_TOK_MINUS
%left EPL_TOK_TIMES EPL_TOK_DIV
%left EPL_TOK_UMINUS
%left EPL_TOK_DOT
%left EPL_TOK_LBRACK

/* for handling dangling-else */
%nonassoc EPL_TOK_RPAREN
%nonassoc EPL_TOK_ELSE

 /*%prec EPL_TOK_DAYS
%prec EPL_TOK_HOURS
%prec EPL_TOK_MINUTES
%prec EPL_TOK_SECONDS
%prec EPL_TOK_MILLISECONDS*/

//%destructor { free ($$) } <*>

/*
 * -*- part 2: grammar rules -*-
 */
%%
%start module;

module:
    EPL_TOK_MODULE identifier EPL_TOK_LBRACE one_or_more_stmdefs 
    EPL_TOK_RBRACE EPL_TOK_SEMICOLON
    {
      Module *module = driver.getModule();
      SymbolExp *name = static_cast<SymbolExp *>($<expval>2);
      module->setName(name);
      $<modval>$ = module;
    }
  | stmdef
    {
      Module *module = driver.getModule();
      ExpMgr *emgr = ExpMgr::getInstance();
      SymbolExp *name = emgr->createSymbol("unnamed");
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
    stm_typedef 
    {
      $<stmval>$ = $<stmval>1;
    }
  | stm_eventdef 
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
/*  | stm_windowdef
    {
      $<stmval>$ = $<stmval>1;
      }*/
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
 * type definition
 */
stm_typedef:
    EPL_TOK_TYPE identifier EPL_TOK_ASSIGN type_spec EPL_TOK_SEMICOLON
    { 
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createTypedef(static_cast<SymbolExp *>($<expval>2), 
                                       $<typval>4);
    }
  ;

type_spec:
    type_void_spec  
    {
      $<typval>$ = $<typval>1;
    }
  | type_scalar_spec 
    {
      $<typval>$ = $<typval>1;
    }
  | type_array_spec
    {
      $<typval>$ = $<typval>1;
    }
  | type_struct_spec
    {
      $<typval>$ = $<typval>1;
    }
  | type_named_spec
    {
      $<typval>$ = $<typval>1;
    }
  ;

type_void_spec:
    EPL_TOK_VOID
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createVoidDatatype();
    }
  ;

type_scalar_spec:
    EPL_TOK_BOOL
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_BOOL);
    }
  | EPL_TOK_CHAR
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_CHAR);
    }
  | EPL_TOK_SHORT
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_SHORT);
    }
  | EPL_TOK_INT
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_INT);
    }
  | EPL_TOK_LONG
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_LONG);
    }
  | EPL_TOK_FLOAT
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_FLOAT);
    }
  | EPL_TOK_DOUBLE
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_DOUBLE);
    }
  | EPL_TOK_STRINGT
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createScalarDatatype(mv::epl::TT_STRING);
    }
  ;

type_array_spec:
    EPL_TOK_ARRAY EPL_TOK_LBRACK number EPL_TOK_RBRACK EPL_TOK_OF 
    type_scalar_spec
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createArrayDatatype($<typval>6, $<ival>3);
    }
  | EPL_TOK_ARRAY EPL_TOK_LBRACK number EPL_TOK_RBRACK EPL_TOK_OF 
    type_named_spec
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      $<typval>$ = dtmgr->createArrayDatatype($<typval>6, $<ival>3);
    }
  ;

type_struct_spec:
    EPL_TOK_LBRACE one_or_more_struct_fields EPL_TOK_RBRACE
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      mv::epl::StructDatatype *structype = dtmgr->createStructDatatype();
      
      std::list<std::pair<std::string, mv::epl::Datatype *> *>::iterator iter;
      for (iter = $<ntyplval>2->begin(); iter != $<ntyplval>2->end(); ++iter) {
        std::pair<std::string, mv::epl::Datatype *> *ntyp = *iter;
        std::string& name = ntyp->first;
        mv::epl::Datatype *type = ntyp->second;
        structype->addField(name, type);
        delete ntyp;
      }
      delete $<ntyplval>2;

      $<typval>$ = structype;
    }
  | EPL_TOK_STRUCT EPL_TOK_LBRACE one_or_more_struct_fields EPL_TOK_RBRACE
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      mv::epl::StructDatatype *structype = dtmgr->createStructDatatype();
      
      std::list<std::pair<std::string, mv::epl::Datatype *> *>::iterator iter;
      for (iter = $<ntyplval>3->begin(); iter != $<ntyplval>3->end(); ++iter) {
        std::pair<std::string, mv::epl::Datatype *> *ntyp = *iter;
        std::string& name = ntyp->first;
        mv::epl::Datatype *type = ntyp->second;
        structype->addField(name, type);
        delete ntyp;
      }
      delete $<ntyplval>3;

      $<typval>$ = structype;
    }
  ;

one_or_more_struct_fields:
    struct_field
    {
      std::list<std::pair<std::string, mv::epl::Datatype *> *> *fields = 
        new std::list<std::pair<std::string, mv::epl::Datatype *> *>;
      fields->push_front($<ntypval>1);
      $<ntyplval>$ = fields;
    }
  | struct_field one_or_more_struct_fields
    {
      $<ntyplval>2->push_front($<ntypval>1);
      $<ntyplval>$ = $<ntyplval>2;
    }
  ;

struct_field:
    identifier EPL_TOK_COLON type_simple_spec EPL_TOK_SEMICOLON
    {
      SymbolExp *symbol = static_cast<SymbolExp *>($<expval>1);
      const std::string& name = symbol->getName();
      $<ntypval>$ = 
        new std::pair<std::string, mv::epl::Datatype *>(name, $<typval>3);
    }
  ;

type_simple_spec:
    type_void_spec 
    {
      $<typval>$ = $<typval>1;
    }
  | type_scalar_spec 
    {
      $<typval>$ = $<typval>1;
    }
  | type_named_spec
    {
      $<typval>$ = $<typval>1;
    }
  ;

type_named_spec:
    identifier
    {
      mv::epl::DatatypeMgr *dtmgr = mv::epl::DatatypeMgr::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>($<expval>1);
      $<typval>$ = dtmgr->createNamedDatatype(symbol);
    }
  ;

/*
 * event definition
 */
stm_eventdef:
    EPL_TOK_EVENT identifier EPL_TOK_ASSIGN type_struct_spec EPL_TOK_SEMICOLON
    { 
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createEventdef(static_cast<SymbolExp *>($<expval>2), 
                                        $<typval>4);
    }
  ;

/*
 * variable definition
 */
stm_vardef:
    EPL_TOK_VAR identifier EPL_TOK_COLON type_simple_spec EPL_TOK_SEMICOLON
    { 
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createVardef(static_cast<SymbolExp *>($<expval>2), 
                                      $<typval>4);
    }
  ;

/*
 * function definition
 */
stm_funcdef:
    EPL_TOK_FUNCTION type_simple_spec identifier param_list stm_block 
    EPL_TOK_SEMICOLON
    { 
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      mv::epl::FuncdefStm *funcdef = NULL;
      funcdef = smgr->createFuncdef(static_cast<SymbolExp *>($<expval>3),
                                    $<typval>2, $<explval>4,
                                    static_cast<BlockStm *>($<stmval>5));
      $<stmval>$ = funcdef;
    }
  ;

param_list:
    EPL_TOK_LPAREN one_or_more_params EPL_TOK_RPAREN
    {
      $<explval>$ = $<explval>2;
    }
  ;

one_or_more_params:
    type_simple_spec identifier
    {
      std::list<mv::epl::Exp *> *exps = new std::list<mv::epl::Exp *>;
      SymbolExp *name = static_cast<SymbolExp *>($<expval>2);
      ParamExp *param = new ParamExp($<typval>1, name);
      exps->push_back(param);
      $<explval>$ = exps;
    }
  | type_simple_spec identifier EPL_TOK_COMMA one_or_more_params
    {
      SymbolExp *name = static_cast<SymbolExp *>($<expval>2);
      ParamExp *param = new ParamExp($<typval>1, name);
      $<explval>4->push_back(param);
      $<explval>$ = $<explval>4;
    }
  ;

/*
 * process definition
 */
stm_procdef:
    EPL_TOK_PROCESS identifier event_list stm_block EPL_TOK_SEMICOLON
    { 
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      mv::epl::ProcdefStm *procdef = NULL;
      procdef = smgr->createProcdef(static_cast<SymbolExp *>($<expval>2),
                                    $<explval>3,
                                    static_cast<BlockStm *>($<stmval>4));
      delete $<explval>3;
      $<stmval>$ = procdef;
    }
  ;

event_list:
    EPL_TOK_LPAREN one_or_more_events EPL_TOK_RPAREN
    {
      $<explval>$ = $<explval>2;
    }
  ;

one_or_more_events:
    identifier
    {
      std::list<mv::epl::Exp *> *exps = new std::list<mv::epl::Exp *>;
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
 * window definition
 */
//stm_windowdef:


/*
 * block statement
 */
stm_block:
    EPL_TOK_LBRACE one_or_more_blkstms EPL_TOK_RBRACE
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      mv::epl::BlockStm *blkstm = smgr->createBlock();
      std::list<Stm *>& blkbody = blkstm->getBody();
      blkbody.assign($<stmlval>2->begin(), $<stmlval>2->end());
      delete $<stmlval>2;
      $<stmval>$ = blkstm;
    }
  ; 

one_or_more_blkstms:
    stm
    {
      std::list<mv::epl::Stm *> *stms = new std::list<mv::epl::Stm *>;
      stms->push_front($<stmval>1);
      $<stmlval>$ = stms;
    }
  | stm_vardef
    {
      std::list<mv::epl::Stm *> *stms = new std::list<mv::epl::Stm *>;
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
    EPL_TOK_IF EPL_TOK_LPAREN exp EPL_TOK_RPAREN stm
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createIf($<expval>3, $<stmval>5);
    }
  ;

/*
 * ifelse statement
 */
stm_ifelse:
    EPL_TOK_IF EPL_TOK_LPAREN exp EPL_TOK_RPAREN stm EPL_TOK_ELSE stm
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createIfElse($<expval>3, $<stmval>5, $<stmval>7);
    }
  ;

/*
 * while statement
 */
stm_while:
    EPL_TOK_WHILE EPL_TOK_LPAREN exp EPL_TOK_RPAREN stm
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createWhile($<expval>3, $<stmval>5);
    }
  ;

/*
 * for statement
 */
stm_for:
    EPL_TOK_FOR EPL_TOK_LPAREN assign_opt EPL_TOK_SEMICOLON 
    exp_opt EPL_TOK_SEMICOLON assign_opt EPL_TOK_RPAREN stm
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createFor($<stmval>3, $<expval>5, $<stmval>7,
                                   $<stmval>9);
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
    assign EPL_TOK_SEMICOLON
    {
      $<stmval>$ = $<stmval>1;
    }
  ;

assign:
    exp EPL_TOK_ASSIGN exp 
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createAssign($<expval>1, $<expval>3);
    }
  ;

/*
 * trigger statement
 */
stm_trigger:
    EPL_TOK_TRIGGER exp EPL_TOK_SEMICOLON
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createTrigger();
    }
  ;

/*
 * return statement
 */
stm_return:
    EPL_TOK_RETURN exp_opt EPL_TOK_SEMICOLON
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createReturn($<expval>2);
    }
  ;

/*
 * continue statement
 */
stm_continue:
    EPL_TOK_CONTINUE EPL_TOK_SEMICOLON
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createContinue();
    }
  ;

/*
 * break statement
 */
stm_break:
    EPL_TOK_BREAK exp EPL_TOK_SEMICOLON
    {
      mv::epl::StmMgr *smgr = StmMgr::getInstance();
      $<stmval>$ = smgr->createBreak();
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
  | EPL_TOK_LPAREN exp EPL_TOK_RPAREN
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
  | exp EPL_TOK_DOT identifier %prec EPL_TOK_DOT
    {
      mv::epl::ExpMgr *emgr = ExpMgr::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>($<expval>3);
      const std::string& name = symbol->getName();
      $<expval>$ = emgr->createFieldref($<expval>1, name);
    }
  | exp EPL_TOK_LBRACK exp EPL_TOK_RBRACK
    {
      mv::epl::ExpMgr *emgr = ExpMgr::getInstance();
      $<expval>$ = emgr->createArrayref($<expval>1, $<expval>3);
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
    EPL_TOK_MINUS exp %prec EPL_TOK_UMINUS
    {
      $<expval>$ = new UnaryExp(UnaryExp::UOT_MINUS, $<expval>2);
    }
  ;

exp_binary:
    exp EPL_TOK_PLUS exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_ADD, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_MINUS exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_SUB, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_TIMES exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_MUL, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_DIV exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_DIV, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_EQ exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_EQ, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_LT exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_LT, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_LE exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_LE, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_GT exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_GT, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_GE exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_GE, $<expval>1, $<expval>3);
    }
  | exp EPL_TOK_AND exp
    {
      $<expval>$ = new BinaryExp(BinaryExp::BOT_AND, $<expval>1, $<expval>3);
    }
  ;
  
identifier:
    EPL_TOK_ID
    {
      ExpMgr *emgr = ExpMgr::getInstance();
      SymbolExp *symbol = emgr->createSymbol(*$<strval>1);
      delete $<strval>1;
      $<expval>$ = symbol;
    }
  ;

number:
    EPL_TOK_NUMBER
    {
      $<expval>$ = new mv::epl::NumberExp($<ival>1);
    }
  ;

string:
    EPL_TOK_STRING
    {
      $<expval>$ = new mv::epl::StringExp(*$<strval>1);
      delete $<strval>1;
    }
  ;

/*
 * -*- part 3: epilogue -*-
 */
%%
/* error member function */
void mv::epl::yy::Parser::error(const mv::epl::yy::Parser::location_type& l,
                                const std::string& m)
{
  driver.error(l, m);
}
