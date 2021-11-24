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
    int rowIndexA;
    int rowIndexB;
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

void addSetElement(Set *set, Element *element)
{

    if (element->lenght < MAX_ELEMENT_LENGTH)
    {
        element->values[element->lenght + 1] = '\0';
    }

    if (set->size > 0 && set->size % DEFAULT_ELEMENT_ARRAY_LENGHT)
    {
        set->elements = (Element **)realloc(set->elements, set->size + sizeof(Element *) * DEFAULT_ELEMENT_ARRAY_LENGHT);
    }

    set->elements[set->size] = element;
    set->size++;

    set->elements[set->size] = initElement();
}

Pair *addRelationPair(Relation *relation)
{
    return NULL;
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
    return NULL;
}

Data *initData()
{
    Data *data = malloc(sizeof(Data));
    data->size = 0;
    return data;
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

//Returns if set has ended
bool parseToSet(Set *set, char c)
{
    Element *element = set->elements[set->size];

    if (c == ' ' || c == '\n')
    {
        if (element != NULL)
        {
            addSetElement(set, element);
        }

        if (c == '\n')
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

Command *parseSetToCommand(Set *set)
{
    Command *command = malloc(sizeof(Command));
    if (set->size > 0)
    {
        strcpy(&command->functionName, &set->elements[0]->values);

        if (set->size > 1)
        {
            command->rowIndexA = atoi(&set->elements[1]->values);
        }
        else
        {
            command->rowIndexA = 0;
        }

        if (set->size > 2)
        {
            command->rowIndexB = atoi(&set->elements[2]->values);
        }
        else
        {
            command->rowIndexA = 0;
        }
    }

    return command;
}

bool parseToRelation(Relation *set, char c)
{
    /*}else if(relation != NULL){
                if(c == '('){
                    printf("here8");
                    if(relation->size == 0){
                        relation->pairs = (Pair*) malloc(sizeof(Pair) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    }else if(relation->size % DEFAULT_ELEMENT_ARRAY_LENGHT){
                        relation->pairs = (Pair*) realloc(relation->pairs, relation->size + sizeof(Pair*) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    }

                    relation->pairs[relation->size] = (Pair**) malloc(sizeof(Pair*) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    pair = &relation->pairs[relation->size];

                    element = initElement();

                    relation->size++;
                } else if(c == ')'){
                    pair->elementB = element;
                }*/
    return false;
}

// --Data processing--

void activateCommand(Command *command, Data *data)
{
    Row *row1 = NULL;
    Row *row2 = NULL;
    if (command->rowIndexA != 0)
    {
        row1 = &data->rows[command->rowIndexA];
    }

    if (command->rowIndexB != 0)
    {
        row2 = &data->rows[command->rowIndexB];
    }
    //control print
    /* printf("Prikaz");
    printCommand(command);
    printf("plati pro mnozinu: \n");

    if(row1 != NULL && row1->set != NULL && !row1->command){
        printSet(row1->set);
    }

    if(row2 != NULL && row2->set != NULL && !row2->command){
        printf(" a zaroven \n");
        printSet(row2->set);
       
    }*/
}

void loadFileData(FILE *file, Data *data)
{
    char c;
    Relation *relation = NULL;
    Command *command = NULL;
    Set *set = NULL;
    Row *row = NULL;
    Pair *pair = NULL;

    bool first = true;

    while ((c = getc(file)) != EOF && data->size <= MAX_ROWS)
    {

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

        // printf("%c \n", c);

        if (relation != NULL)
        {
            first = parseToRelation(relation, c);
        }
        else
        {
            first = parseToSet(set, c);
        }

        if (first)
        {
            data->size++;
        }
    }
}

// --Print functions--
void printSet(Set *set)
{
    for (int x = 0; x < set->size; x++)
    {
        printf(" %s, ", set->elements[x]->values);
    }

    printf("\n");
}

void printCommand(Command *command)
{
    printf("Prikaz %s, na radce %d a %d \n", command->functionName, command->rowIndexA, command->rowIndexB);
}

void printData(Data *data)
{
    for (int i = 0; i <= data->size; i++)
    {
        Row row = data->rows[i];

        if (row.relation != NULL)
        {
            printf("Na radku %d je relation: ", i);
            printRelation(row.relation);
        }
        else if (row.set != NULL)
        {
            if (row.command)
            {
                Command *command = parseSetToCommand(row.set);
                //printf("Na radku je %d je prikaz: ", i);
                //printCommand(command);
            }
            else
            {
                //printf("Na radku je %d je mnozina: ", i);
                //printSet(row.set);
            }
        }
    }
}

void printUniverse(Set *universe)
{
}

void printRelation(Relation *relation)
{
    for (int x = 0; x < relation->size; x++)
    {
        Pair *pair = relation->pairs[x];

        printf(" Hodnota A %s, hodnota B %s, cela relace je (%s %s) ",
               pair->elementA->values,
               pair->elementB->values,
               pair->elementA->values,
               pair->elementB->values);
    }
    printf("\n");
}
//Command lookup tables
 
// --Set functions--

/*
empty A - tiskne true nebo false podle toho, jestli je množina definovaná na øádku A prázdná nebo neprázdná.
*/

void empty(Set *set)
{
    //TODO
    printf("empty");
}

/*
card A - tiskne poèet prvkù v množinì A (definované na øádku A).
*/
void card()
{
    //TODO
}

/*
complement A - tiskne doplnìk množiny A.
*/
void complement()
{
    //TODO
}

/*
union A B - tiskne sjednocení množin A a B.
*/
void setUnion()
{
    //TODO
}

/*
intersect A B - tiskne prùnik množin A a B.
*/
void intersect()
{
    //TODO
}

/*
minus A B - tiskne rozdíl množin A \ B.
*/
void minus()
{
    //TODO
}

/*
subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
*/
void subseteq()
{
    //TODO
}

/*
subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
*/
void subset()
{
    //TODO
}

/*
equals A B - tiskne true nebo false, jestli jsou množiny rovny.
*/
void equals()
{
    //TODO
}

// --Relation functions--
/*
reflexive R - tiskne true nebo false, jestli je relace reflexivní.
*/
void reflexive()
{
    //TODO
}

/*
symmetric R - tiskne true nebo false, jestli je relace symetrická.
*/
void symmetric()
{
    //TODO
}

/*
antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
*/
void antisymmetric()
{
    //TODO
}

/*
transitive R - tiskne true nebo false, jestli je relace tranzitivní.
*/
void transitive()
{
    //TODO
}

/*
function R - tiskne true nebo false, jestli je relace R funkcí.
*/
void function()
{
    //TODO
}

/*
domain R - tiskne definièní obor funkce R (lze aplikovat i na relace - první prvky dvojic).
*/
void domain()
{
    //TODO
}

/*
codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
*/
void codomain()
{
    //TODO
}

/*
injective R - tiskne true nebo false, jestli je funkce R injektivní.
*/
void injective()
{
    //TODO
}

/*
surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
*/
void surjective()
{
    //TODO
}

/*
bijective R - tiskne true nebo false, jestli je funkce R bijektivní.
*/
void bijective()
{
    //TODO
}

//Advanced commmands

/*
closure_ref R - tiskne reflexivní uzávìr relace R
*/
void closureRef()
{
    //TODO
}

/*
closure_sym R - tiskne symetrický uzávìr relace R
*/
void closureSym()
{
    //TODO
}

/*
closure_trans R - tiskne tranzitivní uzávìr relace R
*/
void closureTrans()
{
    //TODO
}

/*
select A N - vybere náhodný prvek z množiny nebo relace A a tiskne ho. V pøípadì, že je množina A prázdná, pøeskoèí vykonávání pøíkazu na øádek N vstupního souboru. N v takovém pøípadì musí oznaèovat existující øádek ve vstupním souboru.
*/
void selectFromRelation()
{
    //TODO
}

void selectFromSet()
{
    //TODO
}
//command functions 

void execute_fun_un_set(Command *com, Data *data)
{
   const static struct
    {
        const char *name;
        void (*func)(Set *set);
    } function_map_un_set[] = {
        {"empty", empty},
        {"card", card},
        {"complement", complement}

    };

     for ( int i = 0; i < (sizeof(function_map_un_set) / sizeof(function_map_un_set[0])); i++)
    {
        if (!strcmp(function_map_un_set[i].name,com->functionName) && function_map_un_set[i].func)
        {
             function_map_un_set[i].func(data->rows[com->rowIndexA].set);
              
        }
    }
}
//Command functions

void function_lookup(Command *com, Data *data)
{

    if (data->rows[com->rowIndexA].set != NULL)
    {
        if (data->rows[com->rowIndexB].set != NULL && data->rows[com->rowIndexB].relation == NULL)
        {
            //execute_fun_bin_set(com,data);
        }
        else
        {
            execute_fun_un_set(com, data);
        }
    }
    
}

// Arguments
/*
Rozšíøení všech pøíkazù, jejichž výsledkem je množina nebo relace, definuje novou množinu nebo relaci identifikovanou èíslem øádku, na kterém se nachází daná operace.
Rozšíøení všech pøíkazù, které tisknou true nebo false o další argument N. V pøípadì, že operace konèí s výsledkem false, následující øádek, který se zpracovává, bude na øádku N (nikoliv bezprostøednì následující).
*/

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

    printData(data);

    for (int i = 0; i < data->size; i++)
    {
        Row *row = &data->rows[i];
        if (row->set != NULL && row->command)
        {
            Command *command = parseSetToCommand(row->set);
            activateCommand(command, data);

            //command handeling
        }
    }

    freeData(data);
    fclose(file);

    return 0;
}
