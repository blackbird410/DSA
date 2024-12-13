#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES                                                            \
  2 // maxinum number of data item each node can hold. Sample Output shows the
    // result when MAX_ENTRIES is 2
#define MAX_NAME_LEN 50 // maxinum number of character for item name
#define LEAF 1
#define ROOT 0

typedef struct {
  double minX, minY;
  double maxX, maxY;
} BoundingBox;

typedef struct {
  double latitude, longitude;
  char name[MAX_NAME_LEN];
} DataItem;

typedef struct RTreeNode {
  int isLeaf;      // 是否為葉節點is leaf or not
  int count;       // number of node
  BoundingBox mbr; // minimum bounding rectangle
  struct RTreeNode *children[MAX_ENTRIES + 1];
  DataItem data[MAX_ENTRIES]; //
} RTreeNode;

typedef struct {
  DataItem item;
  double distance;
} SearchResult;

void printTree(RTreeNode *root, int depth);
void updateMBR(RTreeNode *node);
RTreeNode *createNode(int isLeaf);
double calculateDistance(double x1, double y1, double x2, double y2);
void splitNode(RTreeNode *root, DataItem item);
int findBestChildIndex(RTreeNode *parent, DataItem item);
void swapResults(SearchResult *a, SearchResult *b);
void sortResults(SearchResult *results, int *n);
void searchData(RTreeNode *node, SearchResult *results, DataItem *item, int *n,
                int *nItemsFound);

// insert data
void insert(RTreeNode **root, DataItem item) {
  RTreeNode *node = *root;

  if (node->isLeaf) {
    if (node->count < MAX_ENTRIES) {
      node->data[node->count++] = item;
      updateMBR(node);
    } else {
      RTreeNode *newRoot = createNode(ROOT);
      newRoot->children[newRoot->count++] = node;
      updateMBR(*root);
      *root = newRoot;
      splitNode(newRoot, item);
    }
  } else {
    int bestChildIndex = findBestChildIndex(node, item);
    insert(&(node->children[bestChildIndex]), item);
    updateMBR(node);
  }
}

// search data
SearchResult *search(RTreeNode *root, double latitude, double longitude,
                     int n) {
  // Search a collection of n items that are closer to the given coordinates
  SearchResult *results = (SearchResult *)malloc(n * sizeof(SearchResult));
  int itemsFound = 0;
  DataItem item;
  item.longitude = longitude;
  item.latitude = latitude;

  searchData(root, results, &item, &n, &itemsFound);

  return results;
}

// print tree
void printTree(RTreeNode *root, int depth) {
  for (int i = 0; i < depth; i++)
    printf("  ");
  printf("Node (count=%d, isLeaf=%d):\n", root->count, root->isLeaf);
  for (int i = 0; i < root->count; i++) {
    if (root->isLeaf) {
      for (int j = 0; j < depth + 1; j++)
        printf("  ");
      printf("Data: %s (%.2f, %.2f)\n", root->data[i].name,
             root->data[i].latitude, root->data[i].longitude);
    } else {
      printTree(root->children[i], depth + 1);
    }
  }
}

int main() {
  RTreeNode *root = createNode(1); // 1 means it's a leaf node
  DataItem item;
  double latitude, longitude;
  SearchResult *results;

  char command[50];
  int i;

  while (1) {
    scanf("%s", command);
    if (strcmp("insert", command) == 0) {
      scanf("%lf%lf%s", &item.latitude, &item.longitude, item.name);
      insert(&root, item);
    } else if (strcmp("print", command) == 0) {
      printTree(root, 0);
    } else if (strcmp("exit", command) == 0) {
      break;
    } else if (strcmp("search", command) == 0) {
      scanf("%lf%lf", &latitude, &longitude);
      results = search(root, latitude, longitude, 2);
      for (i = 0; i < 2; i++) {
        printf("%s (%.2f, %.2f) - Distance: %.2f\n", results[i].item.name,
               results[i].item.latitude, results[i].item.longitude,
               results[i].distance);
      }
    } else {
      printf("Unknow command.\n");
    }
  }

  free(results);
  free(root);
  return 0;
}

void updateMBR(RTreeNode *node) {
  int i;
  double minX, minY, maxX, maxY;
  minX = minY = DBL_MAX;
  maxX = maxY = -DBL_MAX;

  if (node->isLeaf) {
    for (i = 0; i < node->count; i++) {
      // Could be simplified for the special case of minX = maxX and minY = maxY
      minX = (node->data[i].longitude < minX) ? node->data[i].longitude : minX;
      minY = (node->data[i].latitude < minY) ? node->data[i].latitude : minY;
      maxX = (node->data[i].longitude > maxX) ? node->data[i].longitude : maxX;
      maxY = (node->data[i].latitude > maxY) ? node->data[i].latitude : maxY;
    }
  } else {
    for (i = 0; i < node->count; i++) {
      minX = (node->children[i]->mbr.minX < minX) ? node->children[i]->mbr.minX
                                                  : minX;
      minY = (node->children[i]->mbr.minY < minY) ? node->children[i]->mbr.minY
                                                  : minY;
      maxX = (node->children[i]->mbr.maxX > maxX) ? node->children[i]->mbr.maxX
                                                  : maxX;
      maxY = (node->children[i]->mbr.maxY > maxY) ? node->children[i]->mbr.maxY
                                                  : maxY;
    }
  }

  node->mbr.minX = minX;
  node->mbr.minY = minY;
  node->mbr.maxX = maxX;
  node->mbr.maxY = maxY;
};

RTreeNode *createNode(int isLeaf) {
  RTreeNode *newNode = (RTreeNode *)malloc(sizeof(RTreeNode));
  newNode->isLeaf = isLeaf;
  newNode->count = 0;
  newNode->mbr.minX = newNode->mbr.minY = DBL_MAX;
  newNode->mbr.maxX = newNode->mbr.maxY = -DBL_MAX;
  memset(newNode->children, 0, sizeof(newNode->children));
  return newNode;
};

double calculateDistance(double x1, double y1, double x2, double y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
};

void splitNode(RTreeNode *root, DataItem item) {
  RTreeNode *oldNode = root->children[0];
  RTreeNode *newNode = createNode(LEAF);
  DataItem allItems[MAX_ENTRIES + 1];
  int i, j, bestIndex;
  double minDistance = DBL_MAX, curDistance = 0;

  root->children[root->count++] = newNode;

  // Decide which half of the list the item should be added to
  // The one that it is closer to
  double distFH =
      calculateDistance(oldNode->data[0].longitude, oldNode->data[0].latitude,
                        item.longitude, item.latitude);
  double distSH = calculateDistance(oldNode->data[oldNode->count / 2].longitude,
                                    oldNode->data[oldNode->count / 2].latitude,
                                    item.longitude, item.latitude);

  // Add the item close to the best child
  for (i = 0; i < oldNode->count; i++) {
    curDistance =
        calculateDistance(oldNode->data[i].longitude, oldNode->data[i].latitude,
                          item.longitude, item.latitude);
    if (curDistance < minDistance) {
      minDistance = curDistance;
      bestIndex = i;
    }
  }

  // Add the items up until the bestIndex
  for (i = 0; i <= bestIndex; i++)
    allItems[i] = oldNode->data[i];
  allItems[i++] = item;

  // Add the rest of the items
  for (; i <= oldNode->count; i++)
    allItems[i] = oldNode->data[i - 1];

  // printf("Collected items: \n");
  // for (int k = 0; k < i; k++)
  //   printf("%s ", allItems[k].name);
  // printf("\n");

  oldNode->count = 0;

  // Split the data items between the new and old node
  for (j = 0; j < i; j++) {
    if (j < (i + (distFH < distSH)) / 2)
      oldNode->data[oldNode->count++] = allItems[j];
    else
      newNode->data[newNode->count++] = allItems[j];
  }

  updateMBR(oldNode);
  updateMBR(newNode);

  updateMBR(root);
}

int findBestChildIndex(RTreeNode *parent, DataItem item) {
  if (!parent || parent->isLeaf)
    return 0;
  double minDistance = DBL_MAX, curDistance;
  int bestIndex = 0;

  for (int i = 0; i < parent->count; i++) {
    curDistance = calculateDistance(parent->children[i]->mbr.minX,
                                    parent->children[i]->mbr.minY,
                                    item.longitude, item.latitude);
    if (curDistance < minDistance) {
      minDistance = curDistance;
      bestIndex = i;
    }
  }

  return bestIndex;
};

void swapResults(SearchResult *a, SearchResult *b) {
  if (!a || !b || a == b)
    return;

  DataItem tmp = a->item;
  a->item = b->item;
  b->item = tmp;

  a->distance += b->distance;
  b->distance = a->distance - b->distance;
  a->distance -= b->distance;
};

void sortResults(SearchResult *results, int *n) {
  if (!results)
    return;
  int swapped = 1, i;

  while (swapped) {
    swapped = 0;
    for (i = 0; i < *n - 1; i++) {
      if (results[i].distance > results[i + 1].distance) {
        swapResults(&results[i], &results[i + 1]);
        swapped = 1;
      }
    }
  }
}

void updateResults(SearchResult *results, int *n, int *nFound,
                   DataItem *nodeItem, double *distance) {
  if (*nFound < *n) {
    results[*nFound].item = *nodeItem;
    results[*nFound].distance = *distance;
    *nFound = *nFound + 1;
  } else {
    int j, k;
    sortResults(results, n);

    // Find and replace farther items
    for (j = 0; j < *n; j++) {
      if (results[j].distance > *distance) {
        // Shift all the right elements to the right by one spot
        for (k = *n - 1; k > j; k--) {
          results[k].item = results[k - 1].item;
          results[k].distance = results[k - 1].distance;
        }
        results[k].item = *nodeItem;
        results[k].distance = *distance;
        break;
      }
    }
  }
};

void searchData(RTreeNode *node, SearchResult *results, DataItem *item, int *n,
                int *nItemsFound) {
  double curDistance = 0;
  int i, j, k;

  // printf("Visiting:\n");
  // printTree(node, 0);
  // printf("\n");
  // printf("\nResults List(%d): ", *nItemsFound);
  // for (int l = 0; l < *nItemsFound; l++)
  //   printf("%s (%.2f) | ", results[l].item.name, results[l].distance);
  // printf("\n");

  if (node->isLeaf) {
    for (i = 0; i < node->count; i++) {
      curDistance =
          calculateDistance(node->data[i].longitude, node->data[i].latitude,
                            item->longitude, item->latitude);

      updateResults(results, n, nItemsFound, &node->data[i], &curDistance);
    }
  } else {
    for (i = 0; i < node->count; i++)
      searchData(node->children[i], results, item, n, nItemsFound);
  }
};
