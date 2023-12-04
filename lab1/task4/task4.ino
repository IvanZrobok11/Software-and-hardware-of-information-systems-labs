const byte led_gpio = 25; // the PWM pin the LED is attached to


// the setup routine runs once when you press reset:
void setup() {
  ledcAttachPin(led_gpio, 0); // assign a led pins to a channel

  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(0, 4000, 8); // 12 kHz PWM, 8-bit resolution
}

// the loop routine runs over and over again forever:
void loop() {
  for(int i = 0; i < 255; i+=5)
  {
     ledcWrite(0, i); 
     delay(5);
  }
  ledcWrite(0, 0); 
}