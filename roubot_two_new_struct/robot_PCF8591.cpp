//PCF8591 头文件
#include <Arduino.h>
#include "robot_PCF8591.h"
#include "PCF8591.h"

//PCF8591 IIC接口宏定义
#define PCF8591_U11 0x4A
#define PCF8591_U12 0x48
#define PCF8591_U7  0x49
#define PCF8591_U13 0x4E
#define PCF8591_U9  0x4C
#define PCF8591_U10 0x4B

//PCF8591 IIC地址配置
PCF8591 pcf8591_U11(PCF8591_U11);
PCF8591 pcf8591_U12(PCF8591_U12);
PCF8591 pcf8591_U7(PCF8591_U7);
PCF8591 pcf8591_U13(PCF8591_U13);
PCF8591 pcf8591_U9(PCF8591_U9);
PCF8591 pcf8591_U10(PCF8591_U10);

//PCF8591初始化函数
void robot_PCF8591_begin(){

	pcf8591_U11.begin();
  pcf8591_U12.begin();
  pcf8591_U7.begin();
  pcf8591_U13.begin();
  pcf8591_U9.begin();
  pcf8591_U10.begin();
}

/*
舵机电流检测函数
pcf————PCF8591芯片选择
  U7，U9-U13  参数为7，9-13
mode————模式选择
  0为全部读取，1为单个引脚读取
value————读取引脚参数
 （全部读取模式该参数不发挥作用） 参数为0-3
该函数执行完毕通过串口输出结果
*/
int current_GET(int32_t pcf,int32_t mode,int32_t value)
{
    PCF8591 pcf8591 = NULL;
    int a0 = pcf8591.analogRead(AIN0);
    int a1 = pcf8591.analogRead(AIN1);
    int a2 = pcf8591.analogRead(AIN2);
    int a3 = pcf8591.analogRead(AIN3);

    switch (pcf)
    {
      case 7:
        pcf8591 = pcf8591_U7;
        break;
      case 9:
        pcf8591 = pcf8591_U9;
        break;
      case 10:
        pcf8591 = pcf8591_U10;
        break;
      case 11:
        pcf8591 = pcf8591_U11;
        break;
      case 12:
        pcf8591 = pcf8591_U12;
        break;
      case 13:
        pcf8591 = pcf8591_U13;
        break;
      default:
        Serial.println("[ERROR]invalid pcf");
        return -1;
        break;
    }
    if(mode==0)
    {
      PCF8591::AnalogInput ai = pcf8591.analogReadAll();
      Serial.println("[debug]current_GET all value");
      Serial.print("A0: ");
      Serial.print(ai.ain0);

      Serial.print(" A1: ");
      Serial.print(ai.ain1);

      Serial.print(" A2: ");
      Serial.print(ai.ain2);

      Serial.print(" A3: ");
      Serial.println(ai.ain3);

      return (ai.ain0 + ai.ain1 + ai.ain2 + ai.ain3)/4;
    }
    else if(mode==1)
    {
      Serial.print("[debug]current_GET pin value ");
      switch (value)
      {
        case 0:
          Serial.print("A0: ");
          Serial.println(a0);
          return a0;
          break;
        case 1:
          Serial.print("A1: ");
          Serial.println(a1);
          return a1;
          break;
        case 2:
          Serial.print("A2: ");
          Serial.println(a2);
          return a2;
          break;
        case 3:
          Serial.print("A3: ");
          Serial.println(a3);
          return a3;
          break;
        default:
          Serial.println("[ERROR]current_GET : invalid value.");
          break;
      }
    }
    else
    {
      Serial.println("[ERROR]void current_GET : mode error");
      return -1;
    }

    return 0;
}

/* 
舵机电流检测函数
pcf————PCF8591芯片选择 
  U7，U9-U13  参数为7，9-13
mode————模式选择
  0为全部读取，1为单个引脚读取
value————读取引脚参数 
 （全部读取模式该参数不发挥作用） 参数为0-3
该函数执行完毕通过串口输出结果 
*/
// void current_GET(int32_t pcf,int32_t mode,int32_t value){
//   //  PCF8591 U7
//   if(pcf==7){
//     if(mode==0){
//       PCF8591::AnalogInput ai = pcf8591_U7.analogReadAll();
//       Serial.print("A0: ");
//       Serial.print(ai.ain0);
      
//       Serial.print(" A1: ");
//       Serial.print(ai.ain1);
      
//       Serial.print(" A2: ");
//       Serial.print(ai.ain2);
      
//       Serial.print(" A3: ");
//       Serial.println(ai.ain3);
//     }
//     if(mode==1){  
//       if(value=0){
//         int a0 = pcf8591_U7.analogRead(AIN0);
//         Serial.print("A0: ");
//         Serial.println(a0);
//         }
//       if(value=1){
//         int a1 = pcf8591_U7.analogRead(AIN1);
//         Serial.print("A1: ");
//         Serial.println(a1);
//         }
//       if(value=2){
//         int a2 = pcf8591_U7.analogRead(AIN2);
//         Serial.print("A2: ");
//         Serial.println(a2);
//         }
//       if(value=3){
//         int a3 = pcf8591_U7.analogRead(AIN3);
//         Serial.print("A3: ");
//         Serial.println(a3);
//         }
//       else{
//         Serial.println("void current_GET : value error");
//         } 
//     }
//     else
//     {
//       Serial.println("void current_GET : mode error");
//     }
//   }
  
//   if(pcf==9){
//     if(mode==0){
//       PCF8591::AnalogInput ai = pcf8591_U9.analogReadAll();
//       Serial.print("A0: ");
//       Serial.print(ai.ain0);
      
//       Serial.print(" A1: ");
//       Serial.print(ai.ain1);
      
//       Serial.print(" A2: ");
//       Serial.print(ai.ain2);
      
//       Serial.print(" A3: ");
//       Serial.println(ai.ain3);
//     }
//     if(mode==1){  
//       if(value=0){
//         int a0 = pcf8591_U9.analogRead(AIN0);
//         Serial.print("A0: ");
//         Serial.println(a0);
//         }
//       if(value=1){
//         int a1 = pcf8591_U9.analogRead(AIN1);
//         Serial.print("A1: ");
//         Serial.println(a1);
//         }
//       if(value=2){
//         int a2 = pcf8591_U9.analogRead(AIN2);
//         Serial.print("A2: ");
//         Serial.println(a2);
//         }
//       if(value=3){
//         int a3 = pcf8591_U9.analogRead(AIN3);
//         Serial.print("A3: ");
//         Serial.println(a3);
//         }
//       else{
//         Serial.println("void current_GET : value error");
//         } 
//     }
//     else
//     {
//       Serial.println("void current_GET : mode error");
//     }
//   }

//   if(pcf==10){
//     if(mode==0){
//       PCF8591::AnalogInput ai = pcf8591_U10.analogReadAll();
//       Serial.print("A0: ");
//       Serial.print(ai.ain0);
      
//       Serial.print(" A1: ");
//       Serial.print(ai.ain1);
      
//       Serial.print(" A2: ");
//       Serial.print(ai.ain2);
      
//       Serial.print(" A3: ");
//       Serial.println(ai.ain3);
//     }
//     if(mode==1){  
//       if(value=0){
//         int a0 = pcf8591_U10.analogRead(AIN0);
//         Serial.print("A0: ");
//         Serial.println(a0);
//         }
//       if(value=1){
//         int a1 = pcf8591_U10.analogRead(AIN1);
//         Serial.print("A1: ");
//         Serial.println(a1);
//         }
//       if(value=2){
//         int a2 = pcf8591_U10.analogRead(AIN2);
//         Serial.print("A2: ");
//         Serial.println(a2);
//         }
//       if(value=3){
//         int a3 = pcf8591_U10.analogRead(AIN3);
//         Serial.print("A3: ");
//         Serial.println(a3);
//         }
//       else{
//         Serial.println("void current_GET : value error");
//         } 
//     }
//     else
//     {
//       Serial.println("void current_GET : mode error");
//     }
//   }
//      // PCF8591  U11

//     if(pcf==11){
//     if(mode==0){
//       PCF8591::AnalogInput ai = pcf8591_U11.analogReadAll();
//       Serial.print("A0: ");
//       Serial.print(ai.ain0);
      
//       Serial.print(" A1: ");
//       Serial.print(ai.ain1);
      
//       Serial.print(" A2: ");
//       Serial.print(ai.ain2);
      
//       Serial.print(" A3: ");
//       Serial.println(ai.ain3);
//     }
//     if(mode==1){  
//       if(value=0){
//         int a0 = pcf8591_U11.analogRead(AIN0);
//         Serial.print("A0: ");
//         Serial.println(a0);
//         }
//       if(value=1){
//         int a1 = pcf8591_U11.analogRead(AIN1);
//         Serial.print("A1: ");
//         Serial.println(a1);
//         }
//       if(value=2){
//         int a2 = pcf8591_U11.analogRead(AIN2);
//         Serial.print("A2: ");
//         Serial.println(a2);
//         }
//       if(value=3){
//         int a3 = pcf8591_U11.analogRead(AIN3);
//         Serial.print("A3: ");
//         Serial.println(a3);
//         }
//       else{
//         Serial.println("void current_GET : value error");
//         } 
//     }
//     else
//     {
//       Serial.println("void current_GET : mode error");
//     }
//   }
//        // PCF8591  U12

//     if(pcf==12){
//     if(mode==0){
//       PCF8591::AnalogInput ai = pcf8591_U12.analogReadAll();
//       Serial.print("A0: ");
//       Serial.print(ai.ain0);
      
//       Serial.print(" A1: ");
//       Serial.print(ai.ain1);
      
//       Serial.print(" A2: ");
//       Serial.print(ai.ain2);
      
//       Serial.print(" A3: ");
//       Serial.println(ai.ain3);
//     }
//     if(mode==1){  
//       if(value=0){
//         int a0 = pcf8591_U12.analogRead(AIN0);
//         Serial.print("A0: ");
//         Serial.println(a0);
//         }
//       if(value=1){
//         int a1 = pcf8591_U12.analogRead(AIN1);
//         Serial.print("A1: ");
//         Serial.println(a1);
//         }
//       if(value=2){
//         int a2 = pcf8591_U12.analogRead(AIN2);
//         Serial.print("A2: ");
//         Serial.println(a2);
//         }
//       if(value=3){
//         int a3 = pcf8591_U12.analogRead(AIN3);
//         Serial.print("A3: ");
//         Serial.println(a3);
//         }
//       else{
//         Serial.println("void current_GET : value error");
//         } 
//     }
//     else
//     {
//       Serial.println("void current_GET : mode error");
//     }
//   }

//          // PCF8591  U13

//     if(pcf==13){
//     if(mode==0){
//       PCF8591::AnalogInput ai = pcf8591_U13.analogReadAll();
//       Serial.print("A0: ");
//       Serial.print(ai.ain0);
      
//       Serial.print(" A1: ");
//       Serial.print(ai.ain1);
      
//       Serial.print(" A2: ");
//       Serial.print(ai.ain2);
      
//       Serial.print(" A3: ");
//       Serial.println(ai.ain3);
//     }
//     if(mode==1){  
//       if(value=0){
//         int a0 = pcf8591_U13.analogRead(AIN0);
//         Serial.print("A0: ");
//         Serial.println(a0);
//         }
//       if(value=1){
//         int a1 = pcf8591_U13.analogRead(AIN1);
//         Serial.print("A1: ");
//         Serial.println(a1);
//         }
//       if(value=2){
//         int a2 = pcf8591_U13.analogRead(AIN2);
//         Serial.print("A2: ");
//         Serial.println(a2);
//         }
//       if(value=3){
//         int a3 = pcf8591_U13.analogRead(AIN3);
//         Serial.print("A3: ");
//         Serial.println(a3);
//         }
//       else{
//         Serial.println("void current_GET : value error");
//         } 
//     }
//     else
//     {
//       Serial.println("void current_GET : mode error");
//     }
//   }
//   else{
//       Serial.println("void current_GET : pcf error");
//   }
// }
