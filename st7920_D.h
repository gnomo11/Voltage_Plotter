
/************************************************************************ 
*							st7920.h									 *
*   Libreria para controlar un LCD Grafico Digole 12864ZW con el         *
*   driver ST7920 usando el compilador CCS.                              * 
*                                                                        * 
*   Pin 1 ------------> Gnd                                              *    
*   Pin 2 ------------> +5volts                                          * 
*   Pin 3 ------------> Contrast                                         * 
*   Pin 4 ------------> RS				-> B1                            *
*   Pin 5 ------------> R/W 			-> B0                            * 
*   Pin 6 ------------> Enable 			-> E2                            * 
*   Pin 7-14 ---------> Bus de Datos 	-> D4 - D7                       * 
*   Pin 15 -----------> PSB (parallel=1 y serial=0) +5V                     * 
*   Pin 16 -----------> No Conexion                                      * 
*   Pin 17 -----------> RST	   			-> E1                            * 
*   Pin 18 -----------> Vout                                             * 
*   Pin 19 -----------> LED Anodo                                        * 
*   Pin 20 -----------> LED Catodo                                       * 
*                                                                        * 
*   Este controlador es utilizando el comunicacion paralela con          *
*   interfaz a 4bit (DB4-DB7).                                           *
*                                                                        * 
*   Realizo: M.C. Julio Cesar Gallo Sanchez                              * 
*	Septiembre del 2014. Ingenieria Electronica.                         *
*   Instituto Tecnologico de Lazaro Cardenas.                            *
*                                                                        * 
*   Lee la hoja de datos de Digole 12864ZW !!!!!                         * 
************************************************************************/ 

// Nota colocar despues de la #include "18f4550.h" la sentencia
//	#device CONST=READ_ONLY

// Puedes cambiar los pines asignados dependiendo del circuito
#define GDB4	PIN_D4				// DD4 configurado
#define GDB5	PIN_D5				// DD5 configurado
#define GDB6	PIN_D6				// DD6 configurado
#define GDB7	PIN_D7				// DD7 configurado

#define  rs  	PIN_B1        	//RS  configurado 
#define  rw  	PIN_B0                  //R/W configurado              
#define  e   	PIN_B2                  //E   configurado                  
#define  rst 	PIN_B3                  //RST configurado    

// El pin PSB debe ser puesto a 1 (5V) para la comunicacion paralela.

#define ON   	1 
#define OFF   	0 
#define XVAL 	16  
#define YVAL 	32 
#define GLCD_WIDTH	128 

////////////////////////////////////////////////////////////////////////////////// 
// Las siguientes funciones estan contenidas en esta libreria:
// glcd_readbyte(address); 
// glcd_comm( instruction ); 
// glcd_data( data );
// glcd_busy(); 
// glcd_update();   Esta instruccion debe ser llamada siempre despues de escribir un pixel
// 					o al usar las funciones de GRAPHICS.C.. Esto solo aplica en modo grafico.
// glcd_fillscreen( ON or OFF); 
// glcd_init_graph(); Inicializa el modo grafico
// glcd_init_basic(); inicializa para acceder a los caracteres almacenados.
//                     puedes usar glcd_data('A') para escribir texto. 
// glcd_pixel(x coordenada, y coordenada, ON o OFF);  
//            - trabaja con la libreria GRAPHIC.C del compilador CCS. 
// glcd_image(width,height,X coor, Y coor, inverse); 
//            - traza el arreglo image[]. Declare primero esta el arreglo. 
// 
////////////////////////////////////////////////////////////////////////////////// 

//char image[];

typedef union { 
  int16 word; 
  int8 nbyte[2]; 
} Dots; 

typedef struct { 
  int1 refresh; 
  Dots pix[YVAL][XVAL];   	// Maxima dimencion para el display (x,y) = (128,64) 
} GD_RAM;             		// (0,0) Corresponde a la esquina a la izquierda superior. 
GD_RAM gdram; 


unsigned int8 glcd_readByte (unsigned int1 address){ 
  unsigned int8 data;   
  if(address == 1){ 
     output_high(rs); 
  } 
  else{ 
     output_low(rs); 
  } 
  data = 0;
  output_high(rw);	
  output_high(e);	
  delay_us(2);
  data = (input(GDB4)*16) + (input(GDB5)*32) + (input(GDB6)*64) + (input(GDB7)*128); 
  output_low(e);	
  output_high(rw);	
  output_high(e);	
  delay_us(2);		 
  data += (input(GDB4)*1) + (input(GDB5)*2) + (input(GDB6)*4) + (input(GDB7)*8);
  output_low(e);	
  return (data); 
} 


void glcd_busy(){ 
   int1 busy = 1; 
   output_low(rs);  
   output_high(rw); 
   while(busy){     
      output_high(e);		
	   delay_us(12); 
      if(!input(GDB7)){ 
         busy = 0; 
      } 
      output_low(e); 
	  delay_us(12); 
	  //output_high(e); 
   	  //delay_us(2);        
      //output_low(e);  
   } 
} 
  

void glcd_comm(unsigned char coma){ 
   glcd_busy(); 
   output_low(rs);    
   output_low(rw);    
   output_bit(GDB4,bit_test(coma,4));	
   output_bit(GDB5,bit_test(coma,5));	
   output_bit(GDB6,bit_test(coma,6));	
   output_bit(GDB7,bit_test(coma,7));	
   output_high(e);    
   delay_us(2);        
   output_low(e);     
   output_bit(GDB4,bit_test(coma,0));	
   output_bit(GDB5,bit_test(coma,1));	
   output_bit(GDB6,bit_test(coma,2));	
   output_bit(GDB7,bit_test(coma,3));	
   output_high(e);    
   delay_us(2);        
   output_low(e);     
} 


void glcd_data(unsigned char dato){ 
   glcd_busy(); 
   output_high(rs);   
   output_low(rw);    
   output_bit(GDB4,bit_test(dato,4));	
   output_bit(GDB5,bit_test(dato,5));	
   output_bit(GDB6,bit_test(dato,6));	
   output_bit(GDB7,bit_test(dato,7));	
   output_high(e); 
   delay_us(2); 
   output_low(e); 
   output_bit(GDB4,bit_test(dato,0));	
   output_bit(GDB5,bit_test(dato,1));	
   output_bit(GDB6,bit_test(dato,2));	
   output_bit(GDB7,bit_test(dato,3));	
   output_high(e); 
   delay_us(2); 
   output_low(e); 
} 
  

void glcd_fillScreen (unsigned int1 color){ 
  int8 v, h; 
  int16 d; 

  d = (color == ON ? 0xFFFF : 0x0000); 

  for(v = 0; v < YVAL; v++) { 
    for(h = 0; h < XVAL; h++) { 
     	gdram.pix[v][h].word = d; 
    } 
  } 
  gdram.refresh = TRUE; 
} 


void glcd_update(){ 
  int8 v, h; 

  if (gdram.refresh) { 
    for(v = 0; v < YVAL; v++) { 
      glcd_comm( 0x80 | v);  	// Vertical
      glcd_comm( 0x80 | 0);   	// Horizontal

      for(h = 0; h < XVAL; h++) { 
        glcd_data(gdram.pix[v][h].nbyte[1]);   	// Byte High. 
        glcd_data(gdram.pix[v][h].nbyte[0]);   	// Byte Low. 
      } 
    } 
    gdram.refresh = FALSE; 
  } 
} 


void glcd_init_graph(){ 
	output_drive(GDB4);   
	output_drive(GDB5);
	output_drive(GDB6);
	output_drive(GDB7);
	output_drive(rs);
	output_drive(rw);
	output_drive(e);
	output_drive(rst); 
	
	output_high(rst);

	delay_ms(150); 
  	output_low(rst);         
   	delay_us(1);                      
   	output_high(rst);        
   	glcd_comm(0x20);  
   	delay_us(150); 
	glcd_comm(0x20);  	// Funcion set
	delay_us(150); 
	glcd_comm(0x0C);   	// Display control 
	delay_us(150); 
	glcd_comm(0x01);   	// Display clear 
	delay_ms(15); 
	glcd_comm(0x06);   	// Entry mode set 
	delay_us(150);                  
	glcd_comm(0x24);    	// Select extended instruction set. 
	delay_ms(15); 
	glcd_comm(0x26);    	// Graphic display ON. 
	delay_ms(15); 
	
	glcd_fillScreen(OFF); 
	glcd_update(); 
} 


void glcd_init_basic(){ 
   delay_ms(150); 
   output_low(rst);       
   delay_us(1);                      
   output_high(rst);      
   glcd_comm(0x20);
   delay_us(150); 
   glcd_comm(0x20);		// Funcion set
   delay_us(150); 
   glcd_comm(0x0C);   	// Display control
   delay_us(150); 
   glcd_comm(0x01);   	// Display clear 
   delay_ms(15); 
   glcd_comm(0x06);   	// Entry mode set 
   delay_us(150);                  
} 


void glcd_pixel(int8 x, int8 y, int1 color) { 
  int8 v, h, b; 
  if(y > 31){
	x += 128; 
	y -= 32;
  } 
  v = y; 
  h = x/16; 
  b = 15 - (x%16); 
  
  if(color == ON) 
	bit_set(gdram.pix[v][h].word, b); 
  else 
	bit_clear(gdram.pix[v][h].word, b); 

  gdram.refresh = TRUE; 
} 

void glcd_image(char *p,int width,int height,int x,int y,int inverse) { 
   	unsigned int i = 0, j = 0, k = 0; 
	
  	for(j = 0; j < height; j++) {    
    	for(; i < width;) { 
            for(k = 8; k > 0; k--){ 					
				if(inverse)
					glcd_pixel(i+x,j+y,~bit_test(*p,(k-1))); 				
               	else								
					glcd_pixel(i+x,j+y,bit_test(*p,(k-1)));                 
               	i++; 
            } 
            p++; 
         } 
      i = 0; 
    } 
} 
