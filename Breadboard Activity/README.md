NOTES:
    - For the popup for Initializing all peripherals with their default mode, click no.  
    - the stm32l4xx_hal_msp.c and stm32l4xx_it.c files were deleted from the  
      Src directory  
    - this was done on a mac, thus a TTY terminal was used.  
        - ls /dev/*modem*  
        - screen /dev/tty.usbmodemxxxxx where the x's are numbers.  

Commands and Functionality in TTY terminal: 
    - RON: Turn on the Red LED  
    - ROFF: Turn off the Red LED  
    - RFLASH: Flash the Red LED  
    - GON: Turn on the Green LED  
    - GOFF: Turn off the GREEN LED  
    - GFLASH: Flash the Green LED  
    - ALLOFF: Turn off all the LEDs  
    - HELP: displays a help menu with all listed commands and their
      functions
    Also supports backspaces and tells the user if they enter an 
    invalid command.  
    Utilized a state machine



