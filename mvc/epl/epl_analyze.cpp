/**
 * @file epl_analyze.h
 * @brief EPL semantic analysis.
 *
 * @author cjeong
 */
#include <iostream>
#include "mv/epl/epl_analyzer.h"
#include "mv/epl/epl_symtab.h"

namespace mv { namespace epl {

/*
 * ExpAnalyzer
 */
class ExpAnalyzer : public ExpVisitor {
public:
  ExpAnalyzer(SymbolTable *symtab) : _symtab(symtab) { }
  ~ExpAnalyzer() { }

  void visitSymbolExp(SymbolExp *e) {
  }

  void visitFieldrefExp(FieldrefExp *e) {
  }

  void visitArrayrefExp(ArrayrefExp *e) {
  }

  void visitNumberExp(NumberExp *e) {
  }

  void visitStringExp(StringExp *e) {
  }

  void visitAliasExp(AliasExp *e) {
  }

  void visitUnaryExp(UnaryExp *e) {
  }

  void visitBinaryExp(BinaryExp *e) {
  }

  void visitParamExp(ParamExp *e) {
  }

  void visitFuncallExp(FuncallExp *e) {
  }

  void visitStreamExp(StreamExp *e) {
  }

  void visitTimeExp(TimeExp *e) {
  }

private:
  SymbolTable *_symtab;
};

/*
 * StmAnalyzer
 */
class StmAnalyzer : public StmVisitor {
public:
  StmAnalyzer(SymbolTable *symtab) : _symtab(symtab) { }
  ~StmAnalyzer() { }

  void visitTypedefStm(TypedefStm *s) {
    SymbolExp *name = s->getName();
    Datatype *datatype = s->getType();

    Value *value = _symtab->lookup(name);
    if (!value) {
      _symtab->add(name, datatype);
    }
    
  }

  void visitEventdefStm(EventdefStm *s) {
  }

  void visitVardefStm(VardefStm *s) {
  }

  void visitProcdefStm(ProcdefStm *s) {
  }

  void visitFuncdefStm(FuncdefStm *s) {
  }


  void visitBlockStm(BlockStm *s) {
  }

  void visitIfStm(IfStm *s) {
  }

  void visitWhileStm(WhileStm *s) {
  }

  void visitForStm(ForStm *s) {
  }

  void visitIfElseStm(IfElseStm *s) {
  }

  void visitAssignStm(AssignStm *s) {
  }

  void visitTriggerStm(TriggerStm *s) {
  }

  void visitReturnStm(ReturnStm *s) {
  }

  void visitContinueStm(ContinueStm *s) {
  }

  void visitBreakStm(BreakStm *s) {
  }

  void visitSelectStm(SelectStm *s) {
  }

  void visitDefineStm(DefineStm *s) {
  }
  
private:
  SymbolTable *_symtab;
};

/*
 * DatatypeAnalyzer
 */
class DatatypeAnalyzer : public DatatypeVisitor {
public:
  DatatypeAnalyzer(SymbolTable *symtab) : _symtab(symtab) { }
  ~DatatypeAnalyzer() { }

  void visitVoidDatatype(VoidDatatype *t) {
  }

  void visitScalarDatatype(ScalarDatatype *t) {
  }

  void visitArrayDatatype(ArrayDatatype *t) {
    //Datatype *basetype = t->getBaseDatatype();
  }

  void visitStructDatatype(StructDatatype *t) {
  }

  void visitNamedDatatype(NamedDatatype *t) {
    SymbolExp *name = t->getName();
    Value *value = _symtab->lookup(name);
    MV_ASSERT(!value && value->getValueTag() == VT_DATATYPE);
    Datatype *datatype = static_cast<Datatype *>(value);
    
  }

private:
  SymbolTable *_symtab;
};


}} /* mv::epl */
