/*
 * @TODO: Make drive() simpler & shorter?
 */


/*
 * Original: https://github.com/thearn/Python-Arduino-Command-API
 * Modified to remove functions un-needed for this project
 * 

LICENSE:

Copyright (c) 2012-2013 Tristan A. Hearn <tristanhearn@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 
 */


#define STEPPER1PINA 5
#define STEPPER1PINB 6
#define STEPPER1PINC 7
#define STEPPER1PIND 8

#define STEPPER2PINA 9
#define STEPPER2PINB 10
#define STEPPER2PINC 11
#define STEPPER2PIND 12

#define MAGPIN 13

boolean connected = false;
int d=5,r=0;


int Str2int (String Str_value)
{
  char buffer[10]; //max length is three units
  Str_value.toCharArray(buffer, 10);
  int int_value = atoi(buffer);
  return int_value;
}

void split(String results[], int len, String input, char spChar) {
  String temp = input;
  for (int i=0; i<len; i++) {
    int idx = temp.indexOf(spChar);
    results[i] = temp.substring(0,idx);
    temp = temp.substring(idx+1);
  }
}

void Version(){
  Serial.println("version");
  connected=true;
}


void SerialParser(void) {
  char readChar[64];
  readChar[0]=0;
  Serial.readBytesUntil(33,readChar,64);
  String read_ = String(readChar);
  //Serial.println(readChar);
  int idx1 = read_.indexOf('%');
  int idx2 = read_.indexOf('$');
  // separate command from associated data
  String cmd = read_.substring(1,idx1);
  String data = read_.substring(idx1+1,idx2);
  
  // determine command sent
  if (cmd == "version") {
      Version();   
      
  }
  else if (cmd=="xy"){
    movexy(data);
  }
}

void setup()  {

  pinMode(STEPPER1PINA, OUTPUT);
  pinMode(STEPPER1PINB, OUTPUT);
  pinMode(STEPPER1PINC, OUTPUT);
  pinMode(STEPPER1PIND, OUTPUT);
  pinMode(STEPPER2PINA, OUTPUT);
  pinMode(STEPPER2PINB, OUTPUT);
  pinMode(STEPPER2PINC, OUTPUT);
  pinMode(STEPPER2PIND, OUTPUT);
  pinMode(MAGPIN, OUTPUT);
  
  Serial.begin(9600); 
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {

  Serial.flush();
  SerialParser();
}


void movexy(String datastr)
{
  String data[3];
  split(data,3,datastr,'%');
  drive(atoi(data[0].c_str()),atoi(data[1].c_str()),atoi(data[2].c_str()));
}

void drive(int x, int y, int m)
{

  
    if(m==1)
    {
      digitalWrite(MAGPIN,HIGH);
    }
    
    while(x > 0 && y > 0)
    {
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);

      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);
      
      x--;
      y--;
    }
    
    while(x > 0 && y < 0)
    {
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);
      
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);
      
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      x--;
      y++;
    }

    while(x < 0 && y > 0)
    {
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      delay(d);

      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      delay(d);
      
      x++;
      y--;
    }
    
    while(x < 0 && y < 0)
    {
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      x++;
      y++;
    }
    
    digitalWrite(STEPPER1PINA,LOW);
    digitalWrite(STEPPER1PINB,LOW);
    digitalWrite(STEPPER1PINC,LOW);
    digitalWrite(STEPPER1PIND,LOW);
    digitalWrite(STEPPER2PINA,LOW);
    digitalWrite(STEPPER2PINB,LOW);
    digitalWrite(STEPPER2PINC,LOW);
    digitalWrite(STEPPER2PIND,LOW);
    
    while(x > 0)
    {
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      delay(d);
      
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      delay(d);
      
      x--;
    }
    
    while(x < 0)
    {
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,LOW);
      digitalWrite(STEPPER1PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER1PINA,LOW);
      digitalWrite(STEPPER1PINB,HIGH);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER1PINA,HIGH);
      digitalWrite(STEPPER1PINB,LOW);
      digitalWrite(STEPPER1PINC,HIGH);
      digitalWrite(STEPPER1PIND,LOW);
      delay(d);
      
      x++;
    }
    digitalWrite(STEPPER1PINA,LOW);
    digitalWrite(STEPPER1PINB,LOW);
    digitalWrite(STEPPER1PINC,LOW);
    digitalWrite(STEPPER1PIND,LOW);


    while(y > 0)
    {
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);
      
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);
      
      y--;
    }
    while(y < 0)
    {
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,LOW);
      digitalWrite(STEPPER2PIND,HIGH);
      delay(d);

      digitalWrite(STEPPER2PINA,LOW);
      digitalWrite(STEPPER2PINB,HIGH);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      digitalWrite(STEPPER2PINA,HIGH);
      digitalWrite(STEPPER2PINB,LOW);
      digitalWrite(STEPPER2PINC,HIGH);
      digitalWrite(STEPPER2PIND,LOW);
      delay(d);
      
      y++;
    }
    digitalWrite(STEPPER2PINA,LOW);
    digitalWrite(STEPPER2PINB,LOW);
    digitalWrite(STEPPER2PINC,LOW);
    digitalWrite(STEPPER2PIND,LOW);


    
    digitalWrite(MAGPIN,LOW); //magnet off

    //Serial.write(1);

}
