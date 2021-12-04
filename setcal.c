/*
 @authors:  Kolarik Cestmír, Cermak  Matous, Balusek Pavel, Hnat Filip
 @name: setcal.c
*/

//Basic libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Default constants
#define MAX_ROWS 1000
#define MAX_ELEMENT_LENGTH 30
#define DEFAULT_ELEMENT_ARRAY_LENGHT 10
#define MAX_FUNCTION_NAME_LENGTH 100

//function counts
#define FUNCTION_COUNT 25

#define LARGEST_NUMBER_CHAR '9'
#define SMALLEST_NUMBER_CHAR '0'

//Elemental unit of an universe
typedef struct
{
    int lenght;
    char value[MAX_ELEMENT_LENGTH + 1];
} Element;

//Set of elements
typedef struct
{
    int size;
    Element **elements;
} Set;

//Pair of two elements
typedef struct
{
    Element *elementA;
    Element *elementB;
} Pair;

//Relation of pairs
typedef struct
{
    int size;
    Pair **pairs;
} Relation;

//Command struct for activating functions
typedef struct
{
    char functionName[MAX_ELEMENT_LENGTH + 1];
    Set *setA;
    Set *setB;
    Relation *rel;
    Set* universe;
} Command;

//Row of an file
//Contains set or relation or set for an command
typedef struct
{
    Set *set;
    Relation *relation;
    bool command;
} Row;

//Rows from a file.
typedef struct
{
    int size;
    Set* universe;
    Row rows[MAX_ROWS];
} Data;

//--Function structs--
typedef struct
{
    char name[MAX_FUNCTION_NAME_LENGTH];
    void (*func)(Command*);
} Function;

// --Init functions--
Element *initElement()
{
    Element *element = malloc(sizeof(Element));
    element->lenght = 0;
    element->value[MAX_ELEMENT_LENGTH] = '\0';
    return element;
}

Set *initSet(int size)
{
    Set *set = malloc(sizeof(Set));
    set->elements = (Element **)malloc(size);
    set->size = 0;
    set->elements[0] = NULL;
    return set;
}

Relation *initRelation()
{
    Relation *relation = malloc(sizeof(Relation));
    relation->pairs = (Pair **) malloc(sizeof(Pair *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
    relation->size = 0;
    relation->pairs[0] = NULL;
    return relation;
}

Pair *initPair(Relation *relation)
{
    Pair *pair = malloc(sizeof(Pair));

    pair->elementA = NULL;
    pair->elementB = NULL;

    relation->pairs[relation->size] = pair;

    return pair;
}

Data *initData()
{
    Data *data = malloc(sizeof(Data));
    data->size = 0;
    return data;
}

Command *initCommand()
{
    Command *command = malloc(sizeof(Command));

    command->setA = NULL;
    command->setB = NULL;
    command->rel = NULL;

    return command;
}

// --Free functions--
void freeElement(Element *element)
{
    free(element);
}

void freePair(Pair *pair)
{
    freeElement(pair->elementA);
    freeElement(pair->elementB);
    free(pair);
}

void freeReleation(Relation *relation)
{
    for (int i = 0; i < relation->size; i++)
    {
        freePair(relation->pairs[i]);
    }

    free(relation);
}

void freeSet(Set *set)
{
    for (int i = 0; i < set->size; i++)
    {
        freeElement(set->elements[i]);
    }
    free(set);
}

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
    }

    free(data);
}

// --Util functions--

/**
 * Uses isCharInRange function
 * Range is set between lowest and highest number ASCII characters
 * @param {char} ch - character to check
 * @returns {bool} true if character is in range of ASCII numbers
 */
bool isCharNumber(char ch)
{
    return ch >= SMALLEST_NUMBER_CHAR && ch <= LARGEST_NUMBER_CHAR ? true : false;
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
        //if character contains non string, then the string is not a number
        if (!isCharNumber(string[i]))
        {
            fprintf(stderr, "Value is not positive number\n", string);
            return false;
        }
        i++;
    }
    return true;
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

Command *parseSetToCommand(Set *set, Data *data)
{
    Command *command = initCommand();
    command->universe = data->universe;

    if (set->size > 0)
    {
        strcpy(&command->functionName, &set->elements[0]->value);

        for (int i = 1; i < set->size && i < 4; i++)
        {
            Row *row = getRow(data, set->elements[i]->value);
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

    return command;
}

void addSetElement(Set *set, Element *element)
{
    set->elements[set->size] = element;
    set->size++;

    if (set->size > 0 && set->size % DEFAULT_ELEMENT_ARRAY_LENGHT)
    {
        set->elements = (Element **)realloc(set->elements, set->size + sizeof(Element *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
    }

    set->elements[set->size] = initElement();
}

void finishElement(Element *element)
{
    if (element->lenght < MAX_ELEMENT_LENGTH)
    {
        element->value[element->lenght + 1] = '\0';
    }
}

void finishPair(Pair *pair)
{
    finishElement(pair->elementA);
    finishElement(pair->elementB);
}

void finishLastPair(Relation *relation)
{
    finishPair(relation->pairs[relation->size]);
    relation->size++;

    if (relation->size > 0 && relation->size % DEFAULT_ELEMENT_ARRAY_LENGHT)
    {
        relation->pairs = (Pair **)realloc(relation->pairs, relation->size + sizeof(Pair *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
    }

    relation->pairs[relation->size] = NULL;
}

//Returns if set has ended
bool parseToSet(Set *set, char c)
{
    Element *element = set->elements[set->size];

    if (c == ' ' || c == '\n' || c == EOF)
    {
        if (element != NULL)
        {
            addSetElement(set, element);
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
        element->value[element->lenght] = c;
        element->lenght++;
    }

    return false;
}

void parseToRelation(Relation *relation, char c)
{
    Pair *pair = relation->pairs[relation->size];
    Element *element = NULL;

    if (c == ' ' && pair == NULL)
    {
        relation->pairs[relation->size] = initPair(relation);
    }
    else if (pair != NULL)
    {
        if (c == '(')
        {
            pair->elementA = initElement();
        }
        else if (c == ')')
        {
            finishLastPair(relation);
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
    }

    if (element != NULL)
    {
        element->value[element->lenght] = c;
        element->lenght++;
    }
}

bool isInSet(char* value, Set* set){
    for (int i = 0; i < set->size; i++)
    {
        if (!strcmp(value, set->elements[i]->value))
        {
            return true;
        }
    }
}

bool isSetValid(Set* set, Set* universe){
    for (int i = 0; i < set->size; i++)
    {
        bool found = isInSet(set->elements[i]->value, universe);

        if(!found){
            return false;
        }
    }

    return true;
}

bool isUniverseValid(Function* functions, Set* set){

    if(isInSet("true", set) || isInSet("false", set))
    {
        return false;
    }

    for (int i = 0; i < FUNCTION_COUNT; i++)
    {
        if(isInSet(functions[i].name, set)){
            return false;
        }
    }

    return true;
}

// --Data processing--
void loadUniverse(Data* data, FILE* file){
    Set *universe;

    char c = getc(file);

    if(c == 'U'){
        universe = initSet(sizeof(Element *) * DEFAULT_ELEMENT_ARRAY_LENGHT);

        do
        {
            c = getc(file);
            parseToSet(universe, c);
        }while (c != '\n' && c != EOF);

        data->universe = universe;
        data->rows[data->size].set = universe;
        data->size++;
    }
}

void loadFileData(FILE *file, Data *data)
{
    Relation *relation = NULL;
    Set *set = NULL;
    Row *row = NULL;

    bool first = true;

    char c;

    do
    {
        c = getc(file);

        if (first)
        {
            row = &data->rows[data->size];

            if (c == 'S' || c == 'C')
            {
                if (c == 'C')
                {
                    row->command = true;
                }

                relation = NULL;
                set = initSet(sizeof(Element *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
            }
            else if (c == 'R')
            {
                row->relation = malloc(sizeof(Relation));
                row->set = NULL;

                set = NULL;
                relation = initRelation();
            }

            row->relation = relation;
            row->set = set;

            first = false;
            continue;
        }

        if (relation != NULL && c != '\n')
        {
            parseToRelation(relation, c);
        }
        else if (set != NULL && parseToSet(set, c) || c == '\n' || c == EOF)
        {
            data->size++;
            first = true;
        }
    } while (c != EOF && data->size <= MAX_ROWS);
}

// --Print functions--

void printBool(bool value){
    value ? printf("true\n") : printf("false\n");
}

void printElements(Element** elements, int size){
    for (int x = 0; x < size; x++)
    {
        printf("%s ", elements[x]->value);
    }
}

void printUniverse(Set *universe){
    if (universe != NULL)
    {
        printf("U ");

        printElements(universe->elements, universe->size);

        printf("\n");
    }
}

void printSet(Set *set)
{
    if (set != NULL)
    {
        printf("S ");

        printElements(set->elements, set->size);

        printf("\n");
    }
}

void printRelation(Relation *relation)
{
    if (relation != NULL)
    {
        printf("R ");
        for (int x = 0; x < relation->size; x++)
        {
            Pair *pair = relation->pairs[x];

            printf("(%s %s) ", pair->elementA->value, pair->elementB->value);
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
    printBool(com->setA->size == 0);
}

/*
card A - tiskne poèet prvkù v množinì A (definované na øádku A).
*/
void card(Command *com)
{
    printf("%d \n", com->setA->size);
}

/*
complement A - tiskne doplnìk množiny A.
*/
void complement(Command *com)
{
    /*
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
    printf("\n");*/
}

/*
union A B - tiskne sjednocení množin A a B.
*/
void setUnion(Command *com)
{
    /*
    printf("S");
    for(int i=0;i<set1->size;i++){
        bool found = false;
        for(int j=0;j<set2->size;j++){
            if(strcmp(set1->elements[i]->value, set2->elements[j]->value)==0){
                found = true;
                break;
                }
            }
            if (found == false){
                printf(" %s", set1->elements[i]->value);
        }
    }
    for(int k=0;k<set2->size;k++){
        printf(" %s", set2->elements[k]->value);
    }
    printf("\n");
    */
}

/*
intersect A B - tiskne prùnik množin A a B.
*/
void intersect(Command *set)
{
    /*
    int size = set1->size > set2->size ? set1->size : set2->size;

    Set* set = initSet(size);

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
    free(set);*/
}

/*
minus A B - tiskne rozdíl množin A \ B.
*/
void minus(Command *set1)
{
    /*
    int size = set1->size > set2->size ? set1->size : set2->size;
    Set* set = initSet(size);

    for (int i = 0; i < set1->size; i++)
    {
        bool found = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (!strcmp(set1->elements[i]->value, set2->elements[j]->value))
            {
                found = true;
            }
        }

        if(!found == false){
            set->elements[set->size] = set1->elements[i];
            set->size++;
        }
    }

    printSet(set);
    free(set);*/
}

/*
subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
*/
void subseteq(Command *set1)
{
    /*
    bool equality = false;
    for (int i = 0; i < set1->size; i++)
    {
        equality = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->value, set2->elements[j]->value) == 0)
            {
                equality = true;
                break;
            }
        }
        if (equality == false)
        {
            break;
        }
    }

    printBool(equality);*/
}

/*
subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
*/
void subset(Command *set1)
{
    /*
    bool equality = false;
    int counter = 0;
    for (int i = 0; i < set1->size; i++)
    {
        equality = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->value, set2->elements[j]->value) == 0)
            {
                counter++;
                equality = true;
                break;
            }
        }
        if (equality == false)
        {
            break;
        }
    }

    printBool(equality && (counter != set2->size));*/
}

/*
equals A B - tiskne true nebo false, jestli jsou množiny rovny.
*/
void equals(Command *set2)
{
    /*
    bool equality = true;
    for (int i = 0; i < set1->size; i++)
    {
        for (int j = 0; j < set2->size + 1; j++)
        {
            if (strcmp(set1->elements[i]->value, set2->elements[j]->value) == 0)
            {
                break;
            }
            else
            {
                if (set2->size == j)
                {
                    equality = false;
                }
            }
        }
        if (equality == false)
        {
            break;
        }
    }

    printBool(equality);*/
}

// --Relation functions--
/*
reflexive R - tiskne true nebo false, jestli je relace reflexivní.
*/
void reflexive(Command *rel)
{
    /*
    printf("reflexive \n");
    //zjistim, zda je vice rel nez universe
    if(rel->size < universe->size)
    {
        printf("FALSE\n");
    }
    else
    {
        int uni[universe->size];
        for(int i = 0; i < universe->size; i++)
        {
            uni[i] = 0;
        }

        for(int i = 0; i < rel->size; i++)
        {
            if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) == 0)
            {
                for(int y = 0; y < universe->size; y++)
                {
                    if(strcoll(rel->pairs[i]->elementA->value, universe->elements[y]->value)==0)
                    {
                        uni[y] = 1;
                    }
                }
            }
        }
        int citac = 0;
        for(int i = 0; i < universe->size; i++)
        {
            if(uni[i] == 1)
            {
                citac++;
            }
        }

        printBool(citac == universe->size);
    }*/
}

/*
symmetric R - tiskne true nebo false, jestli je relace symetrická.
*/
void symmetric(Command *rel)
{
    /*
    int dvojice[rel->size];
    for(int i = 0; i <rel->size;i++)
    {
        dvojice[i] = 0;
    }

    //vezmu prvni dvojici
    for(int i = 0; i < rel->size; i++)
    {
        //printf("[%s, %s]\n", rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value);
        //zjistim, zda je stejna - napr [A, A]
        if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) == 0)
        {
            dvojice[i] = 1;
        }
        else
        {
            //beru dvojici na porovnavani
            for(int y = 0; y < rel->size; y++)
            {
                if(i != y)
                {
                    if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementB->value) == 0)
                    {
                        if(strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value)==0)
                        {
                            dvojice[i] = 1;
                        }
                    }
                }
            }
        }
    }

    int citac = 0;
    for(int i = 0; i < rel->size; i++)
    {
        if(dvojice[i] == 1)
        {
            citac++;
        }
    }

    printBool(citac == rel->size);*/
}

/*
antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
*/
void antisymmetric(Command *rel)
{
    /*
    int symetrie = 0;

    for(int i = 0; i <rel->size; i++)
    {
        //kontrola, zda nejsou stejne
        if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) != 0)
        {
            for(int y = 0; y < rel->size; y++)
            {
                if(i != y)
                {
                    if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementB->value) == 0)
                    {
                        if(strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value)==0)
                        {
                            symetrie++;
                        }
                    }
                }
            }
        }
    }

    printBool(symetrie == 0);*/
}

/*
transitive R - tiskne true nebo false, jestli je relace tranzitivní.
*/
void transitive(Command *rel)
{
    /*
    int chyba = 0;

    for(int i = 0; i < rel->size; i++)
    {
        //kontrola, zda nejsou stejne
        if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value) != 0)
        {
            for(int y = 0; y < rel->size; y++)
            {
                if(i != y)
                {
                    char znak;

                    if(strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value) == 0)
                    {
                        int nalezeno = 0;

                        //printf("[%s %s][%s %s]", rel->pairs[i]->elementA->value, rel->pairs[i]->elementB->value, rel->pairs[y]->elementA->value, rel->pairs[y]->elementB->value);
                        //printf(" hledam [%s %s]\n", rel->pairs[i]->elementA->value, rel->pairs[y]->elementB->value);

                        for(int x = 0; x < rel->size; x++)
                        {
                            if((strcoll(rel->pairs[x]->elementA->value, rel->pairs[i]->elementA->value) == 0)
                              &(strcoll(rel->pairs[x]->elementB->value, rel->pairs[y]->elementB->value) == 0))
                            {
                                nalezeno++;
                                //printf("X [%s %s] \n", rel->pairs[x]->elementA->value, rel->pairs[x]->elementB->value);
                            }
                        }
                        if(nalezeno == 0)
                        {
                            chyba++;
                        }
                    }
                }
            }
        }
    }

    printBool(chyba == 0);
    */
}

/*
function R - tiskne true nebo false, jestli je relace R funkcí.
*/
void function(Command *rel)
{
    /*
    int chyba = 0;

    for(int i = 0; i < rel->size; i++)
    {
        for(int y = 0; y < rel->size; y++)
        {
            if(i != y)
            {
                if(strcoll(rel->pairs[i]->elementA->value, rel->pairs[y]->elementA->value)==0)
                {
                    if(strcoll(rel->pairs[i]->elementB->value, rel->pairs[y]->elementB->value)!=0)
                    {
                        chyba++;
                    }
                }
            }
        }
    }

    printBool(chyba == 0);*/
}

/*
domain R - tiskne definièní obor funkce R (lze aplikovat i na relace - první prvky dvojic).
*/
void domain(Command *rel)
{
    /*
    int vypis[universe->size];

    for(int i = 0; i < universe->size; i++)
    {
        vypis[i] = 0;
    }

    for(int i = 0; i < rel->size; i++)
    {
        for(int y = 0; y < universe->size; y++)
        {
            if(strcoll(rel->pairs[i]->elementA->value, universe->elements[y]->value) == 0)
            {
                vypis[y] = 1;
            }
        }

    }

    for(int i = 0; i < universe->size; i++)
    {
        if(vypis[i] == 1)
        {
            printf("%s ", universe->elements[i]->value);
        }
    }
    printf("\n");*/
}

/*
codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
*/
void codomain(Command *rel)
{
    /*
    int vypis[universe->size];

    for(int i = 0; i < universe->size; i++)
    {
        vypis[i] = 0;
    }

    for(int i = 0; i < rel->size; i++)
    {
        for(int y = 0; y < universe->size; y++)
        {
            if(strcoll(rel->pairs[i]->elementB->value, universe->elements[y]->value) == 0)
            {
                vypis[y] = 1;
            }
        }

    }

    for(int i = 0; i < universe->size; i++)
    {
        if(vypis[i] == 1)
        {
            printf("%s ", universe->elements[i]->value);
        }
    }
    printf("\n");*/
}

/*
injective R - tiskne true nebo false, jestli je funkce R injektivní.
*/
void injective(Command *rel)
{
    //TODO
   /* printf("injective \n");
    printf("relation :\n");
    printRelation(rel);
    printf("first set");
    printSet(set1);
    printf("second set");
    printSet(set2);
    printf("universe:\n");
    printSet(universe); */
}

/*
surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
*/
void surjective(Command *rel)
{
    //TODO
    /* printf("surjective \n");
    printf("relation :\n");
    printRelation(rel);
    printf("first set");
    printSet(set1);
    printf("second set");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);*/
}

/*
bijective R - tiskne true nebo false, jestli je funkce R bijektivní.
*/
void bijective(Command *rel)
{
    //TODO
   /* printf("bijective \n");
    printf("relation :\n");
    printRelation(rel);
    printf("first set");
    printSet(set1);
    printf("second set");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);*/
}

//Advanced commmands

/*
closure_ref R - tiskne reflexivní uzávìr relace R
*/
void closureRef(Command *rel)
{
    //TODO
   /* printf("closureRef \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);*/
}

/*
closure_sym R - tiskne symetrický uzávìr relace R
*/
void closureSym(Command *rel)
{
    //TODO
    /*printf("closureSym \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);*/
}

/*
closure_trans R - tiskne tranzitivní uzávìr relace R
*/
void closureTrans(Command *rel)
{
    //TODO
    /*printf("closureTrans \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);*/
}

//-- Bonus --
/*
select A N - vybere náhodný prvek z množiny nebo relace A a tiskne ho. V pøípadì, že je množina A prázdná, pøeskoèí vykonávání pøíkazu na øádek N vstupního souboru. N v takovém pøípadì musí oznaèovat existující øádek ve vstupním souboru.
*/
void selectFromRelation(Command *rel)
{
    //TODO
    /*printf("selectFromRelation \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);*/
}

void selectFromSet(Command *set)
{
    //TODO
     /*printf("selectFromSet \n");
    printf("mnozina :\n");
    printSet(set);
    printf("universe:\n");
    printSet(universe);*/
}

// Arguments
/*
Rozšíøení všech pøíkazù, jejichž výsledkem je množina nebo relace, definuje novou množinu nebo relaci identifikovanou èíslem øádku, na kterém se nachází daná operace.
Rozšíøení všech pøíkazù, které tisknou true nebo false o další argument N. V pøípadì, že operace konèí s výsledkem false, následující øádek, který se zpracovává, bude na øádku N (nikoliv bezprostøednì následující).
*/
//-- Bonus --

//Command functions
void functionLookup(Function* functions, Command *com, Data *data)
{
    for(int i = 0; i < FUNCTION_COUNT; i++){
        Function function = functions[i];
        if(!strcmp(function.name, com->functionName)){
            function.func(com);
            return;
        }
    }
}

void initFunctions(Function* functions)
{
    //unsetfun
    strcpy(functions[0].name, "empty");
    functions[0].func = empty;

    strcpy(functions[1].name, "card");
    functions[1].func = card;

    strcpy(functions[2].name, "complement");
    functions[2].func = complement;

    strcpy(functions[3].name, "select");
    functions[3].func = selectFromSet;

    //binsetfun
    strcpy(functions[4].name, "union");
    functions[4].func = setUnion;

    strcpy(functions[5].name, "intersect");
    functions[5].func = intersect;

    strcpy(functions[6].name, "minus");
    functions[6].func = minus;

    strcpy(functions[7].name, "subseteq");
    functions[7].func = subseteq;

    strcpy(functions[8].name, "subset");
    functions[8].func = subset;

    strcpy(functions[9].name, "equals");
    functions[9].func = equals;

         //unRelFunMap
    strcpy(functions[10].name, "reflexive");
     functions[10].func = reflexive;

     strcpy(functions[11].name, "symmetric");
     functions[11].func = symmetric;

     strcpy(functions[12].name, "antisymmetric");
     functions[12].func = antisymmetric;

     strcpy(functions[13].name, "transitive");
     functions[13].func = transitive;

     strcpy(functions[14].name, "function");
     functions[14].func = function;

     strcpy(functions[15].name, "domain");
     functions[15].func = domain;

    strcpy(functions[16].name, "codomain");
    functions[16].func = codomain;

    strcpy(functions[17].name, "closureRef");
    functions[17].func = closureRef;

    strcpy(functions[18].name, "closureSym");
   functions[18].func = closureSym;

    strcpy(functions[19].name, "closureTrans");
    functions[19].func = closureTrans;

     strcpy(functions[20].name, "selectFromRelation");
    functions[20].func = selectFromRelation;


         //triRelFunMap
    strcpy(functions[21].name, "injective");
    functions[21].func = injective;

     strcpy(functions[22].name, "surjective");
     functions[22].func = surjective;

    strcpy(functions[23].name, "bijective");
    functions[23].func = bijective;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid argument count\n");
        return 1;
    }

    char *fileName = argv[1];

    //Test if project is running
    Data *data = initData();

    Function functions[FUNCTION_COUNT];

    initFunctions(&functions);

    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open a file");
        return 1;
    }

    loadUniverse(data, file);

    if(isUniverseValid(&initFunctions, data->universe)){
        fprintf(stderr, "Universe is not valid");
        return 1;
    }

    loadFileData(file, data);

    printUniverse(data->universe);

    for (int i = 1; i < data->size; i++)
    {
        Row *row = &data->rows[i];
        if (row->set != NULL)
        {
            if(row->command){
                Command *command = parseSetToCommand(row->set, data);
                functionLookup(&functions, command, data);
                free(command);
            }else{
                printSet(row->set);
            }
        }else if(row->relation != NULL)
        {
            printRelation(row->relation);
        }else{
            fprintf(stderr, "Invalid row");
            return 1;
        }
    }

    freeData(data);
    fclose(file);

    return 0;
}
