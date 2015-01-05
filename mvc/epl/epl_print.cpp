/**
 * @file epl_printer.h
 * @brief EPL abstract syntax trees printer
 *
 * @author cjeong
 */
#include <iostream>
#include "mv/epl/epl_util.h"

namespace mv { namespace epl {

/*
 * ExpPrinter
 */
const static size_t INDENT_SIZE = 4;

const static std::string unary_opers[] = {
  "+",  "-"
};

const static std::string binary_opers[] = {
  "+",  "-",  "*",  "/", "<<", ">>", "||", "&&",
  "==", "!=", "<",  "<=", ">",  ">="
};

class Indenter {
public:
  Indenter(std::ostream& os, size_t indent, size_t indent_size) 
    : _os(os), _indent(indent), _indent_size(indent_size) { }
  ~Indenter() { }

  void indent() {
    space(_indent);
  }

  size_t getIndent() { return _indent; }
  void incIndent() { _indent += _indent_size;  }
  void decIndent() { _indent -= _indent_size;  }

  void space(size_t s = 0) {
    for (size_t i = 0; i < s; ++i)
      _os << " ";
  }

private:
  std::ostream& _os;
  size_t _indent;
  size_t _indent_size;
};

class ExpPrinter : public ExpVisitor, public Indenter {
public:
  ExpPrinter(std::ostream& os, size_t indent) 
    : Indenter(os, indent, INDENT_SIZE), _os(os) { }
  ~ExpPrinter() { }

  void visitSymbolExp(SymbolExp *e) {
    _os << e->getName();
  }

  void visitFieldrefExp(FieldrefExp *e) {
    Util::print(_os, e->getVarref(), 0);
    _os << "." << e->getField();
  }

  void visitArrayrefExp(ArrayrefExp *e) {
    Util::print(_os, e->getVarref(), 0);
    _os << "[";
    Util::print(_os, e->getIndex(), 0);
    _os << "]";
  }

  void visitNumberExp(NumberExp *e) {
    _os << e->getValue();
  }
 
 void visitStringExp(StringExp *e) {
    _os << e->getValue();
  }

  void visitAliasExp(AliasExp *e) {
  }

  void visitUnaryExp(UnaryExp *e) {
    _os << " " << unary_opers[e->getUnaryTag()] << " ";
    e->getExp()->accept((ExpVisitor&) *this);
  }

  void visitBinaryExp(BinaryExp *e) {
    _os << "(";
    e->getLexp()->accept((ExpVisitor&) *this);
    _os << " " << binary_opers[e->getBinaryTag()] << " ";
    e->getRexp()->accept((ExpVisitor&) *this);
    _os << ")";
  }

  void visitParamExp(ParamExp *e) {
    Util::print(_os, e->getType(), 0);
    _os << " ";
    Util::print(_os, e->getName(), 0);
  }

  void visitFuncallExp(FuncallExp *e) {
  }

  void visitStreamExp(StreamExp *e) {
    SymbolExp *event = e->getEvent();
    event->accept((ExpVisitor&) *this);
    uint32_t evcount = e->getEventCount();
    if (evcount > 0) {
      _os << "RETAIN " << evcount << " EVENTS";
    }
    TimeExp *time = e->getTime();
    if (time) {
      time->accept((ExpVisitor&) *this);
    }
  }

  void visitTimeExp(TimeExp *e) {
    _os << "TIME " << e->getMilliseconds() << " MILLISECONDS";
  }

private:
  std::ostream& _os;
};

/*
 * StmPrinter
 */
class StmPrinter : public StmVisitor, public Indenter {
public:
  StmPrinter(std::ostream& os, size_t indent)
    : Indenter(os, indent, INDENT_SIZE), _os(os) { }
  ~StmPrinter() { }

  void visitTypedefStm(TypedefStm *s) {
    indent();
    _os << "type ";
    Util::print(_os, s->getName(), getIndent());
    _os << " = ";
    Util::print(_os, s->getType(), getIndent());
    _os << ";" << std::endl;
  }

  void visitEventdefStm(EventdefStm *s) {
    indent();
    _os << "event ";
    Util::print(_os, s->getName(), getIndent());
    _os << " = ";
    Util::print(_os, s->getType(), getIndent());
    _os << "; " << std::endl;
  }

  void visitVardefStm(VardefStm *s) {
    indent();
    _os << "variable ";
    Util::print(_os, s->getName(), getIndent());
    _os << " : ";
    Util::print(_os, s->getType(), getIndent());
    _os << "; " << std::endl;
  }

  void visitProcdefStm(ProcdefStm *s) {
    indent();
    _os << "process ";
    Util::print(_os, s->getName(), getIndent());
    _os << "(";
    _os << ") ";
    Util::print(_os, s->getBody(), getIndent());
    _os << ";" << std::endl;
  }

  void visitFuncdefStm(FuncdefStm *s) {
    indent();
    _os << "function ";
    Util::print(_os, s->getType(), getIndent());
    _os << " ";
    Util::print(_os, s->getName(), getIndent());

    _os << "(";
    std::list<Exp *> *params = s->getParams();
    std::list<Exp *>::iterator iter;
    for (iter = params->begin(); iter != params->end(); ) {
      ParamExp *param = static_cast<ParamExp *>(*iter);
      Util::print(_os, param, 0); 
      ++iter;
      if (iter != params->end()) {
        _os << ", ";
      }
    }
    _os << ") ";

    Util::print(_os, s->getBody(), getIndent());
    _os << ";" << std::endl;
  }

  void visitBlockStm(BlockStm *s) {
    indent();
    _os << "{" << std::endl;
    incIndent();
    std::list<Stm *>& body = s->getBody();
    std::list<Stm *>::iterator iter;
    for (iter = body.begin(); iter != body.end(); ++iter) {
      Util::print(_os, *iter, getIndent());
    }
    decIndent();
    indent();
    _os << "}";
    _os << std::endl;
  }

  void visitIfStm(IfStm *s) {
    indent();
    _os << "if ";
    Util::print(_os, s->getCond(), getIndent());
    if (needLineBreak(s->getStm())) {
      _os << std::endl;
      incIndent();
    }
    Util::print(_os, s->getStm(), getIndent());
    decIndent();
  }

  void visitWhileStm(WhileStm *s) {
    indent();
    _os << "while (";
    Util::print(_os, s->getCond(), 0);
    _os << ") ";
    if (needLineBreak(s->getBody())) {
      _os << std::endl;
    }
    incIndent();
    Util::print(_os, s->getBody(), getIndent());
    decIndent();
  }

  void visitForStm(ForStm *s) {
    indent();
    _os << "for (";
    Util::print(_os, s->getInit(), 0);
    _os << "; ";
    Util::print(_os, s->getCond(), 0);
    _os << "; ";
    Util::print(_os, s->getStep(), 0);
    _os << ") ";
    if (needLineBreak(s->getBody())) {
      _os << std::endl;
    }
    incIndent();
    Util::print(_os, s->getBody(), getIndent());
    decIndent();
  }

  void visitIfElseStm(IfElseStm *s) {
    indent();
    _os << "if ";
    Util::print(_os, s->getCond(), getIndent());
    if (needLineBreak(s->getThenStm())) {
      _os << std::endl;
    }
    incIndent();
    Util::print(_os, s->getThenStm(), getIndent());
    decIndent();
    indent();
    _os << "else ";
    if (needLineBreak(s->getElseStm())) {
      _os << std::endl;
    }
    incIndent();
    Util::print(_os, s->getElseStm(), getIndent());
    decIndent();
  }

  void visitAssignStm(AssignStm *s) {
    indent();
    Util::print(_os, s->getLhs(), getIndent());
    _os << " = ";
    Util::print(_os, s->getRhs(), getIndent());
    _os << ";";
    _os << std::endl;
  }

  void visitTriggerStm(TriggerStm *s) {
    indent();
    _os << "-> ";
    _os << ";";
    _os << std::endl;
  }

  void visitReturnStm(ReturnStm *s) {
    indent();
    _os << "return";
    if (s->getExp()) {
      _os << " ";
      Util::print(_os, s->getExp(), getIndent());
    }
    _os << ";";
    _os << std::endl;
  }

  void visitContinueStm(ContinueStm *s) {
    _os << "continue";
  }

  void visitBreakStm(BreakStm *s) {
  }

  void visitSelectStm(SelectStm *s) {
    _os << "select ";
    std::vector<Exp *> *selectors = s->getSelectors();
    std::vector<Exp *>::iterator viter;
    for (viter = selectors->begin(); viter != selectors->end(); ++viter) {
      Exp *selector = *viter;
      Util::print(_os, selector);
      if (viter + 1 == selectors->end())
        _os << std::endl;
      else
        _os << " ";
    }

    _os << "from ";
    std::vector<Exp *> *streams = s->getStreams();
    for (viter = streams->begin(); viter != streams->end(); ++viter) {
      Exp *stream = *viter;
      Util::print(_os, stream);
      if (viter + 1 == streams->end())
        _os << std::endl;
      else
        _os << " ";
    }

    if (s->getWhere()) {
      _os << "where ";
      Util::print(_os, s->getWhere());
    }
    _os << std::endl;
  }

  void visitDefineStm(DefineStm *s) {
    _os << "define ";
    Util::print(_os, s->getName());
    _os << " ";
    Util::print(_os, s->getDef());
    _os << std::endl;
  }
  
  bool needLineBreak(Stm *stm) {
    switch (stm->getTag()) {
    case ST_BLOCK:
      return false;
    default:
      return true;
    }
  }

private:
  std::ostream& _os;
};

/*
 * DatatypePrinter
 */
class DatatypePrinter : public DatatypeVisitor, public Indenter {
public:
  DatatypePrinter(std::ostream& os, size_t indent)
    : Indenter(os, indent, INDENT_SIZE), _os(os) { }
  ~DatatypePrinter() { }

  void visitVoidDatatype(VoidDatatype *t) {
    _os << "void";
  }

  void visitScalarDatatype(ScalarDatatype *t) {
    switch (t->getTag()) {
    case TT_BOOL: _os << "bool"; break;
    case TT_CHAR: _os << "char"; break;
    case TT_SHORT: _os << "short"; break;
    case TT_INT: _os << "integer"; break;
    case TT_LONG: _os << "long"; break;
    case TT_FLOAT: _os << "float"; break;
    case TT_DOUBLE: _os << "double"; break;
    case TT_STRING: _os << "string"; break;
    default: assert(0 && "Invalid scalar datatype tag");
    }
  }

  void visitArrayDatatype(ArrayDatatype *t) {
    _os << "array [" << t->getSize() << "] of ";
    Util::print(_os, t->getBaseDatatype());
  }

  void visitStructDatatype(StructDatatype *t) {
    _os << "struct {" << std::endl;
    size_t nfields = t->getSize();
    incIndent();
    for (size_t i = 0; i < nfields; ++i) {
      indent();
      _os << t->getField(i) << " : ";
      Util::print(_os, t->getType(i));
      _os << ";" << std::endl;
    }
    decIndent();
    indent();
    _os << "}";
  }

  void visitNamedDatatype(NamedDatatype *t) {
    Util::print(_os, t->getName());
  }

private:
  std::ostream& _os;
};

/* 
 * Util::print
 */
void Util::print(std::ostream& os, Exp *exp, size_t indent)
{
  if (!exp) return;

  ExpPrinter p(os, indent);
  exp->accept(p);
}

void Util::print(std::ostream& os, Stm *stm, size_t indent)
{
  if (!stm) return;

  StmPrinter p(os, indent);
  stm->accept(p);
}

void Util::print(std::ostream& os, Datatype *type, size_t indent)
{
  if (!type) return;

  DatatypePrinter p(os, indent);
  type->accept(p);
}

void Util::print(std::ostream& os, Module *mod)
{
  os << "module ";
  Util::print(os, mod->getName(), 0);
  os << " {" << std::endl;

  std::list<Stm *>& stms = mod->getStms();
  std::list<Stm *>::iterator iter;
  for (iter = stms.begin(); iter != stms.end(); ++iter) {
    Stm *stm = *iter;
    Util::print(os, stm, INDENT_SIZE);
  }

  os << "};" << std::endl;
}

}} /* mv::epl */
