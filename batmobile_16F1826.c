/*
Temp_Sensor   PIN_A0
Light_Sensor   PIN_A1
PIR_Sensor   PIN_B3
*/
#include <16F1826.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(internal=8MHz)

#use rs232(baud=9600,parity=N,xmit=PIN_B2,rcv=PIN_B1,bits=8,stream=PORT1)

int buffer, data_requested, Light;
long Temp1;
float Temp;
int16 PIR;

#INT_RB
void  RB3_L2H_isr(void)  //Interrupt on change RB3
{
   disable_interrupts(GLOBAL);  // all interrupts OFF
   PIR+=1;
   enable_interrupts(GLOBAL);
}

#INT_RDA
void  RDA_isr(void)  //RS232 interrupt
{
   disable_interrupts(GLOBAL);  // all interrupts OFF
   disable_interrupts(INT_RDA); // RS232 OFF
   
   output_high(PIN_B6);  //test pin 
   putc(0xAA);
   
   //buffer =getchar();
   buffer = fgetc(PORT1);
   putc(buffer);
  
   if (buffer == '1'){
      data_requested=1;  
      putc(0xBB);
   }
   else{
      data_requested =0;
      putc(0xCC);
   }
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   CLEAR_INTERRUPT(INT_RDA);
   
   
   delay_ms(1000);
   output_low(PIN_B6);  //test pin
   
  // putc(data_requested);
}

void main()
{

   SET_TRIS_B( 0b00001100);  //RB2 and RB3_Input
   setup_adc_ports(sAN0|sAN1|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2);

  // enable_interrupts(INT_RB3_L2H);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);

   while(TRUE)
   {
           
   if (data_requested == 1){
         
         data_requested = 0;     //Clean flag
         
         
         set_adc_channel(0);     //Temperature
         delay_us(50);
         Temp1 = read_adc();      //51.28 °C/mV
        // while(adc_done()!=1);

         //delay_ms(1);
         Temp = Temp1;
         //putc(Temp1);
         //printf("%1.2f \n\r",Temp);
         Temp = (Temp*(5.0/1024)*100); //LM35
         //Temp = ((Temp*(5.0/256))-0.4)*51.28; //MCP7001
         //printf("%1.2f \n\r",Temp);
         
         //printf("Degrees %1.2f \n\r",Temp);
         
         
         set_adc_channel(1);     //Light
         delay_us(50);
         Light = read_adc();
         //while(adc_done()!=1)
         
         //printf("Light %3u \n\r",Light);
         //printf("PIR %3Lu \n\r",PIR);
         
         printf("%1.1f %3u %3Lu \n\r",Temp,Light,PIR);
         //putc(Light);   
       
         
         PIR = 0;             //Clear PIR counter
         Temp=0;
         Temp1=0;
         Light=0;
   
      }
   else{
        
      }
         output_toggle(PIN_B5);  //Test
         delay_ms(200);          //Test
         delay_ms(200);          //Test
}
}
