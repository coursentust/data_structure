// Searching a key on a B-tree in C++
#include <deque>
#include <queue>
#include <iostream>
using namespace std;

class TreeNode {
public:
  int *keys;
  int t;
  TreeNode **C;
  int n;
  bool leaf;

  TreeNode(int temp, bool bool_leaf);

  void insertNonFull(int k);
  void splitChild(int i, TreeNode *y);
  void traverse(int level);

  TreeNode *search(int k);

  friend class BTree;
};

class BTree {
public:
  TreeNode *root;
  int t;

  BTree(int temp) {
    root = NULL;
    t = temp;
  }

  void traverse() {
    if (root != NULL)
      root->traverse(0);
  }

  TreeNode *search(int k) {
    return (root == NULL) ? NULL : root->search(k);
  }

  void insert(int k);
};

TreeNode::TreeNode(int t1, bool leaf1) {
  t = t1;
  leaf = leaf1;

  keys = new int[2 * t - 1];
  C = new TreeNode *[2 * t];

  n = 0;
}

void TreeNode::traverse(int level=0) {
  int i;
  for (i = 0; i < n; i++) {
    if (leaf == false)
      C[i]->traverse(level+1);
    printf("level %d i %d %d(n %d t %d)", level,i, keys[i], n,t);
  }

  if (leaf == false)
    C[i]->traverse(level+1);
}

void bfs(BTree* tree){
    if(tree == NULL||tree->root==NULL){
        printf("root is null\n"); return;
    }
    std::deque<TreeNode*> A{tree->root};
    while( A.size()){
        std::deque<TreeNode*> B;
        while( A.size()){
            TreeNode* tmp1 = A.front();
            if(tmp1==nullptr){
                printf(" | ");
                A.pop_front();
                continue;
            }
            auto tmp = *tmp1;
            int i;
            for(i=0; i<tmp.n; i++){
                printf("'%d', ", tmp.keys[i]);
                if(!tmp.leaf){
                    B.push_back(tmp.C[i]);
                    B.push_back(nullptr);
                }
            }
            if(!tmp.leaf)
                B.push_back(tmp.C[i]);
            A.pop_front();
        }
        std::swap(A,B);
    }
}

void bfs2(BTree* tree) {
    auto root = tree->root;
    if (root == NULL) return;

    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
      int count = q.size();
      for (int i = 0; i < count; i++) {
        TreeNode* curr = q.front();
        q.pop();
        for (int j = 0; j < curr->n; j++)
          cout << curr->keys[j] << " ";
        if (!curr->leaf)
          for (int j = 0; j <= curr->n; j++)
            q.push(curr->C[j]);
      }
      cout << endl;
    }
  }

TreeNode *TreeNode::search(int k) {
  int i = 0;
  while (i < n && k > keys[i]){
    i++;
  }
  if (keys[i] == k)
    return this;

  if (leaf == true)
    return NULL;

  return C[i]->search(k);
}

void BTree::insert(int k) {
  if (root == NULL) {
    root = new TreeNode(t, true);
    root->keys[0] = k;
    root->n = 1;
  } else {
    if (root->n == 2 * t - 1) {
      TreeNode *s = new TreeNode(t, false);

      s->C[0] = root;

      s->splitChild(0, root);

      int i = 0;
      if (s->keys[0] < k)
        i++;
      s->C[i]->insertNonFull(k);

      root = s;
    } else
      root->insertNonFull(k);
  }
}

void TreeNode::insertNonFull(int k) {
  int i = n - 1;
  if (leaf == true) {
    while (i >= 0 && keys[i] > k) {
      keys[i + 1] = keys[i];
      i--;}
    keys[i + 1] = k;
    n = n + 1;
  } else {
    while (i >= 0 && keys[i] > k)
      i--;
    if (C[i + 1]->n == 2 * t - 1) {
      splitChild(i + 1, C[i + 1]);
      printf("i %d\n",i);
      if (keys[i + 1] < k)
        {i++;}
    }
    C[i + 1]->insertNonFull(k);
  }
}

void TreeNode::splitChild(int i, TreeNode *y) {
  TreeNode *z = new TreeNode(y->t, y->leaf);
  z->n = t - 1;
  for (int j = 0; j < t - 1; j++){
    z->keys[j] = y->keys[j + t];
  }
  if (y->leaf == false) {
    for (int j = 0; j < t; j++)
        z->C[j] = y->C[j + t];
  }
  y->n = t - 1;
  for (int j = n; j >= i + 1; j--)
    C[j + 1] = C[j];
  C[i + 1] = z;

  for (int j = n - 1; j >= i; j--)
    keys[j + 1] = keys[j];
  keys[i] = y->keys[t - 1];
  n = n + 1;
}



int main() {
  BTree t(3);
//   t.insert(8);
//   t.insert(9);
//   t.insert(10);
//   t.insert(11);
//   t.insert(15);
//   t.insert(16);
//   t.insert(17);
//   t.insert(18);
//   t.insert(20);
//   t.insert(23);
//   for(int i=8; i<24; ++i)t.insert(i);

    vector<int> a={100,35,65,130,180,10,20,40,50,70,80,90,110,120,140,160,190,240,260};
    for(auto i : a){
        t.insert(i);
        bfs(&t);
        // printf("--------\n");
    }
    for(int i=8; i<24; ++i)t.insert(i);

  cout << "The B-tree is: ";
  t.traverse();
  
  bfs(&t);
  bfs2(&t);

  int k = 10;
  (t.search(k) != NULL) ? cout << endl
                 << k << " is found"
              : cout << endl
                 << k << " is not Found";

  k = 2;
  (t.search(k) != NULL) ? cout << endl
                 << k << " is found"
              : cout << endl
                 << k << " is not Found\n";
}