/*GLCD_OSCILOSCOPIO*/


#include <18F45K50.h>
#device adc = 10;

#fuses NOWDT   
#use delay(internal=48MHz)


#include "st7920_D.h" 
#device CONST=READ_ONLY
#include <graphics.c>

long adc0=0;
char multimetro[10],valor_listo=0,volt[1],time[1];
float valor_digital=0,guardar_vd[10],suma=0;
int pos_x=13,pos_y=0,i=0,j=0;

void main(void){

   	glcd_init_graph(); //inicialiar GLCD

	enable_interrupts(GLOBAL);
	enable_interrupts(INT_AD);
	
	setup_adc_ports(sAN0);
	setup_adc(ADC_CLOCK_DIV_16);
	set_adc_channel(0);
	read_adc(ADC_START_ONLY); 

	setup_ccp2(CCP_COMPARE_RESET_TIMER);	
	setup_timer_1(T1_INTERNAL|T1_DIV_BY_4); //Fosc/4 y ademas se vuelve a dividir entre 4
	set_timer1(0); //conteo inicia en cero
	ccp_2=30000; //Cada 10 ms
   
	glcd_line(13,5,13,63,ON);
	glcd_pixel(11,7,ON);glcd_pixel(15,7,ON); 
	glcd_pixel(12,7,ON);glcd_pixel(14,7,ON);
	glcd_pixel(12,6,ON);glcd_pixel(14,6,ON);
	
	glcd_line(14,63,121,63,ON);
	glcd_pixel(119,62,ON);glcd_pixel(119,61,ON);
	glcd_pixel(120,62,ON);
	
	sprintf(volt,"V");
	glcd_text57(3,2,&volt,1,ON);

	sprintf(time,"t");
	glcd_text57(122,56,&time,1,ON);

	sprintf(multimetro,"0.0 V");
	glcd_text57(95,2,&multimetro,1,ON);
	glcd_rect(93,0,125,10,NO,YES); //Marco del multimetro
	glcd_update();
	
	while(true){ 

	        if(valor_listo==1){
			
				valor_digital=0.0049*adc0;
				
				pos_y= -10*valor_digital + 63;

			glcd_pixel(pos_x,pos_y,ON); 

			if(pos_x==113){
				glcd_rect(14,12,113,62,YES,OFF);
				glcd_update();
				pos_x=13;
				}
			
			glcd_update();
			if(i<10){guardar_vd[i]=valor_digital;i++;}
			
			else{
					for(j=0;j<10;j++){
					suma=suma+guardar_vd[j];
					}
			for(j=0;j<10;j++){guardar_vd[j]=0;}	
			
			suma=suma/10;
			
			glcd_rect(95,2,116,9,YES,OFF); //Limpia lectura
			//glcd_update();

			sprintf(multimetro,"%3.1f V",suma);
			glcd_text57(95,2,&multimetro,1,ON);
			glcd_rect(93,0,125,10,NO,YES); //Marco del multimetro
			glcd_update();
			i=0;
			suma=0;
			}
			
			valor_listo=0;
	

			setup_adc(ADC_CLOCK_DIV_16); 
			read_adc(ADC_START_ONLY);
			} 			
     	
   }
}

#int_ad
void muestreo_adc(void){
	
	adc0= read_adc(ADC_READ_ONLY);
	valor_listo=1;
	setup_adc(ADC_OFF);
	pos_x++;
	
}