    -----------------------------------------------------------------------------

                   --- CAEN SpA - Computing Systems Division --- 

    -----------------------------------------------------------------------------

    CAEN V8100  Rel. 1.0

    Installation and Use Instructions

    -----------------------------------------------------------------------------

    September 2008

    -----------------------------------------------------------------------------


 The complete documentation can be found on CAEN's Web Site at http://www.caen.it.


 Content 
 -------
      
 |- CAEN_V8100Readme.txt           :  This file
 |
 |- CAEN_V8100ReleaseNotes.txt     :  Release Notes of the last software release
 |
 |- CAEN_V8100.tar.gz              :  Gzipped tar file with the CAENV8100SDK and demos
      
 System Requirements
 -------------------
 
 - Linux kernel Rel. 2.4/2.6 with gnu C/C++ compiler
 - For the VME8100_Manager:
     - Java Run time >= 1.5
     - RXTX >=2.0 (for RS232 Communication.) Download it at http://users.frii.com/jarvi/rxtx/download.html

 Installation notes
 ------------------
 
 - Decompress the gzipped tar file ( tar -xvf CAEN_V8100.tar.gz ).
 
 - Install the dynamic library ( go to the library folder and type make install ).
 
 Notes
 -----
 - Tested with Linux version 2.6.22, GCC-4.2.1
 
 - Note that the SDK's APIs (and the demos relying on that) may open a physical serial comunication 
   device to inquire the board, so you likely need to be root to have the SDK properly working.
