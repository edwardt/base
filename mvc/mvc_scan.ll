/**
 * @file mvc_scan.ll
 *
 * @brief Flex specification for MVC parser
 */

/*
 * -*- definitions -*-
 */
%{
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <string>
#include "mvc_parser_driver.h"      /* ParserDriver */
#include "mvc_parse.hh"             /* generated by bison */

typedef mvc::yy::Parser::token token;

/* Workaround for incompatibility in flex (2.5.31 through 2.5.33). */
#undef yywrap
#define yywrap() 1

/* By default, yylex returns int, we use token_type. Unfortunately,
   yyterminate by default returns 0, which is not of token_type. */
#define yyterminate() return token::MVC_TOK_END
%}

/* options: reentrant option is not usable as long as yyin needs to set 
   in epl_driver */
%option yylineno
%option full
%option noyywrap 
%option nounput 
%option batch 
%option debug 

/* definitions */
alpha   [A-Za-z]
alnum   [A-Za-z0-9]
digit   [0-9]
quote   [\"]
id      {alpha}{alnum}*
number  {digit}*("."{digit}*)?
strchar [^\"\n\\]
escape  "\\."
/*string  {quote}({strchar}|{escape})*{quote} */
string  {quote}({alpha})*{quote}
blank   [ \t]
newline [\n]

%x COMMENT
%{
#define YY_USER_ACTION yylloc->columns(yyleng);
%}

/* 
 * -*- rules -*- 
 */
%%
%{
  yylloc->step();
%}

"#"                       { BEGIN(COMMENT); }

"time"                    { return token::MVC_TOK_TIME; }
"milliseconds"            { return token::MVC_TOK_MILLISECONDS; }
"seconds"                 { return token::MVC_TOK_SECONDS; }
"minutes"                 { return token::MVC_TOK_MINUTES; }
"hours"                   { return token::MVC_TOK_HOURS; }
"days"                    { return token::MVC_TOK_DAYS; }
"create"                  { return token::MVC_TOK_CREATE; }

"module"                  { return token::MVC_TOK_MODULE; }
"process"                 { return token::MVC_TOK_PROCESS; }
"function"                { return token::MVC_TOK_FUNCTION; }
"event"                   { return token::MVC_TOK_EVENT; }
"var"                     { return token::MVC_TOK_VAR; }
"if"                      { return token::MVC_TOK_IF; }
"else"                    { return token::MVC_TOK_ELSE; }
"while"                   { return token::MVC_TOK_WHILE; }
"for"                     { return token::MVC_TOK_FOR; }
"->"                      { return token::MVC_TOK_TRIGGER; }
"return"                  { return token::MVC_TOK_RETURN; }
"continue"                { return token::MVC_TOK_CONTINUE; }
"break"                   { return token::MVC_TOK_BREAK; }

"{"                       { return token::MVC_TOK_LBRACE; }
"}"                       { return token::MVC_TOK_RBRACE; }
"("                       { return token::MVC_TOK_LPAREN; }
")"                       { return token::MVC_TOK_RPAREN; }
"["                       { return token::MVC_TOK_LBRACK; }
"]"                       { return token::MVC_TOK_RBRACK; }
","                       { return token::MVC_TOK_COMMA; }
":"                       { return token::MVC_TOK_COLON; }
"."                       { return token::MVC_TOK_DOT; }
";"                       { return token::MVC_TOK_SEMICOLON; }

"+"                       { return token::MVC_TOK_PLUS; }
"-"                       { return token::MVC_TOK_MINUS; }
"*"                       { return token::MVC_TOK_TIMES; }
"/"                       { return token::MVC_TOK_DIV; }
"<"                       { return token::MVC_TOK_LT; }
"<="                      { return token::MVC_TOK_LE; }
">"                       { return token::MVC_TOK_GT; }
">="                      { return token::MVC_TOK_GE; }
"=="                      { return token::MVC_TOK_EQ; }
"!="                      { return token::MVC_TOK_NE; }
"&&"                      { return token::MVC_TOK_AND; }
"||"                      { return token::MVC_TOK_OR; }
"="                       { return token::MVC_TOK_ASSIGN; }

{number}                  { 
                            yylval->ival = atoi(yytext); 
                            return token::MVC_TOK_NUMBER; 
                          }

{id}                      { 
                            yylval->strval = new std::string(yytext);
                            return token::MVC_TOK_ID; 
                          }

{string}                  {  
                            yylval->strval = new std::string(yytext);
                            return token::MVC_TOK_STRING; 
                          }

{blank}+                  { yylloc->step(); }

{newline}+                { yylloc->lines(yyleng); yylloc->step(); }

.                         {
                            std::cout << "[ANY:" << yytext << "]" << std::endl;
                          }

<COMMENT>{
{newline}+                { BEGIN(INITIAL); }
.                         { }
}

%%
