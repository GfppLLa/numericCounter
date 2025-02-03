# NumericCounter

## Descrição

Este projeto foi desenvolvido para a placa **Pico** utilizando a placa **BitDogLab**, como parte do programa **Embarcatech** do **CEPEDI** (Centro de Estudos e Pesquisa em Desenvolvimento de Software Embarcado e Sistemas de Tempo Real). O objetivo principal do projeto é criar um contador numérico com funcionalidades avançadas de controle de LEDs, utilizando a programação baseada em interrupções no microcontrolador RP2040.

O contador numérico vai de 9 a 0 em um loop contínuo, com alternância de cores nos LEDs WS2812 conectados à placa, gerando um efeito visual dinâmico e interessante. O LED vermelho no pino 13 pisca continuamente, conforme a solicitação da atividade original.

### Funcionalidades

- **Pinos Utilizados:**
  - **Pino 5:** Botão para incremento do número (acréscimo).
  - **Pino 6:** Botão para decremento do número (decréscimo).
  - **Pino 22:** Função adicional para reiniciar a placa no **modo bootloader**, permitindo a reprogramação da placa via USB. Esta funcionalidade não foi requerida originalmente, mas foi adicionada como um recurso extra.
  - **Pino 7:** Controle da matriz de LEDs **WS2812**, com a exibição de uma sequência de cores alternando enquanto o número do contador é exibido. Cada LED do array pode assumir diferentes cores, criando um efeito visual tipo "mosaico".
  - **Pino 13 (LED vermelho):** Pisca continuamente enquanto o programa está em execução, conforme a especificação da atividade original.

- **Comportamento Adicional (minha contribuição):**
  - O número exibido no display alterna de 9 para 0 e continua em um loop contínuo. A contagem é cíclica, o que significa que quando o número chega a 0, ele volta para 9 e assim por diante.
  - O comportamento visual dos LEDs é modificado para criar um efeito "mosaico", no qual a alternância de cores não se limita apenas ao número exibido, mas também faz os LEDs piscarem de maneira interessante.

### Como Funciona o Sistema

1. **Botões de Controle:**
   - O botão no **Pino 5** é utilizado para aumentar o número exibido no display, incrementando de 1 em 1. O valor do número nunca excede 9 devido à operação cíclica `(numero_display + 1) % 10`.
   - O botão no **Pino 6** serve para diminuir o número, mas o valor nunca será inferior a 0. Quando o número está em 0, pressionar o botão vai "loopar" o valor de volta para 9.
   - O **Pino 22** entra no modo **bootloader** quando pressionado. Isso é útil para reprogramar a placa sem a necessidade de desconectar o dispositivo, permitindo um desenvolvimento ágil.

2. **LEDs e Efeitos Visuais:**
   - O **LED vermelho** no **Pino 13** pisca continuamente enquanto o sistema está em execução, indicando que o contador está ativo.
   - O **array de LEDs WS2812** (conectado ao **Pino 7**) muda de cor a cada atualização do número exibido. Isso cria um efeito visual dinâmico, com as cores alternando de forma sincronizada com a contagem numérica. 

3. **Contagem Cíclica:**
   - O número no display gira entre 9 e 0 sem parar. Isso cria um efeito contínuo, o que é útil em aplicações de contagem onde o valor precisa se manter em constante atualização.

4. **Adição de Funcionalidades Extras:**
   - **Modo Bootloader:** O uso do **Pino 22** para reiniciar a placa no modo bootloader é uma funcionalidade adicional que facilita o processo de reprogramação do dispositivo. Ao pressionar esse botão, a placa reinicia e entra no modo de carregamento de firmware via USB.
   - **Efeito Mosaico nos LEDs:** A alternância das cores no array de LEDs foi implementada de forma a criar um "mosaico visual", onde as cores não apenas indicam o número atual, mas também geram um padrão interessante.



## Dependências

Para compilar e executar este projeto, você precisará das seguintes ferramentas:

- **Placa Raspberry Pi Pico** ou **Placa BitDogLab**.
- **Ambiente de desenvolvimento para RP2040** (CMake e GCC).
- **Bibliotecas necessárias**: As bibliotecas para controle dos LEDs WS2812 e manipulação de GPIO (pico-sdk).

### Como Compilar

Para compilar o código, siga os passos abaixo:

1. Instale o SDK do **PICO** e configure o ambiente de desenvolvimento com CMake.
2. Baixe ou clone o repositório para o seu diretório de trabalho.
3. Use o comando `cmake` para gerar os arquivos de build:

    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. Após a compilação, o binário gerado pode ser carregado diretamente na placa via USB (se o **Pino 22** for pressionado, a placa entrará no modo bootloader).

5. Acesse o dispositivo no seu computador e carregue o código compilado.

### Contato

- **Aluno:** Gleison Fonseca de Paula


---
### Licença

Este projeto está licenciado sob a **Licença GNU**.

<div align="center">
    GNUs not Unix
</div>
