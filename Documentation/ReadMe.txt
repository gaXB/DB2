This sample project is made to demonstrate the following S32K1xx FlexCAN features with SDK FlexCAN PD driver:

1. Configure to receiver the following exact 16 standard ID CAN message with RxFIFO 8x ID filter table with format type-B(2x 16-bit ID)
   Standard ID: 0x11,  0x22,  0x33,  0x44,  0x55,  0x66,  0x77,  0x88,
                0x111, 0x222, 0x333, 0x444, 0x555, 0x666, 0x777, 0x788
   
   The RxFIFO is also configured to use eDMA channel 0 for CAN message receive, user can easily change to use CPU interrupt for RxFIFO in processor expert flexcan component configuration if required.
   
   Note: A. FlexCAN of S32K1xx dose not support to receive CAN-FD message frame with RxFIFO, so no CAN-FD support in this demo project.
         B. all the filter table elements must be configured to contain only standard or extend ID, if it contains both standard and extend ID, the IDE-bit mask will be ignored. 
         C. After RxFIFO enabled, MB0~MB5 is used as the RX FIFO, at least MB6~MB7 are used as the ID filter table(to store the acceptance ID), the actual available MB number is determined by RxFIFO ID filter table size, details please refer to section--55.4.6 Rx FIFO structure of S32K1xx RM Rev.12.1 
   
2. Configure one extra individual MB(MB8) to receive 16 standard ID CAN 2.0 message with the lower 4LSB masked(mask=0x7F0, acceptance ID = 0x123): 0x120 ~ 0x12F;

	Both RxFIFO and individual MB RX use non-blocking receive method/API with MB TX/RX complete ISR callback installed to set a new circle buffer for next message frame receive

3. Configure one individual MB(MB9) to blocking transmit a standard CAN 2.0 message with ID = 0x100 periodically(period = 5ms), and also send back the received CAN messages(if it's available) to the CAN bus as a response.

4. Provide the FlexCAN bus-off manual recovery configuration API and interrupt ISR callback codes for reference, changing the macro CAN_BUSOFF_RECOVERY_MANUAL(in include/Config.h) to select the bus off recovery method(enable the macro definition: manual recovery, comment the macro definition: automatic recovery);

	Note: In this sample project,  the macro CAN_BUSOFF_RECOVERY_MANUA is enabled by default, and manual recovery codes works.
	
	      To make the bus-off recovery callback work, user should replace the flexcan PD driver codes and S32K144_feature.h with S32K1xx RTM 4.0.0(which can be downloaded from nxp.com with registered account login and then installed stand-alone or installed via S32DS v3.3 IDE update). This is already done this sample project!!! 
	      
5. There 3 on-board RGB LED are used to indicate the FlexCAN working status:

   red RGB LED will be toggled after RXFIFO received any CAN message;
   blue RGB LED will be toggled after individual MB received any CAN message;
   green RGB LED will be turn ON after enter bus-off and turn OFF after exist bus-off(recover successfully).

To run this sample project, the following HW and SW require:

SW: S32DS for ARM v2.2 IDE with S32K1xx SDK RTM 3.0.3 installation
HW: S32K144EVB-Q100 Rev.C with a DC-12V adapter for its power supply by J16 and a USB-to-CAN adapter(such as PEAK CAN) to connect PC with J13 of the EVB
   
   
