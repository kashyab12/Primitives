#include "indexPage.h"

/*
indexPage Function: 
0) Prints the Webpage URL
1) Gets all the Text From a Webpage
2) Formats the Text.
3) Forms Words and Sends the Node for Addition to the Trie.
*/
struct trieNode *indexPage(const char *url)
{
	long sumOfWordCount = 0;

	//Declaring the variables to be used within the function and to be sent as an Argument.
	char textBuffer[MAXIMUM_INDEXED_CHARACTERS], tempString[MAXIMUM_INDEXED_CHARACTERS]; // Hold the WEBPAGE Text and Words Respectively.
	long inputBufferSize;																 //Size of the Text Buffer Inputted by the WEBPAGE
	int wordStatus;																		 //State Variable responsible for the formation of Words.

	printf("%s\n", url); //Printing the the WEBPAGE URL

	// Creating the root of the Trie and Allocated Space for it Within the Heap.
	struct trieNode *trieRoot = malloc(sizeof(struct trieNode));

	// Checking whether space for the Trie Root was properly allocated within the Heap.
	if (trieRoot == NULL)
	{
		fprintf(stderr, "Error Allocation Space In Heap for Trie Root Node\n"); //Printing an Error Message if the Space was not Allocated Properly.
		return NULL;
	}

	//Initializing the Trie Root with a Word Count of 0 and making all of its children point to NULL.
	trieRoot->wordCount = 0;
	for (int initTrieNode = 0; initTrieNode < TOTAL_ALPHABETS; initTrieNode++)
	{
		trieRoot->children[initTrieNode] = NULL; //Making Child point to NULL
	}

	//Calling getText to get all the text from the WebPage with specified URL.
	inputBufferSize = getText(url, textBuffer, MAXIMUM_INDEXED_CHARACTERS); //All the text is stored within textBuffer

	if (inputBufferSize < 1)
	{
		fprintf(stderr, "The Webpage is Empty, Unable to Parse Characters\n");
		int destroyTrieCheck = freeTrieMemory(trieRoot); //Calling freeTrieMemory to Traverse through each Node and Free the Allocated Heap Space.
		if (destroyTrieCheck)
		{																  //Checking if an error occured while Destroying the Trie Nodes.
			fprintf(stderr, "Error Occured While Destroying the Trie\n"); // Writing the Error Message to the stderr file.
			return NULL;
		}
		return NULL;
	}

	//Making all the Non-Alphabetic Characters a Space and Converting all the Alphabetic Characters to lower-case.
	for (int formatText = 0; formatText < inputBufferSize; formatText++)
	{
		if (!(isalpha(textBuffer[formatText])) && textBuffer[formatText] != '\0')
		{ // Converting a non-alphabetic to a space.
			textBuffer[formatText] = ' ';
		}
		else
		{
			textBuffer[formatText] = tolower(textBuffer[formatText]); // Coverting to lower-case
		}
	}

	//Setting the STATE Variable's Value to 0 i.e False.
	wordStatus = 0;

	//Loop to form words from the TextBuffer and Send it for Addition to the Trie.
	for (int bufferCounter = 0, stringCounter = 0; bufferCounter < inputBufferSize; bufferCounter++)
	{
		//Checking if the textbuffer character is an Alphabet
		if (isalpha(textBuffer[bufferCounter]))
		{
			//Saving the Character to the Temporary String
			tempString[stringCounter] = textBuffer[bufferCounter];
			stringCounter++; // Incrementing the size of the Added Word.
			wordStatus = 1;	 //A word is being formed therefore Setting to 1 i.e. TRUE

			//Special Case: Checking if the word added was the Last Character of the Text Buffer.
			if (bufferCounter == (inputBufferSize - 1))
			{

				tempString[stringCounter] = '\0'; //Forming the word as a String
				printf("\t%s\n", tempString);	  //Printing the word in the required format

				//Sending the word for Trie Addition
				int state = addWordOccurrence(tempString, stringCounter, trieRoot);
				if (state != 0)
				{ // Checking if an error occured during Trie Addition.
					fprintf(stderr, "An Error Occurred in the Addition of a Node");
					int destroyTrieCheck = freeTrieMemory(trieRoot); //Calling freeTrieMemory to Traverse through each Node and Free the Allocated Heap Space.
					if (destroyTrieCheck)
					{																  //Checking if an error occured while Destroying the Trie Nodes.
						fprintf(stderr, "Error Occured While Destroying the Trie\n"); // Writing the Error Message to the stderr file.
						return NULL;
					}
					return NULL;
				}

				//Setting the String Size and Word Formation State Variable to 0 for a new word to be formed.
				stringCounter = 0;
				wordStatus = 0;
				sumOfWordCount += 1;
			}
		}
		else
		{ //If a Space is Encountered

			//Checking if a Word Formation was taking place
			if (wordStatus == 1)
			{
				tempString[stringCounter] = '\0'; //Forming the word as a string

				printf("\t%s\n", tempString); //Printing the word in the required format

				//Sending the word for Trie Addition
				int state = addWordOccurrence(tempString, stringCounter, trieRoot);
				if (state != 0)
				{ // Checking if an error occured during Trie Addition.
					fprintf(stderr, "An Error Occurred in the Addition of a Node");
					int destroyTrieCheck = freeTrieMemory(trieRoot); //Calling freeTrieMemory to Traverse through each Node and Free the Allocated Heap Space.
					if (destroyTrieCheck)
					{																  //Checking if an error occured while Destroying the Trie Nodes.
						fprintf(stderr, "Error Occured While Destroying the Trie\n"); // Writing the Error Message to the stderr file.
						return NULL;
					}
					return NULL;
				}

				//Setting the String Size and Word Formation State Variable to 0 for a new word to be formed.
				stringCounter = 0;
				wordStatus = 0;
				sumOfWordCount += 1;
			}
		}
	}

	trieRoot->totalWords = sumOfWordCount;

	return trieRoot; // Returning the Trie Root Pointer i.e. Address in Heap to main function.
}

/*
addWordOccurence function:
1) Responsible For Adding a given word to the Trie
2) Performs Addition in a Recursive Manner
*/
int addWordOccurrence(const char *word, const int wordLength, struct trieNode *trieRoot)
{

	if (wordLength == 0)
	{			  // Base Case for which Recursion Stops.
		return 1; //Incrementing the last character of the word by 1.
	}
	else
	{ // Recursive Case

		//Finding the character's mappedIndex i.e index within a Trie Node's Array of Children
		int mappedIndex = word[0] - FIRST_LOWERCASE_ALPHABET_ASCII_VALUE;

		//Checking if the respective Child Exists
		if (trieRoot->children[mappedIndex] == NULL)
		{ //Does not Exist
			//Allocating space for the Child within the Heap.
			trieRoot->children[mappedIndex] = malloc(sizeof(struct trieNode));

			if (trieRoot->children[mappedIndex] == NULL)
			{
				fprintf(stderr, "Error Allocating Memory in Heap for Trie Node\n");
				return -2;
			}

			//Initializing the child with a Word Count of 0 and making all of its children point to NULL.
			trieRoot->children[mappedIndex]->wordCount = 0;
			for (int initTrieNode = 0; initTrieNode < TOTAL_ALPHABETS; initTrieNode++)
			{
				trieRoot->children[mappedIndex]->children[initTrieNode] = NULL;
			}
		}

		//Incremeting the word count of the child via a recursive call to add the next character of the word.
		trieRoot->children[mappedIndex]->wordCount += addWordOccurrence((word + 1), (wordLength - 1), trieRoot->children[mappedIndex]);
	}

	return 0; // Incrementing all the characters of the word by 0 except for the last character.
}

/*
freeTrieMemory function:
1) Traverses through to the leaves of the Trie and free's memory allocated for the Nodes.
2) Traverses through every node in a recursive manner.
*/

int freeTrieMemory(struct trieNode *trieRoot)
{
	if (trieRoot == NULL)
	{ // Checking for errors i.e. if the Trie Root is Null.
		return 1;
	}
	else
	{
		//Traversing through the children of every Trie Node.
		for (int iterateChildren = 0; iterateChildren < TOTAL_ALPHABETS; iterateChildren++)
		{
			if (trieRoot->children[iterateChildren] != NULL)
			{														 // Checking the child exists
				freeTrieMemory(trieRoot->children[iterateChildren]); // Recusively traversing to the Child's children so as to finally reach the leaf node.
			}
		}

		free(trieRoot); //Using free to remove the respective Trie Root from the allocated space within the Heap.
		return 0;		// Successful Case.
	}
}

int findWordOccurence(struct trieNode *trieRoot, const char *word)
{
	if (strlen(word) == 0)
	{
		return trieRoot->wordCount;
	}
	else
	{	
		int mappedIndex = (int)word[0] - FIRST_LOWERCASE_ALPHABET_ASCII_VALUE;
		if (trieRoot->children[mappedIndex] != NULL)
		{
			int termFrequency = findWordOccurence(trieRoot->children[mappedIndex], word + 1);
			return termFrequency;
		}
		else
		{
			return 0;
		}
	}
}

/* You should not need to modify this function */
int getText(const char *srcAddr, char *buffer, const int bufSize)
{
	FILE *pipe;
	int bytesRead;

	snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

	pipe = popen(buffer, "r");
	if (pipe == NULL)
	{
		fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
				buffer);
		return 0;
	}

	bytesRead = fread(buffer, sizeof(char), bufSize - 1, pipe);
	buffer[bytesRead] = '\0';

	pclose(pipe);

	return bytesRead;
}
