 #include <LedControl.h>;
LedControl lc=LedControl(12,11,10,1); //建立物件
void setup() {
Serial.begin(9600);
lc.shutdown(0,false);
lc.setIntensity(0,8);
lc.clearDisplay(0);
}

void loop() { 
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
lc.setDigit(0,3,(byte)(data1/10),false);
lc.setDigit(0,2,(byte)(data1%10),false);
lc.setDigit(0,1,(byte)(data/10),false);
lc.setDigit(0,0,(byte)(data%10),false);
tone(A1,100,100);
}
}
if (inChar == '\n') {
stringComplete = true;
inputString = "";
}
}
}
