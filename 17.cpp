// PREPEND BEGIN
/*some text*/
// PREPEND END

// TEMPLATE BEGIN
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>

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
    out << *(n.data);
    return out;
  }
  friend std::ostream &operator<<(std::ostream &out, Node *n) {
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
    left = NULL;
    right = NULL;
  }
  BinaryTreeNode(T d) : Node<T>(d) {
    left = NULL;
    right = NULL;
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
    if (cur == NULL)
      return;
    inorder(cur->getRight(), n + 1);
    int j;
    for (j = 0; j < n; j++)
      cout << "  ";
    cout << cur << endl;
    inorder(cur->getLeft(), n + 1);
  }

public:
  AVLTree();
  void insert(T d);
  void inorder() { inorder(root, 0); }
};

int main() {
  AVLTree<int> *tree = new AVLTree<int>();
  srand(0);
  int j, k, i;
  for (j = 0; j < 20; j++) {
    tree->insert(rand() % 100);
    tree->inorder();
  }
}
