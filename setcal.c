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

typedef struct{
    int rowIndex;
    int typeIndex;
    char value[MAX_ELEMENT_LENGTH];
} Element;

typedef struct{
    int size;
    Element* element;
} Set;

typedef struct{
    Element* valueA;
    Element* valueB;
} Relation;

typedef struct{
    Relation* relationA;
    Relation* relationB;
} Pair;

typedef struct{
    char functionName[MAX_ELEMENT_LENGTH];
    int rowIndexA;
    int rowIndexB;
} Command;

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
    //Test if project is running
    printf("Hello project \n");

    Element *rows[MAX_ROWS];

    char* fileName = argv[1];

    FILE* file = fopen(fileName, "r");

    char c;

    for(int i = 0; i < MAX_ROWS && (c = getc(file)) != EOF; i++){
         printf("%c", c);
    }

    fclose(file);

    return 0;
}
