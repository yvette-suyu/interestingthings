//#include <EEPROM.h>
    //ds18b20总线设置
#include <OneWire.h> 
OneWire  ds(10);  // 连接arduino10引脚
    //定义光敏电阻模拟输入接口pin2
int potpin=2;
int val;// 定义变量
    //定义旋钮
int MBPin = 0;
int MGPin = 1;
int MRPin = 3; 
int valuer = 0;  //定义旋钮变量
int valueg = 0;  
int valueb = 0; 
  //设置手动自动切换参
int SWPin=11;
int Flag = 0;
int PrefPin=12;
  //红外热释
int Sensor_pin = 5;
int SensorState=0;//存储传感器状态

void setup(void) {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(Sensor_pin,INPUT); //设置人体红外接口为输入状态
}
 
void loop(void) {
  val=analogRead(potpin);//读取光照模拟接口的值
 Serial.println(val);//输出模拟接口的值

    //ds18b20  配置
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
   
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
   
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
 
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
  
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
   
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
   
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
 
  Serial.print("  Data = ");
  Serial.print(present,HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();//read data
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();
 
  // convert the data to actual temperature
 
  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");   
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  Flag = digitalRead(SWPin);
  Serial.print("FLAG IS:"); 
  Serial.println(Flag); 

    // pwm lamp
SensorState=digitalRead(Sensor_pin); //定义参数存储人体红外传感器读到的状态
delay(100);

   // if (Flag == 1)
  if (SensorState == HIGH && Flag == 1)
// if(SensorState == HIGH) //如果检测到有动物运动（在检测范围内）亮起LED
     {
        analogWrite(7, val+16);
        analogWrite(8, val+57);
        analogWrite(9, val+69);//把brightness的值写入端口
        delay(1000);
         if(celsius<27)
            {
               analogWrite(7, val+16 - (27-celsius)/27*104);
               analogWrite(8, val+57 + (27-celsius)/27*145);
               analogWrite(9, val+69);//基准值调光
             }
          else
            {
              analogWrite(7, val+16+(celsius-27)/27*104);
              analogWrite(8, val+57 + (celsius-27)/27*145);
              analogWrite(9, val+69 );
              }
        delay(1000);
       }
    //else if( Flag == 0&& SensorState == HIGH)
    else if( Flag == 0)
     {
     digitalRead(PrefPin);
     //  if(PrefPin=1)
     //  {
     //     valueb=  EEPROM.read(0);
     //     valueg= EEPROM.read(1);
     //     valuer= EEPROM.read(2);    
    //    }  
    //     else
    //     {          
          valuer = analogRead(MRPin);  
          valueg = analogRead(MGPin); 
          valueb = analogRead(MBPin);
    //      }
           
        Serial.println("r:");  
        Serial.println(valuer);  
        Serial.println("g:"); 
        Serial.println(valueg);
        Serial.println("b:"); 
        Serial.println(valueb); 
        Flag = digitalRead(SWPin);  
        analogWrite(7,valueb);
        analogWrite(8,valueg);
        analogWrite(9,valuer);
         //    EEPROM.write(0,valueb);
        //     EEPROM.write(1,valueg);
         //    EEPROM.write(2,valuer); 
        delay(100);
       } 
   // else
    // {
   //    digitalWrite(7,LOW);
    //   digitalWrite(8,LOW);
    //   digitalWrite(9,LOW);
   //    }

}
