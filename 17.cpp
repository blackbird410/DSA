// PREPEND BEGIN
/*some text*/
// PREPEND END

// TEMPLATE BEGIN
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#define LEFT -1
#define RIGHT 1
#define LL 1
#define RR 2
#define LR 3
#define RL 4
#define INVALID -1

template <class T> class Node {
public:
  Node() { data = new T; }
  Node(T d) {
    data = new T;
    (*data) = d;
  }
  Node &operator=(T d) {
    (*data) = d;
    return *this;
  }
  friend std::ostream &operator<<(std::ostream &out, Node n) {
    if (!n.data)
      return out;
    out << *(n.data);
    return out;
  }
  friend std::ostream &operator<<(std::ostream &out, Node *n) {
    if (!n)
      return out;
    out << *(n->data);
    return out;
  }
  void setData(T d) { *data = d; }
  T &getData() const { return *data; }

protected:
  T *data;
};

template <class T> class BinaryTreeNode : public Node<T> {
public:
  BinaryTreeNode() : Node<T>() {
    left = nullptr;
    right = nullptr;
  }
  BinaryTreeNode(T d) : Node<T>(d) {
    left = nullptr;
    right = nullptr;
  }
  BinaryTreeNode(BinaryTreeNode<T> *l, BinaryTreeNode<T> *r) : Node<T>() {
    left = l;
    right = r;
  }
  BinaryTreeNode(T d, BinaryTreeNode<T> *l, BinaryTreeNode<T> *r) : Node<T>(d) {
    left = l;
    right = r;
  }
  void setLeft(BinaryTreeNode<T> *l) { left = l; }
  void setRight(BinaryTreeNode<T> *r) { right = r; }
  BinaryTreeNode<T> *&getLeft() { return left; }
  BinaryTreeNode<T> *&getRight() { return right; }
  const BinaryTreeNode<T> *getLeft() const { return left; }
  const BinaryTreeNode<T> *getRight() const { return right; }

  bool operator>(BinaryTreeNode<T> n) {
    if (*(this->data) > *(n.data))
      return true;
    return false;
  }
  bool operator==(BinaryTreeNode<T> n) {
    if (*(this->data) == *(n.data))
      return true;
    return false;
  }

private:
  BinaryTreeNode<T> *left, *right;
};

template <class T> class AVLTree {
private:
  BinaryTreeNode<T> *root;
  void inorder(BinaryTreeNode<T> *cur, int n) {
    if (cur == nullptr)
      return;
    inorder(cur->getRight(), n + 1);
    int j;
    for (j = 0; j < n; j++)
      std::cout << "  ";
    std::cout << cur << std::endl;
    inorder(cur->getLeft(), n + 1);
  }

public:
  AVLTree() : root(nullptr) {};
  void destroyTree(BinaryTreeNode<T> *n) {
    if (!n)
      return;
    destroyTree(n->getLeft());
    destroyTree(n->getRight());
    delete n;
  }

  ~AVLTree() { destroyTree(root); }

  int balanceFactor(BinaryTreeNode<T> *n) {
    return (n) ? abs(height(n->getLeft()) - height(n->getRight())) : 0;
  };

  void swap(BinaryTreeNode<T> *a, BinaryTreeNode<T> *b) {
    if (!a || !b || a == b)
      return;

    T tmp = a->getData();
    a->setData(b->getData());
    b->setData(tmp);
  };

  void rotate(BinaryTreeNode<T> *n, int direction) {
    if (!n)
      return;
    // printTree();

    BinaryTreeNode<T> *tmp, *curRoot, *nextRoot;
    switch (direction) {
    case RR:
      if (!n->getRight())
        return;
      // std::cout << "RR Rotation at " << n << std::endl;

      // Swap => Avoid breaking parent child relationship
      swap(n, n->getRight());
      tmp = n->getLeft();
      n->setLeft(n->getRight());
      n->setRight(n->getLeft()->getRight());
      n->getLeft()->setRight(n->getLeft()->getLeft());
      n->getLeft()->setLeft(tmp);
      break;
    case LL:
      if (!n->getLeft())
        return;
      // std::cout << "LL Rotation at " << n << std::endl;

      swap(n, n->getLeft());
      tmp = n->getRight();
      n->setRight(n->getLeft());
      n->setLeft(n->getRight()->getLeft());
      n->getRight()->setLeft(n->getRight()->getRight());
      n->getRight()->setRight(tmp);
      break;
    case LR:
      if (!n->getLeft())
        return;
      // std::cout << "LR Rotation at " << n << std::endl;

      rotate(n->getLeft(), RR);
      rotate(n, LL);
      break;
    case RL:
      if (!n->getRight())
        return;
      // std::cout << "RL Rotation at " << n << std::endl;

      rotate(n->getRight(), LL);
      rotate(n, RR);
      break;
    default:
      break;
    }
  };

  int getRotationDirection(BinaryTreeNode<T> *n) {
    if (!n)
      return INVALID;
    int hLeft = height(n->getLeft()), hRight = height(n->getRight());

    if (hLeft > hRight) {
      BinaryTreeNode<T> *leftChild = n->getLeft();
      return (height(leftChild->getLeft()) >= height(leftChild->getRight()))
                 ? LL
                 : LR;
    } else {
      BinaryTreeNode<T> *rightChild = n->getRight();
      return (height(rightChild->getRight()) >= height(rightChild->getLeft()))
                 ? RR
                 : RL;
    }
  }

  void restoreBalance(BinaryTreeNode<T> *n) {
    int bf = balanceFactor(n);
    if (bf < 2)
      return;

    rotate(n, getRotationDirection(n));
  };

  BinaryTreeNode<T> *findLowestImbalancedNode(BinaryTreeNode<T> *root) {
    if (!root)
      return nullptr;

    BinaryTreeNode<T> *left = findLowestImbalancedNode(root->getLeft());
    BinaryTreeNode<T> *right = findLowestImbalancedNode(root->getRight());

    return (balanceFactor(left) >= 2)    ? left
           : (balanceFactor(right) >= 2) ? right
           : (balanceFactor(root) >= 2)  ? root
                                         : nullptr;
  };

  void insert(BinaryTreeNode<T> *n, T d) {
    if (!n)
      return;

    if (d < n->getData()) {
      if (n->getLeft())
        insert(n->getLeft(), d);
      else
        n->getLeft() = new BinaryTreeNode<T>(d);
    } else {
      if (n->getRight())
        insert(n->getRight(), d);
      else
        n->getRight() = new BinaryTreeNode<T>(d);
    }

    restoreBalance(findLowestImbalancedNode(root));
  }

  void insert(T d) {
    if (!root) {
      root = new BinaryTreeNode<T>(d);
      return;
    };
    insert(root, d);
  };
  void inorder() { inorder(root, 0); }

  int height() const { return height(root); };
  int height(const BinaryTreeNode<T> *n) const {
    if (!n)
      return 0;

    int heightLeft = height(n->getLeft()) + 1;
    int heightRight = height(n->getRight()) + 1;

    return (heightLeft > heightRight) ? heightLeft : heightRight;
  };

  void printTree() const {
    if (!root) {
      std::cout << "The tree is empty." << std::endl;
      return;
    }

    int h = height(root);
    int maxWidth = std::pow(2, h) - 1;

    // Use a queue-like approach to store nodes for each level
    std::vector<BinaryTreeNode<T> *> currentLevel = {root};
    std::vector<BinaryTreeNode<T> *> nextLevel;

    int depth = 0;
    while (!currentLevel.empty() && depth < h) {
      int indent = std::pow(2, h - depth - 1) - 1;
      int betweenSpaces = std::pow(2, h - depth) - 1;

      // Print leading indent
      std::cout << std::string(indent * 2, ' ');

      // Print the nodes at the current level
      for (auto node : currentLevel) {
        if (node) {
          std::cout << std::setw(2) << node->getData();
          nextLevel.push_back(node->getLeft());
          nextLevel.push_back(node->getRight());
        } else {
          std::cout << "  "; // Placeholder for an empty node
          nextLevel.push_back(nullptr);
          nextLevel.push_back(nullptr);
        }

        // Print spaces between nodes
        std::cout << std::string(betweenSpaces * 2, ' ');
      }
      std::cout << std::endl;

      // Move to the next level
      currentLevel = nextLevel;
      nextLevel.clear();
      depth++;
    }

    for (int i = 0; i < maxWidth; i++)
      std::cout << "__";
    std::cout << std::endl;
  }
};

void genRandomInputs(int *arr, int n) {
  srand(0);
  for (int i = 0; i < n; i++)
    arr[i] = rand() % 100;
};

int main() {
  AVLTree<int> *tree = new AVLTree<int>();

  char inputs[] = {'H', 'I', 'J', 'B', 'A', 'E', 'C', 'F', 'D', 'G', 'K', 'L'};
  // int n = sizeof(inputs) / sizeof(inputs[0]);
  int n = 20;
  int arr[n] = {};
  genRandomInputs(arr, n);

  int j;
  for (j = 0; j < n; j++) {
    tree->insert(arr[j]);
    tree->inorder();
    // tree->printTree();
  }

  tree->printTree();
  for (const auto &i : arr)
    std::cout << i << " ";
  std::cout << std::endl;
}
