#include "raylib.h"
#include <string.h>
#include <raymath.h>
#include "lauraybulls.h"
#include <stdio.h>

int prevTile[7][100] = {};
int playerTile[7][100] = {};
int checkTile[6][7][100] = {};
int match[6] = {};
int pastHold = 0;
int hold = 0;
int holdCount = 0;
int mixTile(int tile1, int tile2);
int mixCount = 0;
int gameOverFlag = 0;
void updatePlayfield(int x, int y, int color);
void floodFill(int x, int y, int checkedColor);
void deleteFill(int x, int y, int checkedColor);
Color type[7] = {{183, 0, 0, 255}, {127, 127, 0, 255}, {49, 49, 255, 255}, {234, 117, 0, 255}, {0, 172, 86, 255}, {255, 43, 255, 255}, {0, 0, 0, 0}};
int arr = 5;
int das = 10;

void swap(int arr[], int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void shuffle(int arr[], int n){
    for(int i = n-1; i>0; i--){
        int j = GetRandomValue(0, i);
        swap(arr, i, j);
    }
}

int readHighScore() {
    int highScore = 0;
    FILE *file = fopen("score.dat", "rb");

    if (file != NULL) {
        fread(&highScore, sizeof(highScore), 1, file);
        fclose(file);
    }

    return highScore;
}

void saveHighScore(int highScore) {
    FILE *file = fopen("score.dat", "wb");

    if (file != NULL) {
        fwrite(&highScore, sizeof(highScore), 1, file);
        fclose(file);
    }
}




int game(Texture2D laurency, Texture2D bluelevelBG, Music gameBGM, Sound seBomb, Sound seBombReady, Sound seLevelUp, Sound seMove, Sound seHold, Sound seDrop){
    int wildTile = 0;
    const int screenWidth = 640;
    const int screenHeight = 480;
    float countCurtain = 1.0f;
    float count = 0.0f;
    float count2 = 0.0f;
    float count3[7][100] = {}; 
    float count4[7][100] = {}; 
    float countBomb = 0.0f;
    Color fx = type[0];
    Color fx2 = type[0];
    Color fx3[7][100] = {};
    Color fx4[7][100] = {};
    int chain = 0;
    int displayChain = 0;
    int maxChain = 0;
    float chainFlag = 0;
    int level = 1;
    int timer = 120;
    int timer2 = 0;
    int charX = 3;
    int bombX;
    int bombReadyFlag = 0;
    int bonus = 0;
    int score = 0;
    int exp = 0;
    int dasCount = das;
    float endCount = 0.0f;
    int finalFlag = 0;
    
    int highScore = readHighScore();
    SetTargetFPS(60);
    InitBullets();
    // Main game loop
    while (!WindowShouldClose()){
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))){
            ToggleFullscreen();
        }
        
        UpdateMusicStream(gameBGM);
        if (!IsMusicStreamPlaying(gameBGM)) {
            PlayMusicStream(gameBGM);
        }

        timer--;
        UpdateBullets();
        BeginDrawing();
        

        DrawTexture(bluelevelBG, 0, 0, WHITE);

        DrawRectangle(14, 4, 240, 428, ColorAlpha(BLACK, 0.5f));
        DrawRectangle(14, 4, 240, 428, ColorAlpha(BLUE, 0.5f));
        DrawRectangle(12, 2, 240, 428, BLACK);
        
        DrawRectangle(12, 2, 240-timer, 428, {32, 32, 32, 255});
        DrawRectangle(264, 0, 112, 480, ColorAlpha(BLACK, 0.8f));
        
        DrawRectangle(390, 4, 240, 428, ColorAlpha(BLACK, 0.5f));
        DrawRectangle(390, 4, 240, 428, ColorAlpha(BLUE, 0.5f));
        DrawRectangle(388, 2, 240, 428, BLACK);
        DrawTexture(laurency, 415, 189, WHITE);
        
//      <upperRibbon>
        
        
        DrawText(TextFormat("score"), 279, 9, 20, RAYWHITE);
        for(int i=0; i<=1; i++){
            DrawText(TextFormat("%i", score/100000%10), 279+0+i*2+(score/100000+8)%10/9*3, 28, 20, RAYWHITE);
            DrawText(TextFormat("%i", score/10000%10), 279+14+i*2+(score/10000+8)%10/9*3, 28, 20, RAYWHITE);
            DrawText(TextFormat("%i", score/1000%10), 279+28+i*2+(score/1000+8)%10/9*3, 28, 20, RAYWHITE);
            DrawText(TextFormat("%i", score/100%10), 279+42+i*2+(score/100+8)%10/9*3, 28, 20, RAYWHITE);
            DrawText(TextFormat("%i", score/10%10), 279+56+i*2+(score/10+8)%10/9*3, 28, 20, RAYWHITE);
            DrawText(TextFormat("%i", 0), 349+i*2, 28, 20, RAYWHITE);
        }
        
        if (score > highScore) {
            highScore = score;
        }

        DrawText(TextFormat("top"), 298, 48, 10, DARKGRAY);
        for(int i=0; i<=1; i++){
            DrawText(TextFormat("%i", highScore/100000%10), 319+0+i+(highScore/100000+8)%10/9, 48, 10, DARKGRAY);
            DrawText(TextFormat("%i", highScore/10000%10), 319+7+i+(highScore/10000+8)%10/9, 48, 10, DARKGRAY);
            DrawText(TextFormat("%i", highScore/1000%10), 319+14+i+(highScore/1000+8)%10/9, 48, 10, DARKGRAY);
            DrawText(TextFormat("%i", highScore/100%10), 319+21+i+(highScore/100+8)%10/9, 48, 10, DARKGRAY);
            DrawText(TextFormat("%i", highScore/10%10), 319+28+i+(highScore/10+8)%10/9, 48, 10, DARKGRAY);
            DrawText(TextFormat("%i", 0), 354+i, 48, 10, DARKGRAY);
        }

        DrawText(TextFormat("lv.%i", level), 283, 63, 20, RAYWHITE);
        DrawText(TextFormat("%i%%", (exp*100/76)%100), 352-MeasureText(TextFormat("%i%%", (exp*100/76)%100), 10), 91, 10, DARKGRAY);
        DrawRectangle(282, 85, 76, 4, DARKGRAY);
        DrawRectangle(282, 85, exp%76, 4, WHITE);
        
        if(1+exp/76 > level){
            PlaySound(seLevelUp);
            level++;
        }

        if(mixCount > 38) mixCount = 38;
        
        DrawText(TextFormat("RAY"), 283, 102, 20, RAYWHITE);
        DrawText(TextFormat("%i%%", mixCount*100/38), 352-MeasureText(TextFormat("%i%%", mixCount*100/38), 10), 130, 10, DARKGRAY);
        DrawRectangle(282, 124, 76, 4, DARKGRAY);
        DrawRectangle(282, 124, mixCount*2, 4, WHITE);
        
//      </upperRibbon>
        
        DrawText(TextFormat("time"), 302, 326, 20, RAYWHITE);
        DrawText(TextFormat("%i:%i%i", (timer2/3600)%10, (timer2/600)%6, (timer2/60)%10), 320-MeasureText(TextFormat("%i:%i%i", (timer2/3600)%10, (timer2/600)%6, (timer2/60)%10), 40)/2, 347, 40, RAYWHITE);
        timer2++;
        
        if(chain > maxChain) maxChain = chain;
        DrawText(TextFormat("max"), 281, 389, 20, RAYWHITE);
        DrawText(TextFormat("chain"), 281, 407, 20, RAYWHITE);
        DrawText(TextFormat("%i", maxChain), 339, 391, 40, RAYWHITE);
            
        DrawRectangle(280, 439, 32, 32, WHITE);
        DrawText(TextFormat("DAS"), 285, 442, 10, BLACK);
        DrawText(TextFormat("%i", das), 285, 451, 20, BLACK);
        DrawRectangle(328, 439, 32, 32, WHITE);
        DrawText(TextFormat("ARR"), 333, 442, 10, BLACK);
        DrawText(TextFormat("%i", arr), 333, 451, 20, BLACK);
        
        if(charX > 0 && gameOverFlag == 0){
            if(IsKeyPressed(KEY_LEFT)){
                dasCount = das;
                charX--;
                PlaySound(seMove);
            }
            else if(IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) dasCount--;
            else if(dasCount == 0){
                dasCount += arr;
                charX++;
                PlaySound(seMove);
            }
        }
        
        if(charX < 6 && gameOverFlag == 0){
            if(IsKeyPressed(KEY_RIGHT)){
                dasCount = das;
                charX++;
                PlaySound(seMove);
            }
            else if(IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) dasCount--;
            else if(dasCount == 0){
                dasCount += arr;
                charX--;
                PlaySound(seMove);
            }
        }
        
        if(gameOverFlag == 0 && IsKeyPressed(KEY_DOWN) == 1){
            score += timer*level;
            timer = 0;
        }
        
        if(gameOverFlag == 0 && IsKeyPressed(KEY_Z) == 1){
            int prevHoldCount = holdCount;
            if(hold == 0){
                for(int i=12; i>=0; i--){
                    if(playerTile[charX][i] != 0){
                        holdCount++;
                        hold = playerTile[charX][i];
                        playerTile[charX][i] = 0;
                        if(playerTile[charX][i-1] != hold){
                            break;
                        }
                    }
                }
            }
            int flag = 0;
            for(int i=12; i>=0; i--){
                if(playerTile[charX][i] != 0 && playerTile[charX][i] == hold && flag == 0){
                    holdCount++;
                    playerTile[charX][i] = 0;
                }
                if(playerTile[charX][i] != hold && playerTile[charX][i] != 0){
                    flag = 1;
                    break;
                }
            }
            if(prevHoldCount != holdCount){
                count = 1.0f;
                fx = type[hold-1];
                PlaySound(seHold);
            }
        }
        
        if(gameOverFlag == 0 && IsKeyPressed(KEY_A) == 1){
            int prevHoldCount = holdCount;
            int flag = 0;
            if(hold == 0){
                for(int i=12; i>=0; i--){
                    if(playerTile[charX][i] != 0 && flag == 0){
                        holdCount++;
                        flag++;
                        hold = playerTile[charX][i];
                        playerTile[charX][i] = 0;
                        if(playerTile[charX][i-1] != hold){
                            break;
                        }
                    }
                }
            }
            for(int i=12; i>=0; i--){
                if(playerTile[charX][i] != 0 && playerTile[charX][i] == hold && flag == 0){
                    holdCount++;
                    flag++;
                    playerTile[charX][i] = 0;
                }
                if(playerTile[charX][i] != hold && playerTile[charX][i] != 0){
                    flag++;
                    break;
                }
            }
            if(prevHoldCount != holdCount){
                count = 1.0f;
                fx = type[hold-1];
                PlaySound(seHold);
            }
        }
        
        if(gameOverFlag == 0 && IsKeyPressed(KEY_X) == 1){
            match[hold-1] = 0;
            int prevHoldCount = holdCount;
            int prevHold = hold;
            int contactColor;
            int depth = 0;
            for(int i=0; i<=13; i++){
                if(hold != 0 && playerTile[charX][i] == 0){
                    if(i != 0) contactColor = playerTile[charX][i-1];
                    for(int j=1; j<=i; j++){
                        if(playerTile[charX][i-j] == contactColor) depth++;
                        else break;
                    }
                    break;
                }
            }
            
            for(int i=0; i<100; i++){
                if(hold != 0 && playerTile[charX][i] == 0){
                    for(int j=depth-1; j>-1; j--){
                        if(holdCount != 0){
                            int prevHoldCount = playerTile[charX][i-1-j];
                            if(mixTile(prevHoldCount, hold) > 6){
                                fx3[charX][i-1-j] = type[hold-1];
                                count3[charX][i-1-j] = 1.0f;
                                prevTile[charX][i-1-j] = playerTile[charX][i-1-j];
                                playerTile[charX][i-1-j] = mixTile(playerTile[charX][i-1-j], hold)/10;
                                floodFill(charX, i-1-j, mixTile(prevHoldCount, hold)/10);
                                mixCount += 2;
                            }
                            else break;
                            holdCount -= 1;
                        }
                    }
                    for(int j=0; j<holdCount; j++){
                        playerTile[charX][i+j] = hold;
                        floodFill(charX, i+j, hold);
                    }
                    pastHold = hold;
                    hold = holdCount = 0;
                    break;
                }
            }
            if(prevHoldCount != holdCount){
                fx2 = type[prevHold-1];
                count2 = 1.0f;
                PlaySound(seDrop);
            }
        }
        
        if(gameOverFlag == 0 && IsKeyPressed(KEY_S) == 1 && holdCount != 0){
            match[hold-1] = 0;
            int flag = 0;
            int prevHoldCount = holdCount;
            int prevHold = hold;
            int contactColor;
            int depth = 0;
            for(int i=0; i<=13; i++){
                if(hold != 0 && playerTile[charX][i] == 0){
                    if(i != 0) contactColor = playerTile[charX][i-1];
                    for(int j=1; j<=i; j++){
                        if(playerTile[charX][i-j] == contactColor) depth++;
                        else break;
                    }
                    break;
                }
            }
            
            for(int i=0; i<100; i++){
                if(hold != 0 && playerTile[charX][i] == 0){
                    for(int j=depth-1; j>-1; j--){
                        if(holdCount != 0){
                            int prevHoldCount = playerTile[charX][i-1-j];
                            if(mixTile(prevHoldCount, hold) > 6 && flag == 0){
                                fx3[charX][i-1-j] = type[hold-1];
                                count3[charX][i-1-j] = 1.0f;
                                prevTile[charX][i-1-j] = playerTile[charX][i-1-j];
                                playerTile[charX][i-1-j] = mixTile(playerTile[charX][i-1-j], hold)/10;
                                floodFill(charX, i-1-j, mixTile(prevHoldCount, hold)/10);
                                mixCount += 2;
                                flag++;
                            }
                            else if(flag == 0){
                                playerTile[charX][i] = hold;
                                floodFill(charX, i, hold);
                            }
                        }
                    }
                    holdCount--;
                    pastHold = hold;
                    if(holdCount == 0){
                        hold = 0;
                    }
                    break;
                }
            }
            if(prevHoldCount != holdCount){
                fx2 = type[prevHold-1];
                count2 = 1.0f;
                PlaySound(seDrop);
            }
        }

        int flagArray[6][7][100] = {};
        int matchFlag = 0;
        for(int i=0; i<6; i++){
            for(int j=0; j<7; j++){
                for(int k=0; k<100; k++){
                    match[i] += checkTile[i][j][k];
                    if(checkTile[i][j][k] == 1){
                        flagArray[i][j][k] = 1;
                        checkTile[i][j][k] = 0;
                    }
                }
            }
            if(match[i] >= 5 && gameOverFlag != 1){
                for(int j=0; j<7; j++){
                    for(int k=0; k<100; k++){
                        if(flagArray[i][j][k] == 1 && i+1 == playerTile[j][k]){
                            if(playerTile[j][k] > 3){
                                fx4[j][k] = type[playerTile[j][k]-1];
                                prevTile[j][k] = playerTile[j][k];
                                playerTile[j][k] = 0;
                                score += 100;
                                count4[j][k] = 1.0f;
                                exp++;
                                matchFlag = 1;
                            }
                            if(flagArray[i][j][k] != 0) flagArray[i][j][k] = 0;
                        }
                    }
                }
            }

        chain += matchFlag;
        if(chain >= 2 && matchFlag == 1){
            chainFlag = 1.0f;
            displayChain = chain;
            for(int i=0; i<(displayChain+2)*(displayChain+2)*3; i++){
                float speed = GetRandomValue(0, 777);
                
                int angle = GetRandomValue(0, 360);
                FireBullet(130, 170, speed/100, angle, ColorFromHSV(GetRandomValue(-135, -105)+displayChain*60, 0.64, 1), 4, 4, true, 31);
                FireBullet(131, 171, speed/100, angle, ColorFromHSV(GetRandomValue(-135, -105)+displayChain*60, 0.36, 1), 2, 2, true, 50);
            }
        }
        matchFlag = match[i] = 0;
        }


//  player character

        DrawRectangle(22+charX*32, 441, 32, 32, ColorAlpha(BLACK, 0.5f));
        DrawRectangle(22+charX*32, 441, 32, 32, ColorAlpha(BLUE, 0.5f));
        DrawRectangle(20+charX*32, 439, 32, 32, BLACK);
        DrawRectangleLines(21+charX*32, 440, 30, 30, ColorAlpha(fx2, count2));
        DrawRectangleLines(22+charX*32, 441, 28, 28, ColorAlpha(fx2, count2));
        DrawRectangle(35+charX*32, 10, 2, 420, DARKGRAY);
        DrawRectangle(35+charX*32, 10, 2, 420, ColorAlpha(WHITE, count2));
        DrawRectangle(32+charX*32, 10, 1, 420, ColorAlpha(fx, count));
        DrawRectangle(39+charX*32, 10, 1, 420, ColorAlpha(fx, count));
        if(count > 0.0f) count -= 0.1f;
        if(count2 > 0.0f) count2 -= 0.1f;
        if(hold >= 1) DrawRectangle(24+charX*32, 443, 24, 24, type[hold-1]);
        if(holdCount == 1) DrawText(TextFormat("%i", holdCount), 33+charX*32, 446, 20, BLACK);
        else if(holdCount > 1 && holdCount <= 9) DrawText(TextFormat("%i", holdCount), 31+charX*32, 446, 20, BLACK);
        else if(holdCount >= 10 && holdCount <= 19 && holdCount != 11) DrawText(TextFormat("%i", holdCount), 28+charX*32, 446, 20, BLACK);
        else if(holdCount == 11) DrawText(TextFormat("%i", holdCount), 30+charX*32, 446, 20, BLACK);

        if(mixCount == 38 && bombReadyFlag == 0){
            PlaySound(seBombReady);
            bombReadyFlag = 1;
        }
        if(IsKeyPressed(KEY_C) == 1 && mixCount == 38 && gameOverFlag == 0){
            bombX = charX;
            for(int i=0; i<100; i++){
                playerTile[bombX][i] = 0;
            }
            countBomb = 1.0f;
            PlaySound(seBomb);
            mixCount = bombReadyFlag = 0;
        }
        if(countBomb > 0.0f)
        DrawRectangle(20+bombX*32, 10, 32, 420, ColorAlpha(WHITE, countBomb));
        countBomb -= 0.025f;

        for (int i=0; i<=6; i++){
            for(int j=1; j<100; j++){
                while(playerTile[i][j] != 0 && playerTile[i][j-1] == 0){
                    int temp = playerTile[i][j];
                    int temp2 = playerTile[i][j-2];
                    playerTile[i][j] = 0;
                    playerTile[i][j-1] = temp;
                    if(mixTile(temp, temp2) > 6){
                        fx3[i][j-2] = type[playerTile[i][j-1]-1];
                        count3[i][j-2] = 1.0f;
// ja pierdole
                        pastHold = temp;
                        playerTile[i][j-1] = mixTile(temp, temp2)/10;
                        prevTile[i][j-2] = playerTile[i][j-2];
                        playerTile[i][j-2] = 0;
                        mixCount += 2;
                    }
                    if(playerTile[i][j-2] != 0) floodFill(i, j-1, playerTile[i][j-1]);
                }
            }
            
            for (int j=0; j<=12; j++){
                updatePlayfield(i, j, playerTile[i][j]);
                if(pastHold == 1){
                    DrawRectangle(22+i*32, 24+j*32, 28, 4, ColorAlpha(WHITE, count3[i][j]/2));
                    DrawRectangle(22+i*32, 24+j*32, 28, 4, ColorAlpha(fx3[i][j], count3[i][j]));
                }
                else if(pastHold == 2){
                    if(prevTile[i][j] == 1){
                        DrawRectangle(34+i*32, 16+j*32, 4, 4, ColorAlpha(WHITE, count3[i][j]/2));
                        DrawRectangle(34+i*32, 16+j*32, 4, 4, ColorAlpha(fx3[i][j], count3[i][j]));
                    }
                    else{
                        for(int k=0; k<=1; k++){
                            DrawRectangle(26+i*32+k*16, 24+j*32, 4, 4, ColorAlpha(WHITE, count3[i][j]/2));
                            DrawRectangle(26+i*32+k*16, 24+j*32, 4, 4, ColorAlpha(fx3[i][j], count3[i][j]));
                        }
                    }
                }
                else if(pastHold == 3){
                    DrawRectangle(34+i*32, 12+j*32, 4, 28, ColorAlpha(WHITE, count3[i][j]/2));
                    DrawRectangle(34+i*32, 12+j*32, 4, 28, ColorAlpha(fx3[i][j], count3[i][j]));
                }   
                
                DrawRectangleLines(26+i*32, 16+j*32, 20, 20, ColorAlpha(fx4[i][j], count4[i][j]));
                DrawRectangleLines(27+i*32, 17+j*32, 18, 18, ColorAlpha(fx4[i][j], count4[i][j]));
                DrawRectangle(30+i*32, 20+j*32, 12, 12, ColorAlpha(fx4[i][j], count4[i][j]));
                count3[i][j] -= 0.05f;
                count4[i][j] -= 0.05f;
            }
            if(playerTile[i][13] != 0){
                gameOverFlag = 1;
            }
        }

        if(gameOverFlag == 1){
            int fileHighScore = readHighScore();
            if (score > fileHighScore) {
                saveHighScore(score);
            }
            DrawRectangle(12, 2, 240, 428, ColorAlpha(BLACK, 0.8f));
            DrawText(TextFormat("GAME\nOVER"), 48, 80, 60, RED);
            DrawText(TextFormat("GAME\nOVER"), 50, 82, 60, ColorAlpha(RED, 0.375f));
            
            DrawText(TextFormat("press any key"), 59, 260, 20, RED);
            DrawText(TextFormat("press any key"), 60, 261, 20, ColorAlpha(RED, 0.375f));
            DrawText(TextFormat("to return to menu"), 40, 290, 20, RED);
            DrawText(TextFormat("to return to menu"), 41, 291, 20, ColorAlpha(RED, 0.375f));
            timer = 0;
            timer2--;
            score -= level;
            
            if(endCount < 1.0f) endCount += 0.0625f;
            if(GetKeyPressed() > 0 && endCount >= 0.875f) finalFlag = 1;
            if(finalFlag == 1){
            DrawRectangle(0, 0, 640, 480, ColorAlpha(WHITE, endCount-1.0f)); 
            endCount += 0.03125f;
            }
            
            if(endCount >= 2.0f){
            StopMusicStream(gameBGM);

            // Resetting arrays
            memset(prevTile, 0, sizeof(prevTile));
            memset(playerTile, 0, sizeof(playerTile));
            memset(checkTile, 0, sizeof(checkTile));
            memset(match, 0, sizeof(match));

            // Resetting other variables
            pastHold = 0;
            hold = 0;
            holdCount = 0;
            mixCount = 0;
    
            // Resetting game-specific variables
            wildTile = 0;
            countCurtain = 1.0f;
            count = 0.0f;
            count2 = 0.0f;
            count3[7][100] = {};
            count4[7][100] = {};
            countBomb = 0.0f;
            fx = type[0];
            fx2 = type[0];
            fx3[7][100] = {};
            fx4[7][100] = {};
            chain = 0;
            displayChain = 0;
            maxChain = 0;
            chainFlag = 0;
            level = 1;
            timer = 120;
            timer2 = 0;
            charX = 3;
            bombReadyFlag = 0;
            bonus = 0;
            score = 0;
            exp = 0;
            dasCount = das;
            endCount = 0.0f;
            finalFlag = 0;
            
            return 0;
            }
        }
        
        if(chainFlag >= 1 && chainFlag < 2.625f){
            for(int i=-1; i<2; i++){
                for(int j=-1; j<2; j++){
                    DrawText(TextFormat("%i", displayChain), 97+i*2, 108+j*2, 140, ColorAlpha(BLACK, 2.625f-chainFlag));
                    DrawText(TextFormat("chain"), 107+i*2, 225+j*2, 20, ColorAlpha(BLACK, 2.25f-chainFlag));
                }
            }
            DrawText(TextFormat("%i", displayChain), 97, 108, 140, ColorAlpha(RAYWHITE, 2.625f-chainFlag));
            DrawText(TextFormat("chain"), 107, 225, 20, ColorAlpha(RAYWHITE, 2.25f-chainFlag));
            chainFlag += 0.025f;
        }
        
        if(timer == 0){
            int bufferplayerTile[7][14] = {};
            float buffercount3[7][14] = {};
            float buffercount4[7][14] = {};
            Color bufferfx3[7][14] = {};
            Color bufferfx4[7][14] = {};
            int randomTile[7] = {1, 2, 3, 4, 5, 6, 1+wildTile%6};
            
            shuffle(randomTile, 7);
            for (int i=0; i<=6; i++){
                for (int j=12; j>=0; j--){
                    bufferplayerTile[i][j+1] = playerTile[i][j];
                    buffercount3[i][j+1] = count3[i][j];
                    buffercount4[i][j+1] = count4[i][j];
                    bufferfx3[i][j+1] = fx3[i][j];
                    bufferfx4[i][j+1] = fx4[i][j];
                    playerTile[i][j] = 0;
                    count3[i][j] = 0;
                    count4[i][j] = 0;
                    fx3[i][j] = type[7];
                    fx4[i][j] = type[7];
                    playerTile[i][j+1] = bufferplayerTile[i][j+1];
                    count3[i][j+1] = buffercount3[i][j+1];
                    count4[i][j+1] = buffercount4[i][j+1];
                    fx3[i][j+1] = bufferfx3[i][j+1];
                    fx4[i][j+1] = bufferfx4[i][j+1];
                }

                playerTile[i][0] = randomTile[i];
                count3[i][0] = count4[i][0] = 0.0f;
                fx3[i][0] = fx4[i][0] = type[6];
            }
            timer = 260-level*20;
            // LEVEL 1 IS TOO HARD FOR PUZZLE NOVICES!
            wildTile++;
            if(chain >= 2){
                bonus = pow(2, chain-1)*100;
                score += bonus;
                exp += pow(2, chain-1);
            }
            chain = 0;
        }

        DrawRectangle(0, 0, 640, 480, ColorAlpha(WHITE, countCurtain));
        countCurtain -= 0.05f;

        score += level;
        
        DrawBullets();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
    
    UnloadSound(seBomb);
    UnloadSound(seBombReady);
    UnloadSound(seLevelUp);
    UnloadSound(seMove);
    UnloadSound(seHold);
    UnloadSound(seDrop);
    UnloadMusicStream(gameBGM);
    CloseAudioDevice();
    UnloadTexture(laurency);
    exit(0);
}

    int mixTile(int tile1, int tile2) {
        if (gameOverFlag != 1) {
            if ((tile1 == 1 && tile2 == 2) || (tile1 == 2 && tile2 == 1)) return 40;
            else if ((tile1 == 2 && tile2 == 3) || (tile1 == 3 && tile2 == 2)) return 50;
            else if ((tile1 == 1 && tile2 == 3) || (tile1 == 3 && tile2 == 1)) return 60;
        }
        return tile1;
    }
    
    void floodFill(int x, int y, int checkedColor){
        if(playerTile[x][y] == checkedColor){
            checkTile[checkedColor-1][x][y] = 1;
            if(x != 6 && checkTile[checkedColor-1][x+1][y] != 1) floodFill(x+1, y, checkedColor);
            if(y != 100 && checkTile[checkedColor-1][x][y+1] != 1) floodFill(x, y+1, checkedColor);
            if(x != 0 && checkTile[checkedColor-1][x-1][y] != 1) floodFill(x-1, y, checkedColor);
            if(y != 0 && checkTile[checkedColor-1][x][y-1] != 1) floodFill(x, y-1, checkedColor);
        }
    }
    
    void updatePlayfield(int x, int y, int color){
//        char mark[6] = {'R', 'Y', 'B', 'O', 'G', 'P'};
        if(color > 0){
            DrawRectangle(20+x*32, 10+y*32, 32, 32, BLACK);
            if(color < 4){
                DrawRectangle(20+x*32, 10+y*32, 32, 32, ColorAlpha(type[color-1], 0.3125f));
                DrawRectangle(21+x*32, 11+y*32, 30, 30, ColorAlpha(type[color-1], 0.5f));
            }
            else{
                DrawRectangle(20+x*32, 10+y*32, 32, 32, ColorAlpha(type[color-1], 0.625f));
                DrawRectangle(21+x*32, 11+y*32, 30, 30, type[color-1]);
            }

//            DrawText(TextFormat("%c", mark[color-1]), 22+x*32, 32+y*32, 10, {0, 0, 0, 64});
            if(color == 1) DrawRectangle(22+x*32, 24+y*32, 28, 4, {0, 0, 0, 128});
            else if(color == 2){
                for (int i=0; i<=2; i++){
                    DrawRectangle(26+x*32+i*8, 16+y*32+(i+1)%2*8, 4, 4, {0, 0, 0, 128});
                }
            }
            else if(color == 3) DrawRectangle(34+x*32, 12+y*32, 4, 28, {0, 0, 0, 128});
            else if(color == 4){
                DrawRectangle(22+x*32, 24+y*32, 28, 4, {255, 255, 255, 128});
                DrawRectangle(34+x*32, 16+y*32, 4, 4, {255, 255, 255, 128});
            }
            else if(color == 5){
                DrawRectangle(34+x*32, 12+y*32, 4, 28, {255, 255, 255, 128});
                for (int i=0; i<=1; i++){
                    DrawRectangle(26+x*32+i*16, 24+y*32, 4, 4, {255, 255, 255, 128});
                }
            }
            else if(color == 6){
                for (int i=0; i<=1; i++){
                    DrawRectangle(34+x*32, 12+y*32+i*16, 4, 12, {255, 255, 255, 128});
                }
                DrawRectangle(22+x*32, 24+y*32, 28, 4, {255, 255, 255, 128});
            }
        }
    }