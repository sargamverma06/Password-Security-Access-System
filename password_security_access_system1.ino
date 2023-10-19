
//To import libraries
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <string.h>

//Initialise the Serial LCD.
LiquidCrystal lcd( 12,11,5,4,3,13); //These pin numbers are hard coded in the serial backpack board.

//Declaring Keypad
const byte ROWS = 4; //Numbers of rows on the keypad
const byte COLS = 4; //Numbers of cols on the keypad


char keys[ROWS][COLS] = { //2D Array to define keypad
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9}; //pins connected to analog outputs
byte colPins[COLS] = {A0, A1, A2, A3}; //Pins connected to analog inputs

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //Define keypad	

char key; //To store the keys pressed

int count=3;//State

bool show=true;

char *password[] = {"1234", "5678"}; //Default password

char compare[5]; //to compare the password entered by the user

int index;

int position;

void setup(){ //setup function runs once
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.home();
  lcd.print("IoT"); 
}

void loop(){ //loop function runs forever

  Wire.beginTransmission(4); 
  key = keypad.getKey(); //Wait for a key to be pressed 
 
  if(key == '*'){
    count = 3;
    Wire.write(count);// sends one byte  
  	Wire.endTransmission();
    char compare[20];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password");
    searchPassword();
    bool result = validation();
    lcd.clear();
    lcd.setCursor(0, 0);
    if(result){
      count=0;
      lcd.print("Access granted");
     }
   	else{
      show=false;
      count=1;
      lcd.print("Wrong password");
      delay(800);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Forgot Password?");
      lcd.setCursor(0, 1);
      lcd.print("Press 0");      
     }  	
	}
  index = 0;  	
  security();
  Wire.write(count);// sends one byte  
  Wire.endTransmission();
}
     


//Function to search the password
void searchPassword(){
  while(key != 'A'){//A to confirm and quits the loop
    key = keypad.getKey();
    if(key != NO_KEY && key != 'A' ){
      lcd.setCursor(index,1);
      lcd.print("*");
      compare[index]=key;//Store caracters in the array      
      index++;                    
     }
   }
  key = NO_KEY;
}


//Function to do the password validation
bool validation(){
  int size = sizeof(password)/sizeof(password[0]);
  bool flag = false;
	
  for(int i = 0; i < size; i++){
    if(strcmp(compare,password[i])==0){      
      flag=true;         
  	}    
  }
  for(int j = 0; j < 5; j++){
    compare[j]= '\0';
  }
  return flag;//Serial.println(word);
}


//Function to use the security question
void security()
{
  if(show==false){
  if (key == '0'){
    count = 3;
    Wire.write(count);// sends one byte  
  	Wire.endTransmission();
    lcd.clear();
    lcd.setCursor(16,0);
    lcd.print("Answer this question");
    delay(150);
    textLeft();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Address Number");
    char ans[4];
    
   	for(int i = 0; i < 4; i++)
   	{
     label: char key = keypad.getKey();
     if(key != NO_KEY)
     {
   	   ans[i] = key;
     }
     else goto label;
   	}
    ans[4] = { }; // add null terminator to the answer array
    char answer[] = "2004";
    bool correct = true;
    for(int i = 0; i < 4; i++) {
      if(ans[i] != answer[i]){
        correct = false;
        break;
      }
    }
    if(correct) {
      show=true;
      lcd.clear();
      selectPassword();
      changePassword();
       }
    else {
      show=true;
      lcd.clear();
      count=1;
      lcd.setCursor(0,0);
      lcd.print("Press * to try");
      lcd.setCursor(0,1);
      lcd.print("again");
      
    }
  }
    
  }
  
   
}
  


void textLeft(){
  for (int j = 0; j < 36; j++){ 
 	lcd.scrollDisplayLeft();
   	delay(150);
 }
}


void selectPassword(){
  position = 2;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter your user ID");
  
  while(position > 1){

    key = keypad.getKey();
    
  	if(key != NO_KEY && key == '0'){      
      position = 0;
      lcd.setCursor(0, 1);
      lcd.print("*");      
  	}
  
  	else if(key != NO_KEY && key == '1'){
      position = 1;
      lcd.setCursor(0, 1);
      lcd.print("*");
  	}
  }
}

void changePassword() {
  char newPassword[5];
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter new pw");
  
  index = 0;
  while (index < 4) {
    char key = keypad.getKey();
    if (key != NO_KEY) {
      lcd.setCursor(index, 1);
      lcd.print("*");
      newPassword[index] = key;
      index++;
    }
  }
  newPassword[4] = {}; // null terminate the string
  strcpy(password[position], newPassword); // copy the new password to the password variable
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Password changed");
  delay(1000);
}


