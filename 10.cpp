#include <cmath>
#include <iostream>
#include <vector>

#define PREORDER -1
#define POSTORDER 1

using namespace std;

template <class T> class TreeInLinkedList {
private:
  class TreeNode {
  private:
  public:
    TreeNode(T d, TreeNode *p) : data(d), parent(p) {}
    TreeNode *parent;
    T data;
  };
  friend ostream &operator<<(ostream &out, TreeNode *p) {
    out << p->data;
    return out;
  }
  vector<TreeNode *> *nodeList;

public:
  TreeInLinkedList() {
    // Creates an array of pointers to TreeNode objects
    nodeList = new vector<TreeNode *>();
  }
  ~TreeInLinkedList() { nodeList->clear(); }
  void addElement(T data) {
    // How does it add elements ?
    // 1 - Check if we are adding 1, because all other elements in the tree will
    // definitely have 1 as a parent ( for all data, 1 | data)
    int j, k = nodeList->size();
    if (data == 1) {
      // 2 - Clear the tree for new values and add 1 as the root of the tree
      nodeList->clear();
      nodeList = new vector<TreeNode *>();
      TreeNode *newNode = new TreeNode(data, nullptr);
      nodeList->push_back(newNode);
    } else {
      // 2 - Check all the preexisting nodes;
      // If a node value can divide the data, the add a new node for the data to
      // the tree and the node found as its parent The relation in this tree is
      // that the data in a parent of a node can divide the data in the node
      for (j = 0; j < k; j++) {
        if (data % (*nodeList)[j]->data == 0) {
          TreeNode *newNode = new TreeNode(data, (*nodeList)[j]);
          nodeList->push_back(newNode);
        }
      }
    }
  }

  void traverse(TreeNode *p, int order) {
    if (!p)
      return;

    if (order == PREORDER) {
      cout << p << " ";
      for (TreeNode *child : *nodeList)
        if (child->parent == p)
          traverse(child, order);
    } else if (order == POSTORDER) {
      for (TreeNode *child : *nodeList)
        if (child->parent == p)
          traverse(child, order);
      cout << p << " ";
    }
  }

  void displayPreorder() {
    if (!nodeList->size())
      return;

    traverse(nodeList->at(0), PREORDER);
  }

  void displayPostorder() {
    if (!nodeList->size())
      return;

    traverse(nodeList->at(0), POSTORDER);
  }
};

int main() {
  TreeInLinkedList<int> *tree = new TreeInLinkedList<int>();
  int j;
  int n;
  cin >> n;
  for (j = 1; j <= n; j++)
    tree->addElement(j);
  tree->displayPreorder();
  cout << endl;
  tree->displayPostorder();
  cout << endl;
}
