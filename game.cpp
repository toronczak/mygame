#include "raylib.h"
#include "merger.cpp"
#include <stdio.h> // Include for file operations

// Function prototypes
void SaveConfig(int das, int arr);
void LoadConfig(int* das, int* arr);

int main(void){
    InitWindow(640, 480, "Spectral Signal (alpha)");
    InitAudioDevice();

    int choice = 1;
    int count = 0;
    float count2 = 0.0f;
    int flag = 0;
    bool areSoundsLoaded = false;
    
    // Load DAS and ARR settings
    LoadConfig(&das, &arr);
    
    Texture2D logowhite = LoadTexture("GFX/logowhite.png");
    Texture2D mainmenuBG = LoadTexture("GFX/mainmenuBG.png");
    Texture2D mainmenuBG2 = LoadTexture("GFX/mainmenuBG2.png");

    Music BGM = LoadMusicStream("BGM/mainmenu.ogg");
    SetMusicVolume(BGM, 0.25f);
    PlayMusicStream(BGM);
    Sound seCursor = LoadSound("SE/cursor.wav");
    SetSoundVolume(seCursor, 1.0f);
    Sound seDecide = LoadSound("SE/decide.wav");
    SetSoundVolume(seDecide, 1.0f);

    Texture2D logo = LoadTexture("GFX/logo.png");
    Texture2D bluelevelBG = LoadTexture("GFX/bluelevelBG.png");
    Music gameBGM;
    Sound seBomb;
    Sound seBombReady;
    Sound seLevelUp;
    Sound seMove;
    Sound seHold;
    Sound seDrop;
    
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose()){
        // Update music buffer with new stream data
        UpdateMusicStream(BGM);

        // If music is not playing, play it
        if (!IsMusicStreamPlaying(BGM) && flag == 0) {
            PlayMusicStream(BGM);
        }

        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))){
            ToggleFullscreen();
        }
        BeginDrawing();
        
        if(flag == 0 && count2 > 0.0f) count2 -= 0.05f;
        if(flag == 1) count2 += 0.05f;
        if(count2 > 1.6f){
            if (!areSoundsLoaded) {
                gameBGM = LoadMusicStream("BGM/survival.mp3");
                SetMusicVolume(gameBGM, 0.25f);
                PlayMusicStream(gameBGM);
                seBomb = LoadSound("SE/bomb.wav");
                SetSoundVolume(seBomb, 0.25f);
                seBombReady = LoadSound("SE/bombReady.wav");
                SetSoundVolume(seBombReady, 0.25f);
                seLevelUp = LoadSound("SE/levelUp.wav");
                SetSoundVolume(seLevelUp, 0.5f);
                seMove = LoadSound("SE/move.wav");
                SetSoundVolume(seMove, 0.5f);
                seHold = LoadSound("SE/hold.wav");
                SetSoundVolume(seHold, 1.0f);
                seDrop = LoadSound("SE/drop.wav");
                SetSoundVolume(seDrop, 1.0f);
                areSoundsLoaded = true;
                gameOverFlag = 0;
                flag = game(logo, bluelevelBG, gameBGM, seBomb, seBombReady, seLevelUp, seMove, seHold, seDrop);
                count = 0;
                choice = 1;
            }
            else{
                gameOverFlag = 0;
                flag = game(logo, bluelevelBG, gameBGM, seBomb, seBombReady, seLevelUp, seMove, seHold, seDrop);
                count = 0;
                choice = 1;
            }
        }
        
        DrawTexture(mainmenuBG, 0, 0-count%488, WHITE);
        DrawTexture(mainmenuBG, 0, 488-count%488, WHITE);
        DrawTexture(mainmenuBG2, 198, 0+count%488, WHITE);
        DrawTexture(mainmenuBG2, 198, -488+count%488, WHITE);
        
        DrawTexture(logowhite, 0, 0, WHITE);

//        DrawText(TextFormat("%i", choice), 339, 391, 40, BLACK);
        if(choice < 10){
            for(int i=-1; i<2; i++){
                for(int j=-1; j<2; j++){
                    DrawText(TextFormat("*"), 246+i*2, 262+choice*30+j*2, 20, WHITE);
                    DrawText(TextFormat("start game"), 264+i*2, 290+j*2, 20, WHITE);
                    DrawText(TextFormat("settings"), 264+i*2, 320+j*2, 20, WHITE);
                    DrawText(TextFormat("exit"), 264+i*2, 350+j*2, 20, WHITE);
                }
            }
        
            DrawText(TextFormat("*"), 246, 262+choice*30, 20, BLACK);
            DrawText(TextFormat("start game"), 264, 290, 20, BLACK);
            DrawText(TextFormat("settings"), 264, 320, 20, BLACK);
            DrawText(TextFormat("exit"), 264, 350, 20, BLACK);
        }
        
        if(flag == 0 || flag == 2){
            if(flag == 0) {
                if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP)){
                    if(choice == 1) choice = 3;
                    else if(choice == 11) choice = 13;
                    else if(choice < 20) choice--;
                    else if(choice == 21){
                        if(IsKeyPressed(KEY_LEFT)){
                             if(das != 1) das--;
                        }
                        else das++;
                    }
                    else if(choice == 22){
                        if(IsKeyPressed(KEY_LEFT)){
                             if(arr != 1) arr--;
                        }
                        else arr++;
                    }
                    PlaySound(seCursor);
                }
                else if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN)){
                    if(choice == 3) choice = 1;
                    else if(choice == 13) choice = 11;
                    else if(choice < 20) choice++;
                    else if(choice == 21){
                        if(IsKeyPressed(KEY_RIGHT)) das++;
                        else if(das != 1) das--;
                    }
                    else if(choice == 22){
                        if(IsKeyPressed(KEY_RIGHT)) arr++;
                        else if(arr != 1) arr--;
                    }
                    PlaySound(seCursor);
                }
            }
            
            if((IsKeyPressed(KEY_Z) || (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_SPACE)) && choice == 1){
                StopMusicStream(BGM);
                flag = 1;
                PlaySound(seDecide);
            }
            
            if((IsKeyPressed(KEY_Z) || (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_SPACE)) && choice == 2){
                flag = 2;
                choice = 11;
                PlaySound(seDecide);
            }
            
            if(choice > 10){
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        DrawText(TextFormat("*"), 246+i*2, -38+choice*30+j*2, 20, WHITE);
                        DrawText(TextFormat("DAS"), 264+i*2, 290+j*2, 20, WHITE);
                        DrawText(TextFormat("ARR"), 264+i*2, 320+j*2, 20, WHITE);
                        DrawText(TextFormat("back"), 264+i*2, 350+j*2, 20, WHITE);
                    }
                }
                DrawText(TextFormat("*"), 246, -38+choice*30, 20, BLACK);
                DrawText(TextFormat("DAS"), 264, 290, 20, BLACK);
                DrawText(TextFormat("ARR"), 264, 320, 20, BLACK);
                DrawText(TextFormat("back"), 264, 350, 20, BLACK);
                
                if(choice == 11){
                    DrawText(TextFormat("Delayed Auto Shift"), 320, 294, 10, DARKGRAY);
                    DrawText(TextFormat("defines how long"), 320, 306, 10, DARKGRAY);
                    DrawText(TextFormat("the player must hold"), 320, 318, 10, DARKGRAY);
                    DrawText(TextFormat("Left/Right"), 320, 330, 10, DARKGRAY);
                    DrawText(TextFormat("before the input"), 320, 342, 10, DARKGRAY);
                    DrawText(TextFormat("begins to repeat."), 320, 354, 10, DARKGRAY);
                }
                
                if(choice == 12){
                    DrawText(TextFormat("Auto Repeat Rate"), 320, 294, 10, DARKGRAY);
                    DrawText(TextFormat("defines how fast"), 320, 306, 10, DARKGRAY);
                    DrawText(TextFormat("the Left/Right"), 320, 318, 10, DARKGRAY);
                    DrawText(TextFormat("input repeats."), 320, 330, 10, DARKGRAY);
                }
                
                if((IsKeyPressed(KEY_Z) || (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_SPACE))){
                    if(choice == 11 && flag != 2){
                        choice = 21;
                        flag = 2;
                    }
                    else if(choice == 12){
                        choice = 22;
                        flag = 2;
                    }
                    else if(choice == 13) choice = 2;
                    PlaySound(seDecide);
                }
                else flag = 0;
                
                if(IsKeyPressed(KEY_X) == 1){
                    choice -= 10;
                    PlaySound(seDecide);
                }
            }
            
            
            if(choice == 21){
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        DrawText(TextFormat("= %iF", das), 364+i*2, 290+j*2, 20, WHITE);
                    }
                }
                DrawText(TextFormat("= %iF", das), 364, 290, 20, BLACK);
                
                if((IsKeyPressed(KEY_Z) || (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_SPACE)) && flag == 0){
                    SaveConfig(das, arr);
                    choice = 11;
                    PlaySound(seDecide);
                }
            }
            
            if(choice == 22){
                for(int i=-1; i<2; i++){
                    for(int j=-1; j<2; j++){
                        DrawText(TextFormat("= %iF", arr), 364+i*2, 320+j*2, 20, WHITE);
                    }
                }
                DrawText(TextFormat("= %iF", arr), 364, 320, 20, BLACK);
                
                if((IsKeyPressed(KEY_Z) || (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_SPACE)) && flag == 0){
                    SaveConfig(das, arr);
                    choice = 12;
                    PlaySound(seDecide);
                }
            }
            
            if((IsKeyPressed(KEY_Z) || (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) || IsKeyPressed(KEY_SPACE)) && choice == 3){
                StopMusicStream(BGM);
                PlaySound(seDecide);
                ClearBackground(BLACK);
                EndDrawing();
                CloseAudioDevice();
                CloseWindow();
                return 0;
            }
        }
        
        DrawRectangle(0, 0, 640, 480, ColorAlpha(WHITE, count2));
        count++;
        ClearBackground(BLACK);
        EndDrawing();
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void SaveConfig(int das, int arr) {
    FILE* file = fopen("config.dat", "w");
    if (file != NULL) {
        fprintf(file, "%d\n%d", das, arr);
        fclose(file);
    } else {
        // Handle error opening file
    }
}

void LoadConfig(int* das, int* arr) {
    FILE* file = fopen("config.dat", "r");
    if (file != NULL) {
        fscanf(file, "%d\n%d", das, arr);
        fclose(file);
    } else {
        // Handle error opening file or set default values
        *das = 10; // Default DAS value
        *arr = 5;  // Default ARR value
    }
}