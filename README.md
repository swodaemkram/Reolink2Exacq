<H4>What is this program</H4>
I have an Exacq Vision Server and I recently acquired a couple of Reolink 5Mp cameras with AI. These are in no way compatible with Exacq Vision and never will be. They are super inexpensive cameras not ment for commercial use. So, If I wanted to use motion detection and AI capabilities the only choice I have is to write it myself. So Reolink2Exacq is a working daemon that will allow all the above to function with my Exacq vision server.<br>

<H4>About the program</H4>
The program is writen in ANSI C++ it does use the CURL library I may write the API communication sections to not use curl but I wanted to get this working so I could use it on my system quickly. That being said this could be compiled easily to an ARM or other Micro. Right now it will run as a daemon, will run multi-instances, will detect motion from the camera and then relay in searchable text on the Exacq Vision server the type of motion, Human, Animal, Vehical.<br>

<H4>How to use the program:</H4>
Command line utility is ment to run as a daemon on linux OS typical usage:<br>
./Reolink2Exacq [camera i.p.] [camera user name] [camera password] [Exacq Vision Server I.P.] [Exacq Vision Server Port] [Instance Name] [use AI triggering only] & disown<br>
<br>
<br>
Real Example: ./Reolink2Exacq 10.10.10.31 admin pAsswoRd 10.10.10.19 1234 GarageCam & disown
<br>
Example 2: ./Reolink2Exacq 10.10.10.32 admin pAsswoRd 10.10.10.19 1235 TomatoCam AIO & disown
<br>
<br>
of course you need to setup a serial port on your Exacq Vision server for each instance of Reolink2Exacq you are running. Also a serial profile for each instance and an event link for each. 
