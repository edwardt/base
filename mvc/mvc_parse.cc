// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.
// //                    "%code top" blocks.
#line 16 "mvc_parse.yy" // lalr1.cc:397


#line 37 "mvc_parse.cc" // lalr1.cc:397


// First part of user declarations.

#line 42 "mvc_parse.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "mvc_parse.hh"

// User implementation prologue.

#line 56 "mvc_parse.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 40 "mvc_parse.yy" // lalr1.cc:413

  #include "mvc_parser_driver.hh"

#line 62 "mvc_parse.cc" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 13 "mvc_parse.yy" // lalr1.cc:479
namespace mvc { namespace yy {
#line 148 "mvc_parse.cc" // lalr1.cc:479

  /// Build a parser object.
  Parser::Parser (mvc::ParserDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
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
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

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
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 45 "/home/cjeong/work/meadow/mvcode/mvc/mvc_parse.yy" // lalr1.cc:741
{
  /* initialize the initial location. */
  yyla.location.begin.filename = yyla.location.end.filename = &driver.getFile();
}

#line 489 "mvc_parse.cc" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, driver));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 156 "mvc_parse.yy" // lalr1.cc:859
    {
      Module *module = driver.getModule();
      SymbolExp *name = static_cast<SymbolExp *>((yystack_[4].value.expval));
      module->setName(name);
      (yylhs.value.modval) = module;
    }
#line 604 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 3:
#line 163 "mvc_parse.yy" // lalr1.cc:859
    {
      Module *module = driver.getModule();
      ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *name = ef->createSymbol("unnamed");
      module->setName(name);
      module->addStm((yystack_[0].value.stmval));
      (yylhs.value.modval) = module;
    }
#line 617 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 4:
#line 175 "mvc_parse.yy" // lalr1.cc:859
    {
      Module *module = driver.getModule();
      module->addStm((yystack_[0].value.stmval));
      (yylhs.value.modval) = module;
    }
#line 627 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 5:
#line 181 "mvc_parse.yy" // lalr1.cc:859
    {
      (yystack_[0].value.modval)->addStm((yystack_[1].value.stmval));
      (yylhs.value.modval) = (yystack_[0].value.modval);
    }
#line 636 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 6:
#line 192 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 644 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 7:
#line 196 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 652 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 8:
#line 200 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 660 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 9:
#line 204 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 668 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 10:
#line 214 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 676 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 11:
#line 218 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 684 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 12:
#line 222 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 692 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 13:
#line 226 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 700 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 14:
#line 230 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
      }
#line 708 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 15:
#line 234 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 716 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 16:
#line 238 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 724 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 17:
#line 242 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 732 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 18:
#line 246 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 740 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 19:
#line 250 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 748 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 20:
#line 254 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 756 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 21:
#line 264 "mvc_parse.yy" // lalr1.cc:859
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createEventdef(static_cast<SymbolExp *>((yystack_[1].value.expval)));
    }
#line 765 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 22:
#line 275 "mvc_parse.yy" // lalr1.cc:859
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createVardef(static_cast<SymbolExp *>((yystack_[1].value.expval)));
    }
#line 774 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 23:
#line 287 "mvc_parse.yy" // lalr1.cc:859
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::FundefStm *fundef = NULL;
      fundef = sf->createFundef(static_cast<SymbolExp *>((yystack_[5].value.expval)),
                                (yystack_[3].value.explval), static_cast<Stm *>((yystack_[1].value.stmval)));
      (yylhs.value.stmval) = fundef;
    }
#line 786 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 24:
#line 302 "mvc_parse.yy" // lalr1.cc:859
    { 
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::ProcdefStm *procdef = NULL;
      procdef = sf->createProcdef(static_cast<SymbolExp *>((yystack_[5].value.expval)),
                                  (yystack_[3].value.explval),
                                  static_cast<BlockStm *>((yystack_[1].value.stmval)));
      delete (yystack_[4].value.explval);
      (yylhs.value.stmval) = procdef;
    }
#line 800 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 25:
#line 318 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      mvc::BlockStm *blkstm = sf->createBlock();
      std::list<Stm *>& blkbody = blkstm->getBody();
      blkbody.assign((yystack_[1].value.stmlval)->begin(), (yystack_[1].value.stmlval)->end());
      delete (yystack_[1].value.stmlval);
      (yylhs.value.stmval) = blkstm;
    }
#line 813 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 26:
#line 330 "mvc_parse.yy" // lalr1.cc:859
    {
      std::list<mvc::Stm *> *stms = new std::list<mvc::Stm *>;
      stms->push_front((yystack_[0].value.stmval));
      (yylhs.value.stmlval) = stms;
    }
#line 823 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 27:
#line 336 "mvc_parse.yy" // lalr1.cc:859
    {
      std::list<mvc::Stm *> *stms = new std::list<mvc::Stm *>;
      stms->push_front((yystack_[0].value.stmval));
      (yylhs.value.stmlval) = stms;
    }
#line 833 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 28:
#line 342 "mvc_parse.yy" // lalr1.cc:859
    {
      (yystack_[0].value.stmlval)->push_front((yystack_[1].value.stmval));
      (yylhs.value.stmlval) = (yystack_[0].value.stmlval);
    }
#line 842 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 29:
#line 347 "mvc_parse.yy" // lalr1.cc:859
    {
      (yystack_[0].value.stmlval)->push_front((yystack_[1].value.stmval));
      (yylhs.value.stmlval) = (yystack_[0].value.stmlval);
    }
#line 851 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 30:
#line 358 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createIf((yystack_[2].value.expval), (yystack_[0].value.stmval));
    }
#line 860 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 31:
#line 369 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createIfElse((yystack_[4].value.expval), (yystack_[2].value.stmval), (yystack_[0].value.stmval));
    }
#line 869 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 32:
#line 380 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createWhile((yystack_[2].value.expval), (yystack_[0].value.stmval));
    }
#line 878 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 33:
#line 392 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createFor((yystack_[6].value.stmval), (yystack_[4].value.expval), (yystack_[2].value.stmval),(yystack_[0].value.stmval));
    }
#line 887 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 34:
#line 400 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = NULL;
    }
#line 895 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 35:
#line 404 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[0].value.stmval);
    }
#line 903 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 36:
#line 414 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.stmval) = (yystack_[1].value.stmval);
    }
#line 911 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 37:
#line 421 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createAssign((yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 920 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 38:
#line 432 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      FuncallExp *call = static_cast<FuncallExp *>((yystack_[1].value.expval));
      (yylhs.value.stmval) = sf->createFuncall(call);
   }
#line 930 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 39:
#line 444 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createTrigger();
    }
#line 939 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 40:
#line 455 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createReturn((yystack_[1].value.expval));
    }
#line 948 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 41:
#line 466 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createContinue();
    }
#line 957 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 42:
#line 477 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::StmFactory *sf = StmFactory::getInstance();
      (yylhs.value.stmval) = sf->createBreak();
    }
#line 966 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 43:
#line 488 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 974 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 44:
#line 492 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 982 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 45:
#line 496 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 990 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 46:
#line 500 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 998 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 47:
#line 504 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 1006 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 48:
#line 508 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 1014 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 49:
#line 512 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[1].value.expval);
    }
#line 1022 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 50:
#line 519 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = NULL;
    }
#line 1030 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 51:
#line 523 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 1038 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 52:
#line 530 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 1046 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 53:
#line 534 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>((yystack_[0].value.expval));
      const std::string& name = symbol->getName();
      (yylhs.value.expval) = ef->createFieldref((yystack_[2].value.expval), name);
    }
#line 1057 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 54:
#line 541 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      (yylhs.value.expval) = ef->createArrayref((yystack_[3].value.expval), (yystack_[1].value.expval));
    }
#line 1066 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 55:
#line 549 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 1074 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 56:
#line 556 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = (yystack_[0].value.expval);
    }
#line 1082 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 57:
#line 563 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new UnaryExp(mvc::UOT_MINUS, (yystack_[0].value.expval));
    }
#line 1090 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 58:
#line 570 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_ADD, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1098 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 59:
#line 574 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_SUB, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1106 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 60:
#line 578 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_MUL, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1114 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 61:
#line 582 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_DIV, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1122 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 62:
#line 586 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_EQ, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1130 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 63:
#line 590 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_LT, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1138 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 64:
#line 594 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_LE, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1146 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 65:
#line 598 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_GT, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1154 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 66:
#line 602 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_GE, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1162 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 67:
#line 606 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new BinaryExp(mvc::BOT_AND, (yystack_[2].value.expval), (yystack_[0].value.expval));
    }
#line 1170 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 68:
#line 613 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = static_cast<SymbolExp *>((yystack_[3].value.expval));
      (yylhs.value.expval) = ef->createFuncall(symbol, (yystack_[1].value.explval));
    }
#line 1180 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 69:
#line 622 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.explval) = (yystack_[0].value.explval);
    }
#line 1188 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 70:
#line 629 "mvc_parse.yy" // lalr1.cc:859
    {
      std::list<mvc::Exp *> *exps = new std::list<mvc::Exp *>;
      SymbolExp *name = static_cast<SymbolExp *>((yystack_[0].value.expval));
      exps->push_back(name);
      (yylhs.value.explval) = exps;
    }
#line 1199 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 71:
#line 636 "mvc_parse.yy" // lalr1.cc:859
    {
      (yystack_[0].value.explval)->push_front((yystack_[2].value.expval));
      (yylhs.value.explval) = (yystack_[0].value.explval);
    }
#line 1208 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 72:
#line 645 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.explval) = (yystack_[0].value.explval);
    }
#line 1216 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 73:
#line 652 "mvc_parse.yy" // lalr1.cc:859
    {
      std::list<mvc::Exp *> *exps = new std::list<mvc::Exp *>;
      SymbolExp *name = static_cast<SymbolExp *>((yystack_[0].value.expval));
      exps->push_back(name);
      (yylhs.value.explval) = exps;
    }
#line 1227 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 74:
#line 659 "mvc_parse.yy" // lalr1.cc:859
    {
      SymbolExp *name = static_cast<SymbolExp *>((yystack_[2].value.expval));
      (yystack_[0].value.explval)->push_front(name);
      (yylhs.value.explval) = (yystack_[0].value.explval);
    }
#line 1237 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 75:
#line 668 "mvc_parse.yy" // lalr1.cc:859
    {
      mvc::ExpFactory *ef = ExpFactory::getInstance();
      SymbolExp *symbol = ef->createSymbol(*(yystack_[0].value.strval));
      delete (yystack_[0].value.strval);
      (yylhs.value.expval) = symbol;
    }
#line 1248 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 76:
#line 678 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new mvc::IntegerExp((yystack_[0].value.ival));
    }
#line 1256 "mvc_parse.cc" // lalr1.cc:859
    break;

  case 77:
#line 685 "mvc_parse.yy" // lalr1.cc:859
    {
      (yylhs.value.expval) = new mvc::StringExp(*(yystack_[0].value.strval));
      delete (yystack_[0].value.strval);
    }
#line 1265 "mvc_parse.cc" // lalr1.cc:859
    break;


#line 1269 "mvc_parse.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
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
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
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

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char Parser::yypact_ninf_ = -83;

  const signed char Parser::yytable_ninf_ = -1;

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

  const short int
  Parser::yypgoto_[] =
  {
     -83,   -83,   136,   162,   -52,   -83,   -27,   -83,   -83,   -24,
     -38,   -83,   -83,   -83,   -83,    21,   -83,   -82,   -83,   -83,
     -83,   -83,   -83,   -29,    42,   -83,   -83,   -83,   -83,   -83,
     -36,   -83,    41,   156,   159,     1,   -83,   -83
  };

  const short int
  Parser::yydefgoto_[] =
  {
      -1,     6,    24,    25,    51,     8,     9,    10,    11,    53,
      54,    55,    56,    57,    58,   111,    59,    60,    61,    62,
      63,    64,    65,    66,    86,    67,    68,    69,    70,    71,
      79,   130,   131,    26,    27,    73,    74,    75
  };

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


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
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
  "one_or_more_names", "name", "number", "string", YY_NULLPTR
  };


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
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
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
    const unsigned int user_token_number_max_ = 304;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 13 "mvc_parse.yy" // lalr1.cc:1167
} } // mvc::yy
#line 1735 "mvc_parse.cc" // lalr1.cc:1167
#line 694 "mvc_parse.yy" // lalr1.cc:1168

/* error member function */
void mvc::yy::Parser::error(const mvc::yy::Parser::location_type& l,
                            const std::string& m)
{
  driver.error(l, m);
}
