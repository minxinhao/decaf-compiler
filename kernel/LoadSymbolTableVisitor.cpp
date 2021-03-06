// Template
#include "generated/LoadSymbolTableVisitor.h"
#include "indent.h"
#include "static_analyse_driver/class_decl.h"
/*
 public:
  stack<StateType> call_stack;
  ClassEntries top_pool;
  ClassBody current_class;
  InterfaceBody current_interface;
  FuncEntry current_func;
  string current_id;
 private:
  string get_id(node_ptr_t);
*/
string LoadSymbolTableVisitor::get_id(node_ptr_t node) {
  current_id = "@undefined";
  *this << node;
  return current_id;
}
#define HOLD(t) StateHolder sh(call_stack, StateType::t);
void LoadSymbolTableVisitor::visit(Integer *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Double *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(NullPointer *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Call *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Index *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(MemberDot *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(NewArray *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(New *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Read *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(UnaryExpr *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(BinaryExpr *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(This *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Print *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(List *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Break *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Return *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(For *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(While *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Block *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(If *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(Prototype *node) {
  // TODO
  HOLD(PROTOTYPE);
  auto name = get_id(node->identifier);

  if(current_interface.get_function(name)){
    cerr << "overload function not support: " << name;
  }

  auto& container = current_interface.functions;
  auto& entry = current_func = FuncEntry();
  entry.return_type = get_id(node->type);
  for (auto variable_node : node->formals->list) {
    *this << variable_node;
  }
  entry.body = std::nullopt;

  container.append(std::move(name), std::move(entry));
  entry = FuncEntry();
}

void LoadSymbolTableVisitor::visit(Interface *node) {
  HOLD(INTERFACE);
  auto class_name = get_id(node->type_ident);

  auto& container = top_pool;
  if (container.find(class_name)) {
    cerr << "Redeclaration of class: " << class_name << endl;
    exit(-1);
  }

  // auto& mix_entry = top_pool[class_name] = ClassBody();
  auto& entry = current_interface;

  if (node->prototypes) {
    HOLD(FIELD);
    auto list = node->prototypes->list;
    for (auto decl_entry : list) {
      *this << decl_entry;
    }
  }
  container.append(std::move(class_name), std::move(current_interface));
  current_interface = InterfaceBody();
}

void LoadSymbolTableVisitor::visit(ClassDecl *node) {
  HOLD(CLASS);
  auto class_name = get_id(node->type);

  auto& container = top_pool;
  if (container.find(class_name)) {
    cerr << "Redeclaration of class: " << class_name << endl;
    exit(-1);
  }

  // auto& mix_entry = top_pool[class_name] = ClassBody();
  auto& entry = current_class;
  if (node->extender) {
    entry.extender = get_id(node->extender.value());
  }

  if (node->implementor) {
    auto list = node->implementor->list;
    for (auto id_entry : list) {
      entry.implementors.insert(get_id(id_entry));
    }
  }

  if (node->fields) {
    HOLD(FIELD);
    auto list = node->fields->list;
    for (auto decl_entry : list) {
      *this << decl_entry;
    }
  }

  // insert it finally
  container.append(class_name, std::move(current_class));
  current_class = ClassBody();
}

void LoadSymbolTableVisitor::visit(FunctionDecl *node) {
  HOLD(FUNCTION);
  auto name = get_id(node->identifier);

  if(current_class.get_function(name) != nullptr){
    cerr << "Redelaration of function" << name;
  }

  auto& container = current_class.functions;
  auto& entry = current_func = FuncEntry();
  entry.return_type = get_id(node->type);
  for (auto variable_node : node->formals->list) {
    *this << variable_node;
  }
  entry.body = node->body;

  container.append(std::move(name), std::move(entry));
  entry = FuncEntry();
}

void LoadSymbolTableVisitor::visit(TypeArray *node) {
  // TODO
  current_id = get_id(node->base) + "[]";
}

void LoadSymbolTableVisitor::visit(TypeBase *node) {
  // TODO
  string name;
  switch (node->base_type) {
    case T_int:
      name = "int";
      break;
    case T_double:
      name = "double";
      break;
    case T_bool:
      name = "bool";
      break;
    case T_string:
      name = "string";
      break;
    case T_void:
      name = "void";
      break;
    default:
      cerr << "[" << node->base_type << "]";
      name = "@WTF";
  }
  current_id = name;
}

void LoadSymbolTableVisitor::visit(TypeUser *node) {
  // TODO
  current_id = node->type_name;
}

void LoadSymbolTableVisitor::visit(Identifier *node) {
  // TODO
  current_id = node->name;
}

void LoadSymbolTableVisitor::visit(Assign *node) {
  // TODO
}

void LoadSymbolTableVisitor::visit(TypedVariable *node) {
  // TODO
  auto type = get_id(node->type);
  auto id = get_id(node->id);
  switch (call_stack.top()) {
    case StateType::FIELD: {
      auto& entry = current_class;
      if (entry.get_variable(id) != nullptr) {
        cerr << "redeclaration of id: " << id << endl;
        exit(-1);
      }
      entry.variables.append(id, type);
      break;
    }
    case StateType::FUNCTION: 
    case StateType::PROTOTYPE: 
    {
      auto& entry = current_func;
      entry.parameters.emplace_back(type, id);
      break;
    }
  }
}

void LoadSymbolTableVisitor::visit(Program *node) {
  HOLD(PROGRAM);
  current_id = "@undefined";
  // *this << node->decls;
  
  for(auto entry: node->decls->list){
    *this << entry;
  }
}

void LoadSymbolTableVisitor::visit(NoAction *node) {
  // TODO
}


