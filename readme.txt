For this project, I used two computer languages ​​to implement the functions. They are Javascript and C++.

Firstly, I implemented a website using Javascript. The website contains five html pages and each of them has linked to another. 

Then I used Javascript("crawler" package) to download the web pages and save the words in these pages into ".txt" files which are in the "file" folder. These files have been named as "myfile" plus count of the current page. This count will be saved in my trie. For example, the content of the first web page that it downloaded will be saved in file\myfile0.txt. When user search the words that included in this page, the result will be '0' which means the first page. In order to avoid the trie which I will construct to store these words in later program being too big, I restrict the number of pages to download smaller than or equal to 50. At the same time, I save these file names into filelist.txt file and save corrensponding urls into url.txt.

User can write down the websites urls in "crawler\UrlData.json" file. In this file, the first field is the number of websites that are going to crawl and the second field is an array of urls of these websites. The crawler program will read this file and crawl these urls and the hyperlinks pages included in them automatically.

These js scripts are in "crawler" folder and they are running in Nodejs envirement. 

Finally, I used C++ to construct a trie to store the words in these downloaded web pages and provide searching function.

In my C++ program, the program first gets the file names in the "filelist.txt" file, then reads the corresponding files based on these file names and inserts the words reading from those files into the trie. Before the insertion operation, it will judge whether the word is a stopword. If the word is a stopword, it will not perform the insertion operation. There are 155 stop words predefined in this program. These stop words are also stored in a trie. 

After the trie is created, the program will enter the user interaction mode. If the user inputs the words to search,  the program will judge every word that inputed is stopword or not. Then the program will search the non stop words in the tire and display the intersection set of the pages and corresponding urls where the words appear to the user.

In my program, the trie node includes a key which is the character of this node represented, an array of children nodes which point to its 27 children(Note: The valid characters in my program include 26 alphabet and ''', since some words contain character '''.) and a link list of the pages that the word appears.
In the children node array of the trie node, the letters are stored in the order of the alphabet. For example, 'a' corresponds to child[0], 'z' corresponds to child[25]. ''' character is stored in the last position of the array, which is child[26].

The root node of trie does not store any characters.

When inserting a word, start from the first letter of the word. Judge if the child node of root which corresponds to the first letter of the word exist. If it does not exist, create a new node for the child node and save the letter into it. Then, continue to judge the child node which corresponds to the second letter of the word. Repeat this step until all the letters of the word are inserted or already exist in the trie. Then judge if the current page has been stored in the page link list of the node which corresponds to the last letter of the word. If not, insert the new page into the link list. Otherwise, do not save the page again.

When searching a word, start from the child node of root which corresponding to the first letter of the word. Judge the existence of nodes which correspond to each letter in the word. If any of the corresponding node does not exist, the word does not exist in the trie. If all the letters of the word are found but there is no page stored in the node which corresponds to the last letter of the word, the word does not exist in the trie. If the word is found and the page link list exists at the node which corresponds to the last letter of the word, return the link list of the pages.








