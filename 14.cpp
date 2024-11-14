#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

#define MIN_LEVEL -1
#define MAX_LEVEL 1
#define UP 1
#define DOWN -1

template <class T> class MinMaxHeap;

template <class T> class Node {
  Node *left, *right, *parent;
  T value;

public:
  friend class MinMaxHeap<T>;
  Node(T data) : left(nullptr), right(nullptr), parent(nullptr), value(data) {};
  Node(T data, Node *p)
      : left(nullptr), right(nullptr), parent(p), value(data) {};
  Node(T data, Node *l, Node *r, Node *p)
      : left(l), right(r), parent(p), value(data) {};

  friend std::ostream &operator<<(std::ostream &out, const Node *n) {
    if (n)
      out << n->value;
    return out;
  };
};

template <class T> class MinMaxHeap {
public:
  MinMaxHeap() : root(nullptr) {}
  ~MinMaxHeap() { deleteNode(root); };

  void deleteNode(Node<T> *n) {
    if (!n)
      return;

    deleteNode(n->left);
    deleteNode(n->right);

    if (n->parent) {
      if (n->parent->left == n)
        n->parent->left = nullptr;
      else
        n->parent->right = nullptr;
    };

    delete n;
  }
  // The insert function in a Min-Max Heap adds a new element to the heap while
  // maintaining its unique properties.
  void insert(T value) {
    if (!root) {
      root = new Node<T>(value);
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

    restoreHeapProperty(addedNode);
  }

  void restoreHeapProperty(Node<T> *n, int direction = UP) {
    if (!n)
      return;

    // When restoring the heap property
    // We should check first with the closest nodes before advancing

    // First confirm that added node has good relationship with the upper nodes
    // Then with the downward nodes while considering the level of each node
    

    if (direction == UP) {
      Node<T> *parent = n->parent;
      Node<T> *pTrav = n;

      // Checking with all the upper parents
      while (parent) {
        if (!isValidRelationship(pTrav, parent)) {
          swap(pTrav, parent);
          pTrav = parent;
          parent = pTrav->parent;
        } else
          parent = parent->parent;
      };
    } else {
      Node<T> *p = n;

      // Check the relationship with all the downward direct children first
      if (!isValidRelationship(n->left, n))
        swap(n, n->left);
      if (!isValidRelationship(n->right, n))
        swap(n, n->right);


      if (getLevel(p) == MIN_LEVEL) {
        if (n->left && n->left->left && n->left->left->value < p->value)
          p = n->left->left;
        if (n->left && n->left->right && n->left->right->value < p->value)
          p = n->left->right;
        if (n->right && n->right->left && n->right->left->value < p->value)
          p = n->right->left;
        if (n->right && n->right->right && n->right->right->value < p->value)
          p = n->right->right;

      } else {
        if (n->left && n->left->left && n->left->left->value > p->value)
          p = n->left->left;
        if (n->left && n->left->right && n->left->right->value > p->value)
          p = n->left->right;
        if (n->right && n->right->left && n->right->left->value > p->value)
          p = n->right->left;
        if (n->right && n->right->right && n->right->right->value > p->value)
          p = n->right->right;
      }

      if (p != n) {
        swap(n, p);
        restoreHeapProperty(p);
        restoreHeapProperty(p->left);
        restoreHeapProperty(p->right);
        restoreHeapProperty(p, DOWN);
      }
    }
  };

  // Retrieve the minimum element in the heap, throw an exception when heap is
  // empty
  T getMin() const {
    if (!root)
      throw std::runtime_error("Heap is empty");
    return root->value;
  }
  // Retrieve the maximum element in the heap, throw an exception when heap is
  // empty
  T getMax() const {
    if (!root)
      throw std::runtime_error("Heap is empty");
    if (!root->left)
      return root->value;
    if (!root->right)
      return root->left->value;

    return (root->left->value > root->right->value) ? root->left->value
                                                    : root->right->value;
  }

  // Delete the minimum element in the heap, throw an exception when heap is
  // empty
  void deleteMin() {
    if (!root)
      throw std::runtime_error("Heap is empty");
    if (!root->left) {
      delete root;
      root = nullptr;
      return;
    }

    Node<T> *lastNode = findLastNode();
    swap(root, lastNode);

    if (lastNode->parent) {
      if (lastNode == lastNode->parent->left)
        lastNode->parent->left = nullptr;
      else
        lastNode->parent->right = nullptr;
    }
    delete lastNode;

    restoreHeapProperty(root, DOWN);
  }

  // Delete the maximum element in the heap, throw an exception when heap is
  // empty
  void deleteMax() {
    if (!root)
      throw std::runtime_error("Heap is empty");
    if (!root->left) {
      delete root;
      root = nullptr;
      return;
    }

    Node<T> *lastNode = findLastNode();
    Node<T> *max = (root->right && root->right->value > root->left->value)
                       ? root->right
                       : root->left;
    swap(max, lastNode);

    if (lastNode->parent) {
      if (lastNode == lastNode->parent->left)
        lastNode->parent->left = nullptr;
      else
        lastNode->parent->right = nullptr;
    }
    delete lastNode;

    if (max != lastNode)
      restoreHeapProperty(max, DOWN);
  }

  Node<T> *findLastNode() {
    Node<T> *p;
    std::queue<Node<T> *> q;
    q.push(root);

    while (!q.empty()) {
      p = q.front();
      q.pop();

      if (p->left)
        q.push(p->left);
      if (p->right)
        q.push(p->right);
    }

    return p;
  };

  void swap(Node<T> *a, Node<T> *b) {
    if (!a || !b || a == b)
      return;
    //std::cout << std::endl;
    //prettyPrint();
    //std::cout << std::endl << a << " <-> " << b << std::endl;

    T temp = a->value;
    a->value = b->value;
    b->value = temp;
  };

  bool isValidRelationship(Node<T> *n, Node<T> *parent) {
    if (!parent || !n)
      return true;

    int parentLevel = getLevel(parent);
    // std::cout << n << " ??? " << parent << std::endl;

    //if (!(parentLevel == MAX_LEVEL && n->value <= parent->value ||
    //        parentLevel == MIN_LEVEL && n->value >= parent->value))
    //  std::cout << "\nInvalid relationship between " << n << " and " << parent << std::endl;       

    return (parentLevel == MAX_LEVEL && n->value <= parent->value ||
            parentLevel == MIN_LEVEL && n->value >= parent->value);
  };

  bool isDownLevelValid(Node<T> *n) {
    int nodeLevel = getLevel(n);
    return (nodeLevel == MAX_LEVEL &&
            (!n->left || n->left && n->value >= n->left->value) &&
            (!n->right || n->right && n->value >= n->right->value)) ||
           (nodeLevel == MIN_LEVEL &&
            (!n->left || n->left && n->value <= n->left->value) &&
            (!n->right || n->right && n->value <= n->right->value));
  };

  int getLevel(Node<T> *n) {
    return (height(n) % 2 == height() % 2) ? MIN_LEVEL : MAX_LEVEL;
  };

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

private:
  // create any member you need to complete MinMaxHap
  Node<T> *root;
};

int main() {
  int data[] = {9, 58, 83, 29, 25, 26, 21, 76, 45, 81};
  int n = sizeof(data) / sizeof(data[0]);
  MinMaxHeap<int> mmHeap;
  int j;

  srand(time(NULL));
  for (j = 0; j < 10; j++)
    mmHeap.insert(rand() % 100);
  // mmHeap.prettyPrint();

  while (true) {
    try {
      std::cout << mmHeap.getMin() << " ";
      mmHeap.deleteMin();
      // std::cout << std::endl;
      // mmHeap.prettyPrint();
    } catch (std::exception) {
      break;
    }
  }
  std::cout << std::endl;

  for (j = 0; j < 10; j++)
    mmHeap.insert(rand() % 100);

  while (true) {
    try {
      std::cout << mmHeap.getMax() << " ";
      mmHeap.deleteMax();
    } catch (std::exception) {
      break;
    }
  }

  return 0;
}
