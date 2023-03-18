// include the library code:
#include <LiquidCrystal.h>
#include <HardwareSerial.h>

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);
int frame = 0;
int np = 0; // not pressed
int at = 0; // air time
int x = 0; // also counts as score until you "beat" the game
int score = 0;
int y = 0;
int speed = 60-((x/10) * (25/speed));
bool run = true;
bool dead = false;
char field[] = {'@', '_', '_', '_', '#', '_', '_', '_', '#', '_', '_', '_', '_' ,'_', '#', '_'};
char l1[] =    {'@', '_', '_', '_', '#', '_', '_', '_', '#', '_', '_', '_', '_' ,'_', '#', '_'}; // level 1
char l2[] =    {'@', '_', '_', '#', '_', '_', '_', '#', '#', '_', '_', '_', '_' ,'_', '#', '_'}; // level 2
char cacti[] = {'_', '_', '_', '_', '#', '_', '_', '_', '#', '_', '_', '_', '_' ,'_', '#', '_'};
char l1c[] =   {'_', '_', '_', '_', '#', '_', '_', '_', '#', '_', '_', '_', '_' ,'_', '#', '_'}; // level 1 cacti
char l2c[] =   {'_', '_', '_', '#', '_', '_', '_', '#', '#', '_', '_', '_', '_' ,'_', '#', '_'}; // level 2 cacti
char topField[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char topFieldC[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}; // cleared version of top field


String join(char* a, int size)
{
    int i;
    String s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}


void setup() 
{
  pinMode(6, INPUT);
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);

	// Clears the LCD screen
	lcd.clear();
}

void loop() 
{
  if(digitalRead(6) == HIGH){
    if(np == 1 && at == 0){
      np = 0;
      y = 1;
      tone(7, 1000, 100);
      topField[x] = '@';
      field[x] = '_';
    }
  }else{np = 1;}

  if(cacti[x] == '#'){
    if(y == 0){
      dead = true;
      score += x;
      lcd.clear();
      cacti[x] = '_';
    }
  }
  if(x == 15){
    score += x;
    x = 0;
    if(field[4] == l1[4]){
      for(int i = 0; i<16; i++){
        field[i] = l2[i];
        cacti[i] = l2c[i];
        topField[i] = topFieldC[i];
      }
    }else{
      for(int i = 0; i<16; i++){
        field[i] = l1[i];
        cacti[i] = l1c[i];
        topField[i] = topFieldC[i];
      }
    }
  }

  frame += 1;
  if(frame == round(speed)){
    Serial.println(x);    
    speed = 60-((x/10) * (250/(speed+25)));
    if(dead == false){
      if(x == 15){
        y = 0;
      }
      frame = 0;
      if(y == 0){
        field[x + 1] = '@';
        field[x] = '_';
      }else{
        at += 1;
        topField[x + 1] = '@';        
        topField[x] = ' ';
        if(at == 3){
          at = 0;
          y = 0;
          topField[x + 1] = ' ';
          field[x + 1] = '@';
        }
      }
      x += 1;
      lcd.clear();
    }
  }
  if(dead == false){
    lcd.print(join(topField, 16));
  }else{
    lcd.print(score - 1);
    lcd.setCursor(0, 1);
    lcd.print("thats your score");
    if(run == true){
      tone(7, 750, 500);
      delay(500);
      tone(7, 500, 500);
      delay(500);
      tone(7, 250, 750);
      delay(750);
      run = false;
    }
  }

	lcd.setCursor(0, 1);

  if(dead == false){
	  lcd.print(join(field, 16));
  }
}