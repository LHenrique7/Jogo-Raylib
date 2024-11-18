#include "raylib.h"
#include <iostream>
#include <deque>            // Deque usada para comparar e manipular elementos Vector2
#include <raymath.h>        // Adicionar elementos nos Vector2 utilizados

using namespace std;

// Variavel para guardar quando foi o ultimo update do jogo
double ultimoUpdate = 0;

// Função que verifica se a cobra e a comida quando é gerada, estão na mesma posição
bool ElementoNoDeque(Vector2 posicaoComida, deque<Vector2> corpo) {
    for(double i = 0; i < corpo.size(); i++) {

        // Verifica se a posição gerada da comida está na mesma coordenanda de alguma parte do corpo da cobra
        if(Vector2Equals(corpo[i], posicaoComida)) {
            return true;
        }
    }
    return false;
}

// função que verifica se o intervalo de update ja passou
bool UpdateAconteceu(double intervalo) {

    // Armazena o tempo do exato momento que a função foi chamada
    double tempoAtual = GetTime();

    // Verifica se a diferença do tempo atual da função e do tempo da ultima vez que ela foi chamada
    // é igual ao intervalo estabelecida pela dificuldade escolhida
    if(tempoAtual - ultimoUpdate >= intervalo) {

        // Após a verificação, caso o intervalo ja tenha passado, o tempo de quando a função foi chamada é atribuido
        // a váriavel que guarda o tempo do ultimo update
        ultimoUpdate = tempoAtual;
        return true;
    }
    return false;
}



// Variáveis de referência para o GRID invisivel
int tamanhoCelula = 30;
int qtdCelula = 25;
int borda = 75;

// Classe do objeto Cobra
class Cobra {

    public:
        deque<Vector2> corpo = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};   // Corpo inicial da cobra
        Vector2 direcao = {1, 0};                                               // Atributo da direção que a cobra vai andar no GRID
        bool crescimento = false;                                               // Atributo que libera o crescimento do corpo

        // Método que Desenha a cobra no jogo
        void Draw() {

            for (unsigned int i = 0; i < corpo.size(); i++) {
                
                // Pegando as coordenadas de cada parte do corpo e imprimindo na tela
                float x = corpo[i].x;      
                float y = corpo[i].y;

                // Criando um retangulo
                Rectangle pedacoCorpo = Rectangle{borda + x * tamanhoCelula, borda + y * tamanhoCelula, (float)tamanhoCelula, (float)tamanhoCelula};  

                // Imprimindo o retangulo criado com bordas arredondadas
                DrawRectangleRounded(pedacoCorpo, 0.3, 6, BLACK);                               

            }
        }

        // Método que faz o update do corpo
        void Update() {

            // Faz a "animação" de andar adicionando um elemento no vector, somando as coordenadas da cabeça com a direção desejada
            // e retirando o ultimo elemento do vector caso o crescimento seja false, dando a impressão da "animação"
            corpo.push_front(Vector2Add(corpo[0], direcao));

            // Faz a verificação se a cobra deve crescer ou não    
            if (crescimento == true) {                              
                crescimento = false;
            } else {
                // Função que retira o ultimo valor do vector2 caso crescimento = false
                corpo.pop_back();                                    
            }
        }

        // Método que retorna a cobra para a posição e o tamanho inicial do jogo
        void Reset() {
            corpo = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};   
            direcao = {1, 0};
        }


};

// Classe do objeto Comida
class Comida {

    public:
        Vector2 posicao;       // Vetor de posição da comida
        Texture2D texture;     // Variável que será armazenada a imagem da comida


        // Construtor da classe que carrega a imagem da comida
        Comida(deque<Vector2> corpoCobra) {
            Image image = LoadImage("assets/Images/comida.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            posicao = GenerateRandomPos(corpoCobra);
        }

        // Destrutor da classe que também descarrega a textura da comida
        ~Comida() {
            UnloadTexture(texture);
        }

        // Método que Imprimi a comida na tela
        void Draw() {
            DrawTexture(texture, borda + posicao.x * tamanhoCelula, borda + posicao.y * tamanhoCelula, GRAY);
        }
        
        // Método que gera uma nova posição para a comida
        Vector2 GenerateRandomCell() {
            float x = GetRandomValue(0, qtdCelula - 1);
            float y = GetRandomValue(0, qtdCelula - 1);
            return Vector2{x, y};
        }

        // Método que verifica se a posição gerada é válida e agrega as informações ao atributo de posição da classe
        Vector2 GenerateRandomPos(deque<Vector2> corpoCobra) {
            Vector2 posicao = GenerateRandomCell();
            while (ElementoNoDeque(posicao, corpoCobra)) {
                posicao = GenerateRandomCell();
            }
            return posicao;
        }
        
};

// Classe do Jogo
class Game {

    public:
        bool running = true;                    // O jogo só roda caso running = true
        int pontos = 0;                         // Pontos do jogo
        Sound eatSound;                         // Som quando come a comida
        Sound wallSound;                        // Som quando bate
        Cobra cobra = Cobra();                  // Criando objeto cobra
        Comida comida = Comida(cobra.corpo);    // Criando objeto comida

        // Construtor da classe e também atribui o som aos atributos
        Game() {
             // Iniciando sons
            InitAudioDevice();
            eatSound = LoadSound("assets/Sounds/eat.mp3");
            wallSound = LoadSound("assets/Sounds/wall.mp3"); 
        }

        // Destrutor da classe e também descarrega os sons
        ~Game() {
            UnloadSound(eatSound);
            UnloadSound(wallSound);
        }

        // Método para desenhar os objetos do jogo
        void Draw() {
            comida.Draw();            
            cobra.Draw();
        }

        // Método para fazer o Update do jogo, verificando os updates dos objetos
        void Update() {
            if (running) {
                cobra.Update();
                ChecarSeComeuComida();
                ChecarSeBateuNaParede();
                ChecarSeBateuNoCorpo();
            }
        }

        // Método que verifica se comeu a comida
        void ChecarSeComeuComida() {

            //Se comer a comida, é gerada uma nova posição e habilita o crescimento
            if (Vector2Equals(cobra.corpo[0], comida.posicao)) {          
                comida.posicao = comida.GenerateRandomPos(cobra.corpo);
                cobra.crescimento = true;
                pontos++;
                PlaySound(eatSound);
            }
        }

        // Método que Verifica se não colidiu com as paredes
        void ChecarSeBateuNaParede() {
            //Verifica se a cabeça não ultrapassou as paredes
            if(cobra.corpo[0].x == qtdCelula || cobra.corpo[0].x == -1) {
                GameOver();
            }
            if(cobra.corpo[0].y == qtdCelula || cobra.corpo[0].y == -1) {
                GameOver();
            }
        }

        // Método que verifica se não colidiu com o próprio corpo
        void ChecarSeBateuNoCorpo() {

            //Criando um deque sem a "cabeça da cobra
            deque<Vector2> corpoSemCabeca = cobra.corpo;
            corpoSemCabeca.pop_front();

            //Verificando se a cabeça está na mesma coordenada de alguma parte do corpo
            if(ElementoNoDeque(cobra.corpo[0], corpoSemCabeca)) {       
                GameOver();
            }

        }   

        // Método de Game Over que reseta o jogo
        void GameOver() {
            cobra.Reset();
            comida.posicao = comida.GenerateRandomPos(cobra.corpo);
            running = false;
            pontos = 0;
            PlaySound(wallSound);
        }
};

// Rotina do Jogo
int GameScreen(float difficult){ 
    
    // Mensagem no terminal indicando inicio do jogo
    cout << "Iniciando jogo..." << endl;

    // Criando os objetos do jogo
    Game game;

    // Loop do jogo acontece enquanto a janela do jogo não for fechada
    while(!WindowShouldClose()) {           
        
        // Abre o espaço para desenhar os objetos do jogo
        BeginDrawing();                             
        
        // Atualiza o jogo no tempo atribuido na tela de seleção de dificuldade
        if(UpdateAconteceu(difficult)) {          
            game.Update();         
        }

        // Detecção se as teclas de movimento foram apertadas e se não estão sendo acionadas na direção contrária do movimento da cobra
        if (IsKeyPressed(KEY_UP) && game.cobra.direcao.y != 1) {
            game.cobra.direcao = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.cobra.direcao.y != -1) {
            game.cobra.direcao = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.cobra.direcao.x != -1) {
            game.cobra.direcao = {1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.cobra.direcao.x != 1) {
            game.cobra.direcao = {-1, 0};
            game.running = true;
        }

        // Limpa o background da janela e deixa tudo GRAY
        ClearBackground(GRAY);

        // Desenhando a linha em volta do jogo                     
        DrawRectangleLinesEx(Rectangle{(float)borda-5, (float)borda-5, (float)tamanhoCelula*qtdCelula+10, (float)tamanhoCelula*qtdCelula+10}, 5, BLACK);

        // Imprime o titulo do jogo na tela
        DrawText("Jogo da Cobrinha", borda-5, 20, 40, BLACK);

        // Imprime os pontos na tela
        DrawText(TextFormat("%i", game.pontos), borda - 5, borda  + 10  + tamanhoCelula * qtdCelula, 40, BLACK);

        // Desenha os objetos do jogo
        game.Draw();                                

        // Encerra o espaço de desenho do jogo
        EndDrawing();                              
    }

    return 0;
}
