#include <iostream>
#include "raylib.h"
#include "gameScreen.hpp"
#include "difficultScreen.hpp"

int gameRunning = 0;    // Váriavel para puxar as telas do jogo
float dificuldade;      // Váriavel que armazena a dificuldade do jogo
int saiu;

// Imagenzinha do melhor
Texture2D textura;
Image timao = LoadImage("assets/Images/timao.png");      

void Menu() {

    // Tamanho e coordenadas dos botões
    Rectangle playButton = { 350, 500, 200, 50 };
    Rectangle exitButton = { 350, 600, 200, 50 };

    // Limpa a tela com Cinza
    ClearBackground(GRAY);

    // Desenha os botões na tela
    DrawRectangleRounded(playButton, 0.4, 6, BLACK);
    DrawRectangleRounded(exitButton, 0.4, 6, BLACK);

    // Exibe o titulo e os textos nos botões
    DrawText("Jogo da", 150, 120, 70, BLACK);
    DrawText("Cobrinha", 230, 250, 100, BLACK);
    DrawText("JOGAR", playButton.x + 50, playButton.y + 10, 30, WHITE);
    DrawText("SAIR", exitButton.x + 60, exitButton.y + 10, 30, WHITE);
    

    // Verifica se o botão Jogar foi clicado
    if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        UnloadTexture(textura);
        gameRunning = 1; // Muda a variavel para 1, para chamar as telas
    }

    // Verifica se o botão Sair foi clicado
    if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        UnloadTexture(textura);
        CloseWindow(); // Fecha tudo
    }
}

int main() {

    // Iniciando a janela e setando FPS
    InitWindow(900, 900, "Cobrinha");
    SetTargetFPS(60);

    // Atribui a imagem
    textura = LoadTextureFromImage(timao);

    // Loop para acontecimentos do jogo enquanto a janela não for fechada
    while (!WindowShouldClose()) {

        // Abre o espaço de desenho 
        BeginDrawing();

        // Chama o menu e exibe a imagem caso o botão jogar não tenha sido acionado
        if (gameRunning == 0) {
            Menu();
            DrawTexture(textura, 750, 750, BLACK);
        } 
        
        // Caso contrário, se o botão jogar for apertado...
        else if (gameRunning == 1) {
            // Chama a tela de seleção de dificuldade
            dificuldade = difficultScreen();

            // Caso a janela tenha sido fechada pela tela de dificuldade, encerra o programa
            if(dificuldade == 5) {
                CloseWindow();
                return 0;
            }

            // Caso contrário, chama a tela de jogo após escolher a dificuldade
            saiu = GameScreen(dificuldade);

            // Caso a janela tenha sido fechada pela tela do jogo, encerra  o programa
            if (saiu == 0) {
                CloseWindow();
                return 0;
            }
        }

        // Encerra o espaço de desenho
        EndDrawing();
    }

    // Caso aperte para fechar na janela do menu ele encerra o programa também
    CloseWindow();
    return 0;
}
