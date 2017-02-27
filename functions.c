#include <pic32mx.h>
#include <stdint.h>
#include "header.h"


int period = 10000;
int flag = 0;
int speed = 3000000;
int timeoutcount = 0;
char player1[] = "Player 1:   ";
char player2[] = "Player 2:   ";
char scoreCap[] = "Score Cap:  ";
char p1s = '0';
char p2s = '0';
char sc = '5';



void delay(int cyc) {
	int i;
	for(i = cyc; i > 0; i--){

	}
}
void screen (void){

    display_string(0, player1);
    display_string(1, player2);
    display_string(2, scoreCap);
}

//Initiates LEDs, ports and timers
void init(void){

    TRISECLR = 0xff;

    TRISDSET = 0xE0;
    TRISFSET = 0x2;

    PR2 = period;       //Set the timer period to the variable period
    T2CONSET = 0x70;    //Prescale 256
    T2CONSET = 0x8000;  //Timer enable, bit 15 is set to 1
    TMR2 = 0;           //Sets timer to 0

    screen();
    display_update();

}
//This function returns the button status
int getbtns(void){
    int btn1 = (PORTF >> 1) & 0x1;          //Button 1
    int restOfButtons = (PORTD >> 4) & 0xe; //Button 2-4
    int tot = btn1 | restOfButtons;         //Merge these into 1 value and return it
    return tot;
}
//The game manu with score selection
void menu(void){
        delay(5000000);
        while(1){
        int buttonstatus = getbtns();
            if (buttonstatus == 0x1){       //Select score cap with button 1, game will end when either players gets this many points
                delay(2000000);
                sc++;
                if(sc > '9'){               //Highest possible score is 9, will cycle back to 1 if set to higher than 9
                    sc = '1';
                }
            delay(200000);

            }

            p1s = '0';
            p2s = '0';
            player1[11] = p1s;
            player2[11] = p2s;
            scoreCap[11] = sc;
            screen();
            display_update();


            if(buttonstatus == 0x8 || buttonstatus == 0x2){     //If either button 4 or 2 is pressed the game will begin
                if(buttonstatus == 0x8){
                    PORTE = 0x80;
                    flag = 0;
                }
                else if(buttonstatus == 0x2){
                    PORTE = 0x01;
                    flag = 1;
                }
                game();
            }
        }


}

//This is the loop which the game is played in
void game(void){
    delay(200000);
    int start = 0;
    while(1){
        int buttonstatus = getbtns();   //Fetch the button status

        if(buttonstatus == 0x2 & PORTE == 0x01| buttonstatus == 0x8 & PORTE == 0x80){
            start = 1;
        }

        if(buttonstatus == 0x1){        //Go back to player menu if button 1 is pressed
            menu();

        }

        //Checks if the clock period is over (time-out)
        if(IFS(0) & 0x100){
            timeoutcount++;     //Increment timercounter
            IFSCLR(0) = 0x100;  //Reset flag by setting bit 8 back to 0
        }
        if(timeoutcount == 10){

            if(PORTE==0x01 & buttonstatus == 0x2){  //If player 2 manages to return the ball the direction is changed
                flag = 1;                           //Changes direction to left
                PR2 = PR2*0.9;
            }
            else if(buttonstatus == 0x2 & PORTE != 0x01 & flag == 0 & start == 1 | PORTE == 0x00 & flag == 0){   //If player 2 hits too early or not at all
                p1s++;              //player 1 will get another points
                PORTE = 0x80;       //If player 1 scores, then player 1 will start with the ball
                start = 0;          //Pauses game
                PR2 = period;       //Resets the period when a player scores
                delay(400000);
            }


            if(PORTE == 0x80 & buttonstatus == 0x8){    //If player 1 manages to return the ball the direction is changed
                flag = 0;                               //Changes direction to right
                PR2 = PR2*0.9;
            }
            else if(buttonstatus == 0x8 & PORTE != 0x80 & flag == 1 & start == 1 | PORTE == 0x00 & flag == 1){   //If player 1 hits too early or not at all
                p2s++;              //player 2 will get another point
                PORTE = 0x01;       //Player 2 will start with the ball
                start = 0;          //Pauses game
                PR2 = period;       //Period is reset when a player scores
                delay(400000);
            }

            if(flag == 0 & start == 1){              //If flag (direction) is 0 the the LEDS will shift right
                PORTE = (PORTE >> 1);   //Shifting the bits in porte will shift which LED is lit
            }

            if(flag == 1 & start == 1){              ////If flag (direction) is 0 the the LEDS will shift right
                PORTE = (PORTE << 1);
            }


            /*if(p1s > '9'){              //Will be changed in the final version
                p1s = '1';
            }

            if(p2s > '9'){              //Will also be changed in the final version
                p2s = '1';
            }*/


            player1[11] = p1s;          //Updates player 1's score and puts it in char array player1
            player2[11] = p2s;          //same as above, but for player 2
            scoreCap[11] = sc;          //Will remove in final version

            screen();                   //Updates the information which is sent to the display
            display_update();           //Updates the display
            timeoutcount = 0;           //Resets the timer

            if ( p1s == sc | p2s == sc )
            {
                scoreScreen();
            }
        }
    }
}





void scoreScreen ()
{
    display_string (0, "Congratulations ");
    if (p1s<p2s)
    {
        display_string(1, "PLAYER 2 ");
    }

    else
    {
        display_string(1, "PLAYER 1 ");
    }
    display_update();
    delay(100000000);
    menu();

}
