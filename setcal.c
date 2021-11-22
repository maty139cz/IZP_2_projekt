/*
 @authors:  Kolarik Cestmír, Cermak  Matous, Balusek Pavel, Hnat Filip
 @name: setcal.c
*/

//Basic libraries.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROWS 1000
#define MAX_ELEMENT_LENGTH 30
#define DEFAULT_ELEMENT_ARRAY_LENGHT 10

typedef struct{
    int lenght;
    char values[MAX_ELEMENT_LENGTH + 1];
} Element;

typedef struct{
    int size;
    Element** elements;
} Set;

typedef struct{
    Element* elementA;
    Element* elementB;
} Pair;

typedef struct{
    int size;
    Pair** pairs;
} Relation;

typedef struct{
    char functionName[MAX_ELEMENT_LENGTH];
    int rowIndexA;
    int rowIndexB;
} Command;

typedef struct{
    Set* set;
    Relation* relation;
    bool command;
} Row;

typedef struct{
    int size;
    Row rows[MAX_ROWS];
} Data;

// --Util functions--

// --File processing--

void loadFile(){

}

// --Print functions--
void printSet(Set *set){

}

void printUniverse(Set *universe){

}

void printRelation(Relation *relation){

}

// --Set functions--

/*
empty A - tiskne true nebo false podle toho, jestli je množina definovaná na øádku A prázdná nebo neprázdná.
*/

void empty(){
//TODO
}

/*
card A - tiskne poèet prvkù v množinì A (definované na øádku A).
*/
void card(){
//TODO
}

/*
complement A - tiskne doplnìk množiny A.
*/
void complement(){
//TODO
}

/*
union A B - tiskne sjednocení množin A a B.
*/
void setUnion(){
//TODO
}

/*
intersect A B - tiskne prùnik množin A a B.
*/
void intersect(){
//TODO
}

/*
minus A B - tiskne rozdíl množin A \ B.
*/
void minus(){
//TODO
}

/*
subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B.
*/
void subseteq(){
//TODO
}

/*
subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B.
*/
void subset(){
//TODO
}

/*
equals A B - tiskne true nebo false, jestli jsou množiny rovny.
*/
void equals(){
//TODO
}

// --Relation functions--
/*
reflexive R - tiskne true nebo false, jestli je relace reflexivní.
*/
void reflexive(){
//TODO
}

/*
symmetric R - tiskne true nebo false, jestli je relace symetrická.
*/
void symmetric(){
//TODO
}

/*
antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická.
*/
void antisymmetric(){
//TODO
}

/*
transitive R - tiskne true nebo false, jestli je relace tranzitivní.
*/
void transitive(){
//TODO
}

/*
function R - tiskne true nebo false, jestli je relace R funkcí.
*/
void function(){
//TODO
}

/*
domain R - tiskne definièní obor funkce R (lze aplikovat i na relace - první prvky dvojic).
*/
void domain(){
//TODO
}

/*
codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic).
*/
void codomain(){
//TODO
}

/*
injective R - tiskne true nebo false, jestli je funkce R injektivní.
*/
void injective(){
//TODO
}

/*
surjective R - tiskne true nebo false, jestli je funkce R surjektivní.
*/
void surjective(){
//TODO
}

/*
bijective R - tiskne true nebo false, jestli je funkce R bijektivní.
*/
void bijective(){
//TODO
}

//Advanced commmands

/*
closure_ref R - tiskne reflexivní uzávìr relace R
*/
void closureRef(){
//TODO
}

/*
closure_sym R - tiskne symetrický uzávìr relace R
*/
void closureSym(){
//TODO
}

/*
closure_trans R - tiskne tranzitivní uzávìr relace R
*/
void closureTrans(){
//TODO
}

/*
select A N - vybere náhodný prvek z množiny nebo relace A a tiskne ho. V pøípadì, že je množina A prázdná, pøeskoèí vykonávání pøíkazu na øádek N vstupního souboru. N v takovém pøípadì musí oznaèovat existující øádek ve vstupním souboru.
*/
void selectFromRelation(){
//TODO
}

void selectFromSet(){
//TODO
}

// Arguments
/*
Rozšíøení všech pøíkazù, jejichž výsledkem je množina nebo relace, definuje novou množinu nebo relaci identifikovanou èíslem øádku, na kterém se nachází daná operace.
Rozšíøení všech pøíkazù, které tisknou true nebo false o další argument N. V pøípadì, že operace konèí s výsledkem false, následující øádek, který se zpracovává, bude na øádku N (nikoliv bezprostøednì následující).
*/

int main(int argc, char **argv[])
{
    if(argc != 2){
        return 1;
    }

    //Test if project is running
    Data* data = malloc(sizeof(Data));
    data->size = 0;

    char* fileName = argv[1];

    FILE* file = fopen(fileName, "r");

    char c;

    Relation* relation = NULL;
    Command* command = NULL;
    Set* set = NULL;
    Row* row;

    Element* element = malloc(sizeof(Element));
    element->values[MAX_ELEMENT_LENGTH] = '\0';

    Pair* pair = NULL;

    int rowIndex = 0;
    bool isValid = true;

    while((c = getc(file)) != EOF && data->size <= MAX_ROWS){

        if(rowIndex == 0){
            row = &data->rows[data->size];

            if(c == 'U' || c == 'S'){
                if(c == 'C'){
                    row->command = true;
                }
                row->relation = NULL;
                row->set = malloc(sizeof(Set));

                relation = NULL;
                set = row->set;
            }else if(c == 'R'){
                row->relation = malloc(sizeof(Relation));
                row->set = NULL;

                set = NULL;
                relation = row->relation;
            }else{
                isValid = false;
            }
        }

        if(c == '\n'){
            data->size ++;
            rowIndex = 0;
            isValid = true;
        }else if(isValid){
            if (c == ' '){
                if (set == NULL) {
                    pair->elementA = element;
                    element = malloc(sizeof(Element));
                    element->values[MAX_ELEMENT_LENGTH] = '\0';
                }else{
                    if(set->size == 0){
                        set->elements = (Element**) malloc(sizeof(Element*) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    }
                    if(set->size % DEFAULT_ELEMENT_ARRAY_LENGHT){
                        set->elements = (Element**) realloc(set->elements, set->size + sizeof(Element*) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    }

                    set->elements[set->size] = element;
                    set->size++;
                }

                element = malloc(sizeof(Element));
                element->values[MAX_ELEMENT_LENGTH] = '\0';

                rowIndex++;
            }else if(relation != NULL){
                if(c == '('){
                    if(relation->size == 0){
                        relation->pairs = (Pair*) malloc(sizeof(Pair) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    }else if(relation->size % DEFAULT_ELEMENT_ARRAY_LENGHT){
                        relation->pairs = (Pair*) realloc(relation->pairs, relation->size + sizeof(Pair*) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    }

                    relation->pairs[relation->size] = (Pair**) malloc(sizeof(Pair*) * DEFAULT_ELEMENT_ARRAY_LENGHT);
                    pair = &relation->pairs[relation->size];

                    element = malloc(sizeof(Element));
                    element->values[MAX_ELEMENT_LENGTH] = '\0';

                    relation->size++;
                } else if(c == ')'){
                    pair->elementB = element;
                }
            }else{
                element->values[element->lenght] = c;
                element->lenght++;
            }
        }
    }

    for(int i = 0; i <= data->size; i++){
        Row* row = &data->rows[i];

        if(row->relation != NULL){
            printf("Na radku je %d: ", i);

            for(int x = 0; x < row->relation->size; x++){
                Pair* pair = row->relation->pairs[x];

                printf(" Hodnota A s%, hodnota B, cela relace je (%s %s) ",
                       pair->elementA->values,
                       pair->elementB->values,
                       pair->elementA->values,
                       pair->elementB->values);
            }
            printf("\n");
        }else if(command)
        {
            printf("Na radku je %d je prikaz: ", i);
            for(int x = 0; x < row->set->size; x++){
                printf("Hodnota %s ", row->set->elements[x]->values);
            }

            printf("\n");
        }
        else{
            printf("Na radku je %d je mnozina: ", i);
            for(int x = 0; x < row->set->size; x++){
                printf("Hodnota %s ", row->set->elements[x]->values);
            }

            printf("\n");
        }
    }

    fclose(file);

    return 0;
}
