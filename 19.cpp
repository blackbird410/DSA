#include <cstdlib>
#include <ctime>
#include <iostream>

#define LEFT -1
#define RIGHT 1

using namespace std;

template <class T> class BinarySearchThreadedTree;
template <class T> class BinaryThreadedTreeNode {
private:
protected:
  T *data;
  BinaryThreadedTreeNode *left, *right;
  bool isThread;

public:
  BinaryThreadedTreeNode(T d) : right(nullptr), left(nullptr) {
    data = new T(d);
  }
  BinaryThreadedTreeNode(bool isThread)
      : right(nullptr), left(nullptr), isThread(isThread) {};
  BinaryThreadedTreeNode() : data(nullptr), right(nullptr), left(nullptr) {}
  friend class BinarySearchThreadedTree<T>;

  friend std::ostream &operator<<(std::ostream &out,
                                  const BinaryThreadedTreeNode<T> *n) {
    if (n && n->data)
      out << *(n->data);
    return out;
  };
};

template <class T> class BinarySearchThreadedTree : BinaryThreadedTreeNode<T> {

  void CreateThread(BinaryThreadedTreeNode<T> *node,
                    BinaryThreadedTreeNode<T> *&prev) {
    if (!node)
      return;

    // std::cout << "CreateThread(" << node << ", " << prev << ")" << std::endl;
    CreateThread(node->left, prev);

    if (prev && !prev->right) {
      prev->right = node;
      prev->isThread = true;
    }
    prev = node;

    if (!node->isThread)
      CreateThread(node->right, prev);
  }

  void CreateThread() {
    BinaryThreadedTreeNode<T> *prev = nullptr;
    CreateThread(this, prev);
  }

public:
  BinarySearchThreadedTree() : BinaryThreadedTreeNode<T>() {}

  BinarySearchThreadedTree *insert(T d) {
    if (!this->data) {
      this->data = new T(d);
      this->left = new BinarySearchThreadedTree();
      this->right = new BinarySearchThreadedTree();
      return this;
    } else {
      if (*this->data > d)
        return (static_cast<BinarySearchThreadedTree<T> *>(this->left))
            ->insert(d);
      else
        return (static_cast<BinarySearchThreadedTree<T> *>(this->right))
            ->insert(d);
    }
  }

  void print() {
    // std::cout << "Creating threads..." << std::endl;
    CreateThread();

    BinaryThreadedTreeNode<T> *tmp = this;
    while (tmp->left)
      tmp = tmp->left;

    while (tmp) {
      std::cout << tmp;
      if (tmp->isThread)
        tmp = tmp->right;
      else {
        tmp = tmp->right;
        while (tmp && tmp->left)
          tmp = tmp->left;
      }

      if (tmp)
        std::cout << " ";
    }

    std::cout << std::endl;
  }
};

int main() {
  int j, v;
  BinarySearchThreadedTree<int> bstt;
  srand(time(nullptr));
  // std::cout << "Inserting: ";
  for (j = 0; j < 10; j++) {
    v = rand() % 100;
    // std::cout << v << " ";
    bstt.insert(v);
  }
  std::cout << std::endl;
  bstt.print();
}
