$NOMOD51
;------------------------------------------------------------------------------
;
;  Copyright (c) 2015 SONiX Technology Co., Ltd.
;  Version 2.0 - SN8F5702, SN8F57021, SN8F57022, SN8F57023, SN8F57024
;  Version 2.1 - Add code option address for MP5
;  Version 2.2 - Remove LVD option
;  *** <<< Use Configuration Wizard in Context Menu >>> ***
;------------------------------------------------------------------------------
;
;  This preference, such as watchdog, external reset pin, and clock source, is preloaded 
;  during the microcontroller's power-on. It is strongly recommanded to use configuration 
;  wizard to set these parameters up appropriately.
;
;------------------------------------------------------------------------------
ROM_SIZE		EQU		0x1000
;
;   <o> Program Memory Security <0x01=> Disable <0x00=> Enable
    SECURITY_SET    EQU     0x00		;	{0x0FFF}
;   <i> The debug interface cannot read program memory if this security option is enable.
;   <i> Erase Full Chip can be proformmed to erase original code/data and unlock security.
;
;   <o.1..3> CPU Clock Source <0x07=> IHRC 32 MHz
    CLOCKSRC_SET    EQU     0x0E		;	{0x0FFF}
;   <i> IHRC 32 MHz: on-chip internal clock with or without Timer 0 real time clock.
;
;   <h> Reset Sources
;       <o.4..5> External Reset / GPIO Shared Pin <0x00=> Reset with De-bounce <0x02=> Reset without De-bounce <0x03=> GPIO
        RESETPIN_SET    EQU     0x30	;	{0x0FFC}
;       <i> Reset with De-bounce: Triggers reset if this pin is pulled low over 4 ms.
;       <i> Reset without De-bounce: Triggers reset immediately if this pin is pulled low.
;       <i> GPIO: The shared pin is reserved for general purpose input/output.
;       <i> The de-bounce period is based on Internal Low R-C Clock which has a gentle inaccuracy.
;
;       <o.4..7> Watchdog Reset <0x00=> Always <0x05=> Enable <0x0A=> Disable
        WATCHDOG_SET    EQU     0xA0	;	{0x0FFF}
;       <i> Always: Trun on watchdog function including Normal, IDLE, and SLEEP mode.
;       <i> Enable: Turn on watchdog function only in Normal mode.
;       <i> Disable: Turn off watchdog function.
;
;       <o.6..7> Watchdog Overflow Period <0x00=> 64 ms <0x01=> 128 ms <0x02=> 256 ms <0x03=> 512 ms
        WATCHCLK_SET    EQU     0xC0	;	{0x0FFC}
;       <i> The watchdog overflow period is based on Internal Low R-C Clock which has a gentle inaccuracy.
;   </h>


    CSEG    AT      0x0FF6
    DB      0xFF
    DB      0xFF
    DB      0xFF
    DB      0xFF
    DB      0xFF
    DB      0xFF
    DB      WATCHCLK_SET + RESETPIN_SET + 0x06
    DB      0x5A
    DB      0xA5
    DB      WATCHDOG_SET + CLOCKSRC_SET + SECURITY_SET
    END
