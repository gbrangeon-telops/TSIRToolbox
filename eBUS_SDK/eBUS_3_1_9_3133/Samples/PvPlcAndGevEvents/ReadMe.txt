// ***************************************************************************************
//
//     Copyright (c) 2008, Pleora Technologies Inc., All rights reserved.
//
// ***************************************************************************************
//
// File Name....: README.TXT
//
// Date Created: 2008/Oct/08
//
//  Application : PvPlcAndGevEvents sample Project Overview
//
// ***************************************************************************************

This sample program outlines how to perform a few tasks using the eBUS SDK.
The tasks are grouped in the following buttons:

1) "Select/Connect" button. It shows how to connect to an IP engine through Gev Device 
    Selection window.

3) "Acquisition Start" (or "Acquisition Stop") button. It shows how to start (or Stop) 
    continuous grabbing and displaying.
    At acquisition start it resets the miNbOfCallback variable and Counter1 value. 
       

4) "Setup PLC" button. It shows how to set up Pleora IP Enginect PLC
    
	4.1) Set up Signal Routing Block:
	      
	      #ifdef USE_FVAL
                   PLC_I0 =PLC_A4    //PLC_A4 is the FVAL signal from camera
              #else
                   PLC_I0 =PLC_ctrl0
              #endif
              
                   PLC_I4 =PLC_ctrl1
        
	4.2) Set up PLC LUT:
            
                   PLC_Q7     = I0   // PLC_Interrupt
                   PLC_Q17    = I0   // Counter increament signal
                   PLC_Q3     = I4   // Counter reset
        
	4.3) Enable PLC_Interrupt_FIFO_Q7 with GigEVisionEvent notification. 
             Register an event sink for PvDeviceEventSink::OnEvent(). 
             When the PLC interrupt happens the PvDeviceEventSink::OnEvent() callback function will be called, 
             and the EventID should be 36869.

        4.4) Set up Counter1 to count the rising edge of PLC_Q17 signal and be cleared by the 
             PLC_Q3 signal.
        
        4.5) Clear the counter by clean and set the PLC_ctrl1 bits

5) "Generate 1 pulse" button. It is only enabled when USE_FVAL is not defined.
    Every click on this button clean and set the PLC_ctrl0 bit which generates one pulse on 
    both PLC_Q17 and PLC_Q7. This results in counter value increase 1 and OnEvent() callback
    get called once. 

6) "Device control" button. It shows how to bring out the "Gev Device Control" node map.
    User can also clean and set the PLC_ctrl0 bit there which has same effect as click on button
    "Generate 1 pulse".
 
Note: a). USE_FVAL is not defined as default. 
    
      b). If the camera is a line scan camera then there is no FVAL signal. 
          #define USE_FVAL does not work in this situation
    
      c). Based on the GenICam xml implementation in your IP Engine It may not have all the PLC 
          features. To have this sample function properly all the PLC features this sample try to access
          must be present in node map in the "GEV Device Control" window. 
 

