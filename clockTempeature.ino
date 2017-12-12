#include "pitches.h"
#include <LedControl.h>;
#include <Keypad.h>;
LedControl lc=LedControl(12,11,10,1); //建立物件  
unsigned long time2, time1;
int sec;
int hour;
int minute;
int day=1;
int month=1;
int year=2001;
int alsec=0;
int alhour=0;
int alminute=0;
int alday=1;
int almonth=1;
int alyear=2001;
int alarm=0;
long pre, now;
int note;
int noteDuration;
char P='A';
byte colPins[4] = {9, 8, 7, 6}; // 連到 Keypad 的 4 個 Columns: Column0, Column1, Column2
byte rowPins[4] = {5, 4, 3, 2}; // 設定「列」腳位
const char keymap[4][4] = {   // 設定按鍵的「行、列」代表值
  {'0','4','8','C'},
  {'1','5','9','D'},
  {'2','6','A','E'},
  {'3','7','B','F'}
};
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
4, 8, 8, 4, 4, 4, 4, 4
};
byte i, j;    // 暫存迴圈的索引數字
byte scanVal; // 暫存掃描到的按鍵值
Keypad customKeypad = Keypad( makeKeymap(keymap), rowPins, colPins, 4, 4);
//DHT
#include "DHT.h"
#define DHTPIN A2     // what digital pin we're connected to
#define DHTTYPE DHT11
int AH=30;
int AT=30;
int alarm1=0;
int setAlarm1=0;
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  pre=millis();
  note=0;
  Serial.begin(9600);
  time2=millis();  //前一次時間
//keypad
  for (i = 0; i<=3; i++) {
      pinMode(rowPins[i], INPUT_PULLUP);
      pinMode(colPins[i], OUTPUT);
      digitalWrite(colPins[i], HIGH);
    }
//DHT
  dht.begin();
}
void loop() { 
char customKey = customKeypad.getKey();
    Serial.println("key :"+String(customKey));

//DHT
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
switch(P){
case 'A':  //時鐘
      if(sec/3==4 || sec/3==14){  //每到第12,42秒顯示日期3秒
      lc.clearDisplay(0);
      lc.setDigit(0,0,day%10,true);
      lc.setDigit(0,1,day/10,false);
      lc.setChar(0,2,month%10,true);
      lc.setDigit(0,3,month/10,false);
      lc.setDigit(0,4,year%10,true);
      lc.setChar(0,5,(year/10)%10,false);
      lc.setDigit(0,6,(year/100)%10,false);
      lc.setDigit(0,7,year/1000,false);
      }else if(sec/3==9 || sec/3==19 ){  //每到第27,57秒顯示溫度濕度3秒
        if(setAlarm1){
        printNumber4(int(AH));
        printNumber3(int(AT));
      }else{
        printNumber2(int(h));
        printNumber(int(t));
      }
    }
  else{
        lc.clearDisplay(0);
        lc.setDigit(0,0,sec%10,false);  //其他時間顯示時間
        lc.setDigit(0,1,sec/10,false);
        lc.setChar(0,2,'-',false);
        lc.setDigit(0,3,minute%10,false);
        lc.setDigit(0,4,minute/10,false);
        lc.setChar(0,5,'-',false);
        lc.setDigit(0,6,hour%10,false);
        lc.setDigit(0,7,hour/10,false);
  }
  if (customKey=='0'){
    sec++;
  }else if (customKey=='1'){
    sec--;
  }  else if (customKey=='2'){
    minute++;
  }else if (customKey=='3'){
    minute--;
  }else if(customKey=='4'){
    hour++;
  }else if(customKey=='5'){
    hour--;
  }
  if (alarm==1){
    now=millis();
    if (( now - pre) >= 0){
      noteDuration=1000/noteDurations[note];
      tone(A1, melody[note],noteDuration); 
      pre = millis() + noteDuration*1.3;
      note++;
    }
    if(note==8){
      note = 0;
      pre=millis();
      alarm = 0;
    }
  }
  if (sec==alsec && minute==alminute && hour==alhour){
        alarm=1; 
  }
    if(sec==60){
      minute++;
      sec=0;
    }
    if(minute==60){
      hour++;
      minute=0;
    }
    if(hour == 24){
       hour =0;
       day++;
     }
     if(day==31){
       day=1;
       month++;
     }
     if(month==13){
       year++;
       month=1;
     }
     if(alsec==60){
       alminute++;
       alsec=0;
    }
    if(alminute==60){
       alhour++;
       alminute=0;
    }
    if(alhour == 24){
      alhour =0;
     }
    if( millis()-time2 >=1000) {
          time2+=1000;  //前一次時間
     sec++;
    }
  if(customKey=='E'){P='B';}
  if(customKey=='D'){P='C';}
  break;
case 'B':  //鬧鈴設定
      lc.clearDisplay(0);
      lc.setDigit(0,0,alsec%10,false);
      lc.setDigit(0,1,alsec/10,false);
      lc.setChar(0,2,'-',false);
      lc.setDigit(0,3,alminute%10,false);
      lc.setDigit(0,4,alminute/10,false);
      lc.setChar(0,5,'-',false);
      lc.setDigit(0,6,alhour%10,false);
      lc.setDigit(0,7,alhour/10,false);
  if (customKey=='0'){
        alsec+=10;
  }else if (customKey=='1'){
        alsec-=10;
  }else if (customKey=='2'){
        alminute+=1;
  }else if (customKey=='3'){
        alminute-=1;
  }else if (customKey=='4'){
        alhour+=1;
  }else if (customKey=='5'){
        alhour-=1;
  }
  if(sec==60){
      minute++;
      sec=0;
    }
    if(minute==60){
      hour++;
      minute=0;
    }
    if(hour == 24){
      hour =0;
     }
    if(alsec==60){
      alminute++;
      alsec=0;
    }
    if(alminute==60){
      alhour++;
      alminute=0;
    }
    if(alhour == 24){
      alhour =0;
    }
    if( millis()-time2 >=1000) {
              time2+=1000;  //前一次時間
     sec++;
         }
    
     if(customKey=='F'){P='A';}
     if(customKey=='D'){P='C';}
    break;
case 'C':  //日期設定
      lc.clearDisplay(0);
      lc.setDigit(0,0,alday%10,true);
      lc.setDigit(0,1,alday/10,false);
      lc.setChar(0,2,almonth%10,true);
      lc.setDigit(0,3,almonth/10,false);
      lc.setDigit(0,4,alyear%10,true);
      lc.setChar(0,5,(alyear/10)%10,false);
      lc.setDigit(0,6,(alyear/100)%10,false);
      lc.setDigit(0,7,alyear/1000,false);
  if (customKey=='0'){
        alday+=1;
        day=alday;
  }else if (customKey=='1'){
        alday-=1;
        day=alday;
  }else if (customKey=='2'){
        almonth+=1;
        month=almonth;
  }else if (customKey=='3'){
        almonth-=1;
        month=almonth;
  }else if (customKey=='4'){
        alyear+=1;
        year=alyear;
  }else if(customKey=='5'){
        alyear-=1;
        year=alyear;
  }
  if(sec==60){
      minute++;
      sec=0;
    }
    if(minute==60){
      hour++;
      minute=0;
    }
    if(hour == 24){
      hour =0;
      }
    if(alsec==60){
      alminute++;
      alsec=0;
    }
    if(alminute==60){
      alhour++;
      alminute=0;
    }
  if(alhour == 24){
      alhour =0;
      day++;
      day=alday;
      }
    if(alday == 31){
        alday=1;
        almonth++;
        month=almonth;
        }
    if(almonth ==13){
          almonth=1;
          alyear++;
          year=alyear;
          }
    if( millis()-time2 >=1000) {
//           time2=millis();  //前一次時間
          time2+=1000;  //前一次時間
 sec++;
    }
    if(customKey=='E'){
      P='B';
    }
     if(customKey=='F'){
      P='A';
    }
  break;
  }
}

void printNumber(int v) {
int ones;
int tens;
int hundreds;
boolean negative=false; 
if(v<0) {
negative=true;
v=v*-1;
}
ones=v%10;
v=v/10;
tens=v%10;
v=v/10;
hundreds=v;
if(negative) {
lc.setChar(0,7,'-',false);
}else {
lc.setChar(0,7,' ',false);
}
lc.setChar(0,7,' ',false);
lc.setDigit(0,6,(byte)tens,false);
lc.setDigit(0,5,(byte)ones,false);
  lc.setRow(0,4,B01100011);  //up o
}

void printNumber2(int c) {
int ones;
int tens;
int hundreds;
boolean negative=false; 
if(c<0) {
negative=true;
c=c*-1;
}
ones=c%10;
c=c/10;
tens=c%10;
c=c/10;
hundreds=c;
if(negative) {
lc.setChar(0,3,'-',false);
}else {
lc.setChar(0,3,' ',false);
}
//lc.setDigit(0,6,(byte)hundreds,false);
lc.setChar(0,3,' ',false);
lc.setDigit(0,2,(byte)tens,false);
lc.setDigit(0,1,(byte)ones,false);
 lc.setRow(0,0,B00110111);  // H
}

void printNumber4(int b) {
int ones;
int tens;
int hundreds;
boolean negative=false; 
if(b<0) {
negative=true;
b=b*-1;
}
ones=b%10;
b=b/10;
tens=b%10;
b=b/10;
hundreds=b;
if(negative) {
lc.setChar(0,3,'-',false);
}else {
lc.setChar(0,3,' ',false);
}
lc.setChar(0,3,' ',false);
lc.setDigit(0,2,(byte)tens,false);
lc.setDigit(0,1,(byte)ones,false);
  lc.setRow(0,0,B00110111);  // H
}

void printNumber3(int n) {
int ones;
int tens;
int hundreds;
boolean negative=false; 
if(n<0) {
negative=true;
n=n*-1;
}
ones=n%10;
n=n/10;
tens=n%10;
n=n/10;
hundreds=n;
if(negative) {
lc.setChar(0,7,'-',false);
}else {
lc.setChar(0,7,' ',false);
}
lc.setChar(0,7,' ',false);
lc.setDigit(0,6,(byte)tens,false);
lc.setDigit(0,5,(byte)ones,false);
  lc.setRow(0,4,B01100011);  //up o
}
