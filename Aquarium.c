sbit LCD_RS at RC1_bit;
sbit LCD_EN at RC2_bit;
sbit LCD_D4 at RC3_bit;
sbit LCD_D5 at RC4_bit;
sbit LCD_D6 at RC5_bit;
sbit LCD_D7 at RC6_bit;

sbit LCD_RS_Direction at TRISC1_bit;
sbit LCD_EN_Direction at TRISC2_bit;
sbit LCD_D4_Direction at TRISC3_bit;
sbit LCD_D5_Direction at TRISC4_bit;
sbit LCD_D6_Direction at TRISC5_bit;
sbit LCD_D7_Direction at TRISC6_bit;


void idle(){
           PORTD.RD0=1;    //led on
           // fan, heater, pump and lighting off
           PORTB.RB0= 0;
           PORTD.RD1= 0;
           PORTD.RD2= 0;
           PORTD.RD3= 0;
           PORTD.RD4= 0;
           PORTE.RE2= 0;
           Lcd_Out(1,1, "AQUARIUM CONTROL");
           Lcd_Out(2,1, "SYSTEM READY");

}
//temperature
void checkTemperature() {
    int adcValue;
    float temperature;
    adcValue = ADC_Read(0);
    temperature = (adcValue * 5.0 / 1023.0) * 100; // convert to �C
    if(temperature > 25){
        PORTE.RE2 = 1;
        Lcd_Out(2,1,"Ventilation ON");
        PORTD.RD3=1;
        PORTD.RD4=0;
    } else if(temperature < 10){
        PORTD.RD3=0;
        PORTD.RD4=0;
        PORTE.RE2 = 0;
        Lcd_Out(2,1,"Ventilation OFF");
    }

    delay_ms(1000);

}
//water level
void water_level(){
 int water_level_Value;
 water_level_Value= ADC_Read(1);

 if (water_level_Value<511 && water_level_Value>204){
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1,1,"Pump ACTIVATED");
  PORTD.RD1=1;
  PORTD.RD2=0;
  delay_ms(500);
  Lcd_Cmd(_LCD_CLEAR);
  }
 if  (water_level_Value<204) {
 Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1,1,"ALERT");
  PORTD.RD6 = 1;
  delay_ms(2000);
  PORTD.RD6 = 0;
  Lcd_Cmd(_LCD_CLEAR);
 }



}

//lightinng
void lighting(){
int ldrValue;
 ldrValue = ADC_Read(2);
 if (ldrValue<300){
  PORTD.RD5 = 1;
  }
 else {
 PORTD.RD5 = 0;
 }
 
}

void main() {

//config
TRISA=0x07;
TRISB=0x01;
TRISC=0x01;
TRISD=0x00;
TRISE=0x00;

//init
ADC_init();
PORTA=0x00;
PORTB=0x00;
PORTC=0x00;
PORTD=0x00;
PORTE=0x00;

Lcd_Init();
Lcd_Cmd(_LCD_CLEAR);
Lcd_Cmd(_LCD_CURSOR_OFF);
//prog
while (1){
      if (PORTC.RC0==1){
      Lcd_Cmd(_LCD_CLEAR);
      checkTemperature();
      Lcd_Cmd(_LCD_CLEAR);
      water_level();
      }else{
      lighting();
      idle();
      }
}

//end
}
