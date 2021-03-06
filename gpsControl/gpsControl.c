#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
void usage( void )
{
    printf("Usage: gpsControl <mode> -d <device>\n");
    printf("Modes are:\n");  
    printf("   -s = stationary mode\n");  
    printf("   -p = portable mode\n");
    printf("   -d = device \n");    
}
 
unsigned char ReadBByteFromSerial(int fd, int *k)
{
    unsigned char tmp;
   
    (*k)++;
     
    if (read(fd, &tmp, 1) > 0)
    {
        // if (tmp & 0x80)
        // {
            // printf("<%2x> ", tmp);
        // }
        // else
        // {
            // printf("%c", tmp);
        // }
    }
   
    return tmp;
}
 
 
int main(int argc, char *argv[])
{
    const  char cfg_cfg[]  =  {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1D, 0xAB};
   
    const char portable[]  =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10};
    const char sationary[] =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x60};    
    const char walking[]   =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x76};
    const char automotive[] =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x98};    
    const char sea[]       =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0xBA};    
    const char airborne1[] =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC};    
    const char airborne2[] =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0xFE};    
    const unsigned char airborne4[] =  {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, \
                               0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x20};
    const unsigned char ack_ack[] =    {0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x24, 0x32, 0x5B};
    char buff[50000];
   
    unsigned char *toSend;
    int len, fd, Retries;
    int option = 0;
    int k = 0;
    int OK;
    char devName[50];
    struct termios options;
   
    memset( devName, 0, sizeof(devName));
    while(( option = getopt( argc, argv, "spd:")) != -1)
    {
        switch( option)
        {
            case 'd':
                printf("Configuring device %s\n", optarg );
                strncpy( devName, optarg, sizeof(devName ));                
                break;
            case 's':
                printf("Set GPS for stationary mode\n");
                toSend = (char*)sationary;
                len = sizeof(sationary);              
                break;
  /*          case 'a':
                printf("Set GPS for Automotive mode\n");
                toSend = (char*)automotive;
                len = sizeof(automotive);                
                break;
   */          
            case 'p':
                printf("Set GPS for portable mode\n");
                toSend = (char*)portable;
                len = sizeof(portable);              
                break;
/*                
            case 'S':
                printf("Set GPS for Sea mode\n");
                toSend = (char*)sea;
                len = sizeof(sea);                    
                break;
            case 'w':
                printf("Set GPS for Walking mode\n");
                toSend = (char*)walking;
                len = sizeof(walking);                          
                break;                
            case '1':
                printf("Set GPS for Airborne <1G mode\n");
                toSend = (char*)airborne1;
                len = sizeof(airborne1);                            
                break;
            case '2':
                printf("Set GPS for Airborne <2G mode\n");
                toSend = (char*)airborne2;
                len = sizeof(airborne2);                      
                break;
            case '4':
                printf("Set GPS for Airborne <4G mode\n");
                toSend = (char*)airborne4;
                len = sizeof(airborne4);                              
                break;
 */                
            default:
               usage();
               return(0);
        }        
    }
 
    if( devName[0] == 0)
    {
        usage();
        return( 0 );
    }
       
    fd = open(devName, O_RDWR); //  | O_NOCTTY);
    if (fd < 0)
    {
        printf("Cannot open device '%s'\n", devName);
        return 0;
    }
     
    // get the current options
    tcgetattr(fd, &options);
 
    options.c_lflag &= ~ECHO;
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 10;
 
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_oflag &= ~ONLCR;
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~IXON;
    options.c_iflag &= ~IXOFF;
 
    tcsetattr(fd, TCSANOW, &options);
 
    Retries = 0;  
    OK = 0;
   
    while ((Retries < 5) && !OK)
    {
        printf (">>>>>>>>>>>>>>>>>>>>> SENDING >>>>>>>>>>>>>>>\n");
        write(fd, toSend, len);
       
        k = 0;
        Retries++;
       
        while ((k < 1000) && !OK)
        {
            if (ReadBByteFromSerial(fd, &k) == 0xb5)
            {
                if (ReadBByteFromSerial(fd, &k) == 0x62)
                {
                    if (ReadBByteFromSerial(fd, &k) == 0x05)
                    {
                        if (ReadBByteFromSerial(fd, &k) == 0x01)
                        {
                            ReadBByteFromSerial(fd, &k);
                            ReadBByteFromSerial(fd, &k);
                            if (ReadBByteFromSerial(fd, &k) == toSend[2])
                            {
                                if (ReadBByteFromSerial(fd, &k) == toSend[3])
                                {
                                    OK = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
   
    if (OK)
    {
        printf("GPS mode set OK\n");
    }
    else
    {
        printf("** FAILED to set GPS Mode**\n");
    }
   
    close(fd);
 
   
    return 0;
}

