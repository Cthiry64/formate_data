 #include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#define EEPROM_I2C_ADDRESS 0x50

typedef unsigned int uint;    

static const int RXPin = 3, TXPin = 2 ;
static const uint32_t GPSBaud = 9600;
uint written = 0;
uint extracted = 0;
uint last_address = 2;
//uint min_address = 2;
//uint max_address = 32767;
uint n = 12; //number of addresses used for one record
uint b = 0;
int ledPin = 10;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss=SoftwareSerial(RXPin, TXPin);


int read_button_PIN  = 5;
int write_button_PIN = 4;
//int buzzerPin = 12;
int is_writing = 0 ;

int begin_read ;
int begin_write;
int values_written = 0;
byte prev_snds = 61;
byte prev_mins = 61;
byte prev_hour = 25;


void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud) ;
  pinMode(write_button_PIN, INPUT_PULLUP);
  pinMode(read_button_PIN,  INPUT_PULLUP);
  Wire.begin();
  written = -1;
  pinMode(ledPin,OUTPUT);
}


void clignote(int n,int on, int off)
{
  for(short i = 1; i<=n;i++)
  {
    digitalWrite(ledPin,HIGH);
    delay(on);
    digitalWrite(ledPin,LOW);
    delay(off);
  }
  
}


byte readEEPROM(int address)
{
  byte received = 0xFF;
  
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);

  Wire.write((int)(address >> 8));
  Wire.write((int)(address & 0xFF));

  Wire.endTransmission();

  Wire.requestFrom(EEPROM_I2C_ADDRESS, 1);

  received = Wire.read();
  return received;
}



void writeEEPROM(int address, byte val)
{
  Wire.beginTransmission(EEPROM_I2C_ADDRESS);

  Wire.write((int)(address >> 8));  //Most significant byte
  Wire.write((int)(address & 0xFF));//Least significant byte
  Wire.write(val);
  
  /*
  Serial.print("trying to store at address : ");
  Serial.print(address);
  Serial.print(" the value : ");
  Serial.println(val);
  */
  
  Wire.endTransmission();
  delay(5);
}



void convert_to_base_256_and_store(long L)
{
  int r;
  unsigned long d = L;
  if(d<0) 
  {
    //Serial.println("d<0");
    return;
  }
  int i =0;
  while(d!=0)
  {
    //Serial.print("d = ");Serial.print(d);Serial.print(" i= ");Serial.println(i);
    r = d%256;
    d= d/256;
    writeEEPROM(last_address, r);
    last_address++;
    i++;
  }
  writeEEPROM(last_address,d%256);
  last_address++;
  i++;
  //Serial.print("i= ");Serial.println(i);
  while(i<5)
  {
    writeEEPROM(last_address,0);
    last_address++;
    i++;
  }

  if(i!=5)
    {
      Serial.print("ATTENTION : DEBORDEMENT i = ");
      Serial.println(i);
    }
}










void affiche_lat(short i)
{
  long Lat = 0;
  long pow256 = 1;
  
  for(short j = 1; j<=5;j++)
    {
      Lat = Lat + pow256 * readEEPROM(i+j);
      pow256 = pow256*256;
    }

  double mylat = (double)Lat/1000000;
  if(readEEPROM(i))
    Serial.print((-1) * mylat,6);
  else
    Serial.print(mylat,6);
}


void affiche_lng(short i)
{
  long Lng = 0;
  long pow256 = 1;
  
  for(short j = 1; j<=5;j++)
    {
      Lng = Lng + pow256 * readEEPROM(i+j);
      pow256 = pow256*256;
    }
  double mylng = (double)Lng/1000000;
  if(readEEPROM(i))
    Serial.print((-1) * mylng);
  else
    Serial.print(mylng);
}


void affiche_horloge(short i)
{
  Serial.print(readEEPROM(i));
  Serial.print(";");
  Serial.print(readEEPROM(i+1));
  Serial.print(";");
  Serial.println(readEEPROM(i+2));
}


void read_data()
{
  last_address = readEEPROM(0)*256+readEEPROM(1);
  //Serial.println(last_address);
  long Lng = 0;
  short pos;
  for (short i = 2; i < last_address; i = i+15) 
  {
    affiche_lat(i);
    Serial.print(";");
    affiche_lat(i+6);
    Serial.print(";");
    affiche_horloge(i+12);
  }
  Serial.println("E");

}




void write_data()
{
      
      char c = ss.read();
      
      //Serial.println(gps.encode(c));
      if (gps.encode(c)) 
      {
        //delay(5000);
        
        
        
        
        long Lat = (long) (gps.location.lat()*1000000); // [-90 000 000, 90 000 000] casting float to long int
        long Lng = (long) (gps.location.lng()*1000000); // [-180 000 000, 180 000 000]
        byte hours = gps.time.hour();
        byte minutes = gps.time.minute();
        byte snds = gps.time.second();
        
        if((Lat>0 || Lat<0) && (Lng>0 || Lng<0))
        {
          int d = (int)((int)snds - (int)prev_snds + ((int)minutes - (int)prev_mins)*60 + ((int)hours - (int)prev_hour)*3600);
          
          Serial.print(prev_hour);Serial.print(":");Serial.print(prev_mins);Serial.print(":");Serial.println(prev_snds);
          Serial.print(hours);Serial.print(":");Serial.print(minutes);Serial.print(":");Serial.println(snds);
          Serial.print("intervalle de temps : ");Serial.println( d);
          
          
          //converting coordinates to 4 bytes by the euclidean division by 16777216 (to have a quotient below 255) and then storing its the E.D. of its rest by 65536 and so on.
          if(d < 5) 
          {
            delay(1);  
          }
          else
          {
            
            //store data in eeprom
         
            if(Lat<0)
              {
                Lat = Lat * (-1);
                writeEEPROM(last_address,1);
              }
            else
              writeEEPROM(last_address,0);
            last_address++;  
            convert_to_base_256_and_store(Lat);
    
            if(Lng<0)
              {
                Lng = Lng * (-1);
                writeEEPROM(last_address,1);
              }
            else
              writeEEPROM(last_address,0);
            last_address++;
            convert_to_base_256_and_store(Lng);
            
            
            writeEEPROM(last_address, hours);
            writeEEPROM(last_address+1, minutes);
            writeEEPROM(last_address+2,snds);
            last_address = last_address +3;
            
            if(written%10 == 0)
            {
              writeEEPROM(0, (int)(last_address)/256);
              writeEEPROM(1, last_address%256);       //storing the euclidean division of last written address by 255 in the two first slots of the EEPROM
            }
            
            written++;
            clignote(1,500,500);
            //  Serial.println(last_address);
            prev_snds = snds;
            prev_mins = minutes;
            prev_hour = hours; 
          }
        }
      }
}


void loop() {  
  begin_read = digitalRead(read_button_PIN);
  begin_write = digitalRead(write_button_PIN);
  
  /*
  if(written>=5 && is_writing ==1)
  {
    written = 0;
    is_writing = 0;
    //Serial.print(written);Serial.print(" ");Serial.println(is_writing);
  }
  */
  if(begin_write==LOW && is_writing==1 && written >=1)
  {
    //ring(2);
    //Serial.println("Switching off writing");
    is_writing=0;
    values_written=0;
    written =-1;
    writeEEPROM(0, (int)(last_address)/256);
    writeEEPROM(1, last_address%256); 
    clignote(5,500,500);
    //Serial.println(is_writing);
  }
  else if((begin_write==LOW || is_writing==1) && last_address<=32767)
  {
    //Serial.println(written  );
    is_writing = 1;
    if (written==-1)
      {
        //ring(1);
        //Serial.println("Switching on writing");
        //delay(500);
        written++;
        clignote(2,500,500);
        prev_snds = 0;
        prev_mins = 0;
        prev_hour = 0;
      }
    
  	while (ss.available() > 0 ) 
  	{
      //displayInfo();
      write_data();
  	}
  
  	if (millis() > 5000 && gps.charsProcessed() < 10) 
  	{
  		Serial.println(F("No GPS detected: check wiring."));
  		while(true);
  	}
  }
  else if(begin_read == LOW)
  {
    //ring(3);
    //Serial.println("here i read the EEPROM");
    delay(500);
    read_data();
  }
  
}
