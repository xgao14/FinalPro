#ifndef __STOPWORD_H__
#define __STOPWORD_H__

#include <string>

#define Max_WORD 27

struct stop_node
{
  char		  key;
  stop_node*  child[Max_WORD]; //26 letters and char '
  bool        word_flag;  //0: not the end of a word; 1: the end of a word
};

class stopWord
{
    private:
      stop_node*  T_root;
    public:  
      stopWord();
      ~stopWord();
    public:
      bool search(std::string key);  //false: not a stopword; true: is a stop word
      void output();
};

#endif 


