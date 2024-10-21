#include <iostream>
#include <cmath>
#include <queue>

#define PREORDER 1
#define INORDER 2
#define POSTORDER 3

using namespace std;

template<class T>
class BinaryTreeInArray {
private:
  T *array;
  int height;
  int numOfElement;
  void resize(int size) {
    T *temp = new T[numOfElement];
    int j;
    for(j = 0;j < numOfElement;j ++)
      temp[j] = array[j];
    delete[] array;
    array = new T[static_cast<int>(pow(2, height + 1)) - 1];
    for(j = 0;j < numOfElement;j ++)
      array[j] = temp[j];
    height ++;
    delete[] temp;
  }
public:
  BinaryTreeInArray() : array(nullptr), height(0), numOfElement(0) {}
  ~BinaryTreeInArray() {
    delete[] array;
  }
  void addElementAsCompleteTree(T data) {
    int cap = pow(2, height) - 1;
    if(numOfElement + 1 > cap)
      resize((cap + 1) * 2 - 1);
    array[numOfElement] = data;
    numOfElement ++;
  }

  void displayInorder(int i = 0) {
    if (i >= numOfElement) return;
    displayInorder(2 * i + 1);
    cout << array[i] << " ";
    displayInorder(2 * i + 2);
  }

  void displayPreorder(int i = 0) {
    if (i >= numOfElement) return;
    cout << array[i] << " ";   
    displayPreorder(2 * i + 1);
    displayPreorder(2 * i + 2);
  }

  void displayPostorder(int i = 0) {
    if (i >= numOfElement) return;
    displayPostorder(2 * i + 1);
    displayPostorder(2 * i + 2);
    cout << array[i] << " "; 
  }
};

template<class T>
class BinaryTreeInLinkedList {
private:
  class TreeNode {
  private:
  public:
    TreeNode(T d) : data(d), left(nullptr), right(nullptr) {}
    TreeNode *left, *right;
    T data;
  };
  friend ostream& operator<<(ostream &out, TreeNode *p) {
    if (!p) return out;
    out << p->data;
    return out;
  };
  TreeNode *root;
  int numOfElement;
public:
  BinaryTreeInLinkedList() : root(nullptr), numOfElement(0) {}
  ~BinaryTreeInLinkedList() {
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
      TreeNode *p = q.front();
      q.pop();
      if (p->left) q.push(p->left);
      if (p->right) q.push(p->right);
      delete p;
    }
  }
  void addElementAsCompleteTree(T data) {
    TreeNode *newNode = new TreeNode(data);
    if (!root) {
      root = newNode;
      numOfElement++;
      return;
    }
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
      TreeNode *p = q.front();
      q.pop();

      if (!p->left) {
        p->left = newNode;
        break;
      } else {
        q.push(p->left);
      }

      if (!p->right) {
        p->right = newNode;
        break;
      } else {
        q.push(p->right);
      }
    }
    numOfElement++;
  }

  void traverse(TreeNode *p, int order) {
    if (!p) return;

    switch (order) {
      case PREORDER:
        cout << p << " ";
        traverse(p->left, order);
        traverse(p->right, order);
        break;
      case INORDER:
        traverse(p->left, order);
        cout << p << " ";
        traverse(p->right, order);
        break;
      case POSTORDER:
        traverse(p->left, order);
        traverse(p->right, order);
        cout << p << " ";
        break;
      default:
        break;
    }
  }

  void displayInorder() {
    traverse(root, INORDER);
  }
  void displayPreorder() {
    traverse(root, PREORDER);
  }
  void displayPostorder() {
    traverse(root, POSTORDER);
  }
};

int main()
{
  BinaryTreeInArray<int> *b = new BinaryTreeInArray<int>;
  BinaryTreeInLinkedList<int> *tree = new BinaryTreeInLinkedList<int>;
  int j, n;
  cin >> n;
  for(j = 0;j < n;j ++) {
    b->addElementAsCompleteTree(j);
    tree->addElementAsCompleteTree(j);
  }
  b->displayInorder();
  cout << endl;
  tree->displayInorder();
  cout << endl;
  b->displayPreorder();
  cout << endl;
  tree->displayPreorder();
  cout << endl;
  b->displayPostorder();
  cout << endl;
  tree->displayPostorder();
  cout << endl;

  delete b;
  delete tree;
  return 0;
}
