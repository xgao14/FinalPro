const crawler = require("crawler");
const bluebird = require("bluebird");
const Promise = bluebird.Promise;
const fs = bluebird.promisifyAll(require("fs"));
const cra = new crawler();
let obselete = []; //Save the urls that have crawled
let fileCount = 0; //The number of web pages that have saved, each file saves the content of one web page
const filePath = "../file/"; //The relative path of the files that store the content of web pages

/*Read the content of a given file. 
 *In this program it reads the UrlData.json file which store the urls that will crawl
 */
async function getFileAsJSON(path){
    if (!path) {
        throw "You must provide the file path";
    }
    let fileContent = '';
    try{
        fileContent = await fs.readFileAsync(path, "utf-8");
    }catch (e) {
        throw e;
    }
    let obj = NaN; //Store the data in UrlData.json into an object
    try{
        obj = JSON.parse(fileContent);
    }catch (e) {
        throw e;
    }
    return obj;
}

/*Firstly, writes the name of file that store the content of web page in the filelist.txt file
 *Secondly, create and write the content of web page into a new file
 */
async function saveToFile(path, text){
    if (!path || !text) {
        throw "You must provide the file path and text";
    }
    if (typeof text !== "string") {
        throw "The second parameter should be a string"
    }

    try {
        let addFileName = "";
/*        if (fileCount == 0) {
            addFileName = "myfile" + fileCount.toString() + ".txt";
        }
        else {
            addFileName = "\r\n" + "myfile" + fileCount.toString() + ".txt"; //The name of the new file
        }*/
        addFileName = "myfile" + fileCount.toString() + ".txt" + "\r\n";
        await fs.appendFileAsync("../filelist.txt", addFileName); //Add the new file name into filelist.txt
    } catch (e) {
        throw e;
    }

    try{
        await fs.writeFileAsync(path, text); //Write the web page content into the file
    }catch (e) {
        throw e;
    }
    return true;
}

async function saveUrl(url){
    if (!url) {
        throw "You must provide the url";
    }
    if (typeof url !== "string") {
        throw "The second parameter should be a string"
    }

    try {
        let addUrl = "";
        /*if (fileCount == 0) {
            addUrl = url;
        }
        else {
            addUrl = "\r\n" + url; //The name of the new file
        }*/
        addUrl = url + "\r\n";
        await fs.appendFileAsync("../url.txt", addUrl); //Add the new file name into filelist.txt
    } catch (e) {
        throw e;
    }
    return true;
}

/*Crawler a given website
 *Firstly, it crawl the web page that has given
 *Then it will continue crawling the hyperlinks that in current web page
 */
async function crawlerAllPages (url) {
    cra.queue({ //Crawl the given url
        uri: url,
        callback: async (e, res, done) => {
            if(e) {
                throw e;
            } 
            else {
                let $ = res.$; //Contain all the content of the web page
                let content = $.text(); //Extract all the words of the web page
                if(content) { //If content is not null, save them into file
                    let path = filePath + "myfile" + fileCount.toString() + ".txt";
                    await saveToFile(path, content);
                    await saveUrl(url);
                    fileCount++;
                    if(fileCount >= 50) { //In case of the number of files is too much and make the trie too big
                        return;
                    }
                }
                let link = $("a"); //Get the hyperlinks in this page
                Object.keys(link).forEach((item) => { //Crawl the hyperlinks
                    if (link[item].type === 'tag') {
                        let href = link[item].attribs.href;
                        if (href && !obselete.includes(href)) { //If the hyperlink isn't crawled, crawl it
                            href = href.trim();
                            obselete.push(href);
                            
                            setTimeout( async () => { //Slow down the crawl
                                if (href.startsWith('http')) {
                                    await crawlerAllPages(href);
                                }
                                else {
                                    await crawlerAllPages(`${url}${href}`);
                                }
                            }, 5000);
                            console.log(href);
                        }
                    }
                });
            }
            done();
        }
    });
}

/*Get the url in UrlData.json file and begin to crawl
 */
async function main () {
    let path = `${__dirname}/UrlData.json`;
    let urlData;
    try {
        urlData = await getFileAsJSON(path);
    } catch (e) {
        throw e;
    }

    try {// Get the number of files now and then create new file name according the number
        let count = await fs.readFileAsync("../filelist.txt", 'utf-8');
        fileCount +=  count.split('\n').length - 1;
        console.log(fileCount);
    } catch (e) {
        // This means the filelist.txt not exist, it will be created later
    }
    
    for (let i=0; i<urlData.url.length; i++) { //Crawl the url listed in UrlData.json
        if(fileCount > 50) {
            console.log("Too much web pages!");
            return;
        }
        await crawlerAllPages(urlData.url[i]);
    }
}

main().catch(e => {
    console.log(e);
  });
