#include "../headers/arena.h"
#include "stdlib.h"
#include "stdio.h"
#include "raylib.h"
#include "../headers/textures.h"

const int tileSetWidth = 8;
const int tileSetHeight = 8;

void csvToArray(int array[][LEVEL_WIDTH], const char *filename){

    FILE *file = fopen(filename, "r");
    if(!file){
        printf("Failed to open %s\n", filename);
        CloseWindow();
        return;
    }

    for(int y = 0; y < LEVEL_HEIGHT; y++){
        for(int x = 0; x < LEVEL_WIDTH; x++){
            fscanf(file, "%d,", &array[y][x]);
        }
    }

    fclose(file);
}

void drawLevel(int array[][LEVEL_WIDTH]){
    for(int y = 0; y < LEVEL_HEIGHT; y++){
        for(int x = 0; x < LEVEL_WIDTH; x++){
            if(array[y][x] != -1){
                Rectangle sourceRec = {(array[y][x] % tileSetWidth) * 16, (array[y][x]/tileSetHeight) * 16, 16, 16};
                Rectangle destRec = {TILE_SIZE*x, TILE_SIZE*y, TILE_SIZE, TILE_SIZE};
                DrawTexturePro(levelTilesetTexture, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);
            }

            
        }
    }
}
