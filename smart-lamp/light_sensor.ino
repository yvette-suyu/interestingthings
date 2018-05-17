int light_sensorPin = 2; 
int value = 0;  
void setup()  
  { 
    Serial.begin(9600);  
   } 
 void loop()  
  {
    value = analogRead(light_sensorPin);  
    Serial.println(value);  
    delay(100);  
    } 
