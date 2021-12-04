/*
 @authors:  Kolarik Cestmír, Cermak  Matous, Balusek Pavel, Hnat Filip
 @name: setcal.c
*/

//Basic libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS 1000
#define MAX_ELEMENT_LENGTH 30
#define DEFAULT_ELEMENT_ARRAY_LENGHT 10

#define LARGEST_NUMBER_CHAR '9'
#define SMALLEST_NUMBER_CHAR '0'

typedef struct
{
    int lenght;
    char values[MAX_ELEMENT_LENGTH + 1];
} Element;

typedef struct
{
    int size;
    Element **elements;
} Set;

typedef struct
{
    Element *elementA;
    Element *elementB;
} Pair;

typedef struct
{
    int size;
    Pair **pairs;
} Relation;

typedef struct
{
    char functionName[MAX_ELEMENT_LENGTH];
    Set *setA;
    Set *setB;
    Relation *rel;
} Command;

typedef struct
{
    Set *set;
    Relation *relation;
    bool command;
} Row;

typedef struct
{
    int size;
    Row rows[MAX_ROWS];
} Data;

// --Init functions--
Element *initElement()
{
    Element *element = malloc(sizeof(Element));
    element->lenght = 0;
    element->values[MAX_ELEMENT_LENGTH] = '\0';
    return element;
}

Set *initSet()
{
    Set *set = malloc(sizeof(Set));
    set->elements = (Element **)malloc(sizeof(Element *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
    set->size = 0;
    set->elements[0] = NULL;
    return set;
}

Relation *initRelation()
{
    Relation *relation = malloc(sizeof(Relation));
    relation->pairs = (Pair **)malloc(sizeof(Pair *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
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
            fprintf(stderr, "Hodnota %s neni cele kladne cislo\n", string);
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

Row *getRow(Data *data, char *values)
{
    if (isPositiveNumber(values))
    {
        int rowIndex = atoi(values);
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

    if (set->size > 0)
    {
        strcpy(&command->functionName, &set->elements[0]->values);

        //char *values = set->elements[1]->values;

        for (int i = 1; i < set->size && i < 4; i++)
        {
            Row *row = getRow(data, set->elements[i]->values);
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
        element->values[element->lenght + 1] = '\0';
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
        element->values[element->lenght] = c;
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
        element->values[element->lenght] = c;
        element->lenght++;
    }
}

// --Data processing--

void loadFileData(FILE *file, Data *data)
{
    Relation *relation = NULL;
    // Command *command = NULL;
    Set *set = NULL;
    Row *row = NULL;
    //Pair *pair = NULL;

    bool first = true;

    char c;

    do
    {
        c = getc(file);

        if (first)
        {
            row = &data->rows[data->size];

            if (c == 'U' || c == 'S' || c == 'C')
            {
                if (c == 'C')
                {
                    row->command = true;
                }

                relation = NULL;
                set = initSet();
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
void printSet(Set *set)
{
    if (set != NULL)
    {
        for (int x = 0; x < set->size; x++)
        {
            printf(" %s, ", set->elements[x]->values);
        }
    }
    else
    {
        printf("Mnozina je prazdna");
    }

    printf("\n");
}

void printRelation(Relation *relation)
{
    if (relation != NULL)
    {
        for (int x = 0; x < relation->size; x++)
        {
            Pair *pair = relation->pairs[x];

            printf(" Hodnota A %s, hodnota B %s, cela relace je (%s %s) \n",
                   pair->elementA->values,
                   pair->elementB->values,
                   pair->elementA->values,
                   pair->elementB->values);
        }
    }
    else
    {
        printf("Relace je prazdna");
    }
    printf("\n");
}

void printCommand(Command *command)
{
    //control print
    printf("Prikaz %s, pracuje s \n", command->functionName);

    printSet(command->setA);
    printf(" a ");
    printSet(command->setB);
    printf(" a ");
    printRelation(command->rel);
}

void printData(Data *data)
{
    for (int i = 0; i <= data->size; i++)
    {
        Row row = data->rows[i];

        if (row.relation != NULL)
        {
            printf("Na radku %d je relation: \n", i);
            printRelation(row.relation);
        }
        else if (row.set != NULL)
        {
            if (row.command)
            {
                Command *command = parseSetToCommand(row.set, data);
                printf("Na radku je %d je prikaz: ", i);
                printCommand(command);
            }
            else
            {
                printf("Na radku je %d je mnozina: ", i);
                printSet(row.set);
            }
        }
    }
}

// --Set functions--

/*
empty A - tiskne true nebo false podle toho, jestli je množina definovaná na øádku A prázdná nebo neprázdná.
*/

void empty(Set *set, Set *universe)
{
    (void)universe;

    /*  //TODO
    printf("empty");
    printf("first set :\n");
    printSet(set);
    printf("universe:\n");
    printSet(universe);
*/

    if (set->size == 0)
    {
        printf("true \n");
    }
    else
    {
        printf("false \n");
    }
}

/*
card A - tiskne poèet prvkù v množinì A (definované na øádku A).
*/
void card(Set *set, Set *universe)
{
    (void)universe;
    //TODO
    /*  printf("card \n");
    printf("first set :\n");
    printSet(set);
    printf("universe:\n");
    printSet(universe);
*/
    printf("%d \n", set->size);
}

/*
complement A - tiskne doplnìk množiny A.
*/
void complement(Set *set, Set *universe)
{
    /*  //TODO
    printf("complement \n");
    printf("first set :\n");
    printSet(set);
    printSet(universe);
*/
    printf("S");
    for (int i = 0; i < universe->size; i++)
    {
        bool found = false;
        for (int j = 0; j < set->size; j++)
        {
            if (strcmp(universe->elements[i]->values, set->elements[j]->values) == 0)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            printf(" %s", universe->elements[i]->values);
        }
    }
    printf("\n");
}

/*
union A B - tiskne sjednocení množin A a B.
*/
void setUnion(Set *set1, Set *set2, Set *universe)
{
    //TODO
    printf("setUnion \n");
    printf("first set :\n");
    printSet(set1);
    printf("second set :\n");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);


    printf("S");
    for(int i=0;i<set1->size;i++){
        bool found = false;
        for(int j=0;j<set2->size;j++){
            if(strcmp(set1->elements[i]->values, set2->elements[j]->values)==0){
                found = true;
                break;
                }
            }
            if (found == false){
                printf(" %s", set1->elements[i]->values);
        }
    }
    for(int k=0;k<set2->size;k++){
        printf(" %s", set2->elements[k]->values);
    }
    printf("\n");
}

/*
intersect A B - tiskne prùnik množin A a B.
*/
void intersect(Set *set1, Set *set2, Set *universe)
{
    (void)universe;
    /* //TODO
    printf("intersect \n");
    printf("first set :\n");
    printSet(set1);
    printf("second set :\n");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);*/

    printf("S");
    for (int i = 0; i < set1->size; i++)
    {
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->values, set2->elements[j]->values) == 0)
            {
                printf(" %s", set1->elements[i]->values);
                break;
            }
        }
    }
    printf("\n");
}

/*
minus A B - tiskne rozdíl množin A \ B.
*/
void minus(Set *set1, Set *set2, Set *universe)
{
    (void)universe;
    /*//TODO
    printf("minus \n");
    printf("first set :\n");
    printSet(set1);
    printf("second set :\n");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);
    */
    printf("S");
    for (int i = 0; i < set1->size; i++)
    {
        bool found = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->values, set2->elements[j]->values) == 0)
            {
                found = true;
                break;
            }
        }
        if (found == false)
        {
            printf(" %s", set1->elements[i]->values);
        }
    }
    printf("\n");
}

/*
subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
*/
void subseteq(Set *set1, Set *set2, Set *universe)
{
    (void)universe;
    /*//TODO
    printf("subseteq \n");
    printf("first set :\n");
    printSet(set1);
    printf("second set :\n");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);
    */

    bool equality = false;
    for (int i = 0; i < set1->size; i++)
    {
        equality = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->values, set2->elements[j]->values) == 0)
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
    if (equality == true)
    {
        printf("true \n");
    }
    else
    {
        printf("false \n");
    }
}

/*
subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
*/
void subset(Set *set1, Set *set2, Set *universe)
{
    (void)universe;
    /*  //TODO
    printf("subset \n");
    printf("first set :\n");
    printSet(set1);
    printf("second set :\n");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);
    */
    bool equality = false;
    int counter = 0;
    for (int i = 0; i < set1->size; i++)
    {
        equality = false;
        for (int j = 0; j < set2->size; j++)
        {
            if (strcmp(set1->elements[i]->values, set2->elements[j]->values) == 0)
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
    if ((equality == true) && (counter != set2->size))
    {
        printf("true \n");
    }
    else
    {
        printf("false \n");
    }
}

/*
equals A B - tiskne true nebo false, jestli jsou množiny rovny.
*/
void equals(Set *set1, Set *set2, Set *universe)
{
    (void)universe;
    /* //TODO
    printf("equals \n");
    printf("first set :\n");
    printSet(set1);
    printf("second set :\n");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);*/

    bool equality = true;
    for (int i = 0; i < set1->size; i++)
    {
        for (int j = 0; j < set2->size + 1; j++)
        {
            if (strcmp(set1->elements[i]->values, set2->elements[j]->values) == 0)
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
    if (equality == true)
    {
        printf("true \n");
    }
    else
    {
        printf("false \n");
    }
}

// --Relation functions--
/*
reflexive R - tiskne true nebo false, jestli je relace reflexivní.
*/
void reflexive(Relation *rel, Set *universe)
{
    printf("reflexive \n");
    printf("first relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
symmetric R - tiskne true nebo false, jestli je relace symetrická.
*/
void symmetric(Relation *rel, Set *universe)
{
    //TODO
    printf("symmetric \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
*/
void antisymmetric(Relation *rel, Set *universe)
{
    //TODO
    printf("antisymmetric \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
transitive R - tiskne true nebo false, jestli je relace tranzitivní.
*/
void transitive(Relation *rel, Set *universe)
{
    //TODO
    printf("transitive \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
function R - tiskne true nebo false, jestli je relace R funkcí.
*/
void function(Relation *rel, Set *universe)
{
    //TODO
    printf("function \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
domain R - tiskne definièní obor funkce R (lze aplikovat i na relace - první prvky dvojic).
*/
void domain(Relation *rel, Set *universe)
{
    //TODO
    printf("domain \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
*/
void codomain(Relation *rel, Set *universe)
{
    //TODO
    printf("codomain \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
injective R - tiskne true nebo false, jestli je funkce R injektivní.
*/
void injective(Relation *rel, Set *set1, Set *set2, Set *universe)
{
    //TODO
    printf("injective \n");
    printf("relation :\n");
    printRelation(rel);
    printf("first set");
    printSet(set1);
    printf("second set");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);
}

/*
surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
*/
void surjective(Relation *rel, Set *set1, Set *set2, Set *universe)
{
    //TODO
    printf("surjective \n");
    printf("relation :\n");
    printRelation(rel);
    printf("first set");
    printSet(set1);
    printf("second set");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);
}

/*
bijective R - tiskne true nebo false, jestli je funkce R bijektivní.
*/
void bijective(Relation *rel, Set *set1, Set *set2, Set *universe)
{
    //TODO
    printf("bijective \n");
    printf("relation :\n");
    printRelation(rel);
    printf("first set");
    printSet(set1);
    printf("second set");
    printSet(set2);
    printf("universe:\n");
    printSet(universe);
}

//Advanced commmands

/*
closure_ref R - tiskne reflexivní uzávìr relace R
*/
void closureRef(Relation *rel, Set *universe)
{
    //TODO
    printf("closureRef \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
closure_sym R - tiskne symetrický uzávìr relace R
*/
void closureSym(Relation *rel, Set *universe)
{
    //TODO
    printf("closureSym \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

/*
closure_trans R - tiskne tranzitivní uzávìr relace R
*/
void closureTrans(Relation *rel, Set *universe)
{
    //TODO
    printf("closureTrans \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

//-- Bonus --
/*
select A N - vybere náhodný prvek z množiny nebo relace A a tiskne ho. V pøípadì, že je množina A prázdná, pøeskoèí vykonávání pøíkazu na øádek N vstupního souboru. N v takovém pøípadì musí oznaèovat existující øádek ve vstupním souboru.
*/
void selectFromRelation(Relation *rel, Set *universe)
{
    //TODO
    printf("selectFromRelation \n");
    printf("relation :\n");
    printRelation(rel);
    printf("universe:\n");
    printSet(universe);
}

void selectFromSet(Set *set, Set *universe)
{
    //TODO
    printf("selectFromSet \n");
    printf("mnozina :\n");
    printSet(set);
    printf("universe:\n");
    printSet(universe);
}

// Arguments
/*
Rozšíøení všech pøíkazù, jejichž výsledkem je množina nebo relace, definuje novou množinu nebo relaci identifikovanou èíslem øádku, na kterém se nachází daná operace.
Rozšíøení všech pøíkazù, které tisknou true nebo false o další argument N. V pøípadì, že operace konèí s výsledkem false, následující øádek, který se zpracovává, bude na øádku N (nikoliv bezprostøednì následující).
*/
//-- Bonus --

//command functions
void executeFunUnSet(Command *com, Data *data)
{
    const struct
    {
        const char *name;
        void (*func)(Set *set, Set *universe);
    } functionMap[] = {
        {"empty", empty},
        {"card", card},
        {"complement", complement},
        //Bonus
        {"selectFromSet", selectFromSet}

    };

    for (long unsigned int i = 0; i < (sizeof(functionMap) / sizeof(functionMap[0])); i++)
    {
        if (!strcmp(functionMap[i].name, com->functionName) && functionMap[i].func)
        {
            functionMap[i].func(com->setA, data->rows[0].set);
        }
    }
}

void executeFunBinSet(Command *com, Data *data)
{
    const struct
    {
        const char *name;
        void (*func)(Set *set1, Set *set2, Set *universe);
    } functionMap[] = {
        {"setUnion", setUnion},
        {"intersect", intersect},
        {"minus", minus},
        {"subseteq", subseteq},
        {"subset", subset},
        {"equals", equals}

    };
    bool found = false;

    for (long unsigned int i = 0; i < (sizeof(functionMap) / sizeof(functionMap[0])); i++)
    {
        if (!strcmp(functionMap[i].name, com->functionName) && functionMap[i].func && com->setA != NULL && com->setB != NULL)
        {
            found = true;
            functionMap[i].func(com->setA, com->setB, data->rows[0].set);
            break;
        }
    }
    if (!found)
    {
        fprintf(stderr, "function not found");
        exit(1);
    }
}
void executeFunUnRel(Command *com, Data *data)
{

    const struct
    {
        const char *name;
        void (*func)(Relation *relation, Set *universe);
    } functionMap[] = {
        {"reflexive", reflexive},
        {"symmetric", symmetric},
        {"antisymmetric", antisymmetric},
        {"transitive", transitive},
        {"function", function},
        {"domain", domain},
        {"codomain", codomain},
        //Advanced
        {"closureRef", closureRef},
        {"closureSym", closureSym},
        {"closureTrans", closureTrans},
        //Bonus
        {"selectFromRelation", selectFromRelation}};

    for (long unsigned int i = 0; i < (sizeof(functionMap) / sizeof(functionMap[0])); i++)
    {
        if (!strcmp(functionMap[i].name, com->functionName) && functionMap[i].func)
        {
            functionMap[i].func(com->rel, data->rows[0].set);
        }
    }
}

void executeFunTriRel(Command *com, Data *data)
{
    const struct
    {
        const char *name;
        void (*func)(Relation *rel, Set *set1, Set *set2, Set *universe);
    } functionMap[] = {
        {"injective", injective},
        {"surjective", surjective},
        {"bijective", bijective},

    };

    for (long unsigned int i = 0; i < (sizeof(functionMap) / sizeof(functionMap[0])); i++)
    {
        if (!strcmp(functionMap[i].name, com->functionName) && functionMap[i].func)
        {
            functionMap[i].func(com->rel, com->setA, com->setB, data->rows[0].set);
        }
    }
}

//Command functions
void functionLookup(Command *com, Data *data)
{
    if (com->setA != NULL)
    {
        if (com->setB != NULL)
        {
            if (com->rel != NULL)
            {
                executeFunTriRel(com, data);
            }
            else
            {
                executeFunBinSet(com, data);
            }
        }
        else
        {
            executeFunUnSet(com, data);
        }
    }
    else if (com->rel != NULL)
    {
        executeFunUnRel(com, data);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalidni pocet argumentu. \n");
        return 1;
    }

    char *fileName = argv[1];

    //Test if project is running
    Data *data = initData();

    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        fprintf(stderr, "file did not open \n");
        return 1;
    }

    loadFileData(file, data);

    //  printData(data);

    for (int i = 0; i < data->size; i++)
    {
        Row *row = &data->rows[i];
        if (row->set != NULL && row->command)
        {
            Command *command = parseSetToCommand(row->set, data);
            // printCommand(command);
            functionLookup(command, data);
            free(command);
        }
    }

    freeData(data);
    fclose(file);

    return 0;
}
