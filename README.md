---

# 🌡️ Monitor de Pressão Ambiental com BMP180 e Feedback Interativo

Este projeto apresenta um sistema autônomo de monitoramento de pressão e temperatura utilizando o sensor BMP180, complementado por um feedback visual dinâmico através de LEDs e um alerta sonoro ajustável via potenciômetro. Ideal para aplicações que requerem acompanhamento local de condições atmosféricas sem dependências de rede.

---

## 🚀 Funcionalidades Principais

* **Leitura Precisa:** Obtém dados de temperatura, pressão (em Pascais) e calcula a altitude aproximada.
* **Feedback Visual Intuitivo:** Seis LEDs indicadores que se acendem progressivamente para representar diferentes faixas de pressão atmosférica, permitindo uma visualização rápida das condições.
* **Alerta Sonoro Ajustável:** Um buzzer que emite um aviso sonoro quando a pressão excede um limite pré-definido, com a frequência do som controlada por um potenciômetro, permitindo personalizar a intensidade do alerta.
* **Saída Serial Detalhada:** Exibe todas as leituras do sensor e informações de status no Monitor Serial para fácil depuração e registro.
* **Sistema Autônomo:** Projetado para operar independentemente, sem a necessidade de conexão com Wi-Fi ou dashboards externos.

---

## ⚙️ Hardware Necessário

* **Placa Microcontroladora:** ESP32 (ou compatível com a biblioteca Adafruit BMP085)
* **Sensor:** BMP180 (ou BMP085)
* **LEDs:** 6x LEDs de diferentes cores (ex: 2x verdes, 2x amarelos, 2x vermelhos)
* **Resistores:** Resistores adequados para os LEDs (geralmente 220-330 ohms)
* **Buzzer:** 1x Buzzer passivo
* **Potenciômetro:** 1x Potenciômetro (10k ohms recomendado)
* **Jumpers e Protoboard:** Para conexões

---

## 🔌 Esquema de Conexão (Fritzing / Diagrama Básico)

*(Nesta seção, você idealmente incluiria uma imagem de um diagrama de circuito, por exemplo, feito no Fritzing, ou uma descrição em texto simples das conexões.)*

**Exemplo de Conexões (Adaptar conforme sua placa e pinagem):**

* **Sensor BMP180:**
    * SCL $\leftrightarrow$ GPIO 22 (ESP32)
    * SDA $\leftrightarrow$ GPIO 21 (ESP32)
    * VCC $\leftrightarrow$ 3.3V (ESP32)
    * GND $\leftrightarrow$ GND (ESP32)
* **LEDs (com resistores em série):**
    * `ledVerd` $\leftrightarrow$ GPIO 13
    * `ledVerdDOIS` $\leftrightarrow$ GPIO 12
    * `ledAmar` $\leftrightarrow$ GPIO 14
    * `ledAmarDOIS` $\leftrightarrow$ GPIO 27
    * `ledVerm` $\leftrightarrow$ GPIO 26
    * `ledVermDOIS` $\leftrightarrow$ GPIO 25
* **Buzzer:**
    * Pino Positivo $\leftrightarrow$ GPIO 23
    * Pino Negativo $\leftrightarrow$ GND
* **Potenciômetro:**
    * Pino Lateral 1 $\leftrightarrow$ 3.3V (ESP32)
    * Pino Central (Wiper) $\leftrightarrow$ GPIO 34 (Pino Analógico)
    * Pino Lateral 2 $\leftrightarrow$ GND (ESP32)

---

## 💻 Como Usar

1.  **Instale as Bibliotecas:**
    * No Arduino IDE, vá em `Sketch > Incluir Biblioteca > Gerenciar Bibliotecas...` e instale:
        * `Adafruit BMP085 Library`
        * `Wire` (geralmente já vem instalada)
2.  **Conecte o Hardware:** Siga o esquema de conexão acima para montar seu circuito.
3.  **Carregue o Código:**
    * Abra o arquivo `.ino` fornecido neste repositório no Arduino IDE.
    * Selecione sua placa (ex: `ESP32 Dev Module`) e a porta serial correta em `Ferramentas > Placa` e `Ferramentas > Porta`.
    * Faça o upload do código para sua placa.
4.  **Monitore:** Abra o `Monitor Serial` (Ctrl+Shift+M) no Arduino IDE e configure a taxa de comunicação para `9600 bps` para ver as leituras em tempo real e o status dos LEDs/buzzer.

---

## 📈 Lógica de Funcionamento dos LEDs

Os LEDs são ativados com base nos seguintes limiares de pressão (em Pascais - Pa):

* **1 LED Verde:** Pressão entre 100500 Pa e 101499 Pa
* **2 LEDs Verdes:** Pressão entre 101500 Pa e 101999 Pa
* **2 LEDs Verdes + 1 LED Amarelo:** Pressão entre 102000 Pa e 102249 Pa
* **2 LEDs Verdes + 2 LEDs Amarelos:** Pressão entre 102500 Pa e 102749 Pa
* **2 LEDs Verdes + 2 LEDs Amarelos + 1 LED Vermelho:** Pressão entre 102750 Pa e 102999 Pa
* **2 LEDs Verdes + 2 LEDs Amarelos + 2 LEDs Vermelhos:** Pressão entre 103000 Pa e 103249 Pa

---

## 🔊 Lógica do Buzzer

O buzzer será ativado apenas quando a pressão for **superior a 103000 Pa**. A frequência do som (e, portanto, seu "tom") pode ser ajustada girando o potenciômetro conectado ao pino analógico.

---

## 🤝 Contribuições

Sinta-se à vontade para abrir issues, sugerir melhorias ou enviar Pull Requests. Toda contribuição é bem-vinda!

---

Espero que este `README.md` seja o "top" que você estava procurando! Ele cobre os pontos essenciais de forma clara e envolvente.
