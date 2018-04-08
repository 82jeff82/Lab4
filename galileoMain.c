#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

//Linux GPIO number // Aruino Shield Pin Name

/*-----------------------------------------------------------------------potential user inputs--------------------------------------------------------------*/
#define MSG_RESET       (0)
#define MSG_PING        (1)
#define MSG_GET         (2)
#define MSG_TURN30      (30)
#define MSG_TURN90      (90)
#define MSG_TURN120     (120)

/*---------------------------------------------------------Hard codes definitions used throughout the program------------------------------------------*/
#define Strobe     (40) //IO8
#define GP_9       (4)  //IO9
#define GP_10	   (10)   //IO10
#define GP_11	   (5)    // IO11
#define GP_12	   (15)   //IO12
#define GPIO_DIRECTION_IN      (1)
#define GPIO_DIRECTION_OUT     (0)
#define ERROR                  (-1)


int userCommandInterface(void)      //Thread 1: User Command Interface
int sensorControlCenter(void)       //Thread 2: Sensor Control Center
int clientServerCommunication(void) //Thread 3: Client Server Communication
int commandMSG_RESET(int input)     //MSG Reset
int commandMSG_PING(int input)      //MSG Ping
int commandMSG_GET(int input)       //MSG Get
int commandMSG_TURN30(int input)    //MSG Turn 30
int commandMSG_TURN90(int input)    //MSG Turn 90
int commandMSG_TURN120(int input)   //MSG Turn 120

int main(void)
{
    //Create the threads
    

}




int userCommandInterface(void)
{
    int input = 0;                 // input is used to hold the int entered by the user representing the command they want executed
    int ADCvalue = 0;              // the decimal conversion of the binary value read in for the 12 bit value read in by the GET command
    int adcArray[12];             // the array which stores the 12 bit value read in by GET
    int p = 0;                     //used as a loop index for GET
    int status;
        while(1)
        {
            export_pins();          // basically initializes the pins that will be used in the program just incase they are missed anywhere
            printf("Enter an option (PING(1), RESET(0), ADC(2), TURN(30,90,120)): ");  // waits for user input before executing associated command
            scanf("%d",&input);
            printf("\n");

            if (input == MSG_RESET) // sends the reset command to the PIC
            {
                commandMSG_RESET_INPUT(input);
            }

            else if (input == MSG_PING) // sends the PING command to the PIC
            {  
                commandMSG_PING(input);
            }
            else if (input == MSG_GET)  // sends the GET command to the pic
            {
                commandMSG_GET(input);
            }
            else if (input == MSG_TURN30) // command sending the associated command for user input of turn 30
            {
                commandMSG_TURN30(input);
            }

            else if (input == MSG_TURN90)// command sending the associated command for user input of turn 90
            {
                commandMSG_TURN90(input);
            }

            else if (input == MSG_TURN120)// command sending the associated command for user input of turn 120
            {
                commandMSG_TURN120(input);
            }
             
       }
        return 0;
}

int sensorControlCenter(void)
{
     

}

int clientServerCommunication(void)
{
    if //I2C bus failed.
    printf("Error: failed to read I2C bus")


}


int commandMSG_RESET(int input)     //MSG Reset
{
    printf("Sending Reset Message...\n");
    sendMsg(input);
    writeGPIO(Strobe,1);
    usleep(10000);
    status=ackReceive();
    if (status==1)    // if the proper ACK was received
    {
        writeGPIO(Strobe,0);
        printf("Ack Received...\n");
    }
    else            // if ACK was not received print out what was read in for debugging purposes
    {
        printf("Received: %d %d %d %d\n", readGPIO(GP_12),readGPIO(GP_11), readGPIO(GP_10), readGPIO(GP_9) );

        writeGPIO(Strobe,0);
        printf("Comms Failed...\n");
        }
        usleep(10000);
}

int commandMSG_PING(int input)      //MSG Ping
{
              sendMsg(input);
              usleep(10000);
              writeGPIO(Strobe,1);
              usleep(10000);
              status=ackReceive1();
              if (status==1)  // if ACK was received properly sensor working
              {
                  writeGPIO(Strobe,0);
                  printf("Ack Received, Sensor working properly \n");

              }
              else if (status==2)  // if ACK was received properly but sensor not working
              {
                  writeGPIO(Strobe,0);
                  printf("Ack Received, Sensor NOT working properly \n");
              }
              else            // if ACK was not received properly print out what was received for debugging purposes
              {
                  printf("Received: %d %d %d %d\n", readGPIO(GP_12),readGPIO(GP_11), readGPIO(GP_10), readGPIO(GP_9) );

                  writeGPIO(Strobe,0);
                  printf("Comms Failed...\n");
                }
              usleep(10000);
}

int commandMSG_GET(int input)       //MSG Get
{
    printf("Sending Reset Message...\n");
    sendMsg(input);
    usleep(10000);
    writeGPIO(Strobe,1);
    usleep(10000);
    status=ackReceive2();
    if (status==1)            // if ACK was received properly
    {
        p = 0;
        writeGPIO(Strobe,0);  // low strobe tells pick to load first 4 bits of ADC
        usleep(10000);
        printf("Ack Received...\n");
        writeGPIO(Strobe,1);  // at high strobe, Galileo reads and records first 4 bits of ADC
        usleep(10000);
        adcArray[p] = readGPIO(GP_9);
        adcArray[p+1] = readGPIO(GP_10);
        adcArray[p+2] = readGPIO(GP_11);
        adcArray[p+3] = readGPIO(GP_12);
        writeGPIO(Strobe,0);  // low strobe tells PIC to load next 4 bits of ADC
        usleep(10000);
        writeGPIO(Strobe,1);  // at high strobe Galileo reads next 4 bits of ADC
        usleep(10000);
        adcArray[p+4] = readGPIO(GP_9);
        adcArray[p+5] = readGPIO(GP_10);
        adcArray[p+6] = readGPIO(GP_11);
        adcArray[p+7] = readGPIO(GP_12);
        writeGPIO(Strobe,0);  // Low strobe tells PIC to load final 4 bits of ADC
        usleep(10000);
        writeGPIO(Strobe,1);  // high strobe tells Galileo to read final 4 bits of ADC
        usleep(10000);
        adcArray[p+8] = readGPIO(GP_9);
        adcArray[p+9] = readGPIO(GP_10);
        adcArray[p+10] = readGPIO(GP_11);
        adcArray[p+11] = readGPIO(GP_12);
    writeGPIO(Strobe,0);
    printf("the read in ADC value is = ");  // prints out the read in bits from low(left) to high(right) and also converts the binary values into
    for(p=0;p<12;p++)                        // decimal values at the same time
    {
        ADCvalue += adcArray[p]*pow(2,p);
        printf(" %d ",adcArray[p]);           // prints out the individual bits(little-endian)
    }
    printf("  = %d \n", ADCvalue);             // prints out the final decimal equivalent of the binary number received
    ADCvalue = 0;

        }
    else // if ACK not received prints out the
     {
        printf("Received: %d %d %d %d\n", readGPIO(GP_12),readGPIO(GP_11), readGPIO(GP_10), readGPIO(GP_9) );

        writeGPIO(Strobe,0);
        printf("Comms Failed...\n");
    }
    usleep(10000);

}

int commandMSG_TURN30(int input)    //MSG Turn 30
{
                printf("Sending Reset Message...\n");
                sendMsg(input);
                usleep(10000);
                writeGPIO(Strobe,1);
                usleep(10000);
                status=ackReceive3();
                if (status==1)
                {
                    writeGPIO(Strobe,0);
                    printf("Ack Received...\n");

                }
                else
                {
                    printf("Received: %d %d %d %d\n", readGPIO(GP_12),readGPIO(GP_11), readGPIO(GP_10), readGPIO(GP_9) );

                    writeGPIO(Strobe,0);
                    printf("Comms Failed...\n");
                      }
                usleep(10000);


}

int commandMSG_TURN90(int input)    //MSG Turn 90
{
    printf("Sending Reset Message...\n");
    sendMsg(input);
    usleep(10000);
    writeGPIO(Strobe,1);
    usleep(10000);
    status=ackReceive4();
    if (status==1)
    {
        writeGPIO(Strobe,0);
        printf("Ack Received...\n");
    }
    else
    {
        printf("Received: %d %d %d %d\n", readGPIO(GP_12),readGPIO(GP_11), readGPIO(GP_10), readGPIO(GP_9) );

        writeGPIO(Strobe,0);
        printf("Comms Failed...\n");
    }

    usleep(10000);
}

int commandMSG_TURN120(int input)   //MSG Turn 120
{
    int status;
    printf("Sending Reset Message...\n");
    sendMsg(input);
    usleep(10000);
    writeGPIO(Strobe,1);
    usleep(10000);
    status=ackReceive5();
    if (status==1)
    {
        writeGPIO(Strobe,0);
        printf("Ack Received...\n");
    }
    else
    {
        printf("Received: %d %d %d %d\n", readGPIO(GP_12),readGPIO(GP_11), readGPIO(GP_10), readGPIO(GP_9) );

        writeGPIO(Strobe,0);
        printf("Comms Failed...\n");
    }
    
    usleep(10000);


}