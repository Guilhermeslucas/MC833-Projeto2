# Projeto 2 - Monitoramento de Tráfego
## Nome: Guilherme Lucas da Silva RA: 155618
## Nome: Felipe dal Mas Eulalio RA: 155299

## 1. Introdução e Objetivos
Após realizar uma série de experimentos ao longo da matéria, notamos que é extremamente simples realizar a comunição entre dois processos presentes em máquinas diferentes, por meio de protocolos de rede muito bem estabelecidos ao longo dos anos. Usando esses padrões, tivemos a responsabilidade de implementar um sistema que monitora um cruzamento, notando possíveis colisões e tentando evitá-las, por meio de instruções dada aos automóveis, que eram clientes. Essas instruções eram como "acelere" ou "freie". Caso a colisão não consiga ser evitada, então será enviada uma mensagem chamando uma ambulância para o local do acidente.

## 2. Resumo das aplicações
Nesta seção explicaremos resumidamente o funcionamento das aplicações envolvidas no projeto.

### 2.1 Cliente
Nesse projeto, adotamos cada cliente como um automóvel, que possui as seguintes características:**Id, tamanho, velocidade, posição, timestamp e direção **. Assim, obtidas essas informações via linha de comando, é montada uma mensagem com essas características e então essas informações são enviadas para o servidos decidir se vai acontecer alguma colisão e tomar alguma decisão sobre isso. Após realizar o envio da mensagem, o cliente espera para receber as instruções do servidos, sobre se deve **acelerar ou parar**. Caso a mensagem diga para o automóvel parar, ele para e epsera por 4 segundos parado, até tentar voltar a velocidade anterior. Vale ressaltar também, que no primeiro recebimento de mensagem pelo cliente, este define seu **id**, baseado na informação dada pelo servidor. 

### 2.2 Servidor

### 2.3 Script de Teste
Script desenvolvido em Python é extremamente simples e realiza a simples tarefa de executar alguns clientes com características aleatórias baseadas nos limites que definimos no trabalho. Além disso, tem a função de matar todos os clientes que esse mesmo script iniciou.

## 3. Implementação
### 3.1 Cliente
Para a implementação do código do cliente, foram necessárias algumas definições feitas no header **definitions.h**. A estrutura mais importante dessa parte do código é a **ClientMessage** estrutura feita para enviar as informações do servidor ao cliente. Esse possui os seguintes atributos:   

* type: tipo da mensagem (conforto, segurança, entretenimento)
* id: identificador único para o automóvel que está enviando a mensagem 
* size: tamanho do automóvel em questão
* speed: velocidade do automóvel
* position: posição que o automóvel se encontra no mapa
* direction: informação para saber se o carro está se movendo na vertical ou horizontal
* message: mensagem que está enviando

Após obtidos esses campos pela linha de comando, onde o usuário passa os valores como argumentos do **cliente** a estrutura de mensagem é povoado com essas informação e enviada para o servidor, bloqueando até receber a mensagem do mesmo. Para esse processo são usados as seguintes funções:   

* socket: função necessária para a criação de um socket, onde serão enviadas as mensagens
* gethostbyname: obtem o endereço do servidor a partir do nome
* connect: estabelece conexão entre o cliente e servidor
* send: envia mensagem para o servidor
* read: recebe mensagem do servidor

## 4. Testes
Criamos um simples método de testar o software de maneira automatizada. O **teste_script.py** lança clientes com valores aleatórios para testar o projeto. Para testar, basta compilar o servidor e lançá-lo. Feito isso, basta rodar os scripts usando python3 e analisar a saída.