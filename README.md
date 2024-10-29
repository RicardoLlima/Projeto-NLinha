# Projeto_NLinha

Este projeto implementa um jogo de tabuleiro em C, utilizando uma base de dados SQLite para gerir os jogadores. O jogo permite adicionar, listar e remover jogadores, e simula uma partida em que os jogadores podem inserir peças normais e especiais no tabuleiro. As vitórias e jogos realizados são registados na base de dados.

## Sumário

- [Funcionalidades](#funcionalidades)
- [Pré-requisitos](#pré-requisitos)
- [Configuração](#configuração)
- [Como Jogar](#como-jogar)
- [Arquitetura do Código](#arquitetura-do-código)
- [Possíveis Melhorias](#possíveis-melhorias)


## Funcionalidades

1. **Gerir Jogadores**:
   - Adicionar jogadores à base de dados.
   - Listar todos os jogadores.
   - Remover jogadores existentes, exceto o jogador que estiver em jogo.

2. **Jogo de Tabuleiro**:
   - Definir o tamanho do tabuleiro e a sequência de vitória.
   - Inserir peças normais e especiais no tabuleiro.
   - Verificar condições de vitória com base nas peças alinhadas (horizontal, vertical ou diagonal).
   - Registar as estatísticas de jogo (vitórias e número de jogos realizados).

3. **Base de Dados**:
   - Integração com SQLite para guardar e recuperar dados de jogadores.
   - Atualização de informações de jogos e vitórias dos jogadores após cada partida.

## Pré-requisitos

- **Compilador de C** (GCC, Clang, etc.)
- **Biblioteca SQLite3** (para integração com a base de dados)

No Ubuntu, a instalação da biblioteca SQLite3 é feita através do seguinte comando:
sudo apt-get install libsqlite3-dev

## Configuração

  - Clonar o repositório:
git clone https://github.com/seu-usuario/jogo-tabuleiro.git
cd jogo-tabuleiro

  - Compilar o código:
gcc -o jogo main.c -lsqlite3

  - Executar o jogo:
./jogo

## Como Jogar
**Menu Principal**
Após a execução, o jogo exibirá o menu principal com as seguintes opções:
    - 1 - Adicionar um novo jogador à base de dados.
    - 2 - Listar todos os jogadores registados.
    - 3 - Remover um jogador, especificando o ID.
    - 4 - Começar um jogo.
    - 0 - Sair do jogo.

**Configurações do Jogo**
Ao iniciar um novo jogo, o utilizador deverá:
Definir o número de linhas e colunas do tabuleiro.
Informar a sequência de peças necessária para ganhar o jogo.
Definir o tamanho da peça especial (quantas casas ocupa).
Definir a quantidade de peças especiais disponíveis para cada jogador.

**Comandos Durante o Jogo**
1 - Exibir o tabuleiro.
2 - Colocar uma peça normal no tabuleiro.
3 - Colocar uma peça especial, indicando a direção (esquerda ou direita).
4 - Exibir os detalhes do jogo.
5 - Adicionar um novo jogador.
6 - Remover um jogador (exceto o jogador atual).
7 - Desistir do jogo, permitindo ao outro jogador vencer por desistência.

**Condições de Vitória**
Durante o jogo, o programa verifica se algum jogador obteve a sequência de peças definida, nas direções:
Horizontal
Vertical
Diagonal
Se uma dessas condições for cumprida, o jogo regista a vitória e exibe o vencedor.

## Arquitetura do Código
main.c: Arquivo principal do código que contém a implementação de todas as funções e a lógica do jogo.
Base de Dados: As funções da base de dados (sqlite3) gerem a adição, remoção e atualização de informações dos jogadores.

**Principais Funções**
adicionarJogador(): Adiciona um novo jogador à base de dados.
listarJogador(): Lista todos os jogadores registados.
removerJogador(): Remove um jogador com base no ID, exceto o jogador atual.
BuildBoard(): Constrói o tabuleiro de acordo com as configurações fornecidas.
VictoryVerification(): Verifica condições de vitória com base na sequência definida.
atualizarVitoriasJogador(): Atualiza o número de vitórias do jogador na base de dados.
atualizarJogosRealizados(): Atualiza o número de jogos realizados pelo jogador.

## Possíveis Melhorias
Interface Gráfica: Adicionar uma interface gráfica para tornar o jogo mais interativo.
Histórico de Partidas: Implementar uma tabela para guardar o histórico de partidas.
Níveis de Dificuldade: Adicionar dificuldades variáveis com base no tamanho do tabuleiro ou nas sequências de vitória.
Sistema de Rankings: Implementar um ranking com pontuações baseadas em vitórias.

Licença
Esse projeto está licenciado sob a Licença MIT. Para mais informações, consulte o arquivo LICENSE.
