#include "trie.h"
#include <iostream>

trie::trie()
{
    T_root = new trie_node; //Root doesn't store key
    T_root->key = '*'; //Use * represents the root key
    for(int i=0; i<Max; i++)
    {
        T_root->child[i] = NULL;
    }
    T_root->locations = NULL;
}

/*
 *Delete the nodes location
 */
void delLoc(trie_node* node)
{
    node_loc* p = node->locations;
    node_loc* q = NULL;
    while(p != NULL)
    {
        q = p->next;
        delete p;
        p = q;
    }
}

/*
 *Delete the nodes of trie
 */
void delNode(trie_node* node)
{
    for(int i=0; i<Max; i++)
    {
        if(node->child[i] != NULL)
        {
            delNode(node->child[i]);
        }
    }
    delLoc(node);
    delete node;
}

trie::~trie()
{
    delNode(T_root);
}

/*
 *Create a new trie node
 */
trie_node* createNode(char letter)
{
    trie_node* newNode = new trie_node;
    newNode->key = letter;

    for(int i=0; i<Max; i++)
    {
        newNode->child[i] = NULL;
    }
    newNode->locations = NULL;

    return newNode;
}

/*
 *Calculate the index of a letter.
 *The index of a letter is the order that it in the alphabet. ' is the last index which is 26.
 */
int calIndex(char letter)
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
    	index = 26;
	}
    return index;
}

/*
 *Insert a word into trie
 */
void trie::insert(std::string key, int page)
{
    trie_node* p = T_root;
    int index = -1;
    for(int i=0; i<key.length(); i++) //Add the word into trie
    {
        index = calIndex(key[i]); //Calculate the index of the letter
        if(index == -1)//Invalid letter, the word is invalid
        {
            return;
        }
        if(p->child[index] == NULL) //this node isn't exist, add it
        {
            trie_node* newNode = createNode(key[i]);
            p->child[index] = newNode;
        }
        p = p->child[index];
    }
    if(p != T_root) //Add the page of the word into the last node of this word in the trie
    {
        if(p->locations == NULL) //This word hasn't had a page, add a new one
        {
            p->locations = new node_loc;
            p->locations->location = page;
            p->locations->next = NULL;
        }
        else //This word has had pages, add the new page in the front of the location link list
        {
            node_loc* pLoc = p->locations;
            bool is_stored = false;
            while(pLoc != NULL)
            {
                if(pLoc->location == page)//This page has existed, don't need to add again
                {
                    is_stored = true;
                    break;
                }
                pLoc = pLoc->next;
            }
            if(!is_stored)  //Add the new page in the front of the location link list
            {
                node_loc* newLoc = new node_loc;
                newLoc->location = page;
                newLoc->next = p->locations;
                p->locations = newLoc;
            }
        }
    }
}

/*
 *Search a word in the trie
 */
node_loc* trie::search(std::string key)
{
    int index = -1;
    trie_node* p = T_root;
    int i=0;
    for(; i<key.length(); i++)
    {
        index = calIndex(key[i]); //Calculate the index of the letter
        if(index == -1)//Invalid letter means invalid word
        {
            return NULL;
        }
        if(p->child[index] != NULL)
        {
            p = p->child[index];
        }
        else //Can't find all the letter of the word, so the word doesn't exist
        {
        	return NULL;
		}
    }
    if(i == key.length() && p != NULL && p->locations != NULL) //The word exist
    {
        return p->locations;
    }

    return NULL;
}

/*
 *Output trie node for debuging
 */
void output(trie_node* node)
{
    for(int i=0; i<Max; i++)
    {
        if(node->child[i] != NULL)
        {
            output(node->child[i]);
        }
    }
    std::cout << node->key << std::endl;
}

void trie::output_trie()
{
    output(T_root);
}


