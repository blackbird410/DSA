#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

template <class T> class MaxHeap;
template <class T> class Node {
private:
  Node<T> *left, *right, *parent;
  T value;

public:
  Node(T d) : value(d), left(nullptr), right(nullptr), parent(nullptr) {}
  Node(T d, Node<T> *p) : value(d), left(nullptr), right(nullptr), parent(p) {}
  friend std::ostream &operator<<(std::ostream &out, const Node<T> *n) {
    if (n)
      out << n->value;
    return out;
  }
  friend class MaxHeap<T>;
};

template <class T> class MaxHeap {
  Node<T> *root;

public:
  // Constructing a empty heap.
  MaxHeap() : root(nullptr) {}
  ~MaxHeap() { deleteNode(root); }

  void deleteNode(Node<T> *n) {
    if (n) {
      deleteNode(n->left);
      deleteNode(n->right);
      delete n;
    }
  }

  // To add an element to the heap.
  void insert(T value) {
    if (!root) {
      root = new Node(value);
      return;
    }

    Node<T> *addedNode;
    std::queue<Node<T> *> q;
    q.push(root);

    while (!q.empty()) {
      Node<T> *p = q.front();
      q.pop();

      if (!p->left) {
        p->left = new Node<T>(value, p);
        addedNode = p->left;
        break;
      } else {
        q.push(p->left);
      }

      if (!p->right) {
        p->right = new Node<T>(value, p);
        addedNode = p->right;
        break;
      } else {
        q.push(p->right);
      }
    }

    // Restore heap property
    restoreHeapProperty(addedNode);
  }

  void restoreHeapProperty(Node<T> *n) {
    if (!n || !n->parent)
      return;

    if (n->value > n->parent->value) {
      swap(n, n->parent);
      restoreHeapProperty(n->parent);
    }
  }

  T extractLastNode(Node<T> *n, Node<T> *parent, bool isLeft) {
    if (!n->left && !n->right) {
      T value = n->value;
      delete n;

      if (!parent) {
        root = nullptr;
        return value;
      }

      if (isLeft)
        parent->left = nullptr;
      else
        parent->right = nullptr;

      return value;
    }

    return (n->right) ? extractLastNode(n->right, n, false)
                      : extractLastNode(n->left, n, true);
  };

  T extractLastNode() { return extractLastNode(root, nullptr, false); };

  void swap(Node<T> *a, Node<T> *b) {
    T temp = a->value;
    a->value = b->value;
    b->value = temp;
  };

  // To remove the root element in the heap and return it.
  T extract() {
    if (!root)
      return 0;

    T rootData = root->value;

    T lastNodeData = extractLastNode(root, nullptr, false);

    if (!root)
      return rootData;

    root->value = lastNodeData;

    // Restore the heap property
    Node<T> *pTrav = root;
    while (pTrav) {
      Node<T> *maxChild = nullptr;

      if (pTrav->left && pTrav->right) {
        maxChild = (pTrav->left->value > pTrav->right->value) ? pTrav->left
                                                              : pTrav->right;
      } else if (pTrav->left) {
        maxChild = pTrav->left;
      }

      if (!maxChild || pTrav->value >= maxChild->value)
        break;

      swap(pTrav, maxChild);

      pTrav = maxChild;
    }

    return rootData;
  }

  // Return the number of element int the heap.
  int count() { return count(root); };

  int count(Node<T> *n) {
    if (!n)
      return 0;
    return count(n->left) + count(n->right) + 1;
  };

  void print() {
    print(root);
    std::cout << std::endl;
  }

  void print(Node<T> *n) {
    if (!n)
      return;
    std::cout << n->value << " ";
    print(n->left);
    print(n->right);
  }

  int height() const { return height(root); };
  int height(Node<T> *n) const {
    if (!n)
      return 0;

    int heightLeft = height(n->left) + 1;
    int heightRight = height(n->right) + 1;

    return (heightLeft > heightRight) ? heightLeft : heightRight;
  };

  void prettyPrint() const {
    if (!root) {
      std::cout << "The tree is empty." << std::endl;
      return;
    }

    int h = height(root);
    int maxWidth = std::pow(2, h) - 1;

    // Use a queue-like approach to store nodes for each level
    std::vector<Node<T> *> currentLevel = {root};
    std::vector<Node<T> *> nextLevel;

    int depth = 0;
    while (!currentLevel.empty() && depth < h) {
      int indent = std::pow(2, h - depth - 1) - 1;
      int betweenSpaces = std::pow(2, h - depth) - 1;

      // Print leading indent
      std::cout << std::string(indent * 2, ' ');

      // Print the nodes at the current level
      for (auto node : currentLevel) {
        if (node) {
          std::cout << std::setw(2) << node->value;
          nextLevel.push_back(node->left);
          nextLevel.push_back(node->right);
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

int main() {
  srand(time(NULL));
  MaxHeap<int> *m = new MaxHeap<int>();
  for (int i = 0; i < 20; i++)
    m->insert(rand() % 100);

  m->prettyPrint();
  while (m->count()) {
    std::cout << m->extract() << " ";
  }
  std::cout << std::endl;

  delete m;
  return 0;
}
