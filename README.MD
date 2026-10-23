# Vinheria Agnello - Monitoramento IoT

## ★ Descrição do Projeto

Este projeto tem como objetivo monitorar as condições ambientais (luminosidade, umidade, temperatura) de uma adega de vinhos da Vinheria Agnello, garantindo que o armazenamento das garrafas ocorra nas condições ideais de temperatura, umidade e luminosidade. O sistema utiliza dados de sensores através de um ESP32 virtual e transmiti-los em tempo real via protocolo MQTT no padrão FIWARE

## 👥 Integrantes do Grupo

Amanda Silva RM: 572585 |
Beatriz Mantovani RM: 573824 |
Laura Sampaio RM: 570802

## ★ Tecnologias e Componentes Utilizados

- **Simulador:** Wokwi
- **Microcontrolador:** ESP32
- **Sensores:** DHT22 (Temperatura e Umidade) e LDR (Luminosidade)
- **Protocolo de Comunicação:** MQTT
- **Broker MQTT:** `broker.emqx.io` (Público)
- **Clientes MQTT:** MyMQTT (Mobile) / MQTTX (Web)

## ★ Tópicos MQTT e Padrão de Dados (FIWARE)

- **Publicação de Dados (ESP32 -> Broker):** `/TEF/device080/attrs`
  - Formato dos dados (Ultralight): `t|valor|h|valor|l|valor|st|STATUS`
- **Recebimento de Comandos (Broker -> ESP32):** `/TEF/device080/cmd`

## ★ Instruções de Execução

1. **Acessar o Wokwi:** Abra o link do projeto no Wokwi: `https://wokwi.com/projects/478592695408235521` e clique no botão de "Play" para iniciar a simulação
2. **Conectar ao Broker MQTT:**
   - Abra o aplicativo **MyMQTT**
   - Configure o host para `broker.emqx.io` e conecte-se na porta `1883`
3. **Visualizar as Leituras:** Na aba de _Subscribe_, assine o tópico `/TEF/device080/attrs`. Os dados da adega começarão a chegar a cada 5 segundos

## ★ Demonstração Prática

O vídeo com a demonstração do funcionamento do sistema, integrando o Wokwi e o cliente MQTT, pode ser acessado no link abaixo:
`https://www.youtube.com/watch?v=ri9vZTRPWro`
