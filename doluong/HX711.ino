#include "HX711.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
HX711 scale(A1, A0); // HX711.DOUT- pin #A0 ;HX711.PD_SCK- pin #A1		
#define loa    5
float hieuchinh=3;
float zero;
float heso_chia = 1;
int man_hinh = 0;
int xuat=0;
int t=100;
int gam;
void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);					
  scale.set_scale(2280.f);   // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				       // reset the scale to 0
  zero = EEPROM.read(1);
  attachInterrupt(0, ngat0, LOW);   //chan 2, so luong
  attachInterrupt(1, ngat1, LOW); //chan 3 , chinh can 
          lcd.setCursor(0, 0);  
          lcd.print("LOADCELL ARDUINO");
          lcd.setCursor(0, 1);  
          lcd.print("      HX711     ");  
          delay(1500);
          delay(800);
          lcd.clear();
}

void loop() 
{

      //---------------------------- chinh_can ---------------------  
      if(man_hinh ==0 )
        { 
          lcd.setCursor(0, 0);  
          lcd.print("  CAN DIEN TU  ");
          lcd.setCursor(13, 1);  
          lcd.print("Gam");  
          lcd.setCursor(4, 1);
          gam = scale.get_units(1)*hieuchinh-zero;
         if(gam <0 ){
          gam = -(gam);
         }
          lcd.print(gam);
        }

        if(man_hinh == 1 )
        {  lcd.clear();
           lcd.setCursor(0,0);lcd.print(" Hieu chinh      ");
           lcd.setCursor(0,1);lcd.print("        dia can    ");
           zero = scale.get_units(1)*hieuchinh; 
           EEPROM.write(1,zero);
           delay(1500); man_hinh =0; lcd.clear();
        }

    
        //---------------------------- so_luong ---------------------  
    
          if(man_hinh == 2 )
        {  
          lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("LUU GIA TRI MAU   ");
           heso_chia = scale.get_units(1)*hieuchinh-zero;
           delay(1000); 
           man_hinh++; 
           lcd.clear();
        }

         if(man_hinh == 3 )
        {  
           lcd.setCursor(0,0);
           lcd.print(" So Luong : ");
           lcd.setCursor(13, 1);  
           lcd.print("Cai");  
           lcd.setCursor(4, 1);
          lcd.print((scale.get_units(1)*hieuchinh-zero)/ heso_chia);	
        }
  if (Serial.available() >= 1) {
                // đọc chữ liệu
                int buffer = Serial.read();
                if (buffer == '1') {
                  xuat = 4;
                }
                if(buffer == '2'){
                  xuat = 5;
                }
              }
         //---------------------------- Serial ---------------------     
        if(xuat == 0 ){
          Serial.print("Gam = ");
          Serial.println(gam);
        }
        if(xuat == 1){
          Serial.print(" Hieu chinh dia can ");
          xuat=0;
        }
        if(xuat == 2){
        Serial.print(" LUU GIA TRI MAU ");
        xuat = 3;
        }
        if(xuat==3){
          Serial.print(" So Luong = ");
          Serial.print((scale.get_units(1)*hieuchinh-zero)/ heso_chia);
          Serial.println(" Cai ");
        }
        if(xuat == 4){
          Serial.print(" Cân Đường |");
          Serial.print(" Thành tiền = ");
          Serial.print(gam*20);
          Serial.println(" vnd ");
        }
        if(xuat == 5){
          Serial.print(" Cân Bột |");
          Serial.print(" Thành tiền = ");
          Serial.print(gam*30);
          Serial.println(" vnd ");
        }
}
void ngat0()
{
Serial.begin(115200);
  pip_1();
  lcd.clear();
  if(man_hinh == 0 ) {man_hinh = 2;
   xuat = 2;}
  while(digitalRead(2)==0);
  if(man_hinh == 3 ) { man_hinh = 0;xuat =0; lcd.clear();}
  
}
void ngat1()
{
   pip_1();
  lcd.clear();
  man_hinh = 1;
  xuat =1;
}
void pip_1()
{
  digitalWrite(loa,HIGH);
  delay(10000);
  digitalWrite(loa,LOW);

}
