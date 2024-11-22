#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// design your own SuffixTreeNode using in SuffixTree
class SuffixTreeNode {
public:
  std::vector<SuffixTreeNode *> children;
  std::string suffix;
  int travelCounter;

  friend std::ostream& operator<<(std::ostream& out, const SuffixTreeNode* n) {
    if (n) out << n->suffix;
    return out;
  }

  SuffixTreeNode() : suffix(""), travelCounter(0) {};
  SuffixTreeNode(const std::string &s) : suffix(s), travelCounter(0) {};
  ~SuffixTreeNode() {
    for (auto &child : children) {
      delete child;
    }
  }

  int findPrefixIndex(const std::string &substring) {
    int i = 0, l = suffix.length();
    if (!l || !substring.length()) return 0;

    for (auto& c: substring) {
      if (i >= l || c != suffix[i]) return i;
      i++;
    }

    return i;
  }

  bool insert(const std::string &substring) {
    // Root case
    if (suffix.empty()) {
      for (auto &child : children) {
        if (child->insert(substring)) {
          return true;
        }
      }
      children.push_back(new SuffixTreeNode(substring));
      return true;
    }

    int p = findPrefixIndex(substring);
    if (p > 0) {
      std::string prefix = suffix.substr(0, p);

      if (p < suffix.length()) {
        children.push_back(new SuffixTreeNode(suffix.substr(p)));
      }



      if (p < substring.length()) {
        for (auto &child : children) {
          if (child->insert(substring.substr(p))) {
            return true;
          }
        }
        children.push_back(new SuffixTreeNode(substring.substr(p)));
      }

      suffix = prefix;
      return true;
    }

    return false;
  }

  void resetTravel(SuffixTreeNode* n) {
    if (!n) return;
    if (n && (n->children.empty() || !n->travelCounter)) {
      n->travelCounter = 0;
      return;
    }

    for (auto& child: n->children)
      resetTravel(child);
  };

  SuffixTreeNode* findPath(SuffixTreeNode* n, const char& c) {
    // Find a path among the children
    if (!n) return nullptr;

    std::cout << "\nTravelling " << n << " looking for " << c << " while being at " << n->suffix[n->travelCounter] << std::endl;

    int i, len;

    for (auto& child: n->children) {
      // Travel the node value to match the character until length
      i = child->travelCounter;
      if (i < child->suffix.length() && child->suffix[i] == c) { 
        // Increment the counter first before returning the value to continue the matching process
        child->travelCounter++;
        return child; 
      } else {
        std::cout << "Character " << 
      }
    }

    return nullptr;
  }

  bool exist(const std::string &substring, int start = 0) {
    // Find if there is a path from the start of the substring to the end in the tree
    SuffixTreeNode* tmp = this;
    SuffixTreeNode* p;
    int i = 0;
    int len = substring.length();

    while (tmp && i < len) {
      p = tmp;
      tmp = findPath(tmp, substring[i]);
      i++;
    }

    return i == len;
  }

  int count(const std::string &substring) const {
    int suffixLen = suffix.length();

    int c = 0;
    if (!suffixLen) {
      for (auto& child: children) {
        c += child->count(substring);
      }
      return c;
    }

    int substringLen = substring.length();
    int i = 0;
    while (i < suffixLen && i < substringLen && suffix[i] == substring[i]) i++;

    if (i == substringLen && i == suffixLen) {
      c += 1;
      for (auto &child: children) {
        c += child->count(substring);
      }
    } 

    if (i < substringLen && i == suffixLen) {
      for (auto &child: children) {
        c += child->count(substring.substr(i));
      }
    }

    return c;
  }
};

class SuffixTree {
private:
  SuffixTreeNode *root;
  std::string text;

public:
  SuffixTree(const std::string &text) : text(text) {
    root = new SuffixTreeNode();
    for (int i = text.length() - 1; i >= 0; i--) {
      root->insert(text.substr(i));
    }
  }

  bool exist(const std::string &substring) {
    return root->exist(substring);
  }

  int count(const std::string &substring) {
    return root->count(substring);
  }

  void prettyPrint(SuffixTreeNode *node, const std::string &prefix = "") {
    if (!node) return;

    if (!node->suffix.empty()) {
      std::cout << prefix << node->suffix << std::endl;
    }

    for (auto &child : node->children) {
      prettyPrint(child, prefix + "  ");
    }
  }

  void prettyPrint() {
    prettyPrint(root);
  }

  ~SuffixTree() {
    delete root;
  }
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
  tree.prettyPrint();

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
