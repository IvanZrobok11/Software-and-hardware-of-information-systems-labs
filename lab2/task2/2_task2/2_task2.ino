const int LIGHT_SENSOR_PIN = 4;
const int LED_PIN = 25;

hw_timer_t *my_night_timer = NULL;
hw_timer_t *my_day_timer = NULL;

void IRAM_ATTR OnNightTimer();
void IRAM_ATTR OnDayTimer();

void SetupNightTimer();
void SetupDayTimer();

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Встановлення піна LED як вихід
  digitalWrite(LED_PIN, LOW); // Встановлення початкового стану LED (низький рівень)
  Serial.begin(115200); 
  SetupNightTimer();
  SetupDayTimer();  
}

void loop()
{
}

void IRAM_ATTR OnNightTimer()
{
  int analog_value = analogRead(LIGHT_SENSOR_PIN);
  if(analog_value < 1000)
  {
    digitalWrite(LED_PIN, 1);
  }
}
void IRAM_ATTR OnDayTimer()
{
  int analog_value = analogRead(LIGHT_SENSOR_PIN);
  if(analog_value >= 1000)
  {
    digitalWrite(LED_PIN, 0);
  }
}

void SetupNightTimer()
{
   // Створення та налаштування таймера
  my_night_timer = timerBegin(0, 80, true); // Використання таймера 0, ділення 80 (стандартні налаштування для ESP32)
  timerAttachInterrupt(my_night_timer, &OnNightTimer, true); // Прикріплення функції onTimer до таймера
  timerAlarmWrite(my_night_timer, 1000000, true); // Встановлення інтервалу таймера на 1 секунду (в мікросекундах)
  timerAlarmEnable(my_night_timer); // Увімкнення таймера
}

void SetupDayTimer()
{
  my_day_timer = timerBegin(1, 80, true);
  timerAttachInterrupt(my_day_timer, &OnDayTimer, true);
  timerAlarmWrite(my_day_timer, 1000000, true);
  timerAlarmEnable(my_day_timer);
}



