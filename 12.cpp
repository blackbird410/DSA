#include <iostream>
#include <stdexcept>
#include <string>

// Input
// ( -12 + 3 ) * 4 % 5

// Output
// Inorder: (((-12 + 3 )* 4 )% 5 )
// Preorder: % * + -12 3 4 5
// Postorder: -12 3 + 4 * 5 %
// Evaluated result: -1

#define OPERATOR 1
#define OPERAND 0
#define SPACE -1

#define OP_PAR '('
#define CL_PAR ')'

#define PREORDER 1
#define INORDER 0
#define POSTORDER -1

int inputType(char c);
std::string getSubExpression(std::string &expression);

class ExpressionTree {
private:
  struct Node {
    std::string value;
    Node *left;
    Node *right;

    void status() {
      std::cout << "Node: " << value << std::endl;
      std::cout << "LEFT: " << left << std::endl;
      std::cout << "RIGHT: " << right << std::endl;
    }

    Node(const std::string &val) : value(val), left(nullptr), right(nullptr) {}
    friend std::ostream &operator<<(std::ostream &os, const Node *node) {
      if (node)
        os << node->value;
      return os;
    }
  };

  Node *root;

public:
  // Constructor of class ExpressionTree take a string as an infix expression
  // and build the expression tree base on that. there are five different
  // operation you have to consider, +(addition), -(subtraction),
  // *(multiplication), /(division) and %(modulus). you also need to consider
  // -(negative) sign. all operands are integer

  ExpressionTree(const std::string &expression) : root(nullptr) {
    int expressionLength = expression.length();
    if (expressionLength == 0)
      return;

    std::string temp;
    Node *newNode;

    // if (root) root->status();
    // std::cout << std::endl;

    for (int i = 0; i < expressionLength; ++i) {
      if (std::isspace(expression[i]) || expression[i] == CL_PAR)
        continue;

      // std::cout << "Processing: " << expression[i] << std::endl;
      switch (inputType(expression[i])) {
      case OP_PAR: {
        std::string tempExpression = expression.substr(i + 1); // Skip '('
        std::string subExp = getSubExpression(tempExpression);
        i += subExp.length() + 1;

        ExpressionTree *subTree = new ExpressionTree(subExp);
        if (!root)
          root = subTree->root;
        else if (!root->left)
          root->left = subTree->root;
        else
          root->right = subTree->root;
        break;
      }
      case OPERATOR: {
        if (expression[i] == '-' && expression[i + 1] != ' ') {
          temp = "-";
          i++; // Skip the minus sign
          while (i < expressionLength && std::isdigit(expression[i])) {
            temp += expression[i++];
          }
          --i;

          newNode = new Node(temp);
          if (!root) {
            root = newNode;
          } else if (!root->left) {
            root->left = newNode;
          } else {
            root->right = newNode;
          }

        } else {
          // if (root) root->status();
          temp = expression[i];
          newNode = new Node(temp);
          newNode->left = root;
          root = newNode;
          // root->status();
        }
        break;
      }
      case OPERAND: {
        temp = "";
        while (i < expressionLength && std::isdigit(expression[i])) {
          temp += expression[i++];
        }
        --i;

        newNode = new Node(temp);
        if (!root)
          root = newNode;
        else if (!root->left)
          root->left = newNode;
        else
          root->right = newNode;

        // root->status();
        break;
      }
      default:
        break;
      }
    }
  }

  ~ExpressionTree() { deleteTree(root); }

  void deleteTree(Node *node) {
    if (node == nullptr)
      return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }

  void traverse(Node *root, int order) {
    if (!root->left || !root->right) {
      std::cout << root->value << " ";
      return;
    }

    switch (order) {
    case PREORDER:
      std::cout << root->value << " ";
      traverse(root->left, order);
      traverse(root->right, order);
      break;
    case INORDER:
      std::cout << "(";
      traverse(root->left, order);
      std::cout << root->value << " ";
      traverse(root->right, order);
      std::cout << ")";
      break;
    case POSTORDER:
      traverse(root->left, order);
      traverse(root->right, order);
      std::cout << root->value << " ";
      break;
    default:
      break;
    }
  }

  // print all element in infix order.
  void printInFixOrder() {
    if (!root)
      return;
    traverse(root, INORDER);
    std::cout << std::endl;
  }
  // print all element in prefix order.
  void printPreFixOrder() {
    if (!root)
      return;
    traverse(root, PREORDER);
    std::cout << std::endl;
  }
  // print all element in postfix order.
  void printPostFixOrder() {
    if (!root)
      return;
    traverse(root, POSTORDER);
    std::cout << std::endl;
  }
  // evaluate the expression, this function return an int sience % can only work
  // with integer in C
  int evaluate(Node *root) {
    if (!root->left && !root->right) {
      return std::stoi(root->value);
    }

    switch (root->value[0]) {
    case '+':
      return evaluate(root->left) + evaluate(root->right);
    case '-':
      return evaluate(root->left) - evaluate(root->right);
    case '*':
      return evaluate(root->left) * evaluate(root->right);
    case '/':
      // Assuming that the second expression is not returning 0
      return evaluate(root->left) / evaluate(root->right);
    case '%':
      return evaluate(root->left) % evaluate(root->right);
    default:
      return 0;
    }
  };

  int eval() { return evaluate(root); }
};

int main() {
  std::string infix = "( -12 + 3 ) * 4 % 5";
  // std::cin >> infix;
  ExpressionTree exprTree(infix);

  std::cout << "Inorder: ";
  exprTree.printInFixOrder();
  std::cout << "Preorder: ";
  exprTree.printPreFixOrder();
  std::cout << "Postorder: ";
  exprTree.printPostFixOrder();
  std::cout << "Evaluated result: " << exprTree.eval() << std::endl;

  return 0;
}

int inputType(char c) {
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
    return OPERATOR;
  } else if (std::isdigit(c)) {
    return OPERAND;
  } else if (c == OP_PAR) {
    return OP_PAR;
  } else if (c == CL_PAR) {
    return CL_PAR;
  } else if (std::isspace(c)) {
    return SPACE;
  }
  return SPACE;
}

std::string getSubExpression(std::string &expression) {
  int l = expression.length();
  if (l == 0)
    return "";

  std::string subExpression = "";
  int count = 1; // Start counting parentheses from the first one we encounter

  // Start from the character after '('
  for (int i = 1; i < l; ++i) {
    if (expression[i] == OP_PAR) {
      count++;
    } else if (expression[i] == CL_PAR) {
      count--;
    }
    if (count == 0)
      break; // Found the matching closing parenthesis

    subExpression += expression[i];
  }

  return subExpression;
}
