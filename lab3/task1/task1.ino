#include "FS.h"
#include "SPIFFS.h"

const int LIGHT_SENSOR_PIN = 4;
const int LED_PIN = 25;

hw_timer_t *my_night_timer = NULL;
hw_timer_t *my_day_timer = NULL;

bool is_day = false;

void IRAM_ATTR OnNightTimer();
void IRAM_ATTR OnDayTimer();

void SetupNightTimer();
void SetupDayTimer();

void ReadFile ();
void WriteFile();

void ConvertDouble8BitTo12Bit(unsigned char high_byte, unsigned char low_byte, int &analog_value);
void Convert12BitToDouble8Bit(int analog_value, unsigned char &high_byte, unsigned char &low_byte);

void BeginSPIFFS ();
void FormatSPIFFS();

void setup()
{
  pinMode(LED_PIN, OUTPUT); // Встановлення піна LED як вихід
  digitalWrite(LED_PIN, LOW); // Встановлення початкового стану LED (низький рівень)

  Serial.begin(115200); 
  while(not Serial)
  {
    delay(100);
  }
  BeginSPIFFS();
  FormatSPIFFS();

  SetupNightTimer();
  SetupDayTimer();    
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
  timerAlarmWrite(my_day_timer, 2000000, true);
  timerAlarmEnable(my_day_timer);
}

void IRAM_ATTR OnNightTimer()
{
  int analog_value = analogRead(LIGHT_SENSOR_PIN);
  if(analog_value < 1000)
  {
    digitalWrite(LED_PIN, 1);//enable led
    is_day = false;
  }
  else{
    is_day = true;
  }
}
void IRAM_ATTR OnDayTimer()
{
  int analog_value = analogRead(LIGHT_SENSOR_PIN);
  if(analog_value >= 1000)
  {
    digitalWrite(LED_PIN, 0);//disable led
    is_day = true;
  }
  else{
    is_day = false;
  }
}


void loop()
{
  //WriteFile();
  if (is_day) {
    WriteFile();
    ReadFile();
    delay(500);
  }
}

void Convert12BitToDouble8Bit(int analog_value, unsigned char &high_byte, unsigned char &low_byte)
{
  high_byte = (analog_value >> 8) & 0xFF;
  low_byte  = analog_value & 0xFF;
}

void ConvertDouble8BitTo12Bit(unsigned char high_byte, unsigned char low_byte, int &analog_value)
{
  analog_value = (high_byte << 8) | low_byte;
}

void WriteFile()
{
  if(is_day)
  {
    File file_write = SPIFFS.open("/test.txt", "w");
    if(!file_write)
    {
      Serial.println("Failed to open file_write for reading");
      return;
    }
    else
    {
      Serial.println("Successfully to open file_write for writing");
    }

    const int analog_value = analogRead(LIGHT_SENSOR_PIN);
    Serial.print("Write: ");
    Serial.println(analog_value);

    unsigned char high_byte = 0U;
    unsigned char low_byte  = 0U;
    Convert12BitToDouble8Bit(analog_value, high_byte, low_byte);

    file_write.write(high_byte);
    file_write.write(low_byte);

    file_write.close();
  }
}

void ReadFile()
{
  if(is_day)
  {
    File file_read = SPIFFS.open("/test.txt", "r");
    if(!file_read)
    {
      Serial.println("Failed to open file_read for reading");
      return;
    }
    else
    {
      Serial.println("Successfully to open file_read for reading");
    }

    const unsigned char high_byte = file_read.read();
    const unsigned char low_byte  = file_read.read();

    int analog_value = 0;
    ConvertDouble8BitTo12Bit(high_byte, low_byte, analog_value);

    Serial.print("Read: ");
    Serial.println(analog_value);

    file_read.close();
  }
}

void BeginSPIFFS()
{
  if(SPIFFS.begin(true))
  {
    Serial.println("Successfully begin SPIFFS");
  }
  else
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void FormatSPIFFS()
{
  bool formatted = SPIFFS.format();
  if(formatted)
  {
    Serial.println("SPIFFS formatted successfully");
  }
  else
  {
    Serial.println("Error formatting");
    return;
  }
}

