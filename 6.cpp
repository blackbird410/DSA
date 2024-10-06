#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 100

// Swap the node itself rather than the value

using namespace std;

class Node
{
public:
	Node()
	{
		next = NULL;
		pre = NULL;
	}
	Node(int n)
	{
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

class List
{
public:
	List() { list = NULL; }
	List(int n) { generate(n); }
	
	void generate(int n)
	{
		int j;
		list = NULL;
		for(j = 0;j < n;j ++)
			generate();
	}
	
	void generate()
	{
		Node *buf = new Node(rand());
		buf->setNext(list);
		if(list != NULL)
			list->setPre(buf);
		list = buf;
	}
	
	void bubbleSort()
	{
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
	
	void selectionSort()
	{
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
	
	void insertionSort()
	{
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

 
  void swap(Node* a, Node* b) {
    if (a == b) return;

    Node* aPrev = a->getPre();
    Node* aNext = a->getNext();
    Node* bPrev = b->getPre();
    Node* bNext = b->getNext();

    if (a->getNext() == b) {
      a->setNext(bNext);
      a->setPre(b);
      b->setNext(a);
      b->setPre(aPrev);

      if (aPrev != NULL)
          aPrev->setNext(b);

      if (bNext != NULL) 
          bNext->setPre(a);

    } else if (b->getNext() == a) {
      b->setNext(aNext);
      b->setPre(a);
      a->setNext(b);
      a->setPre(bPrev);

      if (bPrev != NULL) 
        bPrev->setNext(a);

      if (aNext != NULL) 
        aNext->setPre(b);

    } else {
      if (aPrev != NULL) 
          aPrev->setNext(b);
      
      if (aNext != NULL) 
          aNext->setPre(b);

      if (bPrev != NULL) 
          bPrev->setNext(a);
      
      if (bNext != NULL) 
          bNext->setPre(a);

      a->setNext(bNext);
      a->setPre(bPrev);
      b->setNext(aNext);
      b->setPre(aPrev);
    }

    if (list == a) 
      list = b;
    else if (list == b) 
      list = a;
  }

	void print()
	{
		Node *cur = list;
		while(cur != NULL)
		{
			cout<<cur->getData()<<" ";
			cur = cur->getNext();
		}
		cout<<endl;
	}
private:
	Node *list;
};

int main()
{
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
