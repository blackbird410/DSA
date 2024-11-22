#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// design your won SuffixTreeNode using in SuffixTree
class SuffixTree;
class SuffixTreeNode {
  std::vector<SuffixTreeNode *> children;
  std::vector<int> indexCounter;
  char data;
  bool isEnd;

public:
  SuffixTreeNode() : data('\0'), isEnd(false) {};
  SuffixTreeNode(char c) : data(c), isEnd(false) {};
  ~SuffixTreeNode() {
    for (auto &child : children)
      delete child;
    children.clear();
  };

  friend class SuffixTree;
  friend std::ostream &operator<<(std::ostream &out, const SuffixTreeNode *n) {
    if (n)
      out << n->data;
    return out;
  };

  SuffixTreeNode* findPath(const SuffixTreeNode* n, char c) {
    if (!n) return nullptr;

    for (auto& child: n->children) 
         if (child->data == c) return child;
    return nullptr;
  };

  SuffixTreeNode* insertNode(SuffixTreeNode* root, char c, int counter) {
    if (!root) return nullptr;

    SuffixTreeNode* newNode = new SuffixTreeNode(c);
    newNode->indexCounter.push_back(counter);
    root->children.push_back(newNode);
    return newNode;
  };

  void insert(const std::string& s, int counter) {
    if (s.empty()) return;

    SuffixTreeNode* pathExists = findPath(this, s[0]);
    if (pathExists) {
      pathExists->insert(s.substr(1), counter);
      pathExists->indexCounter.push_back(counter);
      pathExists->isEnd = true;
    } else {
      SuffixTreeNode* tmp = this;

      for (auto& c: s)
        tmp = insertNode(tmp, c, counter);
      tmp->isEnd = true;
    }
  };

  void print() {
    std::cout << this << " : ";

    for (auto& i: indexCounter) std::cout << i << " ";
    std::cout << std::endl;

    for (const auto& child: children)
      child->print();

    if (isEnd) 
      std::cout << std::endl;
  }
};

class SuffixTree {
private:
  SuffixTreeNode *root;
  std::string text;

public:
  SuffixTree(const std::string &text) : text(text) {
    root = new SuffixTreeNode();

    int len = text.length(), i;
    for (i = 0; i < len; i++)
      root->insert(text.substr(i), i);
  }

  bool exist(const std::string &substring) { 
    SuffixTreeNode* tmp = root, *p;
    int i = 0, len = substring.length();
    while (tmp && i < len) { 
      p = tmp; 
      tmp = tmp->findPath(tmp, substring[i]);
      i++;
    }

    return (i == len && p->isEnd);
  }

  int count(const std::string &substring) {
    int len = substring.length();
    if (!len) return 0;

    int i = 0;
    SuffixTreeNode* tmp = root;

    while (tmp && i < len) {
      tmp = tmp->findPath(tmp, substring[i]);
      i++;
    }

    return (i == len && tmp && tmp->isEnd) ? tmp->indexCounter.size() : 0;
  }

  ~SuffixTree() { delete root; }

  void print() {
    root->print();
  };
};

int main() {
  std::string text = "";
  while (true) {
    std::string temp;
    getline(std::cin, temp);
    if (temp == "")
      break;
    text += temp;
  }

  SuffixTree tree(text);
  tree.print();

  std::string query;
  while (true) {
    getline(std::cin, query);
    if (query == "")
      break;
    std::cout << "Existence of '" << query
              << "': " << (tree.exist(query) ? "Yes" : "No") << std::endl;
    std::cout << "Count of '" << query << "': " << tree.count(query)
              << std::endl;
  }
  return 0;
}
