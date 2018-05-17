// 初始连接在单总线上的单总线设备
#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS 2  //定义ds18b20接口
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int rb=0;
int gb=0;
int bb=0;
int standard=10;
float ab=0;
float at=0;
int abb=0;
int arb=0;
int agb=0;

float t = 0; //temperature
int val=0;

int photocellPin = 2; 

int b = 0;    //定义整数型变量brightness与其初始值，此变量用来表示LED的亮度。
int val1=0;


void setup() 
{  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);// 设置9号口为输出端口
  sensors.begin(); 
} 

void loop()  {

  
 sensors.requestTemperatures();
 val=sensors.getTempCByIndex(0);//获取温度
 val1= analogRead(photocellPin); //获取环境光信息              
 //修改亮度
  
  if (ab>0)
    {
      b = b- ab*10; 
    }  
  else if(ab<0)
  {
      b = b+ ab*10;
    }
  else{b=b;}
  //调色温

  if (at>0)
    {
     arb = -at*2;
    }  
  else if(at<0)
  {
     abb = at*2;
    }
  else{agb=t*0.1;}
  
  t=val;
  b=val1;
  
      rb=standard + b +arb;
      gb=standard + b + agb;
      bb= standard + b + abb;
  analogWrite(7, bb);
  analogWrite(8, gb);
  analogWrite(9, rb);//把brightness的值写入端口
       

 delay(3000); //延时30毫秒                       
}
