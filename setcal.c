/*
 @authors:  Kolarik Cestm�r, Cermak  Matous, Balusek Pavel, Hnat Filip
 @name: setcal.c
*/

//Basic libraries.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROWS 1000
#define MAX_ELEMENT_SIZE 30

typedef struct{
    Element *elementA;
    Element *elementB;
   char a 
} Pair;

typedef struct{
    char value[MAX_ELEMENT_SIZE];
} Element;

typedef struct{
//malloced array element
    Element *elements;
        int row;
} Universe;

typedef struct{
//malloced array element
    Element *elements;
    int row;
} Set;

typedef struct{
//malloced array element
    Pair *elements;
        int row;
} Relation;

typedef struct{
//malloced array element
    Element *elements;
} Command;

// --Util functions--

// --File processing--

void loadFile(){

}

Set* searchOnRowSet(Set **setArray, int searchrow){
    for(int i){
        if( setarray[i]->row == searchrow){
            
            return &setarray[i];
        }
    }

}


Relation* searchOnRowRelation(Relation *set, int row){


}

void parseRow(char *string, int row, Universe* univ, Relation* rel, Command* com){
    int index = 0;
    int size = 0;
    char currentChar = string[index];
    while(currentChar != '\n'){
        currentChar = string[index];
        index ++;

        if(currentChar == ' '){
            size = index - size;  

            Element *element = malloc(sizeof(Element));

            char element[size];

            stringcmp(index - size, index, &string, &element);

            parseElementToUniverse();

             parseElementToRelation();

             parseElementToSet();
        }
    }
}

// --Print functions--
void printSet(Set *set){

}

void printUniverse(Universe *universe){

}

void printRelation(Relation*relation){

}

// --Set functions--

/*
empty A - tiskne true nebo false podle toho, jestli je mno�ina definovan� na ��dku A pr�zdn� nebo nepr�zdn�.
*/

void empty(Set* universe, Set* set1, Relation *relation){
//TODO
}

/*
card A - tiskne po�et prvk� v mno�in� A (definovan� na ��dku A).
*/
void card(){
//TODO
}

/*
complement A - tiskne dopln�k mno�iny A.
*/
void complement(){
//TODO
}

/*
union A B - tiskne sjednocen� mno�in A a B.
*/
void setUnion(){
//TODO
}

/*
intersect A B - tiskne pr�nik mno�in A a B.
*/
void intersect(){
//TODO
}

/*
minus A B - tiskne rozd�l mno�in A \ B.
*/
void minus(){
//TODO
}

/*
subseteq A B - tiskne true nebo false podle toho, jestli je mno�ina A podmno�inou mno�iny B.
*/
void subseteq(){
//TODO
}

/*
subset A B - tiskne true nebo false, jestli je mno�ina A vlastn� podmno�ina mno�iny B.
*/
void subset(){
//TODO
}

/*
equals A B - tiskne true nebo false, jestli jsou mno�iny rovny.
*/
void equals(){
//TODO
}

// --Relation functions--
/*
reflexive R - tiskne true nebo false, jestli je relace reflexivn�.
*/
void reflexive(){
//TODO
}

/*
symmetric R - tiskne true nebo false, jestli je relace symetrick�.
*/
void symmetric(){
//TODO
}

/*
antisymmetric R - tiskne true nebo false, jestli je relace antisymetrick�.
*/
void antisymmetric(){
//TODO
}

/*
transitive R - tiskne true nebo false, jestli je relace tranzitivn�.
*/
void transitive(){
//TODO
}

/*
function R - tiskne true nebo false, jestli je relace R funkc�.
*/
void function(){
//TODO
}

/*
domain R - tiskne defini�n� obor funkce R (lze aplikovat i na relace - prvn� prvky dvojic).
*/
void domain(){
//TODO
}

/*
codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druh� prvky dvojic).
*/
void codomain(){
//TODO
}

/*
injective R - tiskne true nebo false, jestli je funkce R injektivn�.
*/
void injective(){
//TODO
}

/*
surjective R - tiskne true nebo false, jestli je funkce R surjektivn�.
*/
void surjective(){
//TODO
}

/*
bijective R - tiskne true nebo false, jestli je funkce R bijektivn�.
*/
void bijective(){
//TODO
}

//Advanced commmands

/*
closure_ref R - tiskne reflexivn� uz�v�r relace R
*/
void closureRef(){
//TODO
}

/*
closure_sym R - tiskne symetrick� uz�v�r relace R
*/
void closureSym(){
//TODO
}

/*
closure_trans R - tiskne tranzitivn� uz�v�r relace R
*/
void closureTrans(){
//TODO
}

/*
select A N - vybere n�hodn� prvek z mno�iny nebo relace A a tiskne ho. V p��pad�, �e je mno�ina A pr�zdn�, p�esko�� vykon�v�n� p��kazu na ��dek N vstupn�ho souboru. N v takov�m p��pad� mus� ozna�ovat existuj�c� ��dek ve vstupn�m souboru.
*/
void selectFromRelation(){
//TODO
}

void selectFromSet(){
//TODO
}

// Arguments
/*
Roz���en� v�ech p��kaz�, jejich� v�sledkem je mno�ina nebo relace, definuje novou mno�inu nebo relaci identifikovanou ��slem ��dku, na kter�m se nach�z� dan� operace.
Roz���en� v�ech p��kaz�, kter� tisknou true nebo false o dal�� argument N. V p��pad�, �e operace kon�� s v�sledkem false, n�sleduj�c� ��dek, kter� se zpracov�v�, bude na ��dku N (nikoliv bezprost�edn� n�sleduj�c�).
*/

int main(int argc, char const *argv[])
{
    ///Test if project is running
    printf("Hello project");

    Set set = {};

    Universe univ = {};

    Relation rel = {};
    
    neco = randomArray;

    Set universe = {neco, neco, neco}

    Set set2 = randomArray;

    neco = (Relation) randomArray;
    Relation rel = {neco, neco};

    empty(univese, set2, rel);

    char input[] = "S a b c x \n";

    //pavel - teammate 3 = load, parse, create structs, check elements in universe
    //matous - teammate 4 = enable command 

//ostatni
    //teammate 1 = set functions
    //teammate 2 = relation functions

    //matous - teammate 4 = error handling, comments, print

    //file -> load -> parse -> enable command -> create structs -> functions -> print

    //char const* const fileName = argv[1]; /* should check that argc > 1 */
    //FILE* file = fopen(fileName, "r"); /* should check the result */
    //char line[256];

   // while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
     //   printf("%s", line);
    //}
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

   // fclose(file);

    return 0;
}
