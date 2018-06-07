#include "../common.h"
inline auto mkInteger(int num) {
  auto node = make_shared<Integer>();
  node->num = num;
 // Generated
  return node;
}

inline auto mkBinaryExpr(node_ptr_t left, int op, node_ptr_t right) {
  auto node = make_shared<BinaryExpr>();
  node->left = left;
  node->op = op;
  node->right = right;
 // Generated
  return node;
}

inline auto mkBlock() {
  auto node = make_shared<Block>();
 // Generated
  return node;
}
