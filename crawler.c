#include "crawler.h"

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr)
{
  // TODO: complete this

  //Checking if the current pNode is null so as to exit after traversing the list.
  if (pNode == NULL)
  {
    return 0; //If no node has been found to contain the address, we return 0.
  }
  else
  {
    //Checking if pNode contains the given address parameter using strncmp.
    if (strncmp(pNode->addr, addr, MAX_ADDR_LENGTH) == 0)
    { //If the contained address and given address are equal.
      return 1;
    }
    else
    {
      return contains(pNode->next, addr); // Traverse through linked list using recursion.
    }
  }
}

/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr)
{
  // TODO: complete this

  //Checking if we have reached the tail of the linked list.
  if (pNode->next == NULL)
  {

    //Creating a newNode and allocating space for it in the heap.
    struct listNode *newNode = malloc(sizeof(struct listNode));

    if (newNode == NULL)
    { // In case there was no free space in the heap.
      printf("ERROR : Memory Could Not Be Allocated for the New Node.\n");
      return;
    }

    //Copying the given address to the address member of the newNode using strncpy function.
    strncpy(newNode->addr, addr, MAX_ADDR_LENGTH);

    //Setting the newNode's next pointer to NULL as it is being added to the end of the linked list.
    newNode->next = NULL;
    newNode->tfidfScore = 0.0;
    newNode->tfScore = 0.0;
    //Connecting the last node of the linked list to the newNode.
    pNode->next = newNode;

    return;
  }
  else
  {
    insertBack(pNode->next, addr); //Traversing until the last node of the linked list is reached.
  }
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode)
{
  // TODO: complete this

  //Checking if the end of the linked list has been reached.
  if (pNode == NULL)
  {
    return;
  }

  //Traversing to the end of the linked list first so as to begin deleting from the end.

  destroyList(pNode->next); // Recursive Call.
  free(pNode);              // To free chunks of heap memory.
}

int getLink(const char *srcAddr, char *link, const int maxLinkLength)
{
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if (pipe == NULL)
  {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if (numLinks > 0)
  {
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for (linkNum = 0; linkNum < numLinks; linkNum++)
    {
      fgets(buffer, bufSize, pipe);

      if (r < (linkNum + 1.0) / numLinks)
      {
        break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength - 1] = '\0';

    /* get rid of the newline */
    {
      char *pNewline = strchr(link, '\n');
      if (pNewline != NULL)
      {
        *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if (numLinks > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
