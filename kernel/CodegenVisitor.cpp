// Template
#include "generated/CodegenVisitor.h"
#include "indent.h"
#include "llvm_driver/llvm_inc.h"
/*
 private:
  class LLVMEngine& eng;
  llvm::Value* rt_value;
  llvm::Type* rt_type;
  BlockExt* block_aux;
  stack<StateType> call_stack;
  bool right_value;
 public:
  CodegenVisitor& operator<<(node_ptr_t node){
    node->accept(*this);
    return *this;
  }
  CodegenVisitor(class LLVMEngine& eng, BlockExt* block_aux);
  llvm::Value* get_value(node_ptr_t node, bool rvalue = true);
  llvm::Type* get_type(node_ptr_t node);
*/
#define HOLD(t) StateHolder sh(call_stack, StateType::t);
using namespace llvm;
#include "llvm_driver/llvm.h"
CodegenVisitor::CodegenVisitor(LLVMEngine &eng, BlockExt *block_aux)
    : eng(eng), block_aux(block_aux) {
  call_stack.push(StateType::PROGRAM);
}

Value *CodegenVisitor::get_value(node_ptr_t node, bool is_right_value) {
  HOLD(GET_VALUE);
  right_value = is_right_value;
  rt_value = nullptr;
  *this << node;
  assert(rt_value);
  return rt_value;
}
Type *CodegenVisitor::get_type(node_ptr_t node) {
  HOLD(GET_TYPE);
  rt_type = nullptr;
  *this << node;
  assert(rt_type);
  return rt_type;
}

void CodegenVisitor::visit(Integer* node) {
  assert(right_value);
  assert(call_stack.top() == StateType::GET_VALUE);
  rt_value = ConstantInt::get(eng.getContext(), APInt(32, node->num, true));
}

void CodegenVisitor::visit(Double* node) {
  assert(right_value);
  assert(call_stack.top() == StateType::GET_VALUE);
  rt_value = ConstantFP::get(Type::getDoubleTy(eng.getContext()), node->num);
}

void CodegenVisitor::visit(NullPointer* node) {
  // TODO
  rt_value = ConstantPointerNull::get(eng.get_user_type(node->token_type));
}

void CodegenVisitor::visit(Call* node) {
  // TODO
}

void CodegenVisitor::visit(Index* node) {
  // TODO
}

void CodegenVisitor::visit(MemberDot* node) {
  // TODO
}

void CodegenVisitor::visit(NewArray* node) {
  // TODO
}

void CodegenVisitor::visit(New* node) {
  // TODO
}

void CodegenVisitor::visit(Read* node) {
  // TODO
}

void CodegenVisitor::visit(UnaryExpr* node) {
  // TODO
}

void CodegenVisitor::visit(BinaryExpr* node) {
  // TODO
}

void CodegenVisitor::visit(This* node) {
  // TODO
}

void CodegenVisitor::visit(Print* node) {
  // TODO
}

void CodegenVisitor::visit(List* node) {
  // TODO
  switch (call_stack.top()) {
  case StateType::BLOCK: {
    for (auto ptr: node->list) {
      *this << ptr;
    }
    break;
  }
  default: assert(false);
  }
}

void CodegenVisitor::visit(Break* node) {
  // TODO
}

void CodegenVisitor::visit(Return* node) {
  eng().CreateRet(get_value(node->expr));
}

void CodegenVisitor::visit(For* node) {
  // TODO
}

void CodegenVisitor::visit(While* node) {
  // TODO
}

void CodegenVisitor::visit(Block* node) {
  // TODO
  HOLD(BLOCK);
  for (auto&[name, variable]: node->aux.local_uid) {
    auto&[uid, type] = variable;
    eng.define_local_variable(uid, type);
  }
  *this << node->stmt_list;
}

void CodegenVisitor::visit(If* node) {
  // TODO
}

void CodegenVisitor::visit(Prototype* node) {
  // SKIP
}

void CodegenVisitor::visit(Interface* node) {
  // SKIP
}

void CodegenVisitor::visit(ClassDecl* node) {
  // SKIP
}

void CodegenVisitor::visit(FunctionDecl* node) {
  // SKIP
}

void CodegenVisitor::visit(TypeArray* node) {
  // TODO
}

void CodegenVisitor::visit(TypeBase* node) {
  // TODO
}

void CodegenVisitor::visit(TypeUser* node) {
  // TODO
}

void CodegenVisitor::visit(Identifier* node) {
  // TODO
  switch (call_stack.top()) {
  case StateType::GET_VALUE: {
    rt_value = eng.fetch_local_id(node->uid);
    if(right_value){
      rt_value = eng().CreateLoad(rt_value, "ld");
    }
    break;
  }
  }
}

void CodegenVisitor::visit(Assign* node) {
  // TODO
  if(node->right->token_type == "nullptr");
  node->right->token_type = node->left->token_type;
  auto right = get_value(node->right);
  auto left = get_value(node->left, false);
  eng().CreateStore(right, left);
  rt_value = left;
}

void CodegenVisitor::visit(TypedVariable* node) {
  // TODO
}

void CodegenVisitor::visit(Program* node) {
  // TODO
}

void CodegenVisitor::visit(NoAction* node) {
  // TODO
}


