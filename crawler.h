// Header Guards
#ifndef CRAWLER_H_
#define CRAWLER_H_

//Including the Relevant Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

//Defining a constant for the Maximum Address Length
#define MAX_ADDR_LENGTH 1000

// Struvture Definition for a Linked List Node.
struct listNode
{
  char addr[MAX_ADDR_LENGTH];
  struct listNode *next;
  double tfScore, tfidfScore; // Storing the Term Frequency and the tf-idf score for each URL
};

// Function Declerations.

int contains(const struct listNode *pNode, const char *addr);
void insertBack(struct listNode *pNode, const char *addr);
void destroyList(struct listNode *pNode);
int getLink(const char *srcAddr, char *link, const int maxLinkLength);

#endif
// Ending Header Guards