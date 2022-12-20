# LAB 03 - PSPD (FGA0244 - Programação para Sistemas Paralelos e Distribuídos)

## Aluno:

- Gabriel Bonifácio Perez Nunes - 18/0145088

## Objetivo: criar uma aplicação MPI para ler um arquivo com mais de 10
milhões de caracteres, e identificar (i) a quantidade de palavras com menos de 6
caracteres, (ii) a quantidade de palavras que tem entre 6 e 10 caracteres, e (iii) a
quantidade de palavras com mais de 10 caracteres.

## O processo MASTER lê o arquivo em buffers de tamanho variável e
crescente, começando em 128 bytes e chegando até um certo limite.

## Entregáveis:

A) Título do experimento, dados da disciplina e dos componentes do grupo e data
da entrega

- Laboratório – Construindo aplicações distribuídas usando MPI
- FGA0244 - Programação para Sistemas Paralelos e Distribuídos
- 19.12.2022

B) Descrição da solução

 - A solução possui uma estrutura baseada em arquivos txt que é passada como parâmetro no código.
Para rodar, é necessário fazer o clone do repositório e rodar os comandos:

mpicc -o a a.c
mpirun -np 5 ./a input.txt

Falando a utilização do MPI, utilizei um vetor para que ele consiga fazer a contagem de quantas palavras menores que 6 caracteres (6 bytes), quantas palavras entre 7 e 10 (de 7 a 10 bytes) e quantas palavras acima de 10 caracteres (11 bytes +), armazenando assim na posição 0, 1 e 2, respectivamente.

Com isso, o master se encarrega de enviar os elementos divididos para os slaves de forma que eles consigam se dividir e acelerar esse processo de contagem. De início, não iria utilizar o MPI_REDUCE, mas ele se serviu essencial para que eu pudesse passar o valor que havia conseguido para uma outra variável quando ele saísse do processo dos slaves.

Em relação ao buffer, o iniciei em 128 (na verdade foi 32, mas antes de iniciar ele é multiplicado por 4) e fui multiplicando ele por 4 para que esse valor aumentasse e cada vez mais ele conseguisse fazer a contagem das linhas de maneira mais ampla.

Ao final, quando ele atinge um limite de leitura, ele volta para o buffer de 128, faz a leitura novamente e só então finaliza o MPI.

C) Encontrei uma certa dificuldade para iniciar esse laboratório. Rever o conteúdo de MPI e algumas soluções prévias foi fundamental para que eu relembrasse como abordar toda esse estrutura. De um lado, gostei de fazer esse laboratório justamente porque (ao meu ver) consegui realizar o que foi pedido e revisei um conteúdo que já havia estudado no início da disciplina. Realizar em uma máquina me deu a oportunidade de fazer isso mais rapidamente, mais por conta desse tratamento que consegui dar a apenas 1 arquivo (em comunhão com o .txt) e fazer essa solução rodar.

Uma dificuldade encontrada foi que tive problemas de compilação na minha máquina que não consegui resolver. Minha solução foi trocar de máquina para que pudesse compilar e rodar o código, a fim de testar minha solução a cada vez que o alterava.
