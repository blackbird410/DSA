#include <iostream>
#include <string>

using namespace std;

template <class T> class BinarySearchTree {
private:
  struct Node {
    T data;
    Node *left, *right;
  };

  void inorder(Node *cur) {
    if (!cur)
      return;
    inorder(cur->left);
    cout << cur->data << " ";
    inorder(cur->right);
  }

  Node *root;

public:
  BinarySearchTree() : root(nullptr) {}

  void insertElement(Node *n, T data) {
    Node *newNode = new Node();
    newNode->data = data;
    newNode->left = newNode->right = nullptr;

    if (data < n->data) {
      if (!n->left)
        n->left = newNode;
      else
        insertElement(n->left, data);
    } else {
      if (!n->right)
        n->right = newNode;
      else
        insertElement(n->right, data);
    }
  }

  void insertElement(T d) {
    if (root == nullptr) {
      root = new Node();
      root->data = d;
      root->left = root->right = nullptr;
      return;
    }

    insertElement(root, d);
  }

  void print() {
    inorder(root);
    cout << endl;
  }

  bool search(Node *n, T d) {
    if (!n)
      return false;

    return (n->data == d)  ? true
           : (d < n->data) ? search(n->left, d)
                           : search(n->right, d);
  };

  bool search(T d) { return (!root) ? false : search(root, d); }

  int height(Node *n) {
    if (!n)
      return 0;

    int heightLeft = height(n->left) + 1;
    int heightRight = height(n->right) + 1;

    return (heightLeft > heightRight) ? heightLeft : heightRight;
  };

  int height() { return height(root); }
};

int main() {
  int data;
  string command;
  BinarySearchTree<int> *bst = new BinarySearchTree<int>();
  while (true) {
    cout << "Command: ";
    cin >> command;
    if (command == "insert") {
      cin >> data;
      bst->insertElement(data);
    } else if (command == "search") {
      cin >> data;
      if (bst->search(data))
        cout << "true" << endl;
      else
        cout << "false" << endl;
    } else if (command == "height") {
      cout << bst->height() << endl;
    } else if (command == "print") {
      bst->print();
    } else if (command == "exit") {
      break;
    }
  }
}
