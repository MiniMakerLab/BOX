#include <Wire.h>
#include <CustomStepper.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
Adafruit_SSD1306 display = Adafruit_SSD1306();

int botao=2, interval=4000,tempo;
bool logState[14];
unsigned long previousMillis[14]; 
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
char str[5];

void setup(){
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE); 
  display.setTextSize(1);
  display.clearDisplay();
  pinMode(botao,INPUT_PULLUP);
  for(int i=3;i<14;i++){
      pinMode(i,OUTPUT);
      previousMillis[i]=0; 
  }
  for(int i=0;i<14;i++){
      logState[i]=true;
  }
}
void loop(){
    unsigned long currentMillis=millis(); 
    /*Pisca-Pisca e Ambulancia*/  
    if(currentMillis-previousMillis[13]>=1000){
        previousMillis[13] = currentMillis;
        if (logState[13] == true){
          digitalWrite(13, HIGH);
          digitalWrite(3, LOW);
          logState[13]=false;  
        }else{
          digitalWrite(13,LOW);
          digitalWrite(3, HIGH);          
          logState[13]=true;
        }    
    }
    tempo=analogRead(0);
    if(currentMillis-previousMillis[7]>=tempo){
        previousMillis[7] = currentMillis;
        if (logState[7] == true){
          digitalWrite(7, HIGH);          
          logState[7]=false;  
        }else{
          digitalWrite(7,LOW);                    
          logState[7]=true;
        }    
    }
    /*Ambulancia*/
    if(digitalRead(botao)==LOW){
      if(logState[13]==true){
        tone(12,1440);
      }else{
        tone(12,1880);
      }
    }else{
      noTone(12);
    }
    /*Poste de Iluminação Pública*/
    if(analogRead(1)>600){
      digitalWrite(11,HIGH);
    }else{
      digitalWrite(11,LOW);
    }
    /*Semáforo*/
    if(currentMillis-previousMillis[6]>=interval){
        previousMillis[6] = currentMillis;
        if (logState[6] == true){
          digitalWrite(6, LOW);
          digitalWrite(4, HIGH);
          logState[6]=false;                    
        }else if(logState[5] == true){
          digitalWrite(4, LOW);
          digitalWrite(5, HIGH);
          logState[5]=false;
          interval=2000;  
        }else{
          digitalWrite(6, HIGH);
          digitalWrite(5, LOW);
          logState[6]=true;
          logState[5]=true;
          interval=4000;  
        }       
    }
    if(currentMillis-previousMillis[10]>=1000){
        previousMillis[10] = currentMillis;
        if (logState[10] == true && logState[9]== true && logState[8] == true){
          digitalWrite(10, HIGH);
          digitalWrite(9, HIGH);
          digitalWrite(8, HIGH);
          logState[10]=false;                    
        }else if(logState[9]== true && logState[8] == true){
          digitalWrite(10,LOW);
          logState[8]=false;
          logState[10]=true;            
        }else if(logState[10]== true && logState[9] == true){
          digitalWrite(10,HIGH);
          digitalWrite(8,LOW);
          logState[9]=false;
          logState[8]=true;  
        }else if(logState[10]== true && logState[8] == true){
          digitalWrite(9,LOW);
          digitalWrite(8,HIGH);
          logState[8]=false;  
        }else if(logState[10]== true){
          digitalWrite(8,LOW);
          logState[10]=false;
          logState[9]=true;
        }else if(logState[9]== true){
          digitalWrite(10,LOW);
          digitalWrite(9,HIGH);
          logState[9]=false;
          logState[8]=true;    
        }else if(logState[8] == true){
          digitalWrite(9,LOW);
          digitalWrite(8,HIGH);
          logState[10]=true;
          logState[9]=true;
        } 
    }
    Vo = analogRead(3);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    Tc = T - 273.15;
    display.clearDisplay();
    display.setCursor(0,10);
    display.print("Temperatura: "); 
    display.print(Tc);
    display.print(" C");
    display.display();
    delay(1000);       
}
