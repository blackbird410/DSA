#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <vector>

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

template <class T> class ListNode : public Node<T> {
public:
  ListNode() : Node<T>() {
    prev = NULL;
    next = NULL;
  }
  ListNode(T d) : Node<T>(d) {
    prev = NULL;
    next = NULL;
  }
  ListNode(ListNode *p, ListNode *n) : Node<T>() {
    prev = p;
    next = n;
  }
  ListNode(T d, ListNode *p, ListNode *n) : Node<T>(d) {
    prev = p;
    next = n;
  }
  ListNode *getNext() const { return next; }
  ListNode *getPrev() const { return prev; }
  void setNext(ListNode *n) { next = n; }
  void setPrev(ListNode *p) { prev = p; }
  ListNode &operator=(T d) {
    this->setData(d);
    return *this;
  }

private:
  ListNode *prev, *next;
};

template <class T> class LinkList {
public:
  LinkList() {
    head = NULL;
    tail = NULL;
  }
  void addFromHead(T d) {
    ListNode<T> *node = new ListNode<T>(d);
    if (head != NULL) {
      head->setPrev(node);
    }
    node->setNext(head);
    head = node;
    if (tail == NULL)
      tail = node;
  }
  void addFromTail(T d) {
    ListNode<T> *node = new ListNode<T>(d);
    if (tail != NULL) {
      tail->setNext(node);
    }
    node->setPrev(tail);
    tail = node;
    if (head == NULL)
      head = node;
  }
  void addAfter(ListNode<T> *at, T d) {
    if (!exist(at))
      return;
    ListNode<T> *node = new ListNode<T>(d);
    if (at->getNext() != NULL)
      at->getNext()->setPrev(node);
    node->setNext(at->getNext());
    at->setNext(node);
    node->setPrev(at);
    if (node->getNext() == NULL)
      tail = node;
  }
  ListNode<T> *removeFromHead() {
    ListNode<T> *n = head;
    if (head != NULL) {
      head = head->getNext();
      if (head != NULL)
        head->setPrev(NULL);
      else
        tail = NULL;
      n->setNext(NULL);
    }
    return n;
  }
  ListNode<T> *removeFromTail() {
    ListNode<T> *n = tail;
    if (tail != NULL) {
      tail = tail->getPrev();
      if (tail != NULL)
        tail->setNext(NULL);
      else
        head = NULL;
      n->setPrev(NULL);
    }
    return n;
  }
  ListNode<T> *remove(ListNode<T> *n) {
    if (!exist(n))
      return NULL;
    if (n == head)
      return removeFromHead();
    if (n == tail)
      return removeFromTail();
    n->getPrev()->setNext(n->getNext());
    n->getNext()->setPrev(n->getPrev());
    n->setNext(NULL);
    n->setPrev(NULL);
    return n;
  }
  ListNode<T> *exist(T d) {
    ListNode<T> *j = head;
    while (j != NULL) {
      if (j->getData() == d)
        return j;
      j = j->getNext();
    }
    return NULL;
  }
  bool exist(ListNode<T> *n) {
    ListNode<T> *j = head;
    while (j != NULL) {
      if (j == n)
        return true;
      j = j->getNext();
    }
    return false;
  }
  ListNode<T> &operator[](int i) {
    ListNode<T> *j = head;
    int k;
    for (k = 0; k < i && j != NULL; k++)
      j = j->getNext();
    if (j == NULL)
      throw std::invalid_argument("index does not exist.");
    return *j;
  }
  void print() const {
    ListNode<T> *j;
    j = head;
    while (j != NULL) {
      std::cout << (*j) << " ";
      j = j->getNext();
    }
    std::cout << std::endl;
  }

  ListNode<T> *getHead() const { return head; };

protected:
  ListNode<T> *head, *tail;
};

template <class V, class E> class WeightedGraph;
template <class V, class E> class WeightedGraphEdge;

template <class V, class E> class WeightedGraphVertex : public Node<V> {
public:
  WeightedGraphVertex() : Node<V>() { list = new LinkList<E>(); }
  WeightedGraphVertex(V d) : Node<V>(d) {
    list = new LinkList<WeightedGraphEdge<V, E> *>();
  }
  void addEdge(WeightedGraphEdge<V, E> *edge) { list->addFromTail(edge); }
  void remove(WeightedGraphEdge<V, E> *node) {}
  ListNode<WeightedGraphEdge<V, E> *> *operator[](int n) {
    try {
      return (&(*list)[n]);
    } catch (std::invalid_argument e) {
      return NULL;
    }
  }

private:
  LinkList<WeightedGraphEdge<V, E> *> *list;
  friend class WeightedGraph<V, E>;
};

template <class V, class E> class WeightedGraphEdge : public Node<E> {
public:
  WeightedGraphEdge() : Node<E>() {
    end[0] = NULL;
    end[1] = NULL;
  }
  WeightedGraphEdge(E d) : Node<E>(d) {
    end[0] = NULL;
    end[1] = NULL;
  }
  WeightedGraphEdge(E d, WeightedGraphVertex<V, E> *v1,
                    WeightedGraphVertex<V, E> *v2)
      : Node<E>(d) {
    end[0] = v1;
    end[1] = v2;
  }
  WeightedGraphVertex<V, E> *&getAnotherEnd(WeightedGraphVertex<V, E> *v) {
    if (v != end[0])
      return end[0];
    return end[1];
  }
  WeightedGraphVertex<V, E> *operator[](int n) {
    try {
      return end[n];
    } catch (const std::invalid_argument e) {
      return nullptr;
    }
  }

private:
  WeightedGraphVertex<V, E> *end[2];
};

template <class V, class E> class WeightedGraph {
public:
  WeightedGraph() : vertexCount(0), edgeCount(0) {
    vertex = new LinkList<WeightedGraphVertex<V, E> *>();
    edge = new LinkList<WeightedGraphEdge<V, E> *>();
  }

  WeightedGraphVertex<V, E> *operator[](int n) {
    try {
      return (*vertex)[n].getData();
    } catch (std::invalid_argument e) {
      return NULL;
    }
  }
  void addLink(WeightedGraphVertex<V, E> *v1, WeightedGraphVertex<V, E> *v2,
               E w) {
    WeightedGraphEdge<V, E> *edge = new WeightedGraphEdge<V, E>(w, v1, v2);
    v1->addEdge(edge);
    if (v1 != v2)
      v2->addEdge(edge);
    this->edge->addFromTail(edge);
    this->edgeCount++;
  }
  WeightedGraphVertex<V, E> *addVertex(V d) {
    /// Should check if the vertex already exist before adding it to the graph
    ListNode<WeightedGraphVertex<V, E> *> *tmp = getVertexList()->getHead();
    while (tmp) {
      if (tmp->getData()->getData() == d)
        return tmp->getData();
      tmp = tmp->getNext();
    }

    WeightedGraphVertex<V, E> *v = new WeightedGraphVertex<V, E>(d);
    vertex->addFromTail(v);
    vertexCount++;
    return v;
  }
  void adjList() {
    ListNode<WeightedGraphVertex<V, E> *> *cur = &(*vertex)[0];
    while (cur != NULL) {
      WeightedGraphVertex<V, E> *temp = cur->getData();
      std::cout << temp << ": ";
      ListNode<WeightedGraphEdge<V, E> *> *e = (*temp)[0];
      while (e != NULL) {
        std::cout << e->getData()->getAnotherEnd(temp) << "(" << e->getData()
                  << ") ";
        e = e->getNext();
      }
      std::cout << std::endl;
      cur = cur->getNext();
    }
  }
  /*
  return null if n is not a vertex in this graph
  return the minimum spanning tree with v as root
  */
  WeightedGraph *minimumSpanningTree(WeightedGraphVertex<V, E> *v) {
    // Prim's Algorithm
    // ----------------
    // - First select the edge with the minimun weight
    // - Build a new graph by selecting connected edges with minimum weights if
    //    the graph does not have cycle by adding them

    bool sourceExists = false;
    ListNode<WeightedGraphVertex<V, E> *> *tmpV = vertex->getHead();
    while (tmpV) {
      if (tmpV->getData() == v) {
        sourceExists = true;
        break;
      }
      tmpV = tmpV->getNext();
    }
    if (!sourceExists)
      return nullptr;

    // Find the edge with the minimum weight linked to the root v
    ListNode<WeightedGraphEdge<V, E> *> *tmp = v->list->getHead();
    WeightedGraphEdge<V, E> *minEdge = tmp->getData();

    while (tmp) {
      if (tmp->getData()->getData() < minEdge->getData())
        minEdge = tmp->getData();

      tmp = tmp->getNext();
    }

    WeightedGraph *minSpanningTree = new WeightedGraph<V, E>();

    // Get the vertice connected to the minEdge
    WeightedGraphVertex<V, E> *first = minEdge->getAnotherEnd(nullptr), *firstA;
    WeightedGraphVertex<V, E> *second = minEdge->getAnotherEnd(first), *secondA;
    WeightedGraphVertex<V, E> *v1 =
        minSpanningTree->addVertex(first->getData());
    WeightedGraphVertex<V, E> *v2 =
        minSpanningTree->addVertex(second->getData());
    minSpanningTree->addLink(v1, v2, minEdge->getData());

    bool firstVertexExist;
    bool secondVertexExist;
    bool edgeExist;
    ListNode<WeightedGraphVertex<V, E> *> *p = nullptr;
    ListNode<WeightedGraphEdge<V, E> *> *pTrav = nullptr;

    while (true) {
      tmp = edge->getHead();
      minEdge = nullptr;

      // Find the smallest edge that is connected to the already selected
      // vertices
      while (tmp) {
        if (minEdge)
          // Check if the edge is connected any of the others in minSpanningTree
          // Not already included

          // Check the existence of the current edge in the minSpanningTree
          pTrav = minSpanningTree->getEdgeList()->getHead();
        edgeExist = false;
        while (pTrav) {
          // Check the first and second vertex
          first = tmp->getData()->getAnotherEnd(nullptr);
          second = tmp->getData()->getAnotherEnd(first);
          firstA = pTrav->getData()->getAnotherEnd(nullptr);
          secondA = pTrav->getData()->getAnotherEnd(firstA);
          if ((first->getData() == firstA->getData() &&
               second->getData() == secondA->getData()) ||
              (first->getData() == secondA->getData() &&
               second->getData() == firstA->getData())) {
            edgeExist = true;
            break;
          }
          pTrav = pTrav->getNext();
        }

        if (!edgeExist) {
          first = tmp->getData()->getAnotherEnd(nullptr);
          second = tmp->getData()->getAnotherEnd(first);

          // Check the existence of the first vertex
          p = minSpanningTree->getVertexList()->getHead();
          firstVertexExist = false;
          while (p) {
            if (p->getData()->getData() == first->getData()) {
              firstVertexExist = true;
              break;
            }
            p = p->getNext();
          }

          // Check the existence of the second vertex if the first was not found
          secondVertexExist = false;
          p = minSpanningTree->getVertexList()->getHead();
          while (p) {
            if (p->getData()->getData() == second->getData()) {
              secondVertexExist = true;
              break;
            }
            p = p->getNext();
          }

          // Only one of the vertices can exist, otherwise it will cause a cycle
          if ((firstVertexExist && !secondVertexExist) ||
              (!firstVertexExist && secondVertexExist))
            if (!minEdge || tmp->getData()->getData() < minEdge->getData())
              minEdge = tmp->getData();
        }

        tmp = tmp->getNext();
        // std::cin.get();
      }

      if (minEdge) {
        // One of the vertices does not exist in the spanning tree already
        first = minEdge->getAnotherEnd(nullptr);
        second = minEdge->getAnotherEnd(first);

        v1 = minSpanningTree->addVertex(first->getData());
        v2 = minSpanningTree->addVertex(second->getData());
        minSpanningTree->addLink(v1, v2, minEdge->getData());
      } else
        break;
    }

    return minSpanningTree;
  }

  LinkList<WeightedGraphVertex<V, E> *> *getVertexList() const {
    return vertex;
  }
  LinkList<WeightedGraphEdge<V, E> *> *getEdgeList() const { return edge; }
  int getVertexCount() const { return vertexCount; };
  int getEdgeCount() const { return edgeCount; };

private:
  LinkList<WeightedGraphVertex<V, E> *> *vertex;
  LinkList<WeightedGraphEdge<V, E> *> *edge;
  int vertexCount, edgeCount;
};

int main() {
  WeightedGraph<char, int> *g = new WeightedGraph<char, int>();
  WeightedGraph<char, int> *tree;
  // int edges[] = {19, 84, 26, 34, 30 };
  int j, k, i, l;
  srand(time(NULL));
  for (j = 0; j < 5; j++) {
    g->addVertex(j + 'a');
  }
  // g->addLink((*g)[0], (*g)[2], edges[0]);
  // g->addLink((*g)[2], (*g)[1], edges[1]);
  // g->addLink((*g)[1], (*g)[3], edges[2]);
  // g->addLink((*g)[3], (*g)[4], edges[3]);
  // g->addLink((*g)[4], (*g)[0], edges[4]);

  for (j = 0; j < 5; j++) {
    k = rand() % 5;
    i = rand() % 5;
    l = rand() % 100;
    g->addLink((*g)[k], (*g)[i], l);
  }
  g->adjList();
  tree = g->minimumSpanningTree((*g)[0]);
  // std::cout << "--------------------------------------------------" <<
  // std::endl;
  tree->adjList();
  return 0;
}
