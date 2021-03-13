// Header Guards
#ifndef INDEXPAGE_H_
#define INDEXPAGE_H_

// Including the Relevant Libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_ALPHABETS 26                      // The total number of Alphabets within the English Language
#define FIRST_LOWERCASE_ALPHABET_ASCII_VALUE 97 // ASCII Value of 'a'
#define MAXIMUM_INDEXED_CHARACTERS 300000       // Maximum Characters Parsed from the Webpage

// Trie Node Data Structure
struct trieNode
{
  int wordCount;                              //The number of times the word occurs
  struct trieNode *children[TOTAL_ALPHABETS]; //The children of the Trie Node i.e. 26 possible Alphabets.
  long totalWords;                            //The total number of words indexed by the Trie
 };

//Function Declerations

struct trieNode *indexPage(const char *url);

int addWordOccurrence(const char *word, const int wordLength, struct trieNode *);

int freeTrieMemory(struct trieNode *trieRoot);

int findWordOccurence(struct trieNode *trieRoot, const char *word);

int getText(const char *srcAddr, char *buffer, const int bufSize);

#endif