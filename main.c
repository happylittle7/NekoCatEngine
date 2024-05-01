#include "Library/toml.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fatal(const char *msg, const char *msg1) {
  fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
  exit(1);
}

int main() {
  FILE *fp;
  char errbuf[200];


  // 1. Read and parse toml file
  fp = fopen("character.nekocat", "r");
  if (!fp) {
    fatal("cannot open character.nekocat - ", strerror(errno));
  }

  //Open character table and save in character
  toml_table_t *character = toml_parse_file(fp, errbuf, sizeof(errbuf));
  fclose(fp);

  if (!character) {
    fatal("cannot parse - ", errbuf);
  }

  // 2. Print how many characters are in the file
  toml_array_t *charList = toml_array_in(character, "char_list");
  
  //charSize: the number of how many characters in the file
  int charSize = toml_array_nelem(charList);

  printf("Number of characters(charSize): %d\n", charSize);

  // 3. Print all characters idntifiers
  printf("Character in this file: ");
  for(int i=0;i<charSize;i++){
    printf("%s, ",toml_string_at(charList, i).u.s);
  }
  printf("\n\n");

  // 4. Traverse to a table.
  for(int i=0;i<charSize;i++){
    toml_table_t *now = toml_table_in(character, toml_string_at(charList, i).u.s);
    toml_array_t *nowSpriteList = toml_array_in(now,"sprites_list");
    int spriteSize=toml_array_nelem(nowSpriteList);

    printf("Character name : %s\n", toml_string_in(now, "name").u.s);
    printf("Character color : %s\n", toml_string_in(now, "color").u.s);
    printf("The number of sprites this character have: %d\n",spriteSize);
    
    for(int j=0;j<spriteSize;j++){
      printf("%s: \n",toml_string_at(nowSpriteList,j).u.s);
      toml_table_t *nowSprite = toml_table_in(now,"sprites");
      printf("  Filename: %s\n",toml_string_in(nowSprite,toml_string_at(nowSpriteList,j).u.s).u.s);
      //toml_table_t *nowSprite = toml_table_in(now,toml_string_at(nowSpriteList,j));
    }
    
    //printf("%s\n", toml_string_in(nowSprite,"sad").u.s);
    printf("\n");
  }
  

  // 3. Extract values

  
  

  // 4. Free memory

  toml_free(character);
  return 0;
}