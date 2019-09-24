#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "gwion_util.h"

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (256)

// trie node
struct TrieNode {
  struct TrieNode *children[ALPHABET_SIZE];
  int idx;
};

struct TrieRoot {
  struct TrieNode *self;
  int idx;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void) {
  struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
  if(pNode) {
    pNode->idx = 0;
    for(int i = 0; i < ALPHABET_SIZE; i++)
       pNode->children[i] = NULL;
  }
  return pNode;
}
/*
struct TrieRoot *getRoot(int idx) {
  struct TrieRoot *root = (struct TrieRoot *)malloc(sizeof(struct TrieRoot));
  root->self = getNode();
  root->idx = idx;
  return root;
}
*/
// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
//static void insert(struct TrieNode *const root, const char *key) {
static struct TrieNode* insert(struct TrieNode *const restrict root, const char *key) {
  const int length = strlen(key);
  struct TrieNode *pCrawl = root;
  for(int level = 0; level < length; level++)  {
      const int index = key[level];
      if(!pCrawl->children[index])
          pCrawl->children[index] = getNode();
      pCrawl = pCrawl->children[index];
  }
  return pCrawl;
}

// Returns true if key presents in trie, else false
int search(struct TrieNode *const restrict root, const char *key) {
  const int length = strlen(key);
  struct TrieNode *n = root;
  for (int level = 0; level < length; level++) {
      const int index = key[level];
      if(!n->children[index])
          return false;
      n = n->children[index];
  }
  return n ? n->idx : 0;
}

void free_node(struct TrieNode *n) {
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if(n->children[i])
      free_node(n->children[i]);
  }
  free(n);
}


// Driver
struct TrieNode* drive(const Vector v) {
  struct TrieNode *root = getNode();
  for(m_uint i = 0; i < vector_size(v); ++i) {
    struct TrieNode *n = insert(root, (char*)vector_at(v, i));
    n->idx = i + 1;
  }
  return root;
}
