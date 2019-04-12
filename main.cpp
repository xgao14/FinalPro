#include <iostream>
#include <fstream>
#include <windows.h>
#include <set>
#include "trie.h"
#include "stopword.h"
#include "readfile.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

/*Judge if the letter is a valid letter
 *Valid letter include 26 letter and ', such as I'm, we'll etc.
 *It also change the letter into lower case if the letter is upper case
 */
bool isVaild(char* con)
{
	char a = 'a';
    char A = 'A';
    if(*con >= 'a' && *con <= 'z')
    {
        return true;
    }
    else if(*con >= 'A' && *con <= 'Z')
    {
		*con = *con + 32;//change the up letter to low 
        return true;
    }
    else if(*con == '\'')
    {
    	return true;
	}
	else
	{
		return false;
	}
}

/*
 *Read the file and add the words into tire 
 */
void createIndex(const std::string filename, trie* pTrie, stopWord* pStopword)
{
	char str = filename[filename.length()-5]; //the last char of filename represent the current page
	int page = atoi(&str);
	
	std::fstream filecon;
//	std::cout << "filename: " << filename << std::endl;
	filecon.open(filename.c_str());
	if(!filecon.is_open())
	{
		std::cout << "File: " << filename << " opened failed!" << std::endl;
		return;
	}
	char con;
	std::string word = "";
	while(!filecon.eof())
	{
		con = filecon.get(); //Read a char from the file
		bool isValidChar = isVaild(&con); //Judge the char is valid
		if(isValidChar) //If the char is valid, it means this is not the end of a word
		{
			word += con; //Add the char to the current word
		}
		else //If the char isn't valid, it means this is the end of the word
		{
//			cout << "word: " << word << endl;
			if(word != "")
			{
				bool isStopword = pStopword->search(word);//Judge if the word is stopword
				if(!isStopword)//if the word isn't a stop word, add the word into trie
				{
					pTrie->insert(word, page);
				}
				word = ""; // Reset the word
			}
		}
	}
	filecon.close();
}

/*
 *Search the pages include the words
 */
void searchWords(trie* pTrie, stopWord* pStopword, std::string str, std::set<int>* resPage)
{
	node_loc* p = NULL;
	std::string word = "";
	/*Firstly, split the searching string into valid words
	 *Secondly, judge if the words are stopword, we don't need to search stopword
	 *Thirdly, search the words and find the intersective pages of these words
	 */
	for(int i=0; i<str.length(); i++)
	{
		bool isValidChar = isVaild(&str[i]);
		if(isValidChar) //Not the end of a word
		{
			word += str[i];
		}
		else//The end of a word
		{
			if(word != "") 
			{
				bool isStopword = pStopword->search(word);
				if(!isStopword)//If the word isn't a stop word, search the word in the trie
				{
					p = pTrie->search(word);
					if(resPage->empty()) //Add the first word result pages in resPage 
					{
						while(p != NULL)
						{
							resPage->insert(p->location);
							p = p->next;
						}
					}
					else //For the result of pages of second word and the words behand it, save the intersective pages and remove the difference pages 
					{
						std::set<int> temp; //Save the pages
						while(p != NULL)
						{
							temp.insert(p->location);
							p = p->next;
						}
						//Remove the pages that different from resPage
						for(std::set<int>::iterator iter=resPage->begin(); iter != resPage->end(); iter++) 
						{
							if(temp.find(*iter) == temp.end())
							{
								resPage->erase(iter);
							}
						}
						
					}
				}
				word = "";
			}
		}
	}
	if(word != "")//This is the last word
	{
		bool isStopword = pStopword->search(word);
		if(!isStopword)//If the word isn't a stop word, search the word in the trie
		{
			p = pTrie->search(word);
			if(resPage->empty()) //Add the first word result pages in resPage 
			{
				while(p != NULL)
				{
					resPage->insert(p->location);
					p = p->next;
				}
			}
			else //for the result of pages of second word and the words behand it, save the intersection pages and remove the difference pages 
			{
				std::set<int> temp; //Save the result
				while(p != NULL)
				{
					temp.insert(p->location);
					p = p->next;
				}
				for(std::set<int>::iterator iter=resPage->begin(); iter != resPage->end(); iter++) //Remove the pages that different
				{
					if(temp.find(*iter) == temp.end())
					{
						resPage->erase(iter);
					}
				}
						
			}
		}
		word = "";
	}
}

void getUrl(std::string* urlarr)
{
	char buffer[100];
	int icount = 0;
	std::ifstream urllist;
	urllist.open("url.txt", std::ios::in); //Read only, get the url list
	if(!urllist.is_open())
	{
		std::cout << "File: url.txt opened failed!" << std::endl;
		return;
	}
	while(!urllist.eof())
    {
        urllist.getline(buffer, 100); //Get url
        urlarr[icount] = buffer;
        icount++;
    }
    urllist.close();
}

/*main function
 */
int main(int argc, char** argv) {

	trie* ctrie = new trie;
	stopWord* cstopWord = new stopWord;

	readfile cfilelist;
	std::string* filelis = cfilelist.getFileList(); //Get the files name that store the web pages
	int num = cfilelist.getFileNum(); //Get the number of files that store the web pages
	char path[100];
 	GetModuleFileName(NULL, path, 100); //Current program running path
 	std::string filePath(path);
 	filePath = filePath.substr(0, filePath.length()-13) + "file\\";
	for(int i=0; i<num; i++) //Add all the words in the web pages into trie
	{
		createIndex(filePath+filelis[i], ctrie, cstopWord);
	}

	std::string urlarr[50];
	getUrl(urlarr); //Get all url list

	node_loc* pLoc = NULL;
    std::string searchStr = "N"; //If it is Y/y, exit program; else continue
    while(searchStr != "Y" && searchStr != "y")
    {
            searchStr = "";
            std::cout << "Please input the words you want to search or Y/y to exit this program." << std::endl;
            getline (std::cin, searchStr);
            if(searchStr == "Y" || searchStr == "y") //Exit the program
            {
                delete ctrie;
                delete cstopWord;
                return 0;
            }

			std::set<int> resPage;
			searchWords(ctrie, cstopWord, searchStr, &resPage); //Search the words inputed
            if(resPage.empty())
            {
                    std::cout << "No result!" << std::endl;
            }
        	else
        	{
        		std::set<int>::iterator iter;
				std::cout << "Result page: " << std::endl;
				for(iter = resPage.begin() ; iter != resPage.end() ; iter++)
				{
					std::cout << *iter << ": " << urlarr[*iter] << std::endl;
				}
			} 
    }

}



