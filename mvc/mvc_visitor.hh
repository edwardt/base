/**
 * @file mvc_visitor.hh
 *
 * @brief Visitor classes over Stm and Exp objects
 */
#ifndef MVC_VISITOR_HH
#define MVC_VISITOR_HH

namespace mvc {

/* Visitor for Exp */
class SymbolExp;
class FieldrefExp;
class ArrayrefExp;
class IntegerExp;
class FloatExp;
class StringExp;
class UnaryExp;
class BinaryExp;
class FuncallExp;
class TimeExp;
class ExpVisitor {
public:
  virtual void visitSymbolExp(SymbolExp *e) = 0;
  virtual void visitFieldrefExp(FieldrefExp *e) = 0;
  virtual void visitArrayrefExp(ArrayrefExp *e) = 0;
  virtual void visitIntegerExp(IntegerExp *e) = 0;
  virtual void visitFloatExp(FloatExp *e) = 0;
  virtual void visitStringExp(StringExp *e) = 0;
  virtual void visitUnaryExp(UnaryExp *e) = 0;
  virtual void visitBinaryExp(BinaryExp *e) = 0;
  virtual void visitFuncallExp(FuncallExp *e) = 0;
  virtual void visitTimeExp(TimeExp *e) = 0;
};

/* Visitor for Stm */
class TypedefStm;
class EventdefStm;
class VardefStm;
class ProcdefStm;
class FuncdefStm;
class BlockStm;
class IfStm;
class IfElseStm;
class WhileStm;
class ForStm;
class AssignStm;
class FuncallStm;
class TriggerStm;
class ReturnStm;
class ContinueStm;
class BreakStm;
class DefineStm;
class StmVisitor {
public:
  virtual void visitEventdefStm(EventdefStm *s) = 0;
  virtual void visitVardefStm(VardefStm *s) = 0;
  virtual void visitProcdefStm(ProcdefStm *s) = 0;
  virtual void visitFuncdefStm(FuncdefStm *s) = 0;
  virtual void visitBlockStm(BlockStm *s) = 0;
  virtual void visitIfStm(IfStm *s) = 0;
  virtual void visitIfElseStm(IfElseStm *s) = 0;
  virtual void visitWhileStm(WhileStm *s) = 0;
  virtual void visitForStm(ForStm *s) = 0;
  virtual void visitAssignStm(AssignStm *s) = 0;
  virtual void visitFuncallStm(FuncallStm *s) = 0;
  virtual void visitTriggerStm(TriggerStm *s) = 0;
  virtual void visitReturnStm(ReturnStm *s) = 0;
  virtual void visitContinueStm(ContinueStm *s) = 0;
  virtual void visitBreakStm(BreakStm *s) = 0;

  virtual void visitDefineStm(DefineStm *s) = 0;
};

} /* mvc */

#endif /* MVC_VISITOR_HH */
