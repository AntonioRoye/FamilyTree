#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 20

typedef struct person Person;
typedef Person *personPtr;

struct person
{
    int id;
    char name[SIZE];
    int age;
    char status;
    personPtr parents[2];
    personPtr partner;
    personPtr children[5];
    personPtr prev;
    personPtr next;
};

//Empties input buffer
void emptyStdin(void);

//Displays instructions
void instructions(void);

//Add a new person
void newPerson(personPtr *currentPerson, int id, char *name, int age, char status);

//Set a person as a parent of another person
void setParent(personPtr *startingPtr, char *parentName, char *childName);

//Set a person as a partner of another person
void setPartner(personPtr *startingPtr, char *partnerName, char *personName);

//Changes profile of a person
void changeProfile(personPtr *startingPtr, char *name, int age, char status);

//Deletes person
void deletePerson(personPtr *startingPtr, char *name);

//Upadates relationships after deleting a person
void updateRelations(personPtr *startingPtr, char *name);

//Checks if family tree is empty
int isEmpty(personPtr startingPerson);

//Print relatives
void printRelatives(personPtr *startingPtr, char *name);

//Saves all the people from the tree into a file
void saveTree(personPtr *startingPtr, char *fileName);

//Loads tree from a file
void loadTree(personPtr *startingPerson, char *fileName);

//Print Parents
void printParents(personPtr currentPtr);

//Prints Children
void printChildren(personPtr currentPtr);

//Prints names of everyone added
void printPeople(personPtr currentPtr);

//Centering values in table
void centrePrint(char *w, int width);

int main()
{

    personPtr startingPerson = NULL;

    //Declare variable for id, age, name, status, choice
    int id = 1;
    int age;
    char personName[SIZE];
    char status;
    unsigned int choice;

    //Declare variable parent name -- Used in setParent()
    char parentName[SIZE];

    //Variable for partner name -- used in setPartner()
    char partnerName[SIZE];

    //Varibale to store the filename;
    char fileName[SIZE];

    do
    {
        //print instructions
        instructions();

        //Get choice
        printf("%s", ">");
        scanf("%d", &choice);
        puts(" ");
        emptyStdin();

        switch (choice)
        {
        case 1:
            //Get name
            printf("%s", "Enter the person's name: ");
            scanf("%s19", personName);

            //Get age
            printf("%s", "Enter the person's age: ");
            scanf("%3d", &age);
            emptyStdin();

            //Get status
            printf("%s", "Enter the person's life status ('A' for Alive or 'D' for deceased): ");
            scanf("%1c", &status);
            emptyStdin();

            //Set new person
            newPerson(&startingPerson, id, personName, age, status);

            //Increment id so every person added gets a unique id
            id++;

            break;
        case 2:
            //Get child name
            printf("%s", "Enter the child's name: ");
            scanf("%s19", personName);

            //Get parent name
            printf("%s", "Enter the parent's name: ");
            scanf("%s19", parentName);

            //Set parent
            setParent(&startingPerson, parentName, personName);

            break;
        case 3:
            //Get person's name
            printf("%s", "Enter the person's name: ");
            scanf("%s19", personName);

            //Get partners name
            printf("%s", "Enter the partner's name: ");
            scanf("%s19", partnerName);

            //Set parent
            setPartner(&startingPerson, partnerName, personName);
            break;
        case 4:
            //Get name
            printf("%s", "Enter the name of the person whose relatives you want to print: ");
            scanf("%s19", personName);

            printRelatives(&startingPerson, personName);

            break;
        case 5:
            //Get name
            printf("%s", "Enter the person's name: ");
            scanf("%s19", personName);

            //Get age
            printf("%s", "Enter the person's age: ");
            scanf("%3d", &age);
            emptyStdin();

            //Get status
            printf("%s", "Enter the person's life status ('A' for Alive or 'D' for deceased): ");
            scanf("%1c", &status);
            emptyStdin();

            //Set new person
            changeProfile(&startingPerson, personName, age, status);
            break;
        case 6:
            //Get name
            printf("%s", "Enter the name of the person you want to remove: ");
            scanf("%s19", personName);

            deletePerson(&startingPerson, personName);
            updateRelations(&startingPerson, personName);

            break;
        case 7:
            printPeople(startingPerson);
            break;
        case 8:
            //Get file name
            printf("%s", "Enter the name of the file: ");
            scanf("%s", fileName);

            saveTree(&startingPerson, fileName);
            break;
        case 9:
            printf("%s", "Enter the name of the file: ");
            scanf("%s", fileName);

            loadTree(&startingPerson, fileName);
            break;
        default:
            break;
        }

    } while (choice != 0);
}

void emptyStdin(void)
{
    //Use up values in stdin by storing them in varibale c
    for (int c = getchar(); c != '\n' && c != EOF; c = getchar())
    {
        puts("\n");
    }
}

void instructions(void)
{
    puts("\nPlease select from the options below:\n"
         "(0) Exit\n"
         "(1) Add a new person\n"
         "(2) Set a person as a parent of another person\n"
         "(3) Set a person as a partner of another person\n"
         "(4) Print all the relatives of a person\n"
         "(5) Change the profile of a person\n"
         "(6) Remove a person\n"
         "(7) Print all the persons\n"
         "(8) Save the tree into a file\n"
         "(9) Load the tree from a file\n");
}

void newPerson(personPtr *startingPerson, int id, char *name, int age, char status)
{
    personPtr newPerson = malloc(sizeof(Person));

    if (newPerson != NULL)
    {
        //Place values in node
        strcpy(newPerson->name, name);
        newPerson->age = age;
        newPerson->id = id;
        newPerson->status = status;
        newPerson->partner = NULL;

        //Set pointers to NULL to prevent undefined behaviour
        for (size_t i = 0; i < 2; i++)
        {
            newPerson->parents[i] = NULL;
        }

        //Set pointers to NULL to prevent undefined behaviour
        for (size_t i = 0; i < 5; i++)
        {
            newPerson->children[i] = NULL;
        }

        //Temporary pointers to iterate with
        personPtr previousPtr = NULL;
        personPtr currentPtr = *startingPerson;

        //Iterate to last pointer
        while (currentPtr != NULL)
        {
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        //If it is the first item in the list
        if (previousPtr == NULL)
        {
            newPerson->next = *startingPerson;
            *startingPerson = newPerson;
        }
        else
        {
            //Add item at the end of the list
            previousPtr->next = newPerson;
            newPerson->prev = previousPtr;
            newPerson->next = currentPtr;
        }
    }
    else
    {
        printf("%s was not inserted. There is no memory available. \n", name);
    }
}

void setParent(personPtr *startingPtr, char *parentName, char *childName)
{
    //Temp pointers
    personPtr childPtr = NULL;
    personPtr parentPtr = NULL;
    personPtr prevPtr = NULL;
    personPtr currentPtr = *startingPtr;

    //Find child and parent
    while (currentPtr != NULL)
    {
        //If match assign to temp pointer
        if (strcmp(currentPtr->name, childName) == 0)
        {
            childPtr = currentPtr;
            puts("\nChild was found");
        }

        //If match assign to temp pointer
        if (strcmp(currentPtr->name, parentName) == 0)
        {
            parentPtr = currentPtr;
            puts("\nParent was found");
        }

        //Iterate though list
        prevPtr = currentPtr;
        currentPtr = currentPtr->next;
    }

    if (childPtr == NULL)
    {
        puts("\nChild was not found");
        return;
    }

    if (parentPtr == NULL)
    {
        puts("\nParent was not found");
        return;
    }

    //Check to see it the child already has the parent
    size_t i;
    for (i = 0; i < 2; i++)
    {
        if (childPtr->parents[i] != NULL && strcmp(childPtr->parents[i]->name, parentName) == 0)
        {
            puts("\nThe child already has this parent");
            break;
        }
        else
        {
            //Check to see if the child has less than 2 parents
            if (childPtr->parents[i] == NULL)
            {
                size_t j;
                for (j = 0; j < 5; j++)
                {   
                    //Check to see if the parent already has the child
                    if (parentPtr->children[i] != NULL && strcmp(parentPtr->children[i]->name, childName) == 0)
                    {
                        puts("\nThe parent already has this child");
                        break;
                    }
                    else
                    {   
                        //Check to see if the parent has less than 5 children
                        if (parentPtr->children[j] == NULL)
                        {
                            //Assign parent to child and child to parent if they both have an empty spot and don't previously ahve each other
                            parentPtr->children[j] = childPtr;
                            childPtr->parents[i] = parentPtr;
                            puts("\nThe relationship was set succesfully");
                            break;
                        }
                    }
                }
                //If parent has 5 children
                if (j == 5)
                {
                    puts("\nThe parent already has 5 children");
                }

                break;
            }
        }
    }
    //If child has 2 parents
    if (i == 2)
    {
        puts("\nThe child already has 2 parents");
    }
}
void setPartner(personPtr *startingPtr, char *partnerName, char *personName)
{
    //Temp pointers
    personPtr perPtr = NULL;
    personPtr partnerPtr = NULL;
    personPtr prevPtr = NULL;
    personPtr currentPtr = *startingPtr;

    //Find person and partner
    while (currentPtr != NULL)
    {
        //Assign current pointer to temp pointert
        if (strcmp(currentPtr->name, personName) == 0)
        {
            perPtr = currentPtr;
            puts("\nPerson was found");
        }

        //Assign current pointer to temp pointer
        if (strcmp(currentPtr->name, partnerName) == 0)
        {
            partnerPtr = currentPtr;
            puts("\nPartner was found");
        }

        //Iterate through list
        prevPtr = currentPtr;
        currentPtr = currentPtr->next;
    }

    //If person is not found
    if (perPtr == NULL)
    {
        puts("\nPerson was not found.");
        return;
    }

    //If partner is not found
    if (partnerPtr == NULL)
    {
        puts("\nPartner was not found.");
        return;
    }

    //If the person has no partner
    if (perPtr->partner == NULL)
    {
        //If the partner has no partner
        if (partnerPtr->partner == NULL)
        {
            //Assign person to partner and partner to person
            perPtr->partner = partnerPtr;
            partnerPtr->partner = perPtr;
            puts("\nThe relationship was set succesfully");
        }
        else
        {
            printf("\n%s already has a partner\n", partnerPtr->name);
        }
    }
    else
    {
        printf("\n%s already has a partner\n", perPtr->name);
    }
}


void changeProfile(personPtr *startingPtr, char *name, int age, char status)
{
    //Temp pointers
    personPtr previousPtr = NULL;
    personPtr currentPtr = *startingPtr;

    //Iterate though list
    while (currentPtr != NULL)
    {
        //If name is a match to the name in the current pointer
        if (strcmp(currentPtr->name, name) == 0)
        {
            //Change the current pointers age and status and break
            currentPtr->age = age;
            currentPtr->status = status;
            puts("\nInformation was changed\n")
            break;
        }

        //Find next pointer
        previousPtr = currentPtr;
        currentPtr = currentPtr->next;
    }

    //If person was not found
    if (currentPtr == NULL)
    {
        puts("\nPerson was not found.");
        return;
    }
}

void deletePerson(personPtr *startingPtr, char *name)
{
    //If name in first node matches
    if (strcmp((*startingPtr)->name, name) == 0)
    {
        //Create a temp pointer
        personPtr tempPtr = *startingPtr;
        *startingPtr = (*startingPtr)->next; //dethread
        free(tempPtr); //free memory
        puts("\nRemoved succesfully\n");
    }
    else //For nodes that are not the first node
    {   
        //Temp pointers
        personPtr previousPtr = *startingPtr;
        personPtr currentPtr = (*startingPtr)->next;

        //Iterate though list
        while (currentPtr != NULL)
        {   
            //If name in current pointer matches name
            if (strcmp(currentPtr->name, name) == 0)
            {
                //Temp pointer
                personPtr tempPtr = currentPtr;
                previousPtr->next = currentPtr->next; //dethread
                free(tempPtr); //free memory and break
                puts("\nRemoved succesfully\n");
                break;
            }

            //Find next pointer
            previousPtr = currentPtr;
            currentPtr = currentPtr->next;
        }

        //If person was not found
        if (currentPtr == NULL)
        {
            puts("\nPerson was not found.");
            return;
        }
    }
}

void updateRelations(personPtr *startingPtr, char *name)
{
    //Temp pointers
    personPtr previousPtr = NULL;
    personPtr currentPtr = *startingPtr;

    //Iterate through list
    while (currentPtr != NULL)
    {
        //Update parents pointers to NULL
        for (size_t i = 0; i < 5; i++)
        {   
            //If parent has a child with the matching name, make the child pointer equal NULL
            if (currentPtr->children[i] != NULL && strcmp(currentPtr->children[i]->name, name) == 0)
            {
                currentPtr->children[i] = NULL;
                printf("\nChild of %s was removed\n", currentPtr->name);
                break;
            }
        }

        //Update childs pointers to NULL
        for (size_t i = 0; i < 2; i++)
        {
            //If child has a parent with the matching name, make the parent pointer equal NULL
            if (currentPtr->parents[i] != NULL && strcmp(currentPtr->parents[i]->name, name) == 0)
            {
                currentPtr->parents[i] = NULL;
                printf("\nParent of %s was removed\n", currentPtr->name);
                break;
            }
        }

        //If person has a partner with the matching name, make the partner pointer equal NULL
        if (currentPtr->partner != NULL && strcmp(currentPtr->partner->name, name) == 0)
        {
            currentPtr->partner = NULL;
            printf("\nPartner of %s was removed\n", currentPtr->name);
        }

        //Find next pointer
        previousPtr = currentPtr;
        currentPtr = currentPtr->next;
    }
}

void printRelatives(personPtr *startingPtr, char *name)
{
    //Temp pointers
    personPtr previousPtr = NULL;
    personPtr currentPtr = *startingPtr;

    //Iterate through list
    while (currentPtr != NULL)
    {   
        //If name matches the name in current pointer
        if (strcmp(currentPtr->name, name) == 0)
        {
            printf("\nThe relatives of %s are:\n", currentPtr->name);

            //print partner
            if (currentPtr->partner != NULL)
            {
                printf("\nPartner: %s\n", currentPtr->partner->name);
            }
            else
            {
                printf("\n%s has no partner\n", currentPtr->name);
            }

            //print children
            size_t i;
            for (i = 0; i < 5; i++)
            {
                //If there are any children
                if (currentPtr->children[i] != NULL)
                {
                    //Print children function prints all children and then breaks
                    printChildren(currentPtr);
                    puts(" ");
                    break;
                }
            }

            //If loop ran and children were not printed, there are no children
            if (i == 5)
            {
                printf("\n%s has no children\n", currentPtr->name);
            }

            //print parents
            size_t j;
            for (j = 0; j < 2; j++)
            {
                //If there are any parents
                if (currentPtr->parents[i] != NULL)
                {
                    //print parent function prints all parents and then breaks
                    printParents(currentPtr);
                    puts(" ");
                    break;
                }
            }

            //If loop ran and parents were not printed, there are no parents
            if (i == 2)
            {
                printf("\n%s has no parents\n\n", currentPtr->name);
            }

            break;
        }
        //Find next pointer
        previousPtr = currentPtr;
        currentPtr = currentPtr->next;
    }

    //If person was not found
    if (currentPtr == NULL)
    {
        puts("\nPerson was not found.");
        return;
    }
}

void printParents(personPtr currentPtr)
{
    printf("%s", "Parents: ");

    //Iterate though parents in current pointer
    for (size_t i = 0; i < 2; i++)
    {   
        //If parent is not NULL print it
        if (currentPtr->parents[i] != NULL)
        {
            printf("\n* %s", currentPtr->parents[i]->name);
        }
    }
}

void printChildren(personPtr currentPtr)
{
    printf("\n%s\n", "Children: ");

    //Iterate though children in current pointer
    for (size_t i = 0; i < 5; i++)
    {
        //If child is not NULL print it
        if (currentPtr->children[i] != NULL)
        {
            printf("* %s\n", currentPtr->children[i]->name);
        }
    }
}

void printPeople(personPtr currentPtr)
{
    //If list is empty
    if (isEmpty(currentPtr))
    {
        puts("\nTree is empty!");
    }
    else
    {
        printf("%s |         %s         | %s | %s\n", "ID", "Name", "Age", "Status");
        printf("%s\n", "---------------------------------------");

        //Iterate though list
        while (currentPtr != NULL)
        {
            //Print id, name, age, status of every person
            printf("%d  | ", currentPtr->id);
            centrePrint(currentPtr->name, 20);
            printf(" | %d  | ", currentPtr->age);

            if (currentPtr->status == 'A' || currentPtr->status == 'a')
            {
                printf("%s\n", "Alive");
            }
            else
            {
                printf("%s\n", "Deceased");
            }

            //Find next pointer
            currentPtr = currentPtr->next;
        }
    }
}

int isEmpty(personPtr startingPerson)
{   
    //If there is no first node returns 0
    return startingPerson == NULL;
}

void centrePrint(char *w, int width)
{
    //Char array initialized to terminating null character
    char word[20] = {'\0'};
    
    //Length variable
    int len;

    //Input string into char array
    sprintf(word, "%s", w);

    //Find length of word in array
    len = strlen(word);

    //If wors is greater than the desired width just print the word
    if (len >= width)
        printf("%s", word);
    else
    {
        //Space remaining
        int remaining = width - len;
        
        //Space on left and right is the space remaining divided by two
        int spacesRight = remaining / 2;
        int spacesLeft = remaining - spacesRight;

        //Print with width specifier with the predetermined values
        printf("%*s", spacesLeft, "");
        printf("%s", word);
        printf("%*s", spacesRight, "");
    }
}

void saveTree(personPtr *startingPtr, char *fileName)
{
    //Create file pointer
    FILE *fPtr;

    //Open file
    if ((fPtr = fopen(fileName, "w")) == NULL)
    {
        puts("\nFile could not be opened");
    }
    else
    {
        //Create temp pointer to start
        personPtr currentPtr = *startingPtr;

        //Iterate though list
        while (currentPtr != NULL)
        {
            //Variables for parents
            int p1, p2;

            for (size_t i = 0; i < 2; i++)
            {
                //If there are no parents
                if (currentPtr->parents[i] == NULL)
                {
                    //Assign 0 to the parent
                    if (i == 0)
                    {
                        p1 = 0;
                    }

                    if (i == 1)
                    {
                        p2 = 0;
                    }
                }
                else
                {
                    //Assign the id value of the parent in the current pointer to p variable
                    if (i == 0)
                    {
                        p1 = currentPtr->parents[i]->id;
                    }

                    if (i == 1)
                    {
                        p2 = currentPtr->parents[i]->id;
                    }
                }
            }

            //Partner variable
            int partner;
            
            //If person has no partner
            if (currentPtr->partner == NULL)
            {
                //Assign 0 to partner
                partner = 0;
            }
            else
            {
                //Assign id of partner of current pointer
                partner = currentPtr->partner->id;
            }

            //Array to store children ID's
            int childrenID[5];

            for (int i = 0; i < 5; i++)
            {
                //If children are not NULL
                if (currentPtr->children[i] != NULL)
                {
                    //Place ID of child of current pointer into array
                    childrenID[i] = currentPtr->children[i]->id;
                }
                else
                {
                    //Place 0 to represent id 
                    childrenID[i] = 0;
                }
            }

            //print all the data formatted in a line to the file
            fprintf(fPtr, "%d %20s %d %c %d %d %d %d %d %d %d %d\n", currentPtr->id, currentPtr->name, currentPtr->age, currentPtr->status,
                    p1, p2, partner, childrenID[0], childrenID[1], childrenID[2], childrenID[3], childrenID[4]);

            //Go to the next pointer
            currentPtr = currentPtr->next;
        }

        //Close file
        fclose(fPtr); 


        puts("\nThe tree was saved into a file");
    }
}

/* THE LOADING FUNCTION IS NOT WORKING CORRECLTY -- WORKING ON FIXING IT
void loadTree(personPtr *startingPerson, char *fileName)
{
    //Create file pointer
    FILE *fPtr;
    
    //Open file
    if ((fPtr = fopen(fileName, "r")) == NULL)
    {
        puts("\nFile could not be opened");
    }
    else
    {
        //temp pointer
        personPtr currentPtr = *startingPerson;

        //Iterate though list
        while (currentPtr != NULL)
        {
            //Free memory of any previous info in node
            *startingPerson = (*startingPerson)->next;
            free(currentPtr); //dethread
            currentPtr = *startingPerson; //move to next pointer
        }

        //Varibale fore age, id, name, and status
        int id, age;
        char name[20], status;

        //While end of file is not reached
        while (!feof(fPtr))
        {
            //Read lines and input id, name, age, and status in their respective variables
            fscanf(fPtr, "%d %20s %d %c", &id, name, &age, &status);
            newPerson(&currentPtr, id, name, age, status); //Create new node
        }

        rewind(fPtr); //Rewind to beginning of file

        //While end of file is not reaached
        while (!feof(fPtr))
        {
            //Varibales for ID of parent1, parent 2, partner, and integer array for children
            int p1, p2, partner, children[5];

            //Read lines and input ID of people into respective varibles
            fscanf(fPtr, "%d %20s %d %c %d %d %d %d %d %d %d %d\n", &id, name, &age, &status,
                   &p1, &p2, &partner, &children[0], &children[1], &children[2], &children[3], &children[4]);

            //Create temp person pointer
            personPtr tempPerson;

            //iterate though list
            while (currentPtr != NULL)
            {
                //If id of current pointer matches input Id
                if (currentPtr->id == id)
                {
                    //Assign current pointer to temp person and break
                    tempPerson = currentPtr;
                    break;
                }

                //Go to next pointer
                currentPtr = currentPtr->next;
            }

            //Rewind to beginning node pointer
            while (currentPtr != NULL)
            {
                currentPtr = currentPtr->prev;
            }

            //If the person has a parent in p1 spot (not 0)
            if (p1 != 0)
            {
                //Temporary parent pointer
                personPtr tempP1;

                //iterate though list
                while (currentPtr != NULL)
                {
                    //If id of current pointer matches input id
                    if (currentPtr->id == id)
                    {
                        //Assign current pointer to temporary parent pointer and break
                        tempP1 = currentPtr;
                        break;
                    }

                    //Go to next pointer
                    currentPtr = currentPtr->next;
                }

                //Rewind to beginning node in list
                while (currentPtr != NULL)
                {
                    currentPtr = currentPtr->prev;
                }

                //If person is not NULL
                if (tempPerson != NULL)
                {
                    //Run set parent function with name in temp parent and temp person
                    setParent(&currentPtr, tempP1->name, tempPerson->name);
                }
            }

            //If the person has a parent in p2 spot (not 0)
            if (p2 != 0)
            {
                //Temporary parent pointer
                personPtr tempP2;

                //iterate though list
                while (currentPtr != NULL)
                {
                    //If id in current pointer amtches input id
                    if (currentPtr->id == id)
                    {
                        //Assign current pointer to tempoorary parent pointer and break
                        tempP2 = currentPtr;
                        break;
                    }
                    
                    //Go to next pointer
                    currentPtr = currentPtr->next;
                }

                //Rewind to begnning node in list
                while (currentPtr != NULL)
                {
                    currentPtr = currentPtr->prev;
                }

                //If person pointer is not NULL
                if (tempPerson != NULL)
                {
                    //Run set parent function with name of temp parent and temp person
                    setParent(&currentPtr, tempP2->name, tempPerson->name);
                }
            }

            //If there is a partner (id is not 0)
            if (partner != 0)
            {
                //Create temp partner pointer
                personPtr tempPartner;

                //iterate though list
                while (currentPtr != NULL)
                {
                    //If id in current pointer matches input ID assign current pointer to temp partner
                    if (currentPtr->id == id)
                    {
                        tempPartner = currentPtr;
                        break;
                    }

                    //Go to next pointer
                    currentPtr = currentPtr->next;
                }

                //Rewind to beginning node in list
                while (currentPtr != NULL)
                {
                    currentPtr = currentPtr->prev;
                }
                
                //If person is not NULL
                if (tempPerson != NULL)
                {
                    //Run set partner function with names of temp partner and temp person
                    setPartner(&currentPtr, tempPartner->name, tempPerson->name);
                }
            }
        
        }
        
        //Close file
        fclose(fPtr); 

        puts("\nData was loaded from the file");
    }
}*/