// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF

#include <xc.h>

void delay(unsigned int ms)
{
    unsigned int i;
    unsigned char j;
    
 for (i =0; i< ms; i++)
 {
 
  for (j =0 ; j < 200; j++)
   {
      Nop();
      Nop();
      Nop();
      Nop();
      Nop();
   }
 }
}

unsigned char DecimalToBCD (unsigned int Decimal)
{
   //zwraca unsigned char BCD z przeslanej wartosci
   return (((Decimal/10) << 4) | (Decimal % 10));
}
unsigned char BinaryToGray(unsigned int num)
{   
    //zwraca unsigned char z Kodem Graya z przeslanej wartosci
    return num ^ (num >> 1);
}
int checkSnake(unsigned char input)
{
    //Funckcja sprawdzaj?ca czy wartosc display z jaka rozpoczyna si? funkcja,
    //Nadaje si? do kontynuacji w??yka
    int index = 0;
    int val=0;
    do{
        //Sprawdzamy na ktorym miejscu jest pierwsza jedynka
        val=(input >> index) & 1U;
        index++;
    }while(val!=1 && index < 6);
    //Jezeli nie ma zadnej jedynki do 6tego miejsca to sie nie nadaje
    if(val==0)
        return 0;
    index--;
    //Sprawdzamy kolejne 4 bity
    int bit0=(input >> index) & 1U;
    index++;
    int bit1=(input >> index) & 1U;
    index++;
    int bit2=(input >> index) & 1U;
    index++;
    int bit3=0;
    if(index<8)
    {
        bit3=(input >> index) & 1U;
    }
    //Jezeli 3 kolejne bity sa jedynkami a czwarty bit to zero to jest dobrze
    //I mozna dzialac na przeslanej liczbie
    if(bit0==1 && bit1==1 && bit2==1 && bit3==0)
        return 1;
    return 0;
}


void main(void) {
    
    ADCON1=0x0F;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    PORTB = 0;
    
    unsigned char display = 0;
    unsigned char stop = 7;
    unsigned char position = 0;
    unsigned char direction = 0;
    
    while(1)
    {

     ///////////////////////////////////////////////////////////////
     first:
        PORTD = display;
        delay(600); //Opoznienie
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            display=1;
            goto GENERATOR;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
             goto second;
        }
        else
            //inkrementacja
            display++;   
     goto first;
     ///////////////////////////////////////////////////////////////
     second:
        PORTD = display;
        delay(600); //Opoznienie
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            goto first;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
             goto grayUP;
        }
        else
            //dekrementacja
            display--;   
     goto second;
     ///////////////////////////////////////////////////////////////
     grayUP:
        //Wywolanie funkcji, ktora zwroci unsigned chara
        //Do wyswietlenia na porcie D liczby w Kodzie Graya
        PORTD = BinaryToGray((unsigned int)display);
        delay(600); //Opoznienie
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            goto second;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
            goto grayDW;
        }
        else
        {
            //Inkrementacja
           display++;    
        }
     goto grayUP;
     /////////////////////////////////////////////////////////////////////////
     grayDW:
        //Wywolanie funkcji, ktora zwroci unsigned chara
        //Do wyswietlenia na porcie D liczby w Kodzie Graya
        PORTD = BinaryToGray((unsigned int)display);
        delay(600); 
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            goto grayUP;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
            goto BCDUP;
        }
        else
        {
            //dekrementacja
           display--;    
        }
     goto grayDW;
     /////////////////////////////////////////////////////////////////////////
     BCDUP:
        //Wywolanie funkcji, ktora zwroci unsigned chara
        //Do wyswietlenia na porcie D liczby w kodzie BCD
        PORTD = DecimalToBCD((unsigned int)display);
        delay(600); //Opoznienie
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            goto grayDW;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
            goto BCDDW;
        }
        else
        {
            //Inkrementacja
           display++;    
        }
     goto BCDUP;
      /////////////////////////////////////////////////////////////////////////
     BCDDW:
        //Wywolanie funkcji, ktora zwroci unsigned chara
        //Do wyswietlenia na porcie D liczby w kodzie BCD
        PORTD = DecimalToBCD((unsigned int)display);
        delay(600); //Opoznienie
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            goto BCDUP;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            display=0;
            display ^= 1UL << 0;
            display ^= 1UL << 1;
            display ^= 1UL << 2;
            delay(500);
            goto WEZYKF;
        }
        else
        {
           //dekrementacja
           display--;    
        }
     goto BCDDW;
     /////////////////////////////////////////////////////////////////////////
     WEZYKF:
        
        
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            goto BCDDW;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
            display=0;
            goto KOLEJKA;
        }
        else
        {
             //sprawdzenie wartosci na bicie 0
              int bit0=(display >> 0) & 1U;
              //sprawdzenie wartosci na bicie 7
              int bit7=(display >> 7) & 1U;
              //obsluga kierunkow ->
              //jezeli dioda 1 sie zapala to zmiana kierunku
              if(bit0==1)
                  direction=0;
              //jezeli dioda 7 sie zapala to zmiana kierunku
              else if(bit7==1)
                  direction=1;
              if(direction==0)
              {
                  //wyswietlenie wartosci i przesuniecie bitowe o 1 w lewo
                  PORTD=display;
                  display=display<<1;
                  delay(600);
              }
              else
              {
                  //wyswietlenie wartosci i przesuniecie bitowe o 1 w prawo
                  PORTD=display;
                  display=display>>1;
                  delay(600);
              }
          }
        
     goto WEZYKF;
     /////////////////////////////////////////////////////////////////////////
     KOLEJKA:
     while(display != 255)
     {
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            //trzeba tak zrobic bo inaczej zapamietuje ostatni stop i konczy
            //sie szybciej
            stop=7;
            display=0;
            display ^= 1UL << 0;
            display ^= 1UL << 1;
            display ^= 1UL << 2;
            goto WEZYKF;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
            stop=7;
            display=1;
            goto GENERATOR;
        }
        else
        {  
            //jezeli aktualna pozycja jest przed pozycja zatrzymana
            if(position<stop)
            {
                //przewrocenie bitu z 0 na 1 i wyswietlenie
                display ^= 1UL << position;
                PORTD = display;
                //przewrocenie bitu z powrotem na 0
                display ^= 1UL << position;
                delay(1000); //Opoznienie
                //zwiekszenie pozycji o jeden
                position++;
            }
            else
            {
               //przewracamy bit w miejscu zatrzymania 
               display ^= 1UL << stop;
               PORTD = display;
               delay(1000); //Opoznienie
               //dekrementujemy stop, aby zatrzymac wyswietlanie o jeden bit
               //wczesniej
               stop--;
               //ustaiwamy pozycje na 0, aby znowu zaczal od zerowej pozycji
               position=0;
            }
        } 
     }
     display=0;
     stop=7;
     goto KOLEJKA;
 /////////////////////////////////////////////////////////////////////////
     GENERATOR:
        if(PORTBbits.RB3 == 0)
        {
            //przejscie do poprzedniego progrmau
            delay(500);
            display=0;
            goto KOLEJKA;
        }
        else if(PORTBbits.RB4 == 0)
        {
            //przejscie do kolejnego progrmau
            delay(500);
            goto first;
        }
        else
        {  
            //sprawdznie wartosci na pierwszym bicie
            int bit1=(display >> 0) & 1U;
            //sprawdznie wartosci na drugim bicie
            int bit2=(display >> 1) & 1U;
            //sprawdznie wartosci na czwartym bicie
            int bit4=(display >> 3) & 1U;
            //sprawdznie wartosci na piatym bicie
            int bit5=(display >> 4) & 1U;
            //xoruje wszystkie poprzednie wartosci
            int xoro=((bit1^bit2)^bit4)^bit5;
            //jezeli z xorowania wartosci wychodzi 1 to 
            if(xoro==1)
            {
                //zmiana wartosci na ostatnim bicie
                display ^= 1UL << 5;
            }
            //przesuniecie bitowe o 1 w prawo
            display=display >> 1;
            delay(1000);
            //wyswietlenie wartosc
            PORTD = display;
        } 
     goto GENERATOR;

    }
    
    
    return;
}