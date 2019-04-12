#include "stopword.h"
#include <iostream>
#include <queue> 

/*
 *Create a new node
 */
stop_node* creatNode(char letter)
{
    stop_node* newNode = new stop_node;
    newNode->key = letter;
    for(int i=0; i<Max_WORD; i++)
    {
        newNode->child[i] = NULL;
    }
    newNode->word_flag = false;
    return newNode;
}

/*
 *Calculate the index of the letter
 */
int cIndex(char letter)
{
    char a = 'a';
    char A = 'A';
    int index = -1;
    if(letter >= 'a' && letter <= 'z')
    {
        index = (int)letter%(int)a;
    }
    else if(letter >= 'A' && letter <= 'Z')
    {
        index = (int)letter%(int)A;
    }
    else if(letter == '\'')
    {
        index = 26;//last site
    }
    return index;
}

/*
 *Create the stopword trie
 */
stopWord::stopWord()
{
//This stopword list reference from https://github.com/Yoast/YoastSEO.js/blob/develop/src/config/stopwords.js
	std::string wordArr[]= \
  {"a", "about", "above", "after", "again", "against", "all", "am", "an", "and", "any", "are", "as", "at", \
  "be", "because", "been", "before", "being", "below", "between", "both", "but", "by", \
  "can", "could", \
  "did", "do", "does", "doing", "down", "during", \
  "each", \
  "few", "for", "from", "further", \
  "had", "has", "have", "having", "he", "he'd", "he'll", "he's", "her", "here", "here's", "hers", "herself", "him", "himself", "his", "how", "how's", \
  "i", "i'd", "i'll", "i'm", "i've", "if", "in", "into", "is", "it", "it's", "its", "itself", \
  "let's", \
  "me", "more", "most", "my", "myself", \
  "nor", \
  "of", "on", "once", "only", "or", "other", "ought", "our", "ours", "ourselves", "out", "over", "own", \
  "same", "she", "she'd", "she'll", "she's", "should", "so", "some", "such", \
  "than", "that", "that's", "the", "their", "theirs", "them", "themselves", "then", "there", "there's", "these", "they", "they'd", "they'll", "they're", "they've", "this", "those", "through", "to", "too", \
  "under", "until", "up", "us", \
  "very", \
  "was", "we", "we'd", "we'll", "we're", "we've", "were", "what", "what's", "when", "when's", "where", "where's", "which", "while", "who", "who's", "whom", "why", "why's", "with", "would", \
  "you", "you'd", "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves"};  

    T_root = new stop_node; //Root doesn't store key
    T_root->key = '*'; //Use * represents the root key
    for(int i=0; i<Max_WORD; i++)
    {
        T_root->child[i] = NULL;
    }
    T_root->word_flag = false;

    for(int iarr=0; iarr<155; iarr++) //Add all the 155 stopword into stopword trie
    {
    	stop_node* p = T_root;
        int index = -1;

        for(int i=0; i<wordArr[iarr].length(); i++)
        {
            index = cIndex(wordArr[iarr][i]); //Calculate the index
            if(index == -1)//Invalid letter means invalid word
            {
                break;
            }
            if(p->child[index] == NULL) //This node doesn't exist, add it
            {
                stop_node* newNode = creatNode(wordArr[iarr][i]);
                p->child[index] = newNode;
            }
            p = p->child[index];
        }
        if(p != T_root) //Set the end node of the word flag into true
        {
            p->word_flag = true;
        }
    }
    
}

/*
 *Delete the node of trie
 */
void deleNode(stop_node* node)
{
    for(int i=0; i<Max_WORD; i++)
    {
        if(node->child[i] != NULL)
        {
            deleNode(node->child[i]);
        }
    }
    delete node;
}

stopWord::~stopWord()
{
    deleNode(T_root);
}

/*
 *Search a word in the stopword trie
 */
bool stopWord::search(std::string key)
{
    int index = -1;
    stop_node* p = T_root;
    int i=0; 
    for(; i<key.length(); i++)
    {
        index = cIndex(key[i]); //Calculate the index
        if(index == -1)//Invalid letter means invalid word
        {
            return false;
        }
        if(p->child[index] != NULL)
        {
            p = p->child[index];
        }
        else //Can't search until the word end, so the word doesn't exist 
        {
        	return false;
		}
    }
    if(i == key.length() && p != NULL && p->word_flag == true) //The word exists, it is a stopword 
    {
        return true;
    }

    return false;
}

/*
 *Output nodes for debuging
 */
void stopWord::output()
{
    //output_sw(T_root);
    stop_node* p = NULL;
    std::queue<stop_node*> que;
    que.push(T_root);
    while(!que.empty())
    {
    	p = que.front();
    	std::cout << p->key << " " << p->word_flag;
    	que.pop();
        for(int i=0; i<Max_WORD; i++)
        {
            if(p->child[i] != NULL)
            {
                que.push(p->child[i]);
            }
        }
        while(!que.empty())
        {
            p = que.front();
            std::cout << p->key << " " << p->word_flag << std::endl;
            que.pop();
            for(int i=0; i<Max_WORD; i++)
            {
                if(p->child[i] != NULL)
                {
                    que.push(p->child[i]);
                }
            }
        }
        std::cout << "\n" << std::endl;
    }
}
