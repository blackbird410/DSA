#include <iostream>
#include <string>
#include <vector>

#define EMPTY '\0'

using namespace std;

class Trie;
class TrieNode {
private:
  std::vector<TrieNode *> children;
  char data;
  bool isEnd;

public:
  TrieNode() : data(EMPTY), isEnd(false) {};
  TrieNode(const char &letter, bool end = false) : data(letter), isEnd(end) {};

  ~TrieNode() {
    for (auto &child : children) {
      delete child;
    }
    children.clear();
  }

  friend class Trie;

  friend std::ostream &operator<<(std::ostream &out, const TrieNode *n) {
    if (n && n->data == EMPTY)
      out << "[]";
    else if (n)
      out << n->data;
    return out;
  };
};

class Trie {
  TrieNode *root;

public:
  /*
  construct a Trie.
  */
  Trie() : root(new TrieNode(EMPTY, true)) {};
  ~Trie() { delete root; }

  TrieNode *findPath(const TrieNode *node, const char &c) {
    // Find the next character in the trie if available to travel
    for (auto &child : node->children) {
      if (child->data == c)
        return child;
    }

    return nullptr;
  };

  /*
  search trie for key, return true if exist, false if not.
  */
  bool search(string key) {
    int i = 0, len = key.length();
    TrieNode *p = root, *tmp;

    while (p && i < len) {
      p = findPath(p, key[i]);
      if (p)
        tmp = p;
      i += !!p;
    };

    return (i == len && tmp->isEnd);
  };
  /*
  insert value into trie.
  */

  void insert(string value) {
    TrieNode *tmp = root;

    for (const char &c : value) {
      TrieNode *nextNode = findPath(tmp, c);
      if (!nextNode) {
        TrieNode *newNode = new TrieNode(c);
        tmp->children.push_back(newNode);
        tmp = newNode;
      } else {
        tmp = nextNode;
      }
    }

    tmp->isEnd = true;
  }

  /*
  display trie in pre-order, each element in a line, display space befoer
  element base on the level of 	the element. level 1 for 2 space, level 2 for 4
  space, level 3 for 6 space and so on, root is level 	0.
  */

  void traverse(const TrieNode *n, const TrieNode *parent = nullptr,
                int level = 0) {
    if (!n)
      return;

    if (parent && (parent->children.size() > 1 || parent->isEnd))
      for (int i = 0; i < level; i++)
        std::cout << " ";
    std::cout << n;

    if (n->children.size() > 1 || n->isEnd) {
      std::cout << endl;
    }

    for (auto &child : n->children) {
      traverse(child, n, level + 2);
    }
  }

  void preorder() { traverse(root); }
};

int main() {
  Trie *trie = new Trie();
  string command, key, value;
  while (1) {
    cin >> command;
    if (command == "insert") {
      cin >> value;
      trie->insert(value);
    } else if (command == "search") {
      cin >> key;
      if (trie->search(key))
        cout << "exist" << endl;
      else
        cout << "not exist" << endl;
    } else if (command == "print") {
      trie->preorder();
    } else if (command == "exit") {
      break;
    }
  }

  delete trie;
}
