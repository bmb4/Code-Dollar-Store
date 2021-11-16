/* 
Practice for embedded systems, using keypad and LCD display
This is a "security system that will lock or unlock based on the 
entry of a four digit security code.  Uses only bit masking as per 
class requirements.
*/

#include "mbed.h"
#include "1802.h"

//Functions for determining the Column via ISR
void c1isr(void);
void c2isr(void);
void c3isr(void);
void c4isr(void);

//Interrupt objects for the colums(Inputs)
InterruptIn int1 (PE_10, PullDown);
InterruptIn int2 (PE_12, PullDown);
InterruptIn int3 (PE_14, PullDown);
InterruptIn int4 (PE_15, PullDown);

//Object for lcd
CSE321_LCD lcd(16,2,LCD_5x8DOTS, PF_0, PF_1);
//PF0 = SDA
//PF1 = SCL

//Variable for determining which row is powered on keypad
int row = 0;

//Security code to trigger lock state
int secCode [4] = {9,4,8,9};

//Index for checking user input against secCode
int index = 0;

//Var for storing user input
int userInput = 0;

//Var for checking lock state, locked = 0 / unlocked = 1 / error = 2
int locked = 0;

//Ticker for cylcing power through rows
Ticker t1;

//Function for cycling power through rows
void rows(void);

//Function for checking userInput against secCode
void check(void);




int main(){
    //Initialize LCD
    lcd.begin();
    
    //Initialize clock for ports B,E
    RCC -> AHB2ENR |= 0x12;
    
    //Inputs (columns)
    //GPIOE 15 - column 4, 14 - column 3, 12 - column 2, 10 - column 1
    GPIOE -> MODER &= ~(0xF3300000);

    //Outputs (rows)
    //GPIOB 15 - row 4, 14 - Red LED, 13 - row 3, 9 - row 2, 8 - row 1
    GPIOB -> MODER &= ~(0xA80A0000);
    GPIOB -> MODER |= 0x54050000;
    
    //ISR for Keypad coulmns, triggered when button pressed
    int1.rise(&c1isr);
    int2.rise(&c2isr);
    int3.rise(&c3isr);
    int4.rise(&c4isr);

    //enable ISRs
    int1.enable_irq();
    int2.enable_irq();
    int3.enable_irq();
    int4.enable_irq();


    //Initialize power cycling ticker
    t1.attach(&rows,50ms);
    

    while(true){
        
        //This is the only place I can get the lcd to work properly
        if (locked == 0){
            lcd.clear();
            lcd.print("Locked");
        }
        else if(locked == 1){
            lcd.clear();
            lcd.print("Unlocked");
        }
        else{
            lcd.clear();
            lcd.print("Incorrect code");
            //thread_sleep_for(5000);
        }
        //turn off red LED, turned in on the check function to denote acknowledgement of button press
        GPIOB -> ODR &= ~(0x4000);
        thread_sleep_for(300);
    }

}

//ISR functions to determine user input based on which column the interrupt was triggered in
//userInput is set to 10, an illegal value, if a letter or character is pressed
void c1isr(void){
    if (row == 1){
        userInput = 1;
    }
    else if (row == 2){
        userInput = 4;
    }
    else if (row == 3){
        userInput = 7;
    }
    else{
        userInput = 10;
        index = 0;
    }
    check();
    wait_us(700); 
}

void c2isr(void){
    if (row == 1){
        userInput = 2;
    }
    else if (row == 2){
        userInput = 5;
    }
    else if (row == 3){
        userInput = 8;
    }
    else{
        userInput = 0;
    }    
    check();
    wait_us(700); 
}

void c3isr(void){
    //printf("column value = %d\n",col);
    //printf("row val %d\n",row);
    if (row == 1){
        userInput = 3;
    }
    else if (row == 2){
        userInput = 6;
    }
    else if (row == 3){
        userInput = 9;
    }
    else{
        userInput = 10;
        index = 0;
    }
    check();
    wait_us(700); 
}

//No valid inputs in column 4
void c4isr(void){
    //printf("column value = %d\n", col);
    //printf("userInput = invalid\n");
    userInput = 10;
    check();
    wait_us(700); 
}

void check (void){
    //turn on red LED to acknowledge a button has been pressed
    GPIOB -> ODR |= 0x4000;
    printf("userInput = %d\n",userInput);
    //printf("index before function = %d\n",index);
    printf("seCode index = %d\n",secCode[index]);
    if (userInput == secCode[index]){
        if(index < 3){
            index +=1;
        }
        else{
            //If incorrect code is answered it will defaul to locked state for security purposes
           if (locked == 0){
                locked = 1;
                printf("unlocked\n");    
            }
            else{
                locked = 0;
                printf("locked\n");
                /*lcd.clear();
                lcd.print("Locked");*/
                }
            index = 0;
        }
    }
    else{
        locked = 2;
        index = 0;
    }
}

//Power cycling function, turns on one row and the other three each time the main loop is called
void rows(void){
    if(row == 0){
        GPIOB -> ODR &= ~(0xA000 | 0x200);
        GPIOB -> ODR |= 0x100;
        row +=1;
        }
    
    else if (row == 1){
        GPIOB -> ODR &= ~(0xA000 | 0x100);
        GPIOB -> ODR |= 0x200;
        row += 1;
    }
    
    else if (row == 2){
        GPIOB -> ODR &= ~(0x8000 | 0x300);
        GPIOB -> ODR |= 0x2000;
        row +=1;
        }
    
    else{//row = 3;
        GPIOB -> ODR &= ~(0x2000 | 0x300);
        GPIOB -> ODR |= 0x8000;
        row = 0;
    }
}

