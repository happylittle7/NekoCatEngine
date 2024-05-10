#include "ReadCharacter.h"

void ReadCharacter(){
    FILE *fp;
    char errbuf[200];

    
    fp = fopen("character.nekocat", "r");
    if (!fp) {
        fatal("cannot open character.nekocat - ", strerror(errno));
    }
}