#include "raylib.h"
#include <iostream>

float difficultScreen(){ 
    while(!WindowShouldClose()) {           //Loop do jogo acontece enquanto a janela do jogo não for fechada
        
        BeginDrawing();

        // Tamanho e coordenadas dos botões
        Rectangle easyButton = { 350, 400, 200, 50 };
        Rectangle normalButton = { 350, 500, 200, 50 };
        Rectangle hardButton = { 350, 600, 200, 50 };

        // Limpa a tela com Cinza
        ClearBackground(GRAY);

        // Desenha os botões na tela
        DrawRectangleRounded(easyButton, 0.4, 6, BLACK);
        DrawRectangleRounded(normalButton, 0.4, 6, BLACK);
        DrawRectangleRounded(hardButton, 0.4, 6, BLACK);

        // Exibe o titulo e os textos nos botões
        DrawText("Escolha a dificuldade", 80, 120, 70, BLACK);
        DrawText("FÁCIL", easyButton.x + 50, easyButton.y + 10, 30, WHITE);
        DrawText("NORMAL", normalButton.x + 40, normalButton.y + 10, 30, WHITE);
        DrawText("DIFÍCIL", hardButton.x + 40, hardButton.y + 10, 30, WHITE);
        

        // Verifica se a dificuldade escolhida foi fácil
        if (CheckCollisionPointRec(GetMousePosition(), easyButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return 0.2;
        }

        // Verifica se a dificuldade escolhida foi normal
        if (CheckCollisionPointRec(GetMousePosition(), normalButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return 0.1;
        }

        // Verifica se a dificuldade escolhida foi dificil
        if (CheckCollisionPointRec(GetMousePosition(), hardButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return 0.05;
        }

        EndDrawing();
    }

    return 5;
}
