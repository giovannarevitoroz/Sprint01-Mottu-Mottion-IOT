
---

# Mottion - Rastreamento Inteligente de Motos com IoT

## Integrantes

| RM     | Nome Completo                        |
| ------ | ------------------------------------ |
| 558981 | Giovanna Revito Roz                  |
| 558986 | Kaian Gustavo de Oliveira Nascimento |
| 554424 | Lucas Kenji Kikuchi                  |

[![Assista ao vídeo demonstrativo](https://img.youtube.com/vi/bBhhPKVa5ZM/0.jpg)](https://youtu.be/bBhhPKVa5ZM)

## Descrição

O Mottion nasce como uma resposta tecnológica aos desafios enfrentados pela Mottu na gestão física de pátios de motos em suas mais de 100 unidades no Brasil. Cada filial possui layout e fluxo operacional distintos, dificultando:

* Localização de motos
* Registro e acompanhamento de manutenções
* Garantia de segurança contra extravios e furtos

---

## Objetivo

Automatizar e digitalizar o controle das motos no pátio usando uma solução baseada em IoT, com:

* Sensores ESP32 conectados por Wi-Fi
* Comunicação via MQTT
* Controle remoto de status via LEDs coloridos
* Interface de monitoramento com Node-RED

---

## Arquitetura da Solução

### Comunicação IoT

Cada setor possui Wi-Fi dedicado. Cada sensor ESP32:

* Monitora seu setor de forma independente
* Garante redundância e resiliência
* Suporta expansão modular

### Comunicação MQTT

* Broker: `broker.hivemq.com`
* Tópico de publicação: `iot/mottu-mottion/sensormo`
* Tópico de controle de LEDs: `iot/mottu-mottion/statusled`
* Protocolo: MQTT sobre TCP (porta 1883)

#### Exemplo de mensagem publicada:

```json
{
  "id_sensor": "01111",
  "id_moto": "45124",
  "setor": "PATIO",
  "observacao": "Agendada para manutenção",
  "timestamp": 64138
}
```

---

## Funcionamento do Código (ESP32)

### Inicialização

1. Conecta ao Wi-Fi do setor
2. Estabelece conexão com o broker MQTT
3. Configura os pinos dos LEDs

### Publicação

* A cada 2 segundos, publica o status da moto no tópico MQTT

### Recebimento

* Recebe comandos no tópico `statusled` com a cor do LED
* Aciona o LED correspondente
* Publica uma nova observação com base na cor

---

## Mapeamento das Cores dos LEDs

| Cor do LED   | Observação Publicada     |
| ------------ | ------------------------ |
| Vermelho     | Danos Estruturais        |
| Azul         | Reparos Simples          |
| Verde Claro  | Minha Mottu              |
| Verde Escuro | Pronta para aluguel      |
| Amarelo      | Pendência                |
| Cinza        | Agendada para manutenção |
| Roxo         | Sem placa                |
| Bordô        | Motor com defeito        |

---

## Componentes Utilizados

| Componente      | Função                                |
| --------------- | ------------------------------------- |
| ESP32-S2        | Microcontrolador principal            |
| LEDs coloridos  | Indicadores visuais de status         |
| Broker MQTT     | Comunicação entre sensores e Node-RED |
| Wi-Fi por setor | Isolamento de rede física             |

---

## Infraestrutura de Rede

Cada setor do pátio possui rede Wi-Fi dedicada. Os sensores ESP32 se conectam a essas redes e comunicam com o broker MQTT público para garantir envio constante de dados.

---

## Integração com Node-RED

### Componentes da Interface

* MQTT In: Recebe dados do tópico `iot/mottu-mottion/sensormo`
* UI Text: Exibe dados formatados
* Debug: Visualiza dados em tempo real
* MQTT Out: Envia comandos para `statusled`

---

## Instalação e Execução do Node-RED

### Pré-requisitos

* Ter o Node.js instalado (versão recomendada: 14.x ou superior)
* Ter o npm (gerenciador de pacotes do Node.js) disponível

### Instalação do Node-RED

Abra o Prompt de Comando (CMD) e execute:

```bash
npm install -g --unsafe-perm node-red
```

### Executando o Node-RED

No CMD, rode o comando:

```bash
node-red
```

O Node-RED iniciará e exibirá no console a URL para acessar a interface, geralmente:

```
http://127.0.0.1:1880/
```

Abra essa URL no seu navegador.

### Importando o fluxo do projeto

1. Na interface do Node-RED, clique no menu (ícone de três linhas no canto superior direito).
2. Selecione Importar > Arquivo.
3. Escolha o arquivo `Node-RED-Mottu-Mottion.json` no seu computador.
4. Clique em Importar para carregar o fluxo do projeto.
5. Clique em Deploy (botão no canto superior direito) para ativar o fluxo.

---

## Resultados Esperados

* Rastreabilidade total da moto
* Visualização de status em tempo real
* Redução de erros manuais
* Detecção antecipada de problemas
* Facilidade de expansão por setor

---

## Escalabilidade

Projeto pensado para crescimento horizontal:

* Replicação da infraestrutura Wi-Fi
* Replicação da configuração dos ESP32
* Utilização padronizada de tópicos MQTT

---

## Demonstração

Assista ao vídeo demonstrativo clicando na imagem abaixo:

[![Assista ao vídeo demonstrativo](https://img.youtube.com/vi/bBhhPKVa5ZM/0.jpg)](https://youtu.be/bBhhPKVa5ZM)

---
