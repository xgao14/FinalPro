#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>

#define Max 27

struct node_loc
{
    int location;
    node_loc* next;
};
struct trie_node
{
  char		  key;
  trie_node*  child[Max]; //Array of children
  node_loc* locations;  //Link list of locations
};

class trie
{
    private:
      trie_node*  T_root;
    public:  
      trie();
      ~trie();
    public:
      void insert(std::string key, int page); //Insert word into trie
      node_loc* search(std::string key);  //Return location array
      void output_trie();
};
#endif


