//
//  Insight Data Engineering Coding Challenge
//  Daven Mahendra Bhatt
//

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

//////////////////////////////////////////////////////////////////////
struct tweetProp
{
    vector<string> uniqWords;
    vector<int> wordCount;
    int numUniq;
};

struct allTweet
{
    string tweetWord;
    int tweetWordCount;
};


tweetProp getProperties(tweetProp, string);     //Obtain the unique words and frequency from a single tweet
tweetProp updateMaster(tweetProp, tweetProp);   //Update the master list of unique words and their frequency

tweetProp sortFinalList(tweetProp);             //Sorts the master list in alphabetical per ASCII Code
bool wordCompare(const allTweet & lhs, const allTweet & rhs); //Compare function for sorting struct array
double calcMedian(vector<int>);                 //Calculates median of a vector of integers

//////////////////////////////////////////////////////////////////////
int main()
{
 
    tweetProp singleTweet;
    tweetProp masterList;
    masterList.numUniq=0;
    singleTweet.numUniq=0;
    
    vector<int> uniqNumList;
    vector<double> runningMedianList;
    string tweetLine;

    int maxColWidth = 0;
//////////////////////////////////////////////////////////////////////////////

    //READ INPUT FILE
    
    ifstream infile( "tweet_input/tweets.txt" );
    
    if ( ! infile.is_open() )
    {
        cout << "Could not open file!" << "\n";
    }
    
    
//////////////////////////////////////////////////////////////////////////
    
    //Iterating through every tweet in file, update the master list of unique words,
    //their frequency, and the associated running median list
    
    while (!infile.eof())
    {
        //Read in one tweet
        getline (infile,tweetLine);
        
        //Get tweet Properties
        singleTweet = getProperties(singleTweet, tweetLine);

        //Compare with and update MasterList
        masterList = updateMaster(masterList, singleTweet);

        //  update running median list
        uniqNumList.push_back(singleTweet.numUniq);
        runningMedianList.push_back(calcMedian(uniqNumList));
 
    }
    
    masterList = sortFinalList(masterList);

    
//////////////////////////////////////////////////////////////////////////////
    
//Output data to files ft1.txt and ft2.txt
    
    
    //Get width of largest word in list to set max column width
    for (int i=0; i<masterList.uniqWords.size(); i++)
    {
        if (maxColWidth<masterList.uniqWords[i].size())
        {
            maxColWidth=int(masterList.uniqWords[i].size()+10);
        }
    }
    
    //Output Master List of All Unique words and their frequency
    ofstream writeMasterList("tweet_output/ft1.txt");
    for (int i = 0; i<masterList.numUniq; i++)
    {
        writeMasterList<< setw(maxColWidth) << left << masterList.uniqWords[i] <<masterList.wordCount[i]<<endl;
    }

    //Output Running Median List
    ofstream writeRunningMedian("tweet_output/ft2.txt");
    for (int i = 0; i<runningMedianList.size();i++)
    {
        writeRunningMedian << runningMedianList[i]<<endl;
    }
}



tweetProp getProperties(tweetProp tweet, string tweetLine)
{
    
    int checkIndex = 0;
    vector<string> allWords;
    stringstream word(tweetLine);
 
    //Take the string tweetLine and create a vector of strings
    //comprised of all the words within the tweet
    while(!word.eof())
    {
        string tempWord;
        word >> tempWord;
        allWords.push_back(tempWord);
    }
    
    // Sort all words within the vector
    sort( allWords.begin(), allWords.end() );
    
    //Create another string vector to contain only the unique words from the input tweet
    tweet.uniqWords = allWords;
    
    //Remove all duplicates
    tweet.uniqWords.erase( unique( tweet.uniqWords.begin(), tweet.uniqWords.end() ), tweet.uniqWords.end() );
    
    tweet.numUniq = int(tweet.uniqWords.size());
    
    //Initialize the vector containing the frequency each word is used in a tweet
    vector<int> wordCount(tweet.uniqWords.size());
    tweet.wordCount = wordCount;
    
    // Compare each word from Unique list with every unseen word in All Word List
    // increment the word count each time the word is duplicated
    
    for (int i = 0; i < tweet.uniqWords.size(); i++)
    {
        for ( int j = checkIndex; j < allWords.size(); j++)
        {
            
            if (tweet.uniqWords[i] == allWords[j])
            {
                tweet.wordCount[i]++;
                checkIndex++;
            }
            else
            {
                break;
            }
            
        }
    }
    
    return tweet;
};

double calcMedian(vector<int> numUniq)
{
    double median;
    int medianIndex1, medianIndex2;
    int sizeVec = int(numUniq.size());
    
    medianIndex1 = (sizeVec / 2);
    if (sizeVec % 2 == 1) //size is odd. median is center value
    {
        median = numUniq[medianIndex1];
    }
    else //size is even. Median is average of two center values
    {
        medianIndex2=medianIndex1-1;
        median = (numUniq[medianIndex1] + numUniq[medianIndex2])/2.0;
    }
    
    return median;
};


tweetProp updateMaster(tweetProp masterList, tweetProp currentTweet)
{
 
    //Iterate through the words in the current tweet and compare with
    //the Master List of words. Increment Master List if a duplicate exists
    //Add non repeated words from the current list to the Master List
    
    for (int i = 0; i<currentTweet.uniqWords.size();i++)
    {
        int dupFlag = 0;

        for (int j =0; j<masterList.uniqWords.size();j++)
        {
            
            if (currentTweet.uniqWords[i]==masterList.uniqWords[j])
            {
                dupFlag=1; //set flag if word is repeated in master list and increment frequency
                masterList.wordCount[j]+=currentTweet.wordCount[i];
            }
        
        }
        if (dupFlag==0) //if after the loop the current word is not in the master List, add it (and its count) to the master list
        {
            
            masterList.uniqWords.push_back(currentTweet.uniqWords[i]);
            masterList.wordCount.push_back(currentTweet.wordCount[i]);
            
        }
        masterList.numUniq = int(masterList.uniqWords.size());
    }
    
    return masterList;
};

tweetProp sortFinalList(tweetProp masterList)
{
    
    vector<allTweet> finalList(masterList.numUniq);
    
    for (int i=0; i<masterList.numUniq;i++)
    {
        finalList[i].tweetWord=masterList.uniqWords[i];
        finalList[i].tweetWordCount=masterList.wordCount[i];
    }
    
    sort(finalList.begin(), finalList.end(), wordCompare);

    for (int i=0; i<masterList.numUniq;i++)
    {
        masterList.uniqWords[i]=finalList[i].tweetWord;
        masterList.wordCount[i]=finalList[i].tweetWordCount;
    }
     return masterList;
};

bool wordCompare(const allTweet & word1, const allTweet & word2)
{
    return word1.tweetWord < word2.tweetWord;
};

