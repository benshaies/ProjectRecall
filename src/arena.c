#include "../headers/arena.h"
#include "stdlib.h"
#include "stdio.h"
#include "raylib.h"
#include "../headers/textures.h"

#define WALL_TILES 20

const int tileSetWidth = 8;
const int tileSetHeight = 8;

int wallTiles[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, 20, 21, 22, 23};

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

void drawFloor(int array[][FLOOR_WIDTH]){
    for(int y = 0; y < FlOOR_HEIGHT; y++){
        for(int x = 0; x < FLOOR_WIDTH; x++){
            if(array[y][x] != -1){
                Rectangle sourceRec = {(array[y][x] % tileSetWidth) * 16, (array[y][x]/tileSetHeight) * 16, 16, 16};
                Rectangle destRec = {TILE_SIZE*x, TILE_SIZE*y, TILE_SIZE, TILE_SIZE};
                DrawTexturePro(levelTilesetTexture, sourceRec, destRec, (Vector2){0,0}, 0.0f, WHITE);
            }

            
        }
    }
}

bool isWallTile(int tileID){
    bool returnBool = false;
    for(int i = 0; i < WALL_TILES; i++){
        if(tileID == wallTiles[i]){
            returnBool = true;
            break;
        }
        
    }
    return returnBool;
}

int getWallAmount(int array[][LEVEL_WIDTH]){
    int count = 0;
    for(int i = 0; i < LEVEL_HEIGHT; i++){
        for(int j = 0; j < LEVEL_WIDTH; j++){
            if(isWallTile(array[i][j])){
                count++;
            
            }
        }
    }

    return count;
}

void createCollisionRecs(int array[][LEVEL_WIDTH], Rectangle rec[]){
    int i = 0;
    for(int y = 0; y < LEVEL_HEIGHT; y++){
        for(int x = 0; x < LEVEL_WIDTH; x++){

            if(isWallTile(array[y][x])){
                rec[i] = (Rectangle){x*TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                i++;
            }
            
        }
    }
}



