#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  5        //Time ESP32 will go to sleep (in seconds)

RTC_DATA_ATTR int bootCount = 0;

#define RREF 47000  //47k NTC
#define BVALUE 3950  // from datasheet
float fRntc = 0;
float fTemp = 0, teplotaMot = 0, teplotaReg = 0;


void setup() 
{
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.flush();
  delay(10); //Take some time to open up the Serial Monitor
  ++bootCount;
  print_wakeup_reason();

  //Set timer to 5 seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12,1);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");
  
   //blinkLed();
  readTemp();
  Serial.println(testPerformance());

  Serial.end();
  
  //Go to sleep now
  esp_deep_sleep_start();

}

void readTemp()
{
  fRntc = 10000.0f / (1023.0f / (float)analogRead(14) - 1.0f) ;
  fTemp = (1.0f / ( (log(fRntc / RREF)) / BVALUE  + 1.0f / 298.0f)) - 273.0f; //log = ln
  teplotaMot = fTemp;
}

void blinkLed()
{
  digitalWrite(2,0);
}

long unsigned int testPerformance()
{
  long unsigned int current_time = micros();
   
  double fdata[1024*5] = {0};

  for(size_t i = 0; i < sizeof(fdata)/sizeof(fdata[0]); ++i)
  {
    for(size_t j = 0; j < 1;++j)
      if(i > 0)
      {
        fdata[i-j] = (float)i*j;
        fdata[i] = (float)i*j;
        fdata[i+j] = (float)i*j;
      }
      
  }

  return (micros() - current_time);
  
}

void loop() 
{
  
 
}

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
   Serial.println("\n");
  switch(wakeup_reason)
  {
    case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2  : Serial.println("Sending data"); break;
    case 3  : Serial.println("Wakeup caused by timer"); break;
    case 4  : Serial.println("Reading data"); break;
    case 5  : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}
