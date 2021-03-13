// Header Guard
#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H

// Including the Relevant Libraries
#include <math.h>
#include "crawler.h"
#include "indexPage.h"

// Defining constant values relevant to searching queries
#define MAX_QUERY_LEN 50 
#define MAX_TOP_URLS 3
#define TRUE 1

//Function Declerations

double calcTermFrequency(char *term, struct trieNode *indexedPages);
double calcInverseDocFrequency(int totalIndexed, int indexedWithTerm);
void calcTfidfScore(struct listNode *listHead, double idfScore);

#endif
// End of Header Guard