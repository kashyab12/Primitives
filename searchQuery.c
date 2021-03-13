// Including the respective Header File.
#include "searchQuery.h"

//Method to calculate the Term Frequency tf (Given the term and the root of the Trie)
double calcTermFrequency(char *term, struct trieNode *indexedPages)
{
    // Declaring local variables to find the Term Frequency and to compute the score.
    int termFrequency = 0; 
    double tfScore = 0;

    // Equating the number of times a word occures in the Trie to termFrequency.
    termFrequency = findWordOccurence(indexedPages, term);

    if (termFrequency == 0)
    {
        tfScore = 0; // Save Computation Time.
    }
    else
    {
        // Using the formula given in the Project Description to compute the tf score.
        tfScore = (double)((double)termFrequency / (double)indexedPages->totalWords);
    }

    return tfScore;
}

//Method to calculate the Inverse Document Frequency idf (Given the total indexed Pages and the number of pages witht the term.)
double calcInverseDocFrequency(int totalIndexed, int indexedWithTerm)
{
    //Using the formula provided within the Project Description to compute the Inverse Document frequency.
    double logTerm = (double)(((double)(1 + totalIndexed)) / ((double)(1 + indexedWithTerm)));
    double idfScore = log(logTerm);

    return idfScore;
}

//Method to Calculate the tf-idf score
void calcTfidfScore(struct listNode *documentList, double idfScore)
{
    // Going through all the Webpages
    while (documentList != NULL)
    {
        //Adding the tf-idf score to the previous thereby giving a cumulative score for a seach query.
        documentList->tfidfScore += documentList->tfScore * idfScore;
        documentList = documentList->next;
    }

    return;
}
