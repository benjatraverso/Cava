#include <Wire.h>
#include <LiquidCrystal.h>

byte TempHi;
byte TempLo;
boolean P_N;
unsigned int Decimal;
void Cal_Temp();

LiquidCrystal lcd(10, 9, 8, 7, 6, 5);

byte degreeSymbol[8] = {
  B00110,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000,
};

void setup() 
{ 
  lcd.createChar(0, degreeSymbol);
  Wire.begin();
  delay(1000);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print( "Temperature: ");//leave static label in first row
} 
  
void loop() 
{
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print( "Temperatura:");//leave static label in first row

  const int I2C_address = 0x37; 
  delay(100);
  Wire.beginTransmission(I2C_address);
  Wire.write(1);
  Wire.write(0x60);
  Wire.endTransmission(); 
  Wire.beginTransmission(I2C_address);
  Wire.write((byte)0x0);
  Wire.endTransmission(); 
  while (1)
  {
    delay(1000);
    Wire.requestFrom(I2C_address, 2);
    while(Wire.available()) 
    {    
      TempHi = Wire.read(); 
      TempLo = Wire.read();
    } 
    Cal_Temp();

    String value = "";
    value += (int) TempHi;
    value += ".";
    value += (int) Decimal;
    lcd.setCursor(0,1);
    lcd.print( "                 " );
    lcd.setCursor(0,1);
    lcd.print(value);
    lcd.print(" ");
    lcd.write(byte(0));
    lcd.print("C");
  }  
}

void Cal_Temp()
{
  if (TempHi&0x80)
    P_N = 0;
  else 
    P_N = 1;  
  TempHi = TempHi & 0x7F;
  TempLo = TempLo & 0xF0;
  TempLo = TempLo >>4;
  Decimal = TempLo;
  Decimal = Decimal * 625;  
}

