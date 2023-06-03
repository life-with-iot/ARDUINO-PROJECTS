#include <LiquidCrystal.h>
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {

  {'1','2','3','+'},

  {'4','5','6','-'},

  {'7','8','9','*'},

  {'C','0','=','/'}

};

byte rowPins[ROWS] = { 0, 1, 2, 3 };
byte colPins[COLS] = { 4, 5, 6, 7 };

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 long Num1,Num2,Number;
 char key,action;
 boolean result = false;
boolean presentValue = false;
boolean next = false;
boolean final = false;
String num1, num2;
int answer;
char op;
 
void setup() {
  lcd.begin(16, 2);
  lcd.print("Calculator");
  lcd.setCursor(0, 1);
  lcd.print("-TAMIZH INIYAN G");
  pinMode(A4,INPUT);
   delay(2000);
    lcd.clear();
}

void loop(){
  //int f=analogRead(A4);
  //Serial.print(f);
  char key = kpd.getKey();

  if (key != NO_KEY && (key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'||key=='0'))
  {
    if (presentValue != true)
    {
      num1 = num1 + key;
      int numLength = num1.length();
      lcd.setCursor(15 - numLength, 0); //to adjust one whitespace for operator
      lcd.print(num1);
    }
    else 
    {
      num2 = num2 + key;
      int numLength = num2.length();
      lcd.setCursor(15 - numLength, 1);
      lcd.print(num2);
      final = true;
    }
  }

  else if (presentValue == false && key != NO_KEY && (key == '/' || key == '*' || key == '-' || key == '+'))
  {
    if (presentValue == false)
    {
      presentValue = true;
      op = key;
      lcd.setCursor(15,0);
      lcd.print(op);
    }
  }

  else if (final == true && key != NO_KEY && key == '='){
    int f=analogRead(A4);
    if (op=='+' && f==668){answer = num1.toInt() | num2.toInt();}
    else if(op=='-' && f==668){answer = num1.toInt() ^ num2.toInt();}
    else if(op=='*' && f==668){answer = num1.toInt() & num2.toInt();} 
    
    else if (op == '+'){
      answer = num1.toInt() + num2.toInt();
    }
    else if (op == '-'){
      answer = num1.toInt() - num2.toInt();
    }
    else if (op == '*'){
      answer = num1.toInt() * num2.toInt();
    }
    else if (op == '/'){
      answer = num1.toInt() / num2.toInt();
    } 

      lcd.clear();
      lcd.setCursor(15,0);
      
      lcd.autoscroll();
      lcd.print(answer);
      lcd.noAutoscroll();
  }
  else if (key != NO_KEY && key == 'C'){
    lcd.clear();
    presentValue = false;
    final = false;
    num1 = "";
    num2 = "";
    answer = 0;
    op = ' ';
  }
}
