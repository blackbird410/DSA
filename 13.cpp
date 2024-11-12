#include <iostream>

template <class T> class MaxHeap {
  class Node {
  private:
    Node *left, *right;
    T data;

  public:
    Node(T d) : data(d), left(nullptr), right(nullptr) {}
    friend std::ostream &operator<<(std::ostream &out, const Node *n) {
      if (n)
        out << n->data;
      return out;
    }
    friend class MaxHeap;
  };

  Node *root;

public:
  // Constructing a empty heap.
  MaxHeap() : root(nullptr) {}
  ~MaxHeap() { deleteNode(root); }

  void deleteNode(Node *n) {
    if (n) {
      deleteNode(n->left);
      deleteNode(n->right);
      delete n;
    }
  }

  // To add an element to the heap.
  void insert(T data) {
    if (!root) {
      root = new Node(data);
      return;
    }
    insert(data, root);
  }

  void insert(T data, Node *root) {
    if (root->data < data) {
      T replacedData = root->data;
      root->data = data;
      insert(replacedData, this->root);
    } else {
      if (!root->left)
        root->left = new Node(data);
      else if (root->left->data < data)
        insert(data, root->left);
      else if (!root->right)
        root->right = new Node(data);
      else
        insert(data, root->right);
    }
  }

  T extractLastNode(Node *n, Node *parent, bool isLeft) {
    if (!n->left && !n->right) {
      T data = n->data;
      delete n;

      if (!parent) {
        root = nullptr;
        return data;
      }

      if (isLeft)
        parent->left = nullptr;
      else
        parent->right = nullptr;

      return data;
    }

    return (n->right) ? extractLastNode(n->right, n, false)
                      : extractLastNode(n->left, n, true);
  };

  T extractLastNode() { return extractLastNode(root, nullptr, false); };

  void swap(Node *a, Node *b) {
    // Swap two nodes in a tree
    T temp = a->data;
    a->data = b->data;
    b->data = temp;
  };

  // To remove the root element in the heap and return it.
  T extract() {
    if (!root)
      return 0;

    T rootData = root->data;

    T lastNodeData = extractLastNode(root, nullptr, false);

    if (!root)
      return rootData;

    root->data = lastNodeData;

    // Restore the heap property
    Node *pTrav = root;
    while (pTrav) {
      Node *maxChild = nullptr;

      if (pTrav->left && pTrav->right) {
        maxChild = (pTrav->left->data > pTrav->right->data) ? pTrav->left
                                                            : pTrav->right;
      } else if (pTrav->left) {
        maxChild = pTrav->left;
      }

      if (!maxChild || pTrav->data >= maxChild->data)
        break;

      swap(pTrav, maxChild);

      pTrav = maxChild;
    }

    return rootData;
  }

  // Return the number of element int the heap.
  int count() { return count(root); };

  int count(Node *n) {
    if (!n)
      return 0;
    return count(n->left) + count(n->right) + 1;
  };

  void print() {
    print(root);
    std::cout << std::endl;
  }

  void print(Node *n) {
    if (!n)
      return;
    std::cout << n->data << " ";
    print(n->left);
    print(n->right);
  }
};

int main() {
  MaxHeap<int> *m = new MaxHeap<int>();
  m->insert(3);
  m->insert(1);
  m->insert(2);
  m->insert(9);
  m->insert(8);

  m->print(); // Expected output: 3 1 2
  std::cout << "Count: " << m->count() << std::endl;
  std::cout << "Extract: " << m->extract() << std::endl;
  m->print();
  std::cout << "Count: " << m->count() << std::endl;

  delete m;
  return 0;
}
