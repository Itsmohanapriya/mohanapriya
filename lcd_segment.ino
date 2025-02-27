int led1=1;
int led2=2;
int led3=3;
int led4=4;
int led5=5;
int led6=6;
int led7=7;

#include <LiquidCrystal.h>
LiquidCrystal lcd (13,12,11,10,9,8);

void setup() {
  // put your setup code here, to run once:
   
   pinMode(led1,OUTPUT);
   pinMode(led2,OUTPUT);
   pinMode(led3,OUTPUT);
   pinMode(led4,OUTPUT);
   pinMode(led5,OUTPUT);
   pinMode(led6,OUTPUT);
   pinMode(led7,OUTPUT);

   lcd.begin(16,2);
 

}

void loop() {
  // put your main code here, to run repeatedly:


   digitalWrite(led1,0);     //0
  digitalWrite(led7,1);
  digitalWrite(led3,1);
  digitalWrite(led4,1);
  digitalWrite(led5,1);
  digitalWrite(led6,1);
  digitalWrite(led2,1);
 

  lcd.setCursor(0,0);   //0
  lcd.print("0");
  delay(1000);
  lcd.clear();
  delay(10);

  
  digitalWrite(led3,1);      //1
  digitalWrite(led4,0);
  digitalWrite(led2,1);
  digitalWrite(led7,0);
  digitalWrite(led6,0);
  digitalWrite(led5,0);
  digitalWrite(led1,0);
 


  lcd.setCursor(0,0);   //1
  lcd.print("1");
  delay(1000);
  lcd.clear();
  delay(10);


  
  digitalWrite(led7,1);  //2
  digitalWrite(led2,0);
  digitalWrite(led4,1);
  digitalWrite(led5,0);
  digitalWrite(led3,1);
  digitalWrite(led6,1);
  digitalWrite(led1,1);
 

  lcd.setCursor(0,0);   //2
  lcd.print("2");
  delay(1000);
  lcd.clear();
  delay(10);

      
  digitalWrite(led7,1);  //3
  digitalWrite(led2,0);
  digitalWrite(led6,1);
  digitalWrite(led5,1);
  digitalWrite(led3,0);
  digitalWrite(led4,1);
  digitalWrite(led1,1);
   


  lcd.setCursor(0,0);   //3
  lcd.print("3");
  delay(1000);
  lcd.clear();
  delay(10);

    
  digitalWrite(led3,0);  //4
  digitalWrite(led2,1);
  digitalWrite(led6,1);
  digitalWrite(led5,1);
  digitalWrite(led7,0);
  digitalWrite(led4,0);
  digitalWrite(led1,1);


  lcd.setCursor(0,0);   //4
  lcd.print("4");
  delay(1000);
  lcd.clear();
  delay(10);

 

  digitalWrite(led7,1);     //5
  digitalWrite(led3,0);
  digitalWrite(led2,1);
  digitalWrite(led6,0);
  digitalWrite(led5,1);
  digitalWrite(led4,1);
  digitalWrite(led1,1);


  lcd.setCursor(0,0);   //5
  lcd.print("5");
  delay(1000);
  lcd.clear();
  delay(10);
  

  digitalWrite(led2,1);     //6
  digitalWrite(led3,1);
  digitalWrite(led4,1);
  digitalWrite(led5,1);
  digitalWrite(led6,0);
  digitalWrite(led7,1);
  digitalWrite(led1,1);
   



  lcd.setCursor(0,0);   //6
  lcd.print("6");
  delay(1000);
  lcd.clear();
  delay(10);

  
  digitalWrite(led2,0);     //7
  digitalWrite(led3,0);
  digitalWrite(led4,0);
  digitalWrite(led5,1);
  digitalWrite(led6,1);
  digitalWrite(led7,1);
  digitalWrite(led1,0);

  lcd.setCursor(0,0);   //7
  lcd.print("7");
  delay(1000);
  lcd.clear();
  delay(10);
    
  digitalWrite(led2,1);     //8
  digitalWrite(led3,1);
  digitalWrite(led4,1);
  digitalWrite(led5,1);
  digitalWrite(led6,1);
  digitalWrite(led7,1);
  digitalWrite(led1,1);

  lcd.setCursor(0,0);   //8
  lcd.print("8");
  delay(1000);
  lcd.clear();
  delay(10);

  
 digitalWrite(led2,1);     //9
  digitalWrite(led3,0);
  digitalWrite(led4,0);
  digitalWrite(led5,1);
  digitalWrite(led6,1);
  digitalWrite(led7,1);
  digitalWrite(led1,1);
 
 lcd.setCursor(0,0);   //9
  lcd.print("9");
  delay(1000);
  lcd.clear();
  delay(10);

  lcd.setCursor(0,0);   //1
  lcd.print("1");
  delay(1000);
  lcd.clear();
  delay(10);



  lcd.print("2");
  delay(1000);
  lcd.clear();
  delay(10);

  

}

