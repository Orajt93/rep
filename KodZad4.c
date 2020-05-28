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
#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0
#define L_CR	0x0F		
#define L_NCR	0x0C	

#define L_CFG   0x38

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

unsigned int adc(unsigned char kanal)
{
    switch(kanal)
    {
        case 0: ADCON0=0x01; break; //P1
        case 1: ADCON0=0x05; break; //P2
        case 2: ADCON0=0x09; break; 
    }
    
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}

void lcd_wr(unsigned char val)
{
  LPORT=val;
}

void lcd_cmd(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=0;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}
 
void lcd_dat(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=1;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}

void lcd_init(void)
{
	LENA=0;
	LDAT=0;
	delay(20);
	LENA=1;
	
	lcd_cmd(L_CFG);
	delay(5);
	lcd_cmd(L_CFG);
        delay(1);
	lcd_cmd(L_CFG); //configura
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON); //liga
	lcd_cmd(L_CLR); //limpa
	lcd_cmd(L_CFG); //configura
        lcd_cmd(L_L1);
}

void lcd_str(char* str)
{
 unsigned char i=0;
 while(str[i] != 0 )
 {
   lcd_dat(str[i]);
   i++;
 }

}
//funkcja czyszczaca LCD i ustawiajaca karetki w pierwszej linii
void clearLCD()
{
    lcd_cmd(L_CLR);     
    lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
}
//funckcja wyswietlajaca inty na LCD
void lcd_int(int liczba, bool moc)
{
    unsigned char i=0;
    char temp[8];
    //wyswietlenie czasu
    if(moc==false)
    {
        //wyciagniecie minut z sekund i rzucenie na lcd
        sprintf(temp, "%i", liczba/60);
        while(temp[i] != 0 )
        {
            lcd_dat(temp[i]);
            i++;
        }
        //dorzucenie dwukropka
        lcd_dat(':');
        //wyciagniecie pozostalych sekund poprzez modulo, je?eli liczba sekund
        //mniejsza niz 10 to dopisuje 0.
        if(liczba%60<10)
        {
            lcd_dat('0');
        }
        //wyswietlenie na lcd sekund
        sprintf(temp, "%i", liczba%60);
        i=0;
        while(temp[i] != 0 )
        {
            lcd_dat(temp[i]);
            i++;
        }
    }
    //jezeli wybrana moc, to wyswietla tylko sama liczbe bez formatu czasu
    else
    {
        sprintf(temp, "%i", liczba);
        while(temp[i] != 0 )
        {
            lcd_dat(temp[i]);
            i++;
        }
    }
    
         
}
//funkcja odpowiedzialna za wybranie mocy
int wybierzMoc()
{
    int moc=200;
    int poziomMocy=0;
    while(PORTBbits.RB2 != 0)
    {
        //obsluga przycisku RB5
        if(PORTBbits.RB5 == 0)
        {
            ///wyswietlenie komunikatu na LCD o aktualnie wybranje mocy
           
           poziomMocy++;
           delay(500);
           if(poziomMocy==1)
           {
              moc=350; 
           }
           else if(poziomMocy==2)
           {
              moc=600; 
           }
           else if(poziomMocy==3)
           {
              moc=800; 
           }
           else
           {
               poziomMocy=0;
               moc=200;
           }
            clearLCD();
            lcd_int(moc,true);
            lcd_dat('W');
            lcd_cmd(L_L2);
            lcd_str("RB2 - OK");
        }
        //obsluga przycisku funkcyjnego RB2
        
        if(PORTBbits.RB2 == 0)
        {
            //petla ktora byla potrzebna aby nie przerzucic dzialania przycisku
            //rb 2 na kolejna funkcje
            while(PORTBbits.RB2 == 0)
            {
                delay(1000);
                PORTBbits.RB2=1;
            }
           
            return moc;
        }        
    }
}
int wybierzCzas()
{
    int lsekund1=0;
    bool wypad=false;
    //tutaj to samo na wszelki wypadek aby dzialanie rb2 nie przeszlo na 
    //ta funkcje
    while(PORTBbits.RB2 == 0)
    {
        delay(200);
        PORTBbits.RB2 = 1;
    }
    //wysiwetlenie informacji o ustawieniu czasu
    clearLCD();
    lcd_str("Ustaw czas");
    lcd_cmd(L_L2);
    lcd_str("RB3+10s RB4+1min");
    delay(1500);
    clearLCD();
    while(wypad==false)
    {
    //obsluga przycisku rb3 dodanie 10 sekund
    if(PORTBbits.RB3 == 0)
    {
       delay(500);
       clearLCD();
       lsekund1=lsekund1+10;
       lcd_int(lsekund1,false);
       lcd_cmd(L_L2);
       lcd_str("RB2 - OK");
       
    }
    //obsluga przycisku rb4 dodanie 60 sekund
    if(PORTBbits.RB4 == 0)
    {
       delay(500);
       clearLCD();
       lsekund1=lsekund1+60;
       lcd_int(lsekund1,false);
       lcd_cmd(L_L2);
       lcd_str("RB2 - OK");
    }
    //jezeli wcisniete rb2 i liczba sekund wieksza od 0 to przechodzi do pracy
    //mikrofali
    if(PORTBbits.RB2 == 0 && lsekund1>0)
    {
        wypad=true;
            
        return lsekund1;
    }
    }
    return 0;
}

void main(void) {
    
    //Inicjalizacja konwertera analogowo cyfrowego
    ADCON0=0x01;
    ADCON1=0x0B;
    ADCON2=0x01;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    lcd_init(); //Inicjalizacja wy?wietlacza
    lcd_cmd(L_CLR); //Czyszczenie wy?wietlacza
    int lsekund=0;
    bool pause=false;
    bool started=false;
    bool endTime=false;
    int moc=0;
    while(1)
    {
    //obsluga przycisku RB3 dodanie 10 sekund
    if(PORTBbits.RB3 == 0)
    {
        if(started==true)
        {
           delay(300);
           lsekund=lsekund+10;
        }  
    }
    //obsluga przycisku RB4 dodanie 60 sekund
    if(PORTBbits.RB4 == 0)
    {
        if(started==true)
        {
           delay(300);
           lsekund=lsekund+60;
        }
    }
    //obsluga funkcyjnego przycisku RB2
    if(PORTBbits.RB2 == 0)
    {
        //Jezeli praca nie rozpoczeta i nie ma pauzy to inicjuje funkcje
        //Do wybrania mocy i do ustawienia czasu
        
        if(started==false && pause==false)
        {
            lcd_str("Wybierz moc");
            lcd_cmd(L_L2);
            lcd_str("RB5 - zmiana");
            delay(1500);
            clearLCD();
            //inicjalizacja funckji do wybrania Mocy
            moc=wybierzMoc();
            delay(1000);
            PORTBbits.RB2 = 1;
            //inicjalizacja funkcji do wybrania Czasu.
            lsekund=wybierzCzas();
            started=true;              
        }
        //jezeli praca rozpoczeta i nie ma pauzy to pauzuje
        else if(started ==true && pause==false)
        {
            pause=true;
            delay(1000);
        }
        //jezeli praca rozpoczeta i jest pausa to wlacza ponownie
        else if(started ==true && pause==true)
        {
            pause=false;
            delay(1000);
        }
        
    }
    //przycisk rb1 przerywajacy prace
    if(PORTBbits.RB1 == 0)
    {
        if(started==true)
        {
            clearLCD();
            lcd_str("Zakonczono");
            delay(2000);
            clearLCD();
            started=false;
            pause=false;
            lsekund=0;  
        }
    }

        //sprawdza czy pauzowac
        if(started==true && pause==true )
        {
            //komunikaty o pauzie
            clearLCD();
            lcd_str("Pauza");
            lcd_cmd(L_L2);
            //wyswietlenie czasu
            lcd_int(lsekund,false);
           
        }
        //jezeli nie ma pauzy i praca rozpoczeta
        if(started==true && pause==false)
        {   
            if(lsekund!=0)
            {
                clearLCD();
                //wyswietlenie czasu
                lcd_int(lsekund,false);
                lsekund--;
                //jezeli koniec czasu to przechodzi do zakonczenia programu
                if(lsekund==0)
                    endTime=true;
            }
             
            //konczenie dzialania programu
            if(endTime==true)
            {
                delay(1000);
                started=false;
                endTime=false;
                int warun=3;
                //petla wyswietlajaca 3krotnie komunikat o koncu pracy
                for(warun;warun>0;warun--)
                {
                    clearLCD();
                    delay(100);
                    lcd_str("Wyjmij jedzenie"); //napis
                    delay(1000);
                    clearLCD();
                    lcd_str(" "); //napis
                    delay(1000);
                    clearLCD();
                }  
            } 
        }
        delay(1000);
    }
    
    return;
}
