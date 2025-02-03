# **numericCounter** - Projeto EmbarcaTech: Controle de Display com Efeitos de LED

## Descrição

Este projeto foi desenvolvido utilizando a placa **BitDogLab**, baseada no **Raspberry Pi Pico**, como parte do programa **EmbarcaTech** do **CEPEDI**. O objetivo é criar um sistema embarcado com controle de LEDs e interação com o usuário através de botões.

## Funcionalidades

- **Contagem Cíclica**: O valor exibido no display gira em círculos de 9 a 0, sem parar.
- **Efeitos de LED**: O array de LEDs WS2812, conectado ao pino 7, é usado para criar efeitos visuais interessantes com variação de cores.
- **LED Vermelho Piscando**: O LED vermelho no GPIO 13 pisca continuamente para indicar atividade.

## Configuração de Pinos

- **Botão de Acréscimo**: Pino 5 (aumenta o número exibido)
- **Botão de Decréscimo**: Pino 6 (diminui o número exibido)
- **Botão de Bootloader**: Pino 22 (entra em modo bootloader)
- **LEDs WS2812**: Pino 7 (controle de LEDs RGB)
- **LED Vermelho**: GPIO 13 (piscando continuamente)

## Como Funciona

O programa permite interagir com o sistema por meio de três botões:
- **Botão A (Pino 5)**: Aumenta o valor exibido no display de 0 a 9, girando continuamente.
- **Botão B (Pino 6)**: Diminui o valor exibido no display de 9 a 0.
- **Botão C (Pino 22)**: Entra no modo bootloader para reiniciar o sistema.

Enquanto isso, os LEDs WS2812 são usados para criar efeitos visuais, alterando suas cores dinamicamente.

## Requisitos

- **Placa BitDogLab** com Raspberry Pi Pico
- **LEDs WS2812**
- **Botões de Entrada**
- **Ambiente de Desenvolvimento**: SDK do Raspberry Pi Pico

## Autor

**Aluno**: Gleison Fonseca de Paula

## Como Usar

1. Conecte os componentes de acordo com a configuração de pinos mencionada acima.
2. Compile e carregue o código na placa BitDogLab.
3. Interaja com o sistema utilizando os botões de acréscimo, decréscimo e bootloader.
4. Observe os efeitos visuais dos LEDs.

Para mais informações sobre o programa EmbarcaTech e o CEPEDI, consulte as fontes oficiais:

[EmbarcaTech CEPEDI](https://www.cepedi.org.br/)

## Licença

Este projeto está licenciado sob a **GNU General Public License**.

---

<center>GNU's Not Unix</center>
