/*
 @authors:  Kolarik Cestmír, Cermak  Matous, Balusek Pavel, Hnat Filip
 @name: setcal.c
*/

// Basic libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//--Default constants--
// max rows in a file
#define MAX_ROWS 1000
// maximum number of characters in an element
#define MAX_ELEMENT_LENGTH 30
// Number of initial allocated elements
#define DEFAULT_ELEMENT_ARRAY_LENGHT 10
// Max lenght of an function name
#define MAX_FUNCTION_NAME_LENGTH 100
// Number of functions in a program
#define FUNCTION_COUNT 18
// Number of possible arguments in a command
#define COMMAND_ARG_SIZE 4

// Char constants
#define LARGEST_C_LETTER_CHAR 'Z'
#define SMALLEST_C_LETTER_CHAR 'A'

#define LARGEST_LETTER_CHAR 'z'
#define SMALLEST_LETTER_CHAR 'a'

#define LARGEST_NUMBER_CHAR '9'
#define SMALLEST_NUMBER_CHAR '0'

// Elemental unit of an universe
typedef struct
{
    int lenght;
    char value[MAX_ELEMENT_LENGTH + 1];
} Element;

// Set of elements
typedef struct
{
    int size;
    Element **elements;
} Set;

// Pair of two elements
typedef struct
{
    Element *elementA;
    Element *elementB;
} Pair;

// Relation as an array of pairs
typedef struct
{
    int size;
    Pair **pairs;
} Relation;

// Command struct for activating functions and providing with variables
typedef struct
{
    int count;
    Set *setA;
    Set *setB;
    Relation *rel;
    Set *universe;
    bool success;
    Element data[COMMAND_ARG_SIZE];
} Command;

// Row of an file
// Contains set, relation or an command
typedef struct
{
    Set *set;
    Relation *relation;
    Command *command;
} Row;

// Parsed rows from a file.
typedef struct
{
    int size;
    Set *universe;
    Row rows[MAX_ROWS + 1];
} Data;

// Struct containing a name of an function and a pointer to that function.
typedef struct
{
    char name[MAX_FUNCTION_NAME_LENGTH];
    void (*func)(Command *);
} Function;

// --Initializers/Constructors--
/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
Element *initElement()
{
    Element *element = malloc(sizeof(Element));
    element->lenght = 0;
    element->value[MAX_ELEMENT_LENGTH] = '\0';
    return element;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
Set *initSet(int size)
{
    Set *set = malloc(sizeof(Set));
    set->elements = (Element **)malloc(size * sizeof(Element *));
    set->size = 0;
    set->elements[0] = NULL;
    return set;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
Relation *initRelation()
{
    Relation *relation = malloc(sizeof(Relation));
    relation->pairs = (Pair **)malloc(sizeof(Pair *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
    relation->size = 0;
    relation->pairs[0] = NULL;
    return relation;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
Pair *initPair(Relation *relation)
{
    Pair *pair = malloc(sizeof(Pair));

    pair->elementA = NULL;
    pair->elementB = NULL;

    relation->pairs[relation->size] = pair;

    return pair;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
Data *initData()
{
    Data *data = malloc(sizeof(Data));
    data->size = 0;
    return data;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
Command *initCommand()
{
    Command *command = malloc(sizeof(Command));

    command->setA = NULL;
    command->setB = NULL;
    command->rel = NULL;
    command->universe = NULL;
    command->count = 0;
    command->success = true;

    for (int i = 0; i < COMMAND_ARG_SIZE; i++)
    {
        command->data[i].lenght = 0;
    }

    return command;
}

// --Destructors--
/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void freeElement(Element *element)
{
    free(element);
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void freePair(Pair *pair)
{
    freeElement(pair->elementA);
    freeElement(pair->elementB);
    free(pair);
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void freeReleation(Relation *relation)
{
    for (int i = 0; i < relation->size; i++)
    {
        freePair(relation->pairs[i]);
    }
    free(relation->pairs);
    free(relation);
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void freeSet(Set *set)
{
    for (int i = 0; i < set->size; i++)
    {
        freeElement(set->elements[i]);
    }
    free(set->elements);
    free(set);
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void freeData(Data *data)
{
    for (int i = 0; i < data->size; i++)
    {
        Row row = data->rows[i];

        if (row.relation != NULL)
        {
            freeReleation(row.relation);
        }

        if (row.set != NULL)
        {
            freeSet(row.set);
        }
        else if (row.command != NULL)
        {
            free(row.command);
        }
    }

    free(data);
}

void safeExit(Data *data, FILE *file, int state)
{
    freeData(data);
    fclose(file);
    exit(state);
}

// --Print functions--
/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void printElements(Element **elements, int size)
{
    for (int x = 0; x < size; x++)
    {
        printf(" %s", elements[x]->value);
    }
}

// --Util functions--
/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
bool isCharInRange(char ch, int minChar, int maxChar)
{
    return ch >= minChar && ch <= maxChar;
}

/**
 * Uses isCharInRange function
 * Range is set between lowest and highest capital letters ASCII characters
 * @param {char} ch - character to check
 * @returns {bool} true if character is in range of ASCII capital letters
 */
bool isCharCapitalLetter(char ch)
{
    return isCharInRange(ch, SMALLEST_C_LETTER_CHAR, LARGEST_C_LETTER_CHAR);
}

/**
 * Uses isCharInRange function
 * Range is set between lowest and highest letters ASCII characters
 * @param {char} ch - character to check
 * @returns {bool} true if character is in range of ASCII letters
 */
bool isCharLetter(char ch)
{
    return isCharInRange(ch, SMALLEST_LETTER_CHAR, LARGEST_LETTER_CHAR);
}

/**
 * Uses isCharInRange function
 * Range is set between lowest and highest number ASCII characters
 * @param {char} ch - character to check
 * @returns {bool} true if character is in range of ASCII numbers
 */
bool isCharNumber(char ch)
{
    return isCharInRange(ch, SMALLEST_NUMBER_CHAR, LARGEST_NUMBER_CHAR);
}

/**
 * Checks if string is a number
 * @param {char*} Pointer to a string;
 */
bool isPositiveNumber(char *string)
{
    int i = 0;
    while (string[i] != '\0')
    {
        // if character contains non string, then the string is not a number
        if (!isCharNumber(string[i]))
        {
            fprintf(stderr, "Value is not positive number\n");
            return false;
        }
        i++;
    }
    return true;
}

bool isInSet(char *value, Set *set)
{
    for (int i = 0; i < set->size; i++)
    {
        if (!strcmp(value, set->elements[i]->value))
        {
            return true;
        }
    }

    return false;
}

bool isPairInRelation(Pair *pair, Relation *rel)
{
    for (int i = 0; i < rel->size; i++)
    {
        if (!strcmp(pair->elementA->value, rel->pairs[i]->elementA->value) &&
            !strcmp(pair->elementB->value, rel->pairs[i]->elementB->value))
        {
            return true;
        }
    }

    return false;
}

bool isRelationValid(Relation *relation, Set *universe)
{

    for (int i = 0; i < relation->size; i++)
    {
        Pair *pair = relation->pairs[i];
        if (!isInSet(pair->elementA->value, universe) || !isInSet(pair->elementB->value, universe))
        {
            fprintf(stderr, "Element of a relation is not in universe.\n");
            return false;
        }
    }

    return true;
}

bool isSetValid(Set *set, Set *universe)
{
    for (int i = 0; i < set->size; i++)
    {
        Element *element = set->elements[i];
        if (!isInSet(element->value, universe))
        {
            fprintf(stderr, "Element of a set is not in universe.\n");
            return false;
        }
    }
    return true;
}

bool isUniverseValid(Function *functions, Set *set)
{

    if (isInSet("true", set) || isInSet("false", set))
    {
        return false;
    }

    for (int i = 0; i < FUNCTION_COUNT; i++)
    {
        if (isInSet(functions[i].name, set))
        {
            return false;
        }
    }

    return true;
}

bool isBiCommand(Command *command)
{
    return command->setA != NULL && command->setB != NULL && command->universe != NULL;
}

Set *getUnSet(Command *command)
{
    if (command->universe != NULL)
    {
        if (command->setA != NULL)
        {
            return command->setA;
        }
        else if (command->setB != NULL)
        {
            return command->setB;
        }
    }
    return NULL;
}

bool isRelCommand(Command *command)
{
    return command->rel != NULL && command->universe != NULL;
}

// --Final functions--
/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void finishElement(Element *element)
{
    if (element->lenght < MAX_ELEMENT_LENGTH)
    {
        element->value[element->lenght + 1] = '\0';
    }
}

void addRowToCommand(Command *command, Row *row)
{
    if (row->relation == NULL)
    {
        if (command->setA == NULL)
        {
            command->setA = row->set;
        }
        else
        {
            command->setB = row->set;
        }
    }
    else if (row->relation != NULL && command->rel == NULL)
    {
        command->rel = row->relation;
    }
}

Row *getRow(Data *data, char *value)
{
    if (isPositiveNumber(value))
    {
        int rowIndex = atoi(value) - 1;
        if (data->size > rowIndex)
        {
            return &data->rows[rowIndex];
        }
    }

    return NULL;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void finishCommand(Command *command, Data *data)
{
    Element *element = &command->data[command->count];

    if (element->lenght > 0)
    {
        finishElement(element);
        command->count++;
    }

    for (int i = 1; i < command->count; i++)
    {
        Row *row = getRow(data, command->data[i].value);
        if (row != NULL)
        {
            addRowToCommand(command, row);
        }
        else
        {
            break;
        }
    }
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
void finishPair(Pair *pair)
{
    finishElement(pair->elementA);
    finishElement(pair->elementB);
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
bool finishLastPair(Relation *relation)
{

    Pair *pair = relation->pairs[relation->size];

    if (isPairInRelation(pair, relation))
    {
        fprintf(stderr, "Pair already in relation");
        return false;
    }

    finishPair(pair);
    relation->size++;

    if (relation->size > 0 && relation->size % DEFAULT_ELEMENT_ARRAY_LENGHT)
    {
        Pair **pairs = (Pair **)realloc(relation->pairs, sizeof(Pair *) * relation->size + sizeof(Pair *) * DEFAULT_ELEMENT_ARRAY_LENGHT);

        if (pairs != NULL)
        {
            relation->pairs = pairs;
        }
        else
        {
            fprintf(stderr, "Could not allocate more space for a pair");
            return false;
        }
    }

    relation->pairs[relation->size] = NULL;

    return true;
}

// --Parsing functions--
/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
bool parseToCommand(Command *command, char c)
{
    if (command->count >= COMMAND_ARG_SIZE)
    {
        fprintf(stderr, "Too many arguments.\n");
        return false;
    }

    Element *element = &command->data[command->count];

    if (c == ' ')
    {
        if (element->lenght > 0)
        {
            finishElement(element);
            command->count++;
        }
    }
    else
    {
        element->value[element->lenght] = c;
        element->lenght++;
    }

    return true;
}

/**
 * Check if a character is in range of ASCII values
 * @param {char} ch - character to check
 * @param {int} minChar - lowest ASCII value
 * @param {int} maxChar - highest ASCII value
 * @returns {bool} true if character is in range of values
 */
bool addSetElement(Set *set, Element *element)
{
    if (isInSet(element->value, set))
    {
        fprintf(stderr, "Element %s already in set. \n", element->value);
        return false;
    }

    set->elements[set->size] = element;
    set->size++;

    if (set->size > 0 && set->size % DEFAULT_ELEMENT_ARRAY_LENGHT)
    {
        Element **elements = (Element **)realloc(set->elements, sizeof(Element *) * set->size + sizeof(Element *) * DEFAULT_ELEMENT_ARRAY_LENGHT);

        if (elements != NULL)
        {
            set->elements = elements;
        }
        else
        {
            fprintf(stderr, "Could not allocate more memory for a set");
            return false;
        }
    }

    set->elements[set->size] = NULL;
    return true;
}
bool addCharToElement(Element *element, char c)
{
    if (element->lenght < MAX_ELEMENT_LENGTH)
    {
        if (isCharCapitalLetter(c) || isCharLetter(c))
        {
            element->value[element->lenght] = c;
            element->lenght++;
        }
        else
        {
            fprintf(stderr, "Element has special character or number");
            return false;
        }
    }
    else
    {
        fprintf(stderr, "Size of element reached");
        return false;
    }

    return true;
}

// Returns if set has ended
bool parseToSet(Set *set, char c, Data *data, FILE *file)
{
    if (set == NULL)
    {
        return false;
    }

    Element *element = set->elements[set->size];

    if (c == ' ' || c == '\n' || c == EOF)
    {
        if (element != NULL && !addSetElement(set, element))
        {
            safeExit(data, file, 1);
        }

        if (c == '\n' || c == EOF)
        {
            return true;
        }
        else
        {
            set->elements[set->size] = initElement();
        }
    }
    else
    {
        if (element == NULL || !addCharToElement(element, c))
        {
            fprintf(stderr, "Wrong set initialization");
            safeExit(data, file, 1);
        }
    }

    return false;
}

bool parseToRelation(Relation *relation, char c)
{
    Pair *pair = relation->pairs[relation->size];
    Element *element = NULL;

    if (c == ' ' && pair == NULL)
    {
        initPair(relation);
    }
    else if (pair != NULL)
    {
        if (c == '(')
        {
            pair->elementA = initElement();
        }
        else if (c == ')')
        {
            if (!finishLastPair(relation))
            {
                return false;
            }
        }
        else if (c == ' ' && pair->elementB == NULL)
        {
            pair->elementB = initElement();
        }
        else
        {
            if (pair->elementB != NULL)
            {
                element = pair->elementB;
            }
            else
            {
                element = pair->elementA;
            }
        }

        if (element != NULL && !addCharToElement(element, c))
        {
            return false;
        }
    }
    else
    {
        fprintf(stderr, "Wrong relation initialization");
        return false;
    }

    return true;
}

// --Data processing--
void loadUniverse(Data *data, FILE *file)
{
    char c = getc(file);

    if (c == 'U')
    {
        Set *universe = initSet(DEFAULT_ELEMENT_ARRAY_LENGHT);

        do
        {
            c = getc(file);
        } while (!parseToSet(universe, c, data, file));

        data->universe = universe;
        data->rows[data->size].set = universe;
        data->size++;
    }
    else
    {
        fprintf(stderr, "Universe not found.\n");
        safeExit(data, file, 1);
    }
}

void loadFileData(FILE *file, Data *data)
{
    bool hasCommand = false;
    bool hasRelset = false;

    Relation *relation = NULL;
    Set *set = NULL;
    Command *command = NULL;
    Row *row = NULL;

    bool first = true;

    char c;

    do
    {
        c = getc(file);

        if (first)
        {
            row = &data->rows[data->size];

            relation = NULL;
            set = NULL;
            command = NULL;

            if (c == 'S')
            {
                hasRelset = true;
                set = initSet(DEFAULT_ELEMENT_ARRAY_LENGHT);
            }
            else if (c == 'R')
            {
                hasRelset = true;
                relation = initRelation();
            }
            else if (c == 'C')
            {
                command = initCommand();
                command->universe = data->universe;
                hasCommand = true;
            }

            row->relation = relation;
            row->set = set;
            row->command = command;

            first = false;
            continue;
        }

        if (c != '\n' && c != EOF)
        {
            if (command != NULL && !parseToCommand(command, c))
            {
                safeExit(data, file, 1);
            }
            else if (relation != NULL && !parseToRelation(relation, c))
            {
                safeExit(data, file, 1);
            }
        }

        if (parseToSet(set, c, data, file) || c == '\n' || c == EOF)
        {
            if (set != NULL && !isSetValid(set, data->universe))
            {
                safeExit(data, file, 1);
            }
            else if (relation != NULL && !isRelationValid(relation, data->universe))
            {
                safeExit(data, file, 1);
            }

            data->size++;
            first = true;
        }

    } while (c != EOF && data->size <= MAX_ROWS);

    if (data->size > MAX_ROWS)
    {
        fprintf(stderr, "Too many rows, maximum is 1000.\n");
        safeExit(data, file, 1);
    }
    else if (data->size < 2)
    {
        fprintf(stderr, "There is only an universe.\n");
        safeExit(data, file, 1);
    }

    if (!hasRelset)
    {
        fprintf(stderr, "No relation or set found\n");
        safeExit(data, file, 1);
    }

    if (!hasCommand)
    {
        fprintf(stderr, "No command found.\n");
        safeExit(data, file, 1);
    }
}

// --Print functions--

void printBool(bool value)
{
    value ? printf("true\n") : printf("false\n");
}

void printUniverse(Set *universe)
{
    if (universe != NULL)
    {
        printf("U");

        printElements(universe->elements, universe->size);

        printf("\n");
    }
}

void printSet(Set *set)
{
    if (set != NULL)
    {
        printf("S");

        printElements(set->elements, set->size);

        printf("\n");
    }
}

void printRelation(Relation *relation)
{
    if (relation != NULL)
    {
        printf("R");
        for (int x = 0; x < relation->size; x++)
        {
            Pair *pair = relation->pairs[x];

            printf(" (%s %s)", pair->elementA->value, pair->elementB->value);
        }
        printf("\n");
    }
}

// --Set functions--
/*
empty A - tiskne true nebo false podle toho, jestli je množina definovaná na øádku A prázdná nebo neprázdná.
*/

void empty(Command *com)
{
    Set *set = getUnSet(com);

    if (isRelCommand(com) || isBiCommand(com) || set == NULL || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    printBool(com->setA->size == 0);
}

/*
card A - tiskne poèet prvkù v množinì A (definované na øádku A).
*/
void card(Command *com)
{
    Set *set = getUnSet(com);

    if (isRelCommand(com) || isBiCommand(com) || set == NULL || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    printf("%d\n", com->setA->size);
}

/*
complement A - tiskne doplnìk množiny A.
*/
void complement(Command *com)
{
    Set *set = getUnSet(com);

    if (isRelCommand(com) || isBiCommand(com) || set == NULL || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Set *universe = com->universe;

    printf("S");
    for (int i = 0; i < universe->size; i++)
    {
        bool found = false;
        for (int j = 0; j < set->size; j++)
        {
            if (strcmp(universe->elements[i]->value, set->elements[j]->value) == 0)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            printf(" %s", universe->elements[i]->value);
        }
    }
    printf("\n");
}

/*
union A B - tiskne sjednocení množin A a B.
*/
void setUnion(Command *com)
{
    if (!isBiCommand(com) || isRelCommand(com) || com->count - 1 > 2)
    {
        com->success = false;
        return;
    }

    Set *set1 = com->setA;
    Set *set2 = com->setB;

    printf("S");
    for (int i = 0; i < set1->size; i++)
    {
        bool found = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->value, set2->elements[j]->value) == 0)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            printf(" %s", set1->elements[i]->value);
        }
    }
    for (int k = 0; k < set2->size; k++)
    {
        printf(" %s", set2->elements[k]->value);
    }
    printf("\n");
}

/*
intersect A B - tiskne prùnik množin A a B.
*/
void intersect(Command *com)
{
    if (!isBiCommand(com) || isRelCommand(com) || com->count - 1 > 2)
    {
        com->success = false;
        return;
    }

    Set *set1 = com->setA;
    Set *set2 = com->setB;

    int size = set1->size > set2->size ? set1->size : set2->size;

    Set *set = initSet(size);

    for (int i = 0; i < set1->size; i++)
    {
        for (int j = 0; j < set2->size; j++)
        {
            if (!strcmp(set1->elements[i]->value, set2->elements[j]->value))
            {
                set->elements[set->size] = set1->elements[i];
                set->size++;
            }
        }
    }

    printSet(set);
    freeSet(set);
}

/*
minus A B - tiskne rozdíl množin A \ B.
*/
void minus(Command *com)
{
    if (!isBiCommand(com) || isRelCommand(com) || com->count - 1 > 2)
    {
        com->success = false;
        return;
    }

    Set *set1 = com->setA;
    Set *set2 = com->setB;

    int size = set1->size > set2->size ? set1->size : set2->size;
    Set *set = initSet(size);

    for (int i = 0; i < set1->size; i++)
    {
        bool found = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (!strcmp(set1->elements[i]->value, set2->elements[j]->value))
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            set->elements[set->size] = set1->elements[i];
            set->size++;
        }
    }

    printSet(set);
    freeSet(set);
}

/*
subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
*/
void subseteq(Command *com)
{
    if (!isBiCommand(com) || isRelCommand(com) || com->count - 1 > 2)
    {
        com->success = false;
        return;
    }

    Set *set1 = com->setA;
    Set *set2 = com->setB;

    if (com->setA->size > 0)
    {

        for (int i = 0; i < set1->size; i++)
        {
            Element *elementA = set1->elements[i];

            if (!isInSet(elementA->value, set2))
            {
                printBool(false);
                return;
            }
        }
    }

    printBool(com->setA->size <= set2->size);
}

/*
subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
*/
void subset(Command *com)
{
    if (!isBiCommand(com) || isRelCommand(com) || com->count - 1 > 2)
    {
        com->success = false;
        return;
    }

    Set *set1 = com->setA;
    Set *set2 = com->setB;

    if (com->setA->size > 0)
    {

        for (int i = 0; i < set1->size; i++)
        {
            Element *elementA = set1->elements[i];

            if (!isInSet(elementA->value, set2))
            {
                printBool(false);
                return;
            }
        }
    }

    printBool(com->setA->size < set2->size);
}

/*
equals A B - tiskne true nebo false, jestli jsou množiny rovny.
*/
void equals(Command *com)
{
    if (!isBiCommand(com) || isRelCommand(com) || com->count - 1 > 2)
    {
        com->success = false;
        return;
    }

    Set *set1 = com->setA;
    Set *set2 = com->setB;

    if (com->setA->size > 0)
    {

        for (int i = 0; i < set1->size; i++)
        {
            Element *elementA = set1->elements[i];

            if (!isInSet(elementA->value, set2))
            {
                printBool(false);
                return;
            }
        }
    }

    printBool(com->setA->size == set2->size);
}

// --Relation functions--
/*
reflexive R - tiskne true nebo false, jestli je relace reflexivní.
*/
void reflexive(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;
    Set *universe = com->universe;

    // zjistim, zda je vice rel nez universe
    if (rel->size < universe->size)
    {
        printBool(false);
    }
    else
    {
        int uni[universe->size];
        for (int i = 0; i < universe->size; i++)
        {
            uni[i] = 0;
        }

        for (int i = 0; i < rel->size; i++)
        {
            if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) == 0)
            {
                for (int y = 0; y < universe->size; y++)
                {
                    if (strcoll(rel->pairs[i]->elementA->value, universe->elements[y]->value) == 0)
                    {
                        uni[y] = 1;
                    }
                }
            }
        }
        int citac = 0;
        for (int i = 0; i < universe->size; i++)
        {
            if (uni[i] == 1)
            {
                citac++;
            }
        }

        printBool(citac == universe->size);
    }
}

/*
symmetric R - tiskne true nebo false, jestli je relace symetrická.
*/
void symmetric(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;

    int dvojice[rel->size];
    for (int i = 0; i < rel->size; i++)
    {
        dvojice[i] = 0;
    }

    // vezmu prvni dvojici
    for (int i = 0; i < rel->size; i++)
    {
        // printf("[%s, %s]\n", rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value);
        // zjistim, zda je stejna - napr [A, A]
        if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) == 0)
        {
            dvojice[i] = 1;
        }
        else
        {
            // beru dvojici na porovnavani
            for (int y = 0; y < rel->size; y++)
            {
                if (i != y)
                {
                    if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementB->value) == 0)
                    {
                        if (strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value) == 0)
                        {
                            dvojice[i] = 1;
                        }
                    }
                }
            }
        }
    }

    int citac = 0;
    for (int i = 0; i < rel->size; i++)
    {
        if (dvojice[i] == 1)
        {
            citac++;
        }
    }

    printBool(citac == rel->size);
}

/*
antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
*/
void antisymmetric(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;

    int symetrie = 0;

    for (int i = 0; i < rel->size; i++)
    {
        // kontrola, zda nejsou stejne
        if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) != 0)
        {
            for (int y = 0; y < rel->size; y++)
            {
                if (i != y)
                {
                    if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementB->value) == 0)
                    {
                        if (strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value) == 0)
                        {
                            symetrie++;
                        }
                    }
                }
            }
        }
    }

    printBool(symetrie == 0);
}

/*
transitive R - tiskne true nebo false, jestli je relace tranzitivní.
*/
void transitive(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;

    int chyba = 0;

    for (int i = 0; i < rel->size; i++)
    {
        // kontrola, zda nejsou stejne
        if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) != 0)
        {
            for (int y = 0; y < rel->size; y++)
            {
                if (i != y)
                {
                    if (strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value) == 0)
                    {
                        int nalezeno = 0;

                        for (int x = 0; x < rel->size; x++)
                        {
                            if ((strcoll(rel->pairs[x]->elementA->value, rel->pairs[i]->elementA->value) == 0) & (strcoll(rel->pairs[x]->elementB->value, rel->pairs[y]->elementB->value) == 0))
                            {
                                nalezeno++;
                            }
                        }
                        if (nalezeno == 0)
                        {
                            chyba++;
                        }
                    }
                }
            }
        }
    }

    printBool(chyba == 0);
}

/*
function R - tiskne true nebo false, jestli je relace R funkcí.
*/
void function(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;

    int chyba = 0;

    for (int i = 0; i < rel->size; i++)
    {
        for (int y = 0; y < rel->size; y++)
        {
            if (i != y)
            {
                if (strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementA->value) == 0)
                {
                    if (strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementB->value) != 0)
                    {
                        chyba++;
                    }
                }
            }
        }
    }

    printBool(chyba == 0);
}

/*
domain R - tiskne definièní obor funkce R (lze aplikovat i na relace - první prvky dvojic).
*/
void domain(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;
    Set *universe = com->universe;

    int vypis[universe->size];

    for (int i = 0; i < universe->size; i++)
    {
        vypis[i] = 0;
    }

    for (int i = 0; i < rel->size; i++)
    {
        for (int y = 0; y < universe->size; y++)
        {
            if (strcoll(rel->pairs[i]->elementA->value, universe->elements[y]->value) == 0)
            {
                vypis[y] = 1;
            }
        }
    }

    printf("S");
    for (int i = 0; i < universe->size; i++)
    {
        if (vypis[i] == 1)
        {
            printf(" %s", universe->elements[i]->value);
        }
    }
    printf("\n");
}

/*
codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
*/
void codomain(Command *com)
{
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }

    Relation *rel = com->rel;
    Set *universe = com->universe;

    int vypis[universe->size];

    for (int i = 0; i < universe->size; i++)
    {
        vypis[i] = 0;
    }

    for (int i = 0; i < rel->size; i++)
    {
        for (int y = 0; y < universe->size; y++)
        {
            if (strcoll(rel->pairs[i]->elementB->value, universe->elements[y]->value) == 0)
            {
                vypis[y] = 1;
            }
        }
    }

    printf("S");
    for (int i = 0; i < universe->size; i++)
    {
        if (vypis[i] == 1)
        {
            printf(" %s", universe->elements[i]->value);
        }
    }
    printf("\n");
}

/*
injective R - tiskne true nebo false, jestli je funkce R injektivní.
*/
void injective(Command *com)
{
<<<<<<< Updated upstream
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
=======
    checkArguments(com, 3);
    if(!isRelCommand(com) || isBiCommand(com)){
        exitFunction();
>>>>>>> Stashed changes
    }
    Relation *rel=com->rel;


  int ok = 1;

<<<<<<< Updated upstream
    // printRelation(com->rel);
=======
    for(int i = 0; i < rel->size; i++)
    {
        for(int y = 0; y < rel->size; y++)
        {
            if(i != y)
            {
                if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementA->value) == 0)
                {
                    ok = 0; //konec
                }
            }
        }
    }

    printBool(ok==1);
>>>>>>> Stashed changes
}

/*
surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
*/
void surjective(Command *com)
{
<<<<<<< Updated upstream
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }
    // printRelation(com->rel);
=======
    checkArguments(com, 3);
    if(!isRelCommand(com) || isBiCommand(com)){
        exitFunction();
    }
    Relation *rel = com->rel;
     int ok = 1;

    for(int i = 0; i < rel->size; i++)
    {
        for(int y = 0; y < rel->size; y++)
        {
            if(i != y)
            {
                if(strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementB->value) == 0)
                {
                    ok = 0; //konec
                }
            }
        }
    }
    printBool(ok==1);
>>>>>>> Stashed changes
}

/*
bijective R - tiskne true nebo false, jestli je funkce R bijektivní.
*/
void bijective(Command *com)
{
<<<<<<< Updated upstream
    if (!isRelCommand(com) || isBiCommand(com) || com->count - 1 > 1)
    {
        com->success = false;
        return;
    }
    // printRelation(com->rel);
=======
    checkArguments(com, 3);
    if(!isRelCommand(com) || isBiCommand(com)){
        exitFunction();
    }
    Relation *rel = com->rel;
   int ok = 1;

    for(int i = 0; i < rel->size; i++)
    {
        for(int y = 0; y < rel->size; y++)
        {
            if(i != y)
            {
                if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementA->value) == 0)
                {
                    ok = 0; //konec
                }
                
                if(strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementB->value) == 0)
                {
                    ok = 0; //konec
                }
            }
        }
    }
    printBool(ok==1);
}

//Advanced commmands

/*
closure_ref R - tiskne reflexivní uzávìr relace R
*/
void closureRef(Command *com)
{
    checkArguments(com, 1);
    if(!isRelCommand(com) || isBiCommand(com)){
        exitFunction();
    }
      //  printRelation(com->rel);
}

/*
closure_sym R - tiskne symetrický uzávìr relace R
*/
void closureSym(Command *com)
{
    checkArguments(com, 1);
    if(!isRelCommand(com) || isBiCommand(com)){
        exitFunction();
    }
      //  printRelation(com->rel);
}

/*
closure_trans R - tiskne tranzitivní uzávìr relace R
*/
void closureTrans(Command *com)
{
    if(!isRelCommand(com) || isBiCommand(com)){
        exitFunction();
    }
       // printRelation(com->rel);
}

//-- Bonus --
/*
select A N - vybere náhodný prvek z množiny nebo relace A a tiskne ho. V pøípadì, že je množina A prázdná, pøeskoèí vykonávání pøíkazu na øádek N vstupního souboru. N v takovém pøípadì musí oznaèovat existující øádek ve vstupním souboru.
*/
void selectFromRelation(Command *com)
{
    checkArguments(com, 1);
     //   printRelation(com->rel);
}

void selectFromSet(Command *com)
{
   (void)com;
>>>>>>> Stashed changes
}

// Command functions
bool functionLookup(Function *functions, Command *com)
{
    for (int i = 0; i < FUNCTION_COUNT; i++)
    {
        Function function = functions[i];
        if (!strcmp(function.name, com->data[0].value))
        {
            function.func(com);
            if (!com->success)
            {
                fprintf(stderr, "Function arguments do not meet requirements.\n");
                return false;
            }
            return true;
        }
    }

    fprintf(stderr, "Function %s not found", com->data[0].value);
    return false;
}

void initFunctions(Function *functions)
{
    // unsetfun
    strcpy(functions[0].name, "empty");
    functions[0].func = empty;

    strcpy(functions[1].name, "card");
    functions[1].func = card;

    strcpy(functions[2].name, "complement");
    functions[2].func = complement;

    // binsetfun
    strcpy(functions[3].name, "union");
    functions[3].func = setUnion;

    strcpy(functions[4].name, "intersect");
    functions[4].func = intersect;

    strcpy(functions[5].name, "minus");
    functions[5].func = minus;

    strcpy(functions[6].name, "subseteq");
    functions[6].func = subseteq;

    strcpy(functions[7].name, "subset");
    functions[7].func = subset;

    strcpy(functions[8].name, "equals");
    functions[8].func = equals;

    // unRelFunMap
    strcpy(functions[9].name, "reflexive");
    functions[9].func = reflexive;

    strcpy(functions[10].name, "symmetric");
    functions[10].func = symmetric;

    strcpy(functions[11].name, "antisymmetric");
    functions[11].func = antisymmetric;

    strcpy(functions[12].name, "transitive");
    functions[12].func = transitive;

    strcpy(functions[13].name, "function");
    functions[13].func = function;

    strcpy(functions[14].name, "domain");
    functions[14].func = domain;

    strcpy(functions[15].name, "codomain");
    functions[15].func = codomain;

    strcpy(functions[16].name, "injective");
    functions[16].func = injective;

    strcpy(functions[17].name, "surjective");
    functions[17].func = surjective;

    strcpy(functions[18].name, "bijective");
    functions[18].func = bijective;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid argument count\n");
        return 1;
    }

    char *fileName = argv[1];

    // Test if project is running
    Data *data = initData();

    Function functions[FUNCTION_COUNT];

    initFunctions(functions);

    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open a file");
        return 1;
    }

    loadUniverse(data, file);

    if (!isUniverseValid(functions, data->universe))
    {
        fprintf(stderr, "Universe is not valid");
        safeExit(data, file, 1);
    }

    printUniverse(data->universe);

    loadFileData(file, data);

    for (int i = 1; i < data->size; i++)
    {
        Row *row = &data->rows[i];
        if (row->command != NULL)
        {
            Command *command = row->command;
            finishCommand(command, data);
            if (!functionLookup(functions, command))
            {
                safeExit(data, file, 1);
            }
        }
        else if (row->set != NULL)
        {
            printSet(row->set);
        }
        else if (row->relation != NULL)
        {
            printRelation(row->relation);
        }
        else
        {
            fprintf(stderr, "Invalid row");
            safeExit(data, file, 1);
        }
    }

    safeExit(data, file, 0);
}
