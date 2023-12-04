const int LED_PIN       = 25;
const int PUSH_BUTTON_PIN = 18;

int counter = 0;

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

void IRAM_ATTR EnableLed()
{
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    ++counter;
    digitalWrite(LED_PIN, not digitalRead(LED_PIN));
    last_button_time = button_time;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(PUSH_BUTTON_PIN, EnableLed, HIGH);
}

void loop()
{
  Serial.print("Counter: ");
  Serial.println(counter);
  delay(2000);
}

//Instead this code we can use attachInterrupt
/*int enabledLed = 0;
void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(PUSH_BUTTON_PIN);
  Serial.println(value);
  if(value == 0)
  {
    if(enabledLed == 0)
    {
      digitalWrite(LED_PIN, 1);
      enabledLed = 1;
    }
    else{
      digitalWrite(LED_PIN, 0);
      enabledLed = 0;
    }    
    //digitalWrite(LED_PIN, not digitalRead(LED_PIN));
  }
  delay(200);
}*/
