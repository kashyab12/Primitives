/*
    Name : Kashyab Ambarani
    Class : Intro. to C & Unix
    Instructor : Dr. Zesheng Chen
    Project 3
*/

// Including the respective .h file.
#include "webSearch.h"

int main(int argc, char *argv[])
{
    int parsedCharacters;                               // The Number of Characters parsed by Input Function sscanf
    int maximumHops, indexedPages = 0, currentHops = 0; // Declaring Max. Hops and the indexed Pages as defined within the Algorithm.
    FILE *urlFile;                                      // URL File where the required URLs are stored with the hop values
    char *parseLine;                                    // Stores a single read line from the URL File.
    char startURL[MAX_ADDR_LENGTH];                     // Stores the URL from the parsed line.
    char indexedURL[MAX_ADDR_LENGTH];                   // Stores the indexedURL modfified via getLink function.

    struct listNode *crawlerListHead; // Declaring a Linked List Head Node.
    struct trieNode **indexedTries;   // An Array of TrieNode Pointers.
    int MAX_NO_PAGES;                 // The Max Number of Pages to Index

    int queryState = 0;                    // Storing the current query state (Error Checking)
    char *queryInput;                      // Stores the query to be searched for.
    char *topUrl[MAX_TOP_URLS];            // Stores the top ranked URLs for a given Query.
    long seedValue;                        // Given as per the Command Line Argument values.
    int bytesParsed = 0;                   // Represents the number of bytes / characters parsed.
    char tempString[500];                  // Used to Store Segmented Words within the Query
    char *queryWord;                       // Temporary Variable pointing to the start of the segmented string.
    double scoreOne, scoreTwo, scoreThree; // tf-idf scores of the Top 3 Webpages.

    // Checking if the right number of command line arguments were provided.
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Please enter the correct number of arguments.\n");
        return -1;
    }

    // Converting the string to long and setting seed value.
    seedValue = atol(argv[3]);
    srand(seedValue);

    urlFile = fopen(argv[1], "r");

    if (urlFile == NULL)
    {
        fprintf(stderr, "The url file %s could not be opened\n", argv[1]);
        return -1;
    }

    // Converting string to integer to retrieve the Maximum number of indexed Pages.
    MAX_NO_PAGES = atoi(argv[2]);

    // Throwing an error for invalid Max Page Value.
    if (MAX_NO_PAGES < 1)
    {
        fprintf(stderr, "The number of url's mentioned is less than 1\n");
        fclose(urlFile);
        return -1;
    }

    // Allocating space for the parsed line within the heap.
    parseLine = malloc(sizeof(char) * MAX_LINE_LEN);

    // Checking for Heap Allocation Errors
    if (parseLine == NULL)
    {
        fprintf(stderr, "Error allocating space in the heap for URL File String\n");
        fclose(urlFile);
        return -1;
    }

    // Allocating the space for the Linked List Head and initiating its attributes.
    crawlerListHead = malloc(sizeof(struct listNode));
    crawlerListHead->next = NULL;
    crawlerListHead->tfScore = 0;
    crawlerListHead->tfidfScore = 0;

    // Copying the Word Begin to the head node.
    strncpy(crawlerListHead->addr, "Begin", MAX_ADDR_LENGTH);

    // Checking for Heap Allocation Error
    if (crawlerListHead == NULL)
    {
        fprintf(stderr, "Error allocating space in the heap for the Crawler List\n");
        free(parseLine);
        fclose(urlFile);
        return -1;
    }

    // Allocating space for the Max No. of Trie Node roots as specified by the Command Line Arguments.
    indexedTries = malloc(sizeof(struct trieNode *) * MAX_NO_PAGES);

    // Checking for Heap Allocation Errors.
    if (indexedTries == NULL)
    {
        fprintf(stderr, "Error allocating space in the heap for the Trie Root Pointer\n");
        destroyList(crawlerListHead);
        free(parseLine);
        fclose(urlFile);
        return -1;
    }

    // ----------------------------- Part 1 - Crawling and Indexing Multiple Pages -----------------------------

    printf("Indexing...\n");

    // Starting to read the URL File line by line as well as checking if the indexed pages is not maximum
    while (fgets(parseLine, MAX_LINE_LEN, urlFile) != 0 && indexedPages < MAX_NO_PAGES)
    {
        // Scanning the String Buffer for a String and an integer.
        parsedCharacters = sscanf(parseLine, "%s %d", startURL, &maximumHops);

        // Checking if the correct number of inputs have been parsed.
        if (parsedCharacters != 2)
        {
            // Freeing the heap allocated memory

            fprintf(stderr, "The file given is not formatted correctly.\n");
            fclose(urlFile);

            for (int ctr = 0; ctr < indexedPages; ctr++)
            {
                freeTrieMemory(indexedTries[ctr]);
            }

            free(indexedTries);
            destroyList(crawlerListHead);
            free(parseLine);

            return -1;
        }

        // Counter for the current hopped.
        currentHops = 0;

        // Crawl through the line's URL and Index it.
        while (TRUE)
        {
            // Making sure the url has not already been crawled
            if (contains(crawlerListHead, startURL) == 0)
            {
                //Add it to Crawl and Index it
                insertBack(crawlerListHead, startURL);
                indexedTries[indexedPages] = indexPage(startURL); // NULL if page is empty
                indexedPages += 1;                                // Incrementing the number of indexed Pages.
            }

            // Incrementing the number of hopped urls
            currentHops += 1;

            //Making sure the maximum hops hasnt been reached and the max number of pages havent been indexed.
            if (currentHops <= maximumHops && indexedPages < MAX_NO_PAGES)
            {
                //Crawling Process
                int validLink = getLink(startURL, indexedURL, MAX_ADDR_LENGTH);

                if (!validLink)
                {
                    break;
                }
                else
                {
                    // Copying the indexed URL to the startUrl and iterating the loop.
                    strncpy(startURL, indexedURL, MAX_ADDR_LENGTH);
                    startURL[MAX_ADDR_LENGTH - 1] = '\0';
                }
            }
            else
            {
                break;
            }
        }
    }

    // ----------------------------- Part 2 - Handling Queries -----------------------------

    // Allocating memory for the queryInput c-string of size MAX_QUERY_LEN
    queryInput = malloc(sizeof(char) * MAX_QUERY_LEN);

    // Allocating space for the top url strings
    for (int ctr = 0; ctr < MAX_TOP_URLS; ctr += 1)
    {
        topUrl[ctr] = malloc(sizeof(char) * MAX_ADDR_LENGTH);
    }

    // Giving initial values to the top urls so as to perform initial comparision.
    strncpy(topUrl[0], "httpsv", MAX_ADDR_LENGTH);
    strncpy(topUrl[1], "http", MAX_ADDR_LENGTH);
    strncpy(topUrl[2], "https", MAX_ADDR_LENGTH);

    // Checking for Heap Memory Allocation Error
    if (queryInput == NULL)
    {
        fprintf(stderr, "Error allocating space in the heap for query input\n");
        fclose(urlFile);

        for (int ctr = 0; ctr < indexedPages; ctr++)
        {
            freeTrieMemory(indexedTries[ctr]);
        }

        for (int ctr = 0; ctr < MAX_TOP_URLS; ctr += 1)
        {
            free(topUrl[ctr]);
        }

        destroyList(crawlerListHead);
        free(indexedTries);
        free(parseLine);
        return -1;
    }

    while (TRUE)
    {
        // Asking for User Input
        printf("\nEnter a web query: ");

        // Getting a line of the user's input via stdin i.e the Terminal
        if (fgets(queryInput, MAX_QUERY_LEN, stdin) != NULL)
        {
            //Checking for Errors in Input
            if (strlen(queryInput) == 1 && queryInput[0] == '\n')
            {
                queryState = -2;
            }
            else
            {
                //Replacing newline characters with null teriminating (to-avoid comparision issues within the Trie)
                if (queryInput[strlen(queryInput) - 1] == '\n')
                {
                    queryInput[strlen(queryInput) - 1] = '\0';
                }

                for (int i = 0; i < strlen(queryInput); i++)
                {
                    //Checking if the correctly formatted String has been entered.

                    if (!islower(queryInput[i]) && !isspace(queryInput[i]))
                    {
                        queryState = -1;
                        break;
                    }
                    else if (i == strlen(queryInput) - 1 && isspace(queryInput[i]))
                    {
                        queryState = -1;
                        break;
                    }
                    else if (isspace(queryInput[i]) && !isalpha(queryInput[i - 1]) && !isalpha(queryInput[i + 1]))
                    {
                        queryState = -1;
                        break;
                    }
                }
            }

            /*
            Query State of -1 represents an incorrectly formatted String
            Query State of -2 represents no character being entered
            */

            if (queryState == -1)
            {
                printf("Please enter a query containing only lower-case letters.\n");
                queryState = 0;
                continue;
            }
            else if (queryState == -2)
            {
                printf("Exiting the program\n");
                queryState = 0;
                break;
            }
            else
            {
                // Outputting the Segemented Strings

                printf("Query is \"%s\".\nIDF scores are\n", queryInput);

                // Instantiating the bytes parsed and query word.
                bytesParsed = 0;
                queryWord = queryInput;

                // Calculation of the scores for each segmented string.

                while (sscanf(queryWord, "%s%n", tempString, &bytesParsed) == 1)
                {
                    int loopCtr = 0;
                    double idfScore = 0;
                    int indexedWithTerm = 0;

                    // Iterating through the Linked List to Calculate Term Frequencies
                    for (struct listNode *iterNode = crawlerListHead->next; iterNode != NULL; iterNode = iterNode->next, loopCtr += 1)
                    {
                        if (indexedTries[loopCtr] != NULL) // If the URL is not empty i.e. it contains words
                        {
                            iterNode->tfScore = calcTermFrequency(tempString, indexedTries[loopCtr]);

                            // Checking if the Term Frequency is not 0 i.e. the page contains the term ATLEAST ONCE
                            if (iterNode->tfScore != 0)
                            {
                                indexedWithTerm += 1;
                            }
                        }
                        else
                        {
                            iterNode->tfScore = 0;
                        }
                    }

                    // Calculating the Inverse Document Frequency of the Document / Page
                    idfScore = calcInverseDocFrequency(indexedPages, indexedWithTerm);

                    // Printing the Inverse Document Frequency of the Web Page
                    printf("IDF(%s): %.7f\n", tempString, idfScore);

                    // Calculating the tf-idf score for all the Pages.
                    calcTfidfScore(crawlerListHead->next, idfScore);

                    // Moving the queryWord ptr by the amount of bytes already processed.
                    queryWord += bytesParsed;
                }

                // Outputting the Top Webpages

                printf("Web pages:\n");

                // Setting the Top Scores to 0
                scoreOne = scoreTwo = scoreThree = 0;

                // Looping throught the Linked List Nodes to Compare and Retreieve the 3 Largest Scores.

                for (struct listNode *loopNode = crawlerListHead->next; loopNode != NULL; loopNode = loopNode->next)
                {
                    if (loopNode->tfidfScore > scoreThree)
                    {
                        if (loopNode->tfidfScore > scoreTwo)
                        {
                            if (loopNode->tfidfScore > scoreOne)
                            {
                                scoreThree = scoreTwo;
                                strncpy(topUrl[2], topUrl[1], MAX_ADDR_LENGTH);

                                scoreTwo = scoreOne;
                                strncpy(topUrl[1], topUrl[0], MAX_ADDR_LENGTH);

                                scoreOne = loopNode->tfidfScore;
                                strncpy(topUrl[0], loopNode->addr, MAX_ADDR_LENGTH);
                            }
                            else
                            {
                                scoreThree = scoreTwo;
                                strncpy(topUrl[2], topUrl[1], MAX_ADDR_LENGTH);

                                scoreTwo = loopNode->tfidfScore;
                                strncpy(topUrl[1], loopNode->addr, MAX_ADDR_LENGTH);
                            }
                        }
                        else
                        {
                            scoreThree = loopNode->tfidfScore;
                            strncpy(topUrl[2], loopNode->addr, MAX_ADDR_LENGTH);
                        }
                    }
                }

                //Checking if less than 3 scores are present with a non-zero value

                // All Top Scores are 0 print nothing
                if (scoreOne == 0)
                {
                }
                else if (scoreTwo == 0) // If score two and three are zero
                {
                    printf("1. %s (score: %.4f)\n", topUrl[0], scoreOne);
                }
                else if (scoreThree == 0) // If score three is zero
                {
                    printf("1. %s (score: %.4f)\n", topUrl[0], scoreOne);
                    printf("2. %s (score: %.4f)\n", topUrl[1], scoreTwo);
                }
                else // If all are non zero values.
                {
                    printf("1. %s (score: %.4f)\n", topUrl[0], scoreOne);
                    printf("2. %s (score: %.4f)\n", topUrl[1], scoreTwo);
                    printf("3. %s (score: %.4f)\n", topUrl[2], scoreThree);
                }

                // Making all the tf and tf-idf scores zero for the Pages to start the next search query
                for (struct listNode *iterNode = crawlerListHead->next; iterNode != NULL; iterNode = iterNode->next)
                {
                    iterNode->tfidfScore = 0;
                    iterNode->tfScore = 0;
                }
            }
        }
        else
        {
            //Exitting the program if the File no longer has any lines.
            printf("Exiting the program\n");
            break;
        }
    }

    // Closing the URL file
    fclose(urlFile);

    // Deallocatting all the space allocated within the Heap via free calls

    free(parseLine);
    destroyList(crawlerListHead);
    free(queryInput);

    for (int ctr = 0; ctr < indexedPages; ctr++)
    {
        freeTrieMemory(indexedTries[ctr]);
    }

    for (int ctr = 0; ctr < 3; ctr += 1)
    {
        free(topUrl[ctr]);
    }

    free(indexedTries);

    return 0;
}
