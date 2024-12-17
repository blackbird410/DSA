#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <unordered_map>

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

  LinkList<WeightedGraphEdge<V, E> *> *getEdgeList() const { return list; };

private:
  LinkList<WeightedGraphEdge<V, E> *> *list;
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

private:
  WeightedGraphVertex<V, E> *end[2];
};

template <class V, class E> class WeightedGraph {
public:
  WeightedGraph() {
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

    // Should check the existence of the edge between the two vertices and
    // update if necessary
    ListNode<WeightedGraphEdge<V, E> *> *tmp = edge->getHead();
    while (tmp) {
      if (tmp->getData()->getAnotherEnd(v1) == v2) {
        // Update the edge weight without creating another edge between the same
        // vertices
        tmp->getData()->setData(w);
        return;
      }
      tmp = tmp->getNext();
    }

    WeightedGraphEdge<V, E> *edge = new WeightedGraphEdge<V, E>(w, v1, v2);
    v1->addEdge(edge);
    if (v1 != v2)
      v2->addEdge(edge);
    this->edge->addFromTail(edge);
    this->edgeCount++;
  }
  WeightedGraphVertex<V, E> *addVertex(V d) {
    /// Should check if the vertex already exist before adding it to the graph
    ListNode<WeightedGraphVertex<V, E> *> *tmp = this->vertex->getHead();
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
  return the shorest path tree with v as root
  */
  WeightedGraph *shortestPathTree(WeightedGraphVertex<V, E> *v) {
    // Contains a tuple of the vertex and the computed distance
    std::unordered_map<WeightedGraphVertex<V, E> *, int> distance;
    bool sourceExists = false;

    // Check if the vertex is in the graph and initialize the distances while
    // you are at it
    ListNode<WeightedGraphVertex<V, E> *> *tmp = vertex->getHead();
    while (tmp) {
      if (tmp->getData() == v) {
        distance[tmp->getData()] = 0;
        sourceExists = true;
      } else
        distance[tmp->getData()] = INT_MAX;
      tmp = tmp->getNext();
    }
    if (!sourceExists)
      return nullptr;

    WeightedGraph<V, E> *shortestPath = new WeightedGraph<V, E>();

    // Contains all the visited vertices and the parent vertex by which they
    // were accessed
    std::unordered_map<WeightedGraphVertex<V, E> *, WeightedGraphVertex<V, E> *>
        visited; // { current, previous }

    // Contains a set that will help the BFS traversal of the graph and works
    // like a priority queue
    auto cmp = [](std::pair<WeightedGraphVertex<V, E> *, int> a,
                  std::pair<WeightedGraphVertex<V, E> *, int> b) {
      return a.second < b.second;
    };
    std::set<std::pair<WeightedGraphVertex<V, E> *, int>> set;

    // Enqueue the source
    set.insert({v, 0});

    // BFS traversal
    WeightedGraphVertex<V, E> *otherEnd = nullptr;
    WeightedGraphVertex<V, E> *prevVertex = nullptr;
    WeightedGraphVertex<V, E> *v1, *v2;
    ListNode<WeightedGraphEdge<V, E> *> *tmpEdge = nullptr;
    int newDistance;
    while (!set.empty()) {
      auto [curVertex, curDist] = *(set.begin());
      set.erase(*set.begin());

      // Visit the neighbors while updating the distance if less than the one
      // stored
      tmpEdge = curVertex->getEdgeList()->getHead();

      // Should add the visited vertex to the graph and get all the distances as
      // edge values
      v1 = shortestPath->addVertex(curVertex->getData());
      while (tmpEdge) {
        otherEnd = tmpEdge->getData()->getAnotherEnd(curVertex);
        if (!visited[otherEnd]) {
          newDistance = distance[curVertex] + tmpEdge->getData()->getData();
          if (newDistance < distance[otherEnd]) {
            // Find and update the distance in the priority queue if it exist,
            // add it if not
            auto it = set.find({otherEnd, distance[otherEnd]});
            if (it != set.end())
              set.erase(it);

            distance[otherEnd] = newDistance;
            set.insert({otherEnd, distance[otherEnd]});
            v2 = shortestPath->addVertex(otherEnd->getData());

            shortestPath->addLink(v1, v2, newDistance);
          }
        }
        tmpEdge = tmpEdge->getNext();
      }

      // Once you are done visiting all the neighbors, mark the current node as
      // visited and move on to the next element in the top of the heap
      visited[curVertex] = prevVertex;
      prevVertex = curVertex;
    }

    return shortestPath;
  }

private:
  LinkList<WeightedGraphVertex<V, E> *> *vertex;
  LinkList<WeightedGraphEdge<V, E> *> *edge;
  int vertexCount, edgeCount;
};

int main() {
  WeightedGraph<char, int> *g = new WeightedGraph<char, int>();
  WeightedGraph<char, int> *tree;
  int j, k, i, l, n = 5;
  srand(time(NULL));
  for (j = 0; j < n; j++) {
    g->addVertex(j + 'a');
  }
  for (j = 0; j < n; j++) {
    k = rand() % n;
    i = rand() % n;
    l = rand() % 100;
    g->addLink((*g)[k], (*g)[i], l);
  }
  g->adjList();
  tree = g->shortestPathTree((*g)[0]);
  tree->adjList();
  return 0;
}
