#include <EEPROM.h>
#include <LiquidCrystal.h>

const int player1Button = 8;
const int player2Button = 9;
const String player1 = "Shritesh";
const String player2 = "Saloni";

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

enum buttonState {
  notPressed,
  pressing,
  pressed
};

buttonState player1ButtonState;
buttonState player2ButtonState;

float amount;
float wager;

void setup() {
  lcd.begin(16, 2);
  
  pinMode(player1Button, INPUT);
  pinMode(player2Button, INPUT);
  player1ButtonState = notPressed;
  player2ButtonState = notPressed;
  
  EEPROM.get(0, amount);
  
  // Force initial redraw below
  wager = NAN;
}

void loop() {
  bool redraw = false;

  float newWager = (analogRead(A0) / 30) * 0.25;
  if (newWager != wager) {
    redraw = true;
    wager = newWager;
  }

  player1ButtonState = newButtonState(player1ButtonState, digitalRead(player1Button));
  if (player1ButtonState == pressed) {
    amount += wager;
    redraw = true;
    player1ButtonState = notPressed;
  }

  player2ButtonState = newButtonState(player2ButtonState, digitalRead(player2Button));
  if (player2ButtonState == pressed) {
    amount -= wager;
    redraw = true;
    player2ButtonState = notPressed;
  }

  if (redraw) {
    EEPROM.put(0, amount);
    
    lcd.clear();

    lcd.print("Wager: $");
    lcd.print(wager);
  
    lcd.setCursor(0,1);
    
    if (amount == 0) {
      lcd.print("Tie!!");
    } else if (amount > 0) {
      lcd.print(player1);
      lcd.print(": $");
      lcd.print(amount);
    } else {
      lcd.print(player2);
      lcd.print(": $");
      lcd.print(- amount);
    }
  }
  
  delay(10);
}

buttonState newButtonState(buttonState currentButtonState, int buttonValue) {
 if (buttonValue == 1) {
  return pressing;
 } else {
  if (currentButtonState == pressing) {
    return pressed;
  } else {
    return notPressed;
  }
 }
}
