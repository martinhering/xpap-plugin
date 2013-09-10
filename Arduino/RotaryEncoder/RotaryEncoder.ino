int brightness = 127;    // how bright the LED is, start at half brightness

const int pin_A = 7;  // pin 12
const int pin_B = 6;  // pin 11
unsigned char encoder_A;
unsigned char encoder_B;
unsigned char encoder_A_prev=0;

void setup()  {
  // declare pin 9 to be an output:
  pinMode(9, OUTPUT);
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  
  Serial.begin(9600);
} 

void loop()  {

    encoder_A = digitalRead(pin_A);    // Read encoder pins
    encoder_B = digitalRead(pin_B);   
    
    if((!encoder_A) && (encoder_A_prev))
    {
      // A has gone from high to low 
      if(encoder_B)
      {
        // B is high so clockwise
        brightness = min(brightness+1, 255);           
      }   
      else {
        // B is low so counter-clockwise      
        brightness = max(0, brightness-1);             
      }   

      Serial.println(brightness);
    }   
    encoder_A_prev = encoder_A; 
    
    // set the brightness of pin 9:
    analogWrite(9, brightness);   


  
  //delay(5);
                           
}

