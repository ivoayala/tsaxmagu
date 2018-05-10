/*
Temp_Sensor   PIN_A0
Light_Sensor   PIN_A1
PIR_Sensor   PIN_B0
*/
#include <18F1320.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES BORV45                   //Brownout reset at 4.5V
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(internal=8000000)

#use rs232(baud=9600,parity=N,xmit=PIN_B1,rcv=PIN_B4,bits=8,stream=PORT1)

int buffer, data_requested, Light;
long Temp1;
float Temp;
int16 PIR;


//********************Interrupt****************************
#INT_RDA
void  RDA_isr(void)           //RS232 interrupt
{
   disable_interrupts(GLOBAL);  // all interrupts OFF
   disable_interrupts(INT_RDA); // RS232 OFF
   
   buffer =getchar();
  
   if (buffer == '1'){
      data_requested=1;     
   }
   else{
      data_requested =0;
   }
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
  // putc(data_requested);
}

#INT_EXT
void  EXT_isr(void){           //INTn pin interrupt
   disable_interrupts(GLOBAL);  // all interrupts OFF
   PIR+=1;
   enable_interrupts(GLOBAL);
}

//*******************MAIN**********************************************
void main()
{
   SET_TRIS_B( 0b00010001);
   
   setup_adc_ports(sAN0|sAN1|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2|ADC_TAD_MUL_4);

   enable_interrupts(INT_RDA);
   enable_interrupts(INT_EXT);
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
         //Temp = (Temp*(5.0/1024)*100); //LM35
         Temp = ((Temp*(5.0/1024))-0.4)*51.28; //MCP7001
         //printf("%1.2f \n\r",Temp);
         
         //printf("Degrees %1.2f \n\r",Temp);
         
         
         set_adc_channel(1);     //Light
         delay_us(50);
         Light = read_adc();
         //while(adc_done()!=1)
         
         //printf("Light %3u \n\r",Light);
         //printf("PIR %3Lu \n\r",PIR);
         
         printf("%1.1f, %3u, %3Lu, \n\r",Temp,Light,PIR);
         //putc(Light);   
       
         
         PIR = 0;             //Clear PIR counter
         Temp=0;
         Temp1=0;
         Light=0;
   
      }
   else{
        
      }
   
  // output_toggle(PIN_B5);  //Test
   delay_ms(200);          //Test
  // delay_ms(200);          //Test
  //output_high(PIN_B6);  //test pin
  //output_high(PIN_B7);  //test pin
  // output_low(PIN_B6);  //test pin
  // output_low(PIN_B7);  //test pin
   
   }
   
   
   }
   
