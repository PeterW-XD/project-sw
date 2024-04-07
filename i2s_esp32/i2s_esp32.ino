#include <driver/i2s.h>

// Buttons
#define BUTTON 13

// Connections to INMP441 I2S microphone
#define I2S_WS 5
#define I2S_SD 18
#define I2S_SCK 19
 
#define I2S_WS2 16
#define I2S_SD2 17
#define I2S_SCK2 21

// Use I2S Processor 0： there are two of them
#define I2S_PORT I2S_NUM_0
#define I2S_PORT2 I2S_NUM_1

// Define input buffer length
#define bufferLen 64
int16_t sBuffer[bufferLen]; // left + right + ... output format
int16_t sBuffer2[bufferLen];

// const int duration = 3000; // sampling for 3 sec

void i2s_install() {
  // Set up I2S Processor configuration
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,       
    .dma_buf_len = bufferLen, // dma_buffer_size = dma_buf_len * slot_num * data_bit_width / 8 <= 4092
    .use_apll = false
  };
 
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_driver_install(I2S_PORT2, &i2s_config, 0, NULL);
}
 
void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  const i2s_pin_config_t pin_config2 = {
    .bck_io_num = I2S_SCK2,
    .ws_io_num = I2S_WS2,
    .data_out_num = -1,
    .data_in_num = I2S_SD2
  };
 
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_set_pin(I2S_PORT2, &pin_config2);
}
 
void setup() {
 
  // Set up Serial Monitor
  Serial.begin(115200);
  Serial.println(" ");
 
  delay(1000);
 
  // Set up buttons
  pinMode(BUTTON, INPUT_PULLUP);

  // Set up I2S
  i2s_install();
  i2s_setpin();
  i2s_start(I2S_PORT);
  i2s_start(I2S_PORT2);
 
 
  delay(500);
}
 
void loop() {
 
  // Get I2S data and place in data buffer
  size_t bytesIn = 0;
  size_t bytesIn2 = 0;

  // Buttons
  if (!digitalRead(BUTTON)) {
    unsigned long startTime = millis();
    // while (millis() - startTime < duration) {}

    // False print statements to "lock range" on serial plotter display
    // Change rangelimit value to adjust "sensitivity"
    int rangelimit = 100;
    Serial.print(rangelimit * -1);
    Serial.print(" ");
    Serial.print(rangelimit);
    Serial.print(" ");

    esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);
    esp_err_t result2 = i2s_read(I2S_PORT2, &sBuffer2, bufferLen, &bytesIn2, portMAX_DELAY);
  
    if (result == ESP_OK && result2 == ESP_OK)
    {
      // Read I2S data buffer
      int16_t samples_read = bytesIn / 8;
      if (samples_read > 0) {
        float mean1L = 0;
        float mean1R = 0;
        float mean2L = 0;
        float mean2R = 0;
        for (int16_t i = 0; i < samples_read; ++i) {
          if (i % 2) {
            mean1R += (sBuffer[i]);
            mean2R += (sBuffer2[i]);
          } else {
            mean1L += (sBuffer[i]);
            mean2L += (sBuffer2[i]);
          }
          //Serial.println(sBuffer[i]);
        }
  
        // Average the data reading
        mean1L /= (samples_read / 2);
        mean1R /= (samples_read / 2);
        mean2L /= (samples_read / 2);
        mean2R /= (samples_read / 2);
  
        // Print to serial plotter
        Serial.print(mean1L);
        Serial.print(" ");
        Serial.print(mean1R);
        Serial.print(" ");
        Serial.print(mean2L);
        Serial.print(" ");
        Serial.println(mean2R);

        unsigned long stopTime = millis();
      }
    }
  }
  Serial.println(stopTime - startTime);
}