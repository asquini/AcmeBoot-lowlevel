#ifndef board_lowlevel_h
#define board_lowlevel_h

// USER modifiable clock and noise parameters


#define LOW_NOISE_SDRAM     // if defined it will increase the rise and fall 
                            // times of SDRAM clock and signals to reduce EMC noise 
                            // (reduced slew rate)
#define SELF_REFRESH_SDRAM  // if defined will reduce drastically the EMC noise 
                            // from the SDRAM signals enabling the SDRAM clock 
                            // only when the SDRAM is accesses. 
                            // (slows down slightly the CPU-SDRAM interface)
//#define MASTER_CLOCK_ON_PLLB  // if defined the master clock is derived 
                              // from the PLL B instead of PLL A.
                              // CPU clock will be 48/96 MHz and SDRAM clock will be 16/32 MHz based on kernel setup of PLLB

#define CPU_CLOCK_MULA  56      // valid only if not defined MASTER_CLOCK_ON_PLLB
                              // it is the multiplication value for the PLLA, responsible 
                              // to the cpu clock generation.
                              // the following values can give a correct baudrate for the serial ports:
															// CPU_CLOCK_MULA     CPU clock     SDRAM clock
                              // 32                 304 MHz       101 MHz
                              // 34                 323 MHz       108 MHz 
                              // 35                 332 MHz       111 MHz
                              // 37                 350 MHz       117 MHz
                              // 38                 359 MHz       120 MHz
                              // 40                 378 MHz       126 MHz
                              // 41                 387 MHz       129 MHz 
                              // 42                 396 MHz       132 MHz     default Netus
													    // 43                 405 MHz       135 MHz
                              // 44                 415 MHz       138 MHz
                              // 46                 433 MHz       144 MHz
                              // 47                 442 MHz       147 MHz
                              // 49                 461 MHz       154 MHz  
                              // 50                 470 MHz       157 MHz 
                              // 52                 488 MHz       163 MHz
                              // 53                 498 MHz       166 MHz
                              // 55                 516 MHz       172 MHz
                              // 56                 523 MHz       175 MHz


// USER end


//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
/*
    Constants: Clock and PLL settings

        BOARD_OSCOUNT - Startup time of main oscillator (in number of slow clock
                        ticks). 
        BOARD_USBDIV - USB PLL divisor value to obtain a 48MHz clock.
        BOARD_CKGR_PLL - PLL frequency range.
        BOARD_PLLCOUNT - PLL startup time (in number of slow clock ticks).
        BOARD_MUL - PLL MUL value.
        BOARD_DIV - PLL DIV value.
        BOARD_PRESCALER - Master clock prescaler value.
*/
#define BOARD_OSCOUNT           (AT91C_CKGR_OSCOUNT & (64 << 8))
#define BOARD_CKGR_PLLA         (AT91C_CKGR_SRCA | AT91C_CKGR_OUTA_0)
#define BOARD_PLLACOUNT         (0x3F << 8)
#define BOARD_MULA              (AT91C_CKGR_MULA & (CPU_CLOCK_MULA << 16))
#define BOARD_DIVA              (AT91C_CKGR_DIVA & 1)
#define BOARD_PRESCALER         (0x00001300) 

#ifdef MASTER_CLOCK_ON_PLLB

#define BOARD_USBDIV            ( 2 << 28 )         // sets PLLB at 96 MHz and derives the USB 48 MHz clock 
                                                    // by a further division by 2
																										// Linux then switches to 48 MHz CPU and 16 MHz SDRAM clock and it 
                                                    // goes in error if kernel starts ETH at 100 Mbps.
                                                    // starting at 10 Mbps goes ok.
                                                    // probably kernel reprogram PLLB for USB			 
#define BOARD_CKGR_PLLB         AT91C_CKGR_OUTB_0
#define BOARD_PLLBCOUNT         BOARD_PLLACOUNT
#define BOARD_MULB              (51 << 16)
#define BOARD_DIVB              5
#else
#define BOARD_USBDIV            AT91C_CKGR_USBDIV_1
#define BOARD_CKGR_PLLB         AT91C_CKGR_OUTB_0
#define BOARD_PLLBCOUNT         BOARD_PLLACOUNT
#define BOARD_MULB              (25 << 16)
#define BOARD_DIVB              5
#endif

#define AT91C_MATRIX_CS1A_SDRAMC_LOWNOISE  (0x10001 <<  1)   // using this setting when enabling the CS of the SDRAM
																														 // will set the SLOW slew rate for the SDRAM signals
                                                             // datasheet 9g20 page 768	
// use this MASTERCLK on main.c to enable the correct baudrate on DBGU
#ifdef MASTER_CLOCK_ON_PLLB
#define MASTERCLK  (int)(18432000 * ((BOARD_MULB>>16)+1) / (BOARD_DIVB*6))  
#else
#define MASTERCLK  (int)(18432000 * ((BOARD_MULA>>16)+1) / (BOARD_DIVA*6))
#endif


#endif
