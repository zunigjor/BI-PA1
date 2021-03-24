#include <stdio.h>

int main(void) {
    int input;
    printf ("ml' nob:\n") ;
    if (scanf ("%d", &input) != 1 ){
        printf("luj\n");
        return 0;
    }
    if( input == 1 ){
        printf("Qapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
    }
    else if (input == 2) {
        printf ("Qapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
    }
    else if (input==3){
        printf ("Qapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
    }
    else if (input==4){
        printf ("Qapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
    }
    else if (input==5){
        printf ("Qapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
    }
    else {
        printf("luj\n");
    }
    return 0;
}