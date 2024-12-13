#include <cstdlib>
#include <ctime>
#include <iostream>

#define SIZE 100

using namespace std;

class Node {
public:
  Node() {
    next = NULL;
    pre = NULL;
  }
  Node(int n) {
    data = n;
    next = NULL;
    pre = NULL;
  }
  int getData() { return data; }
  Node *getNext() { return next; }
  Node *getPre() { return pre; }
  void setData(int d) { data = d; }
  void setNext(Node *n) { next = n; }
  void setPre(Node *p) { pre = p; }

private:
  int data;
  Node *next, *pre;
};

class List {
public:
  List() { list = NULL; }
  List(int n) { generate(n); }

  void generate(int n) {
    int j;
    list = NULL;
    for (j = 0; j < n; j++)
      generate();
  }

  void generate() {
    Node *buf = new Node(rand());
    buf->setNext(list);
    if (list != NULL)
      list->setPre(buf);
    list = buf;
  }

  void bubbleSort() {
    int swapped = 1;
    Node *current;
    Node *next;

    while (swapped) {
      swapped = 0;
      current = list;
      next = current->getNext();
      while (next != NULL) {
        if (current->getData() > next->getData()) {
          swap(current, next);
          swapped = 1;
        }

        current = next;
        next = next->getNext();
      }
    }
  }

  void selectionSort() {
    // Find the smallest value from the left of the current index
    Node *current = list;
    Node *smallest;
    Node *pTrav;

    while (current != NULL) {
      pTrav = current->getNext();
      smallest = current;

      while (pTrav != NULL) {
        if (pTrav->getData() < smallest->getData())
          smallest = pTrav;
        pTrav = pTrav->getNext();
      }

      if (smallest->getData() < current->getData())
        swap(current, smallest);

      current = current->getNext();
    }
  }

  void insertionSort() {
    // Starting from the second element in the list, insert each current element
    // in its right place in the left sorted list
    if (list != NULL) {
      Node *current = list->getNext();
      Node *prev;
      Node *pTrav;

      while (current != NULL) {
        prev = current->getPre();
        pTrav = current;

        while (prev != NULL && pTrav->getData() < prev->getData()) {
          swap(pTrav, prev);
          pTrav = prev;
          prev = prev->getPre();
        }

        current = current->getNext();
      }
    }
  }

  void swap(Node *a, Node *b) {
    a->setData(a->getData() + b->getData());
    b->setData(a->getData() - b->getData());
    a->setData(a->getData() - b->getData());
  }

  void print() {
    Node *cur = list;
    while (cur != NULL) {
      cout << cur->getData() << " ";
      cur = cur->getNext();
    }
    cout << endl;
  }

private:
  Node *list;
};

int main() {
  srand(time(NULL));
  List *l = new List(10);
  l->print();
  l->bubbleSort();
  l->print();

  l = new List(10);
  l->print();
  l->insertionSort();
  l->print();

  l = new List(10);
  l->print();
  l->selectionSort();
  l->print();
}
