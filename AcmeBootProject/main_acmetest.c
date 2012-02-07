/* ----------------------------------------------------------------------------
 *         DaisyTest - Test utility for Daisy 1 boards and FOX Board G20
 *         http://www.acmesystems.it
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <board_memories.h>
#include <pio/pio.h>
#include <irq/irq.h>
#include <cp15/cp15.h>
#include <dbgu/dbgu.h>
#include <utility/assert.h>
#include <utility/trace.h>

#include "main.h"
//#include "daisy.h"

#include <boot.h>
#include <board_lowlevel.h>

// Used by FoxLED
#define FOXG20_LED   {1 << 7, AT91C_BASE_PIOC, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}



/// Indicates the current state (on or off) of the FOX LED.
volatile unsigned char stateFoxLED = 1;
static const Pin FoxLED = FOXG20_LED;



//------------------------------------------------------------------------------
/// Jump to the first address to execute application
//------------------------------------------------------------------------------


//static void GoToJumpAddress(unsigned int jumpAddr, unsigned int matchType)
//{
//    typedef void(*fctType)(volatile unsigned int, volatile unsigned int);
//    void(*pFct)(volatile unsigned int r0_val, volatile unsigned int r1_val);
//    
//    pFct = (fctType)jumpAddr;
//    pFct(0/*dummy value in r0*/, matchType/*matchType in r1*/);
//
//    while(1);//never reach
//}


#define be32_to_cpu(a) ((a)[0] << 24 | (a)[1] << 16 | (a)[2] << 8 | (a)[3])
#define SDRAM_START 0x20400000
#define KERNEL_UIMAGE "uimage"

#define ACME_TEST_VERSION "clock1"
//------------------------------------------------------------------------------
/// Bootstrap main application.
/// Transfer data from media to main memory and return the next application entry
/// point address.
//------------------------------------------------------------------------------
int main()
{
		int i=0;
//, j=0;
//		int Dc=0, Dp=0;

    // Enable User Reset
    AT91C_BASE_RSTC->RSTC_RMR |= AT91C_RSTC_URSTEN | (0xA5<<24);

    //-------------------------------------------------------------------------
    // Configure traces
    //-------------------------------------------------------------------------
    TRACE_CONFIGURE_ISP(DBGU_STANDARD, 115200, MASTERCLK);

    printf("\n\r");
    printf("-- Acme Test version %s --\n\r", ACME_TEST_VERSION);
    printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
        
    printf("Setting: masterclk = %d Hz\n\r",MASTERCLK);
    printf("         CPU Clock = %d Hz\n\r", MASTERCLK*3);

	  printf("AT91C_BASE_DBGU->DBGU_BRGR = %d\n\r", (int)AT91C_BASE_DBGU->DBGU_BRGR);
    printf("BOARD_MULA = 0x%0x\n\r", (int)BOARD_MULA>>16);
    printf("BOARD_DIVA = 0x%0x\n\r", (int)BOARD_DIVA);

    printf("AT91C_BASE_PMC->PMC_PLLAR = %0x\n\r", (int)AT91C_BASE_PMC->PMC_PLLAR);
    printf("MULA = 0x%0x OUTA = 0x%0x PLLACOUNT = 0x%0x DIVA = 0x%0x \n\r", (AT91C_BASE_PMC->PMC_PLLAR&0x00ff0000)>>16, 
																									 (AT91C_BASE_PMC->PMC_PLLAR&0x0000c000)>>14,
																									 (AT91C_BASE_PMC->PMC_PLLAR&0x00003f00)>>8,
																									 (AT91C_BASE_PMC->PMC_PLLAR&0x000000ff));
    printf("BOARD_OSCOUNT = %0x; AT91C_CKGR_MOSCEN = %0x \n\r",BOARD_OSCOUNT,AT91C_CKGR_MOSCEN);





	// Configure FoxG20 LED
    PIO_Configure(&FoxLED, 1);  // configure it as an output with level = logical 1. It should start with red led ON.
/*	init_Daisy1();
	init_Daisy2();
	init_Daisy3();
	init_Daisy4();
	init_Daisy5();
	init_Daisy6();
	init_Daisy7();
	init_Daisy8();
*/
    //-------------------------------------------------------------------------
    // Enable I-Cache
    //-------------------------------------------------------------------------
    CP15_EnableIcache();

    //-------------------------------------------------------------------------
    // Configure external RAM where the application will be transfered
    //-------------------------------------------------------------------------

    // SDRAM
    #if defined(DESTINATION_sdram)
    printf("Init SDRAM\n\r");
    BOARD_ConfigureSdram(BOARD_SDRAM_BUSWIDTH);
    #endif

    printf("1");
    printf("2");
    printf("3");
    printf("4");
    printf("5");
    printf("6");
    printf("8\n");

    printf("LED toggles every 300000 cycles\n\r");
#if 0
	while(1) {
		for (Dc=1; Dc<9; Dc++) {
			for (Dp=2; Dp<9; Dp++) {
				DaisyClear(Dc, Dp);
				DaisySet(Dc, Dp);
			}
		}
  }
#endif

	while(1) {
		i++;
		if (i%300000 == 0) {
			if (stateFoxLED == 1) {
				  PIO_Clear(&FoxLED);
				  stateFoxLED = 0;
			} else {
				  PIO_Set(&FoxLED);			
				  stateFoxLED = 1;
			}
    }
/*
			j++;
			switch(j) {
				case 1:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 3);
						DaisySet(Dc, 2);
					}
				break;			
				case 2:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 2);
						DaisySet(Dc, 3);
					}
				break;			
				case 3:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 3);
						DaisySet(Dc, 4);
					}
				break;			
				case 4:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 4);
						DaisySet(Dc, 5);
					}
				break;			
				case 5:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 5);
						DaisySet(Dc, 6);
					}
				break;			
				case 6:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 6);
						DaisySet(Dc, 7);
					}
				break;			
				case 7:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 7);
						DaisySet(Dc, 8);
					}
				break;			
				case 8:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 8);
						DaisySet(Dc, 9);
					}
				break;			
				case 9:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 9);
						DaisySet(Dc, 8);
					}
				break;			
				case 10:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 8);
						DaisySet(Dc, 7);
					}
				break;			
				case 11:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 7);
						DaisySet(Dc, 6);
					}
				break;			
				case 12:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 6);
						DaisySet(Dc, 5);
					}
				break;			
				case 13:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 5);
						DaisySet(Dc, 4);
					}
				break;			
				case 14:
					for (Dc=1; Dc<9; Dc++) {
						DaisyClear(Dc, 4);
						DaisySet(Dc, 3);
					}
					j=0;				
			}
		}
*/
	}


//    printf("Jump to the Kernel image at %x\n\r",entry_point);
//    GoToJumpAddress(entry_point, MACH_TYPE);
    return 0;
}

