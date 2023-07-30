#include <stdio.h>
#include <stdlib.h>

int HASH_SIZE = 19;

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Node for the linked list
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
	struct Node* head;
};

// Compute the hash function
int hash(int x)
{
    int prime1 = 31;
    int prime2 = 47;
    int prime3 = 71;
    int h = (prime1 * x * x + prime2 * x + prime3) % HASH_SIZE;
    
    if (h < 0)
        h += HASH_SIZE;

    return h;
}



// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].head != NULL)
        {
            printf("Index %d -> ", i);
            struct Node* current = pHashArray[i].head;
            while (current != NULL)
            {
                printf("%d, %c, %d", current->data.id, current->data.name, current->data.order);
                if (current->next != NULL)
                {
                    printf(" -> ");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
	// Your hash implementation

    struct HashType *hashTable = (struct HashType *)malloc(sizeof(struct HashType) * HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashTable[i].head = NULL;
    }

    for (int i = 0; i < recordSz; i++)
    {
        int h = hash(pRecords[i].id);
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->data = pRecords[i];
        newNode->next = NULL;

        if (hashTable[h].head == NULL)
        {
            hashTable[h].head = newNode;
        }
        else
        {
            struct Node *current = hashTable[h].head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    displayRecordsInHash(hashTable, HASH_SIZE);
    for (int i = 0; i < HASH_SIZE; i++)
    {
        struct Node* current = hashTable[i].head;
        while (current != NULL)
        {
            struct Node* next = current->next;
            free(current);
            current = next;
        }
    }

    free(hashTable);
    free(pRecords);

    return 0;
}