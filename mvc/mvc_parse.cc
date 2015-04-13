/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */
/* "%code top" blocks.  */

/* Line 286 of lalr1.cc  */
#line 16 "mvc_parse.yy"




/* Line 286 of lalr1.cc  */
#line 42 "mvc_parse.cc"


/* First part of user declarations.  */


/* Line 293 of lalr1.cc  */
#line 49 "mvc_parse.cc"


#include "mvc_parse.hh"

/* User implementation prologue.  */


/* Line 299 of lalr1.cc  */
#line 58 "mvc_parse.cc"
/* Unqualified %code blocks.  */

/* Line 300 of lalr1.cc  */
#line 40 "mvc_parse.yy"

  #include "mvc_parser_driver.hh"



/* Line 300 of lalr1.cc  */
#line 69 "mvc_parse.cc"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                               \
 do                                                                    \
   if (N)                                                              \
     {                                                                 \
       (Current).begin = YYRHSLOC (Rhs, 1).begin;                      \
       (Current).end   = YYRHSLOC (Rhs, N).end;                        \
     }                                                                 \
   else                                                                \
     {                                                                 \
       (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;        \
     }                                                                 \
 while (false)
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 382 of lalr1.cc  */
#line 13 "mvc_parse.yy"
namespace mvc { namespace yy {

/* Line 382 of lalr1.cc  */
#line 157 "mvc_parse.cc"

  /// Build a parser object.
  Parser::Parser (mvc::ParserDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 565 of lalr1.cc  */
#line 45 "mvc_parse.yy"
{
  /* initialize the initial location. */
  yylloc.begin.filename = yylloc.end.filename = &driver.getFile();
}

/* Line 565 of lalr1.cc  */
#line 313 "mvc_parse.cc"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, driver);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 690 of lalr1.cc  */
#line 156 "mvc_parse.yy"
    {
      Module *module = driver.getModule();
      SymbolExp *name = static_cast<SymbolExp *>((yysemantic_stack_[(6) - (2)].expval));
      module->setName(name);
      (yyval.modval) = module;
    }
    break;

  case 3:

/* Line 690 of lalr1.cc  */
#line 163 "mvc_parse.yy"
    {
      Module *module = driver.getModule();
      ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *name = ef->createSymbol("unnamed");
      module->setName(name);
      module->addStm((yysemantic_stack_[(1) - (1)].stmval));
      (yyval.modval) = module;
    }
    break;

  case 4:

/* Line 690 of lalr1.cc  */
#line 175 "mvc_parse.yy"
    {
      Module *module = driver.getModule();
      module->addStm((yysemantic_stack_[(1) - (1)].stmval));
      (yyval.modval) = module;
    }
    break;

  case 5:

/* Line 690 of lalr1.cc  */
#line 181 "mvc_parse.yy"
    {
      (yysemantic_stack_[(2) - (2)].modval)->addStm((yysemantic_stack_[(2) - (1)].stmval));
      (yyval.modval) = (yysemantic_stack_[(2) - (2)].modval);
    }
    break;

  case 6:

/* Line 690 of lalr1.cc  */
#line 192 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 7:

/* Line 690 of lalr1.cc  */
#line 196 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 8:

/* Line 690 of lalr1.cc  */
#line 200 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 9:

/* Line 690 of lalr1.cc  */
#line 204 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 10:

/* Line 690 of lalr1.cc  */
#line 214 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 11:

/* Line 690 of lalr1.cc  */
#line 218 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 12:

/* Line 690 of lalr1.cc  */
#line 222 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 13:

/* Line 690 of lalr1.cc  */
#line 226 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 14:

/* Line 690 of lalr1.cc  */
#line 230 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
      }
    break;

  case 15:

/* Line 690 of lalr1.cc  */
#line 234 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 16:

/* Line 690 of lalr1.cc  */
#line 238 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 17:

/* Line 690 of lalr1.cc  */
#line 242 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 18:

/* Line 690 of lalr1.cc  */
#line 246 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 19:

/* Line 690 of lalr1.cc  */
#line 250 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 20:

/* Line 690 of lalr1.cc  */
#line 254 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 21:

/* Line 690 of lalr1.cc  */
#line 264 "mvc_parse.yy"
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createEventdef(static_cast<SymbolExp *>((yysemantic_stack_[(3) - (2)].expval)));
    }
    break;

  case 22:

/* Line 690 of lalr1.cc  */
#line 275 "mvc_parse.yy"
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createVardef(static_cast<SymbolExp *>((yysemantic_stack_[(3) - (2)].expval)));
    }
    break;

  case 23:

/* Line 690 of lalr1.cc  */
#line 287 "mvc_parse.yy"
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::FundefStm *fundef = NULL;
      fundef = sf->createFundef(static_cast<SymbolExp *>((yysemantic_stack_[(7) - (2)].expval)),
                                (yysemantic_stack_[(7) - (4)].explval), static_cast<Stm *>((yysemantic_stack_[(7) - (6)].stmval)));
      (yyval.stmval) = fundef;
    }
    break;

  case 24:

/* Line 690 of lalr1.cc  */
#line 302 "mvc_parse.yy"
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::ProcdefStm *procdef = NULL;
      procdef = sf->createProcdef(static_cast<SymbolExp *>((yysemantic_stack_[(7) - (2)].expval)),
                                  (yysemantic_stack_[(7) - (4)].explval),
                                  static_cast<BlockStm *>((yysemantic_stack_[(7) - (6)].stmval)));
      delete (yysemantic_stack_[(7) - (3)].explval);
      (yyval.stmval) = procdef;
    }
    break;

  case 25:

/* Line 690 of lalr1.cc  */
#line 318 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::BlockStm *blkstm = sf->createBlock();
      std::list<Stm *>& blkbody = blkstm->getBody();
      blkbody.assign((yysemantic_stack_[(3) - (2)].stmlval)->begin(), (yysemantic_stack_[(3) - (2)].stmlval)->end());
      delete (yysemantic_stack_[(3) - (2)].stmlval);
      (yyval.stmval) = blkstm;
    }
    break;

  case 26:

/* Line 690 of lalr1.cc  */
#line 330 "mvc_parse.yy"
    {
      std::list<mvc::Stm *> *stms = new std::list<mvc::Stm *>;
      stms->push_front((yysemantic_stack_[(1) - (1)].stmval));
      (yyval.stmlval) = stms;
    }
    break;

  case 27:

/* Line 690 of lalr1.cc  */
#line 336 "mvc_parse.yy"
    {
      std::list<mvc::Stm *> *stms = new std::list<mvc::Stm *>;
      stms->push_front((yysemantic_stack_[(1) - (1)].stmval));
      (yyval.stmlval) = stms;
    }
    break;

  case 28:

/* Line 690 of lalr1.cc  */
#line 342 "mvc_parse.yy"
    {
      (yysemantic_stack_[(2) - (2)].stmlval)->push_front((yysemantic_stack_[(2) - (1)].stmval));
      (yyval.stmlval) = (yysemantic_stack_[(2) - (2)].stmlval);
    }
    break;

  case 29:

/* Line 690 of lalr1.cc  */
#line 347 "mvc_parse.yy"
    {
      (yysemantic_stack_[(2) - (2)].stmlval)->push_front((yysemantic_stack_[(2) - (1)].stmval));
      (yyval.stmlval) = (yysemantic_stack_[(2) - (2)].stmlval);
    }
    break;

  case 30:

/* Line 690 of lalr1.cc  */
#line 358 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createIf((yysemantic_stack_[(5) - (3)].expval), (yysemantic_stack_[(5) - (5)].stmval));
    }
    break;

  case 31:

/* Line 690 of lalr1.cc  */
#line 369 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createIfElse((yysemantic_stack_[(7) - (3)].expval), (yysemantic_stack_[(7) - (5)].stmval), (yysemantic_stack_[(7) - (7)].stmval));
    }
    break;

  case 32:

/* Line 690 of lalr1.cc  */
#line 380 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createWhile((yysemantic_stack_[(5) - (3)].expval), (yysemantic_stack_[(5) - (5)].stmval));
    }
    break;

  case 33:

/* Line 690 of lalr1.cc  */
#line 392 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createFor((yysemantic_stack_[(9) - (3)].stmval), (yysemantic_stack_[(9) - (5)].expval), (yysemantic_stack_[(9) - (7)].stmval),(yysemantic_stack_[(9) - (9)].stmval));
    }
    break;

  case 34:

/* Line 690 of lalr1.cc  */
#line 400 "mvc_parse.yy"
    {
      (yyval.stmval) = NULL;
    }
    break;

  case 35:

/* Line 690 of lalr1.cc  */
#line 404 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(1) - (1)].stmval);
    }
    break;

  case 36:

/* Line 690 of lalr1.cc  */
#line 414 "mvc_parse.yy"
    {
      (yyval.stmval) = (yysemantic_stack_[(2) - (1)].stmval);
    }
    break;

  case 37:

/* Line 690 of lalr1.cc  */
#line 421 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createAssign((yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 38:

/* Line 690 of lalr1.cc  */
#line 432 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      FuncallExp *call = static_cast<FuncallExp *>((yysemantic_stack_[(2) - (1)].expval));
      (yyval.stmval) = sf->createFuncall(call);
   }
    break;

  case 39:

/* Line 690 of lalr1.cc  */
#line 444 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createTrigger();
    }
    break;

  case 40:

/* Line 690 of lalr1.cc  */
#line 455 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createReturn((yysemantic_stack_[(3) - (2)].expval));
    }
    break;

  case 41:

/* Line 690 of lalr1.cc  */
#line 466 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createContinue();
    }
    break;

  case 42:

/* Line 690 of lalr1.cc  */
#line 477 "mvc_parse.yy"
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yyval.stmval) = sf->createBreak();
    }
    break;

  case 43:

/* Line 690 of lalr1.cc  */
#line 488 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 44:

/* Line 690 of lalr1.cc  */
#line 492 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 45:

/* Line 690 of lalr1.cc  */
#line 496 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 46:

/* Line 690 of lalr1.cc  */
#line 500 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 47:

/* Line 690 of lalr1.cc  */
#line 504 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 48:

/* Line 690 of lalr1.cc  */
#line 508 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 49:

/* Line 690 of lalr1.cc  */
#line 512 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(3) - (2)].expval);
    }
    break;

  case 50:

/* Line 690 of lalr1.cc  */
#line 519 "mvc_parse.yy"
    {
      (yyval.expval) = NULL;
    }
    break;

  case 51:

/* Line 690 of lalr1.cc  */
#line 523 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 52:

/* Line 690 of lalr1.cc  */
#line 530 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 53:

/* Line 690 of lalr1.cc  */
#line 534 "mvc_parse.yy"
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>((yysemantic_stack_[(3) - (3)].expval));
      const std::string& name = symbol->getName();
      (yyval.expval) = ef->createFieldref((yysemantic_stack_[(3) - (1)].expval), name);
    }
    break;

  case 54:

/* Line 690 of lalr1.cc  */
#line 541 "mvc_parse.yy"
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      (yyval.expval) = ef->createArrayref((yysemantic_stack_[(4) - (1)].expval), (yysemantic_stack_[(4) - (3)].expval));
    }
    break;

  case 55:

/* Line 690 of lalr1.cc  */
#line 549 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 56:

/* Line 690 of lalr1.cc  */
#line 556 "mvc_parse.yy"
    {
      (yyval.expval) = (yysemantic_stack_[(1) - (1)].expval);
    }
    break;

  case 57:

/* Line 690 of lalr1.cc  */
#line 563 "mvc_parse.yy"
    {
      (yyval.expval) = new UnaryExp(mvc::UOT_MINUS, (yysemantic_stack_[(2) - (2)].expval));
    }
    break;

  case 58:

/* Line 690 of lalr1.cc  */
#line 570 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_ADD, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 59:

/* Line 690 of lalr1.cc  */
#line 574 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_SUB, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 60:

/* Line 690 of lalr1.cc  */
#line 578 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_MUL, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 61:

/* Line 690 of lalr1.cc  */
#line 582 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_DIV, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 62:

/* Line 690 of lalr1.cc  */
#line 586 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_EQ, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 63:

/* Line 690 of lalr1.cc  */
#line 590 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_LT, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 64:

/* Line 690 of lalr1.cc  */
#line 594 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_LE, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 65:

/* Line 690 of lalr1.cc  */
#line 598 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_GT, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 66:

/* Line 690 of lalr1.cc  */
#line 602 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_GE, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 67:

/* Line 690 of lalr1.cc  */
#line 606 "mvc_parse.yy"
    {
      (yyval.expval) = new BinaryExp(mvc::BOT_AND, (yysemantic_stack_[(3) - (1)].expval), (yysemantic_stack_[(3) - (3)].expval));
    }
    break;

  case 68:

/* Line 690 of lalr1.cc  */
#line 613 "mvc_parse.yy"
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>((yysemantic_stack_[(4) - (1)].expval));
      (yyval.expval) = ef->createFuncall(symbol, (yysemantic_stack_[(4) - (3)].explval));
    }
    break;

  case 69:

/* Line 690 of lalr1.cc  */
#line 622 "mvc_parse.yy"
    {
      (yyval.explval) = (yysemantic_stack_[(1) - (1)].explval);
    }
    break;

  case 70:

/* Line 690 of lalr1.cc  */
#line 629 "mvc_parse.yy"
    {
      std::list<mvc::Exp *> *exps = new std::list<mvc::Exp *>;
      SymbolExp *name = static_cast<SymbolExp *>((yysemantic_stack_[(1) - (1)].expval));
      exps->push_back(name);
      (yyval.explval) = exps;
    }
    break;

  case 71:

/* Line 690 of lalr1.cc  */
#line 636 "mvc_parse.yy"
    {
      (yysemantic_stack_[(3) - (3)].explval)->push_front((yysemantic_stack_[(3) - (1)].expval));
      (yyval.explval) = (yysemantic_stack_[(3) - (3)].explval);
    }
    break;

  case 72:

/* Line 690 of lalr1.cc  */
#line 645 "mvc_parse.yy"
    {
      (yyval.explval) = (yysemantic_stack_[(1) - (1)].explval);
    }
    break;

  case 73:

/* Line 690 of lalr1.cc  */
#line 652 "mvc_parse.yy"
    {
      std::list<mvc::Exp *> *exps = new std::list<mvc::Exp *>;
      SymbolExp *name = static_cast<SymbolExp *>((yysemantic_stack_[(1) - (1)].expval));
      exps->push_back(name);
      (yyval.explval) = exps;
    }
    break;

  case 74:

/* Line 690 of lalr1.cc  */
#line 659 "mvc_parse.yy"
    {
      SymbolExp *name = static_cast<SymbolExp *>((yysemantic_stack_[(3) - (1)].expval));
      (yysemantic_stack_[(3) - (3)].explval)->push_front(name);
      (yyval.explval) = (yysemantic_stack_[(3) - (3)].explval);
    }
    break;

  case 75:

/* Line 690 of lalr1.cc  */
#line 668 "mvc_parse.yy"
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = ef->createSymbol(*(yysemantic_stack_[(1) - (1)].strval));
      delete (yysemantic_stack_[(1) - (1)].strval);
      (yyval.expval) = symbol;
    }
    break;

  case 76:

/* Line 690 of lalr1.cc  */
#line 678 "mvc_parse.yy"
    {
      (yyval.expval) = new mvc::IntegerExp((yysemantic_stack_[(1) - (1)].ival));
    }
    break;

  case 77:

/* Line 690 of lalr1.cc  */
#line 685 "mvc_parse.yy"
    {
      (yyval.expval) = new mvc::StringExp(*(yysemantic_stack_[(1) - (1)].strval));
      delete (yysemantic_stack_[(1) - (1)].strval);
    }
    break;



/* Line 690 of lalr1.cc  */
#line 1180 "mvc_parse.cc"
	default:
          break;
      }
    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (!yy_pact_value_is_default_ (yyn))
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -83;
  const short int
  Parser::yypact_[] =
  {
        88,    -6,    -6,    -6,    -6,    -6,    27,   -83,   -83,   -83,
     -83,   -83,   -83,    23,    24,    31,    49,    37,   -83,    46,
     -83,    -6,    -6,   -83,    47,    46,    50,   -83,    53,    57,
      54,   -83,    85,    -6,    85,   -83,    74,    82,   -83,    87,
       2,     2,   -83,   -83,    94,    98,    99,     2,     2,    97,
       2,    74,    74,   -83,   105,   -83,   -83,   -83,   -83,   -83,
     101,   -83,   -83,   -83,   -83,   -83,   167,   -83,   -83,   -83,
     -83,   -83,   118,   111,   -83,   -83,   -83,   -83,    25,   -83,
      26,     2,     2,     2,   183,   249,   119,   -83,   199,   -83,
     -83,   -83,   -83,     2,    -6,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,   -83,     2,   -83,   126,
     151,   120,   -83,   -83,   -83,   -83,   217,   -83,   249,    75,
      75,    26,    26,   265,   265,   265,   265,   265,    43,   233,
     131,   -83,   108,   108,     2,   -83,     2,   -83,   117,   -83,
     128,   -83,   108,     2,   -83,   154,   108,   -83
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     0,     0,     0,     0,     0,     0,     3,     6,     7,
       8,     9,    75,     0,     0,     0,     0,     0,     1,     0,
      21,     0,     0,    22,     0,     4,     0,    72,    73,     0,
       0,     5,     0,     0,     0,     2,     0,     0,    74,     0,
       0,     0,    77,    76,     0,     0,     0,     0,    50,     0,
       0,    26,    27,    10,     0,    11,    14,    12,    13,    15,
       0,    16,    17,    18,    19,    20,     0,    43,    44,    45,
      46,    47,    48,    52,    55,    56,    24,    23,     0,    48,
      57,     0,     0,    34,     0,    51,     0,    41,     0,    28,
      29,    25,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,     0,    49,     0,
       0,     0,    35,    39,    40,    42,     0,    53,    37,    58,
      59,    60,    61,    63,    64,    65,    66,    62,    67,    70,
       0,    69,     0,     0,    50,    54,     0,    68,    30,    32,
       0,    71,     0,    34,    31,     0,     0,    33
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
       -83,   -83,   136,   162,   -52,   -83,   -27,   -83,   -83,   -24,
     -38,   -83,   -83,   -83,   -83,    21,   -83,   -82,   -83,   -83,
     -83,   -83,   -83,   -29,    42,   -83,   -83,   -83,   -83,   -83,
     -36,   -83,    41,   156,   159,     1,   -83,   -83
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     6,    24,    25,    51,     8,     9,    10,    11,    53,
      54,    55,    56,    57,    58,   111,    59,    60,    61,    62,
      63,    64,    65,    66,    86,    67,    68,    69,    70,    71,
      79,   130,   131,    26,    27,    73,    74,    75
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
        72,   112,    13,    14,    15,    16,    17,    40,    37,    52,
      39,    78,    80,    89,    90,    72,    72,    41,    84,    85,
      12,    88,    28,    28,    52,    52,    19,    18,    12,    42,
      43,   108,    93,    93,    28,    20,    21,    94,    94,    96,
      97,    98,    99,   100,   101,   102,   103,   104,    23,   105,
      93,    30,   109,   110,    22,    94,    32,    96,    97,    98,
      99,   112,    33,    34,   116,    35,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    36,   129,    40,
     138,   139,    93,     2,     3,     4,     5,    94,    36,    41,
     144,    98,    99,    76,   147,   117,    72,    72,    77,    81,
      12,    42,    43,    82,    83,    85,    72,   129,    87,    91,
      72,    36,    92,    40,     5,    44,   107,    45,    46,    47,
      48,    49,    50,    41,     1,     2,     3,     4,     5,   106,
     114,   134,   132,    93,    12,    42,    43,   137,    94,   143,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    44,
     105,    45,    46,    47,    48,    49,    50,   133,    93,   142,
     146,    31,     7,    94,   145,    96,    97,    98,    99,   100,
     101,   102,   103,   104,    93,   105,   140,   141,    29,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      93,   105,    38,     0,   113,    94,     0,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    93,   105,     0,     0,
     115,    94,     0,    96,    97,    98,    99,   100,   101,   102,
     103,   104,     0,   105,    93,   135,     0,     0,     0,    94,
       0,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      93,   105,   136,     0,     0,    94,     0,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    93,   105,     0,     0,
       0,    94,     0,    96,    97,    98,    99,   100,   101,   102,
     103,   104,    93,   105,     0,     0,     0,    94,     0,    96,
      97,    98,    99,     0,     0,     0,     0,     0,     0,   105
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        36,    83,     1,     2,     3,     4,     5,     5,    32,    36,
      34,    40,    41,    51,    52,    51,    52,    15,    47,    48,
      26,    50,    21,    22,    51,    52,     3,     0,    26,    27,
      28,     6,     7,     7,    33,    11,     5,    12,    12,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    11,    24,
       7,     4,    81,    82,     5,    12,     6,    14,    15,    16,
      17,   143,     9,     6,    93,    11,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,     3,   107,     5,
     132,   133,     7,    37,    38,    39,    40,    12,     3,    15,
     142,    16,    17,    11,   146,    94,   132,   133,    11,     5,
      26,    27,    28,     5,     5,   134,   142,   136,    11,     4,
     146,     3,    11,     5,    40,    41,     5,    43,    44,    45,
      46,    47,    48,    15,    36,    37,    38,    39,    40,    11,
      11,    11,     6,     7,    26,    27,    28,     6,    12,    11,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    41,
      24,    43,    44,    45,    46,    47,    48,     6,     7,    42,
       6,    25,     0,    12,   143,    14,    15,    16,    17,    18,
      19,    20,    21,    22,     7,    24,   134,   136,    22,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       7,    24,    33,    -1,    11,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     7,    24,    -1,    -1,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    24,     7,     8,    -1,    -1,    -1,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       7,    24,     9,    -1,    -1,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     7,    24,    -1,    -1,
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     7,    24,    -1,    -1,    -1,    12,    -1,    14,
      15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,    24
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,    36,    37,    38,    39,    40,    51,    53,    55,    56,
      57,    58,    26,    85,    85,    85,    85,    85,     0,     3,
      11,     5,     5,    11,    52,    53,    83,    84,    85,    83,
       4,    52,     6,     9,     6,    11,     3,    59,    84,    59,
       5,    15,    27,    28,    41,    43,    44,    45,    46,    47,
      48,    54,    56,    59,    60,    61,    62,    63,    64,    66,
      67,    68,    69,    70,    71,    72,    73,    75,    76,    77,
      78,    79,    80,    85,    86,    87,    11,    11,    73,    80,
      73,     5,     5,     5,    73,    73,    74,    11,    73,    60,
      60,     4,    11,     7,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    24,    11,     5,     6,    73,
      73,    65,    67,    11,    11,    11,    73,    85,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      81,    82,     6,     6,    11,     8,     9,     6,    54,    54,
      74,    82,    42,    11,    54,    65,     6,    54
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    50,    51,    51,    52,    52,    53,    53,    53,    53,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    55,    56,    57,    58,    59,    60,    60,    60,    60,
      61,    62,    63,    64,    65,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    73,    73,    73,    73,    73,    73,
      74,    74,    75,    75,    75,    76,    77,    78,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    80,    81,
      82,    82,    83,    84,    84,    85,    86,    87
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     6,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     7,     7,     3,     1,     1,     2,     2,
       5,     7,     5,     9,     0,     1,     2,     3,     2,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     3,     4,     1,     1,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     1,
       1,     3,     1,     1,     3,     1,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "\"{\"", "\"}\"", "\"(\"",
  "\")\"", "\"[\"", "\"]\"", "\",\"", "\":\"", "\";\"", "\".\"", "\"=\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"<\"", "\"<=\"", "\">\"", "\">=\"",
  "\"==\"", "\"!=\"", "\"&&\"", "\"||\"", "MVC_TOK_NAME", "MVC_TOK_STRING",
  "MVC_TOK_NUMBER", "MVC_TOK_TIME", "MVC_TOK_MILLISECONDS",
  "MVC_TOK_SECONDS", "MVC_TOK_MINUTES", "MVC_TOK_HOURS", "MVC_TOK_DAYS",
  "MVC_TOK_CREATE", "MVC_TOK_MODULE", "MVC_TOK_EVENT", "MVC_TOK_REACTOR",
  "MVC_TOK_FUNCTION", "MVC_TOK_PROP", "MVC_TOK_IF", "MVC_TOK_ELSE",
  "MVC_TOK_WHILE", "MVC_TOK_FOR", "MVC_TOK_TRIGGER", "MVC_TOK_RETURN",
  "MVC_TOK_CONTINUE", "MVC_TOK_BREAK", "MVC_TOK_UMINUS", "$accept",
  "module", "one_or_more_stmdefs", "stmdef", "stm", "stm_eventdef",
  "stm_vardef", "stm_fundef", "stm_procdef", "stm_block",
  "one_or_more_blkstms", "stm_if", "stm_ifelse", "stm_while", "stm_for",
  "assign_opt", "stm_assign", "assign", "stm_funcall", "stm_trigger",
  "stm_return", "stm_continue", "stm_break", "exp", "exp_opt",
  "exp_varref", "exp_number", "exp_string", "exp_unary", "exp_binary",
  "exp_funcall", "exp_list", "one_or_more_exps", "name_list",
  "one_or_more_names", "name", "number", "string", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        51,     0,    -1,    36,    85,     3,    52,     4,    11,    -1,
      53,    -1,    53,    -1,    53,    52,    -1,    55,    -1,    56,
      -1,    57,    -1,    58,    -1,    59,    -1,    61,    -1,    63,
      -1,    64,    -1,    62,    -1,    66,    -1,    68,    -1,    69,
      -1,    70,    -1,    71,    -1,    72,    -1,    37,    85,    11,
      -1,    40,    85,    11,    -1,    39,    85,     5,    83,     6,
      59,    11,    -1,    38,    85,     5,    83,     6,    59,    11,
      -1,     3,    60,     4,    -1,    54,    -1,    56,    -1,    54,
      60,    -1,    56,    60,    -1,    41,     5,    73,     6,    54,
      -1,    41,     5,    73,     6,    54,    42,    54,    -1,    43,
       5,    73,     6,    54,    -1,    44,     5,    65,    11,    74,
      11,    65,     6,    54,    -1,    -1,    67,    -1,    67,    11,
      -1,    73,    13,    73,    -1,    80,    11,    -1,    45,    73,
      11,    -1,    46,    74,    11,    -1,    47,    11,    -1,    48,
      73,    11,    -1,    75,    -1,    76,    -1,    77,    -1,    78,
      -1,    79,    -1,    80,    -1,     5,    73,     6,    -1,    -1,
      73,    -1,    85,    -1,    73,    12,    85,    -1,    73,     7,
      73,     8,    -1,    86,    -1,    87,    -1,    15,    73,    -1,
      73,    14,    73,    -1,    73,    15,    73,    -1,    73,    16,
      73,    -1,    73,    17,    73,    -1,    73,    22,    73,    -1,
      73,    18,    73,    -1,    73,    19,    73,    -1,    73,    20,
      73,    -1,    73,    21,    73,    -1,    73,    24,    73,    -1,
      85,     5,    81,     6,    -1,    82,    -1,    73,    -1,    73,
       9,    82,    -1,    84,    -1,    85,    -1,    85,     9,    84,
      -1,    26,    -1,    28,    -1,    27,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Parser::yyprhs_[] =
  {
         0,     0,     3,    10,    12,    14,    17,    19,    21,    23,
      25,    27,    29,    31,    33,    35,    37,    39,    41,    43,
      45,    47,    51,    55,    63,    71,    75,    77,    79,    82,
      85,    91,    99,   105,   115,   116,   118,   121,   125,   128,
     132,   136,   139,   143,   145,   147,   149,   151,   153,   155,
     159,   160,   162,   164,   168,   173,   175,   177,   180,   184,
     188,   192,   196,   200,   204,   208,   212,   216,   220,   225,
     227,   229,   233,   235,   237,   241,   243,   245
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   154,   154,   162,   174,   180,   191,   195,   199,   203,
     213,   217,   221,   225,   229,   233,   237,   241,   245,   249,
     253,   263,   274,   285,   300,   317,   329,   335,   341,   346,
     357,   368,   379,   390,   400,   403,   413,   420,   431,   443,
     454,   465,   476,   487,   491,   495,   499,   503,   507,   511,
     519,   522,   529,   533,   540,   548,   555,   562,   569,   573,
     577,   581,   585,   589,   593,   597,   601,   605,   612,   621,
     628,   635,   644,   651,   658,   667,   677,   684
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 289;
  const int Parser::yynnts_ = 38;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 18;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 50;

  const unsigned int Parser::yyuser_token_number_max_ = 304;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1136 of lalr1.cc  */
#line 13 "mvc_parse.yy"
} } // mvc::yy

/* Line 1136 of lalr1.cc  */
#line 1748 "mvc_parse.cc"


/* Line 1138 of lalr1.cc  */
#line 694 "mvc_parse.yy"

/* error member function */
void mvc::yy::Parser::error(const mvc::yy::Parser::location_type& l,
                            const std::string& m)
{
  driver.error(l, m);
}

