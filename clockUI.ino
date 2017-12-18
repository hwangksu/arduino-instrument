#include <LedControl.h>
#include <Keypad.h>
LedControl lc=LedControl(12,11,10,1);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
{'0','1','2','3'},
{'4','5','6','7'},
{'8','9','A','B'},
{'C','D','E','F'}
};
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
unsigned long time2, time1;
int sec;
int hour;
int minute;
int day;
int month;
int year;
int alsec=0;
int alhour=0;
int alminute=0;
int alday=0;
int almonth=0;
int alyear=0;
int alarm=0;
int setAlarm=0;
int temp=0;
void setup(){
Serial.begin(9600);
time2=millis();
lc.shutdown(0,false);
lc.setIntensity(0,8);
lc.clearDisplay(0);
pinMode(A1,OUTPUT);
}

void loop(){
char customKey = customKeypad.getKey();
if (customKey){
Serial.println(customKey);
if (customKey=='A'){
minute++;
}else if (customKey=='B'){
minute--;
}else if (customKey=='C'){
day++;
}else if (customKey=='D'){
day--;
}else if (customKey=='0'){
year++;
}else if (customKey=='1'){
year--;
}else if (customKey=='2'){
month++;
}else if (customKey=='3'){
month--;
}else if (customKey=='6'){
hour++;
}else if (customKey=='7'){
hour--;
}else if (customKey=='8'){
alminute++;
}else if (customKey=='9'){
alminute--; 
}else if (customKey=='4'){
alhour++;
}else if (customKey=='5'){
alhour--;
}else if (customKey=='E'){
if(setAlarm==1){
setAlarm=0;
}else{
setAlarm=1;
}
}else if (customKey=='F'){
alarm=0;
noTone(A1);
}
}
if(alarm==1){
tone(A1,500);
}

if( millis()-time2 >=1000) {
time2+=1000;
sec++;
if(sec==60){
minute++;
sec=0;
}
if(minute==60){
hour++;
minute=0;
}
if(hour==24){
day++;
hour=0;
}
Serial.print("Time: ");
Serial.print(year);
Serial.print(":"); 
Serial.print(month);
Serial.print(":");
Serial.print(day);
Serial.print(":");
Serial.print(hour);
Serial.print(":"); 
Serial.print(minute);
Serial.print(":");
Serial.println(sec);
if ( minute==alminute && hour==alhour){
alarm=1;
}

}
if(sec%10==9){
lc.clearDisplay(0);
lc.setDigit(0,0,day%10,false);
lc.setDigit(0,1,day/10,false);
lc.setChar(0,2,'.',false);
lc.setDigit(0,3,month%10,false);
lc.setDigit(0,4,month/10,false);
lc.setChar(0,5,'.',false);
lc.setDigit(0,6,year%10,false);
lc.setDigit(0,7,year/10,false);
}else if(setAlarm){
lc.clearDisplay(0);
lc.setDigit(0,0,alsec%10,false);
lc.setDigit(0,1,alsec/10,false);
lc.setChar(0,2,' ',false);
lc.setDigit(0,3,alminute%10,false);
lc.setDigit(0,4,alminute/10,false);
lc.setChar(0,5,' ',false);
lc.setDigit(0,6,alhour%10,false);
lc.setDigit(0,7,alhour/10,false);
}else{
lc.clearDisplay(0);
lc.setDigit(0,0,sec%10,false);
lc.setDigit(0,1,sec/10,false);
lc.setDigit(0,3,minute%10,false);
lc.setDigit(0,4,minute/10,false);
lc.setDigit(0,6,hour%10,false);
lc.setDigit(0,7,hour/10,false);
if(sec%2){
lc.setChar(0,2,'-',false);
lc.setChar(0,5,'-',false);
}else{
lc.setChar(0,2,' ',false);
lc.setChar(0,5,' ',false);
}
}
}

String inputString = ""; 
boolean stringComplete = false; 
int num;
int data,data1;

void serialEvent() {
while (Serial.available()) {
char inChar = (char)Serial.read();
inputString += inChar;
if(inputString.length() >=6){
num=inputString.indexOf("abc:");
if(num>=0){
data=byte(inputString.charAt(num+4));
data1=byte(inputString.charAt(num+5));
minute=data1;
hour=data;
//lc.setDigit(0,3,(byte)(data1/10),false);
//lc.setDigit(0,2,(byte)(data1%10),false);
//lc.setDigit(0,1,(byte)(data/10),false);
//lc.setDigit(0,0,(byte)(data%10),false);
tone(A1,100,100);
}
num=inputString.indexOf("alm:");
if(num>=0){
data=byte(inputString.charAt(num+4));
data1=byte(inputString.charAt(num+5));
alminute=data1;
alhour=data;
tone(A1,100,100);
}
}
if (inChar == '\n') {
stringComplete = true;
inputString = "";
}
}
}

