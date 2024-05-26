/*
 ============================================================================
 Name        : Reolink2Exacq.c
 Author      : Mark Meadows
 Version     : 0.1.0b
 Copyright   : copyright 2024
 Description : Reolink2Exacq in C, Ansi-style
 Allows Reolink Camera to be used with Exacq Vision NVR
 (Implaments Motion Detection and AI (Person,Car,Animal) Detection)
=============================================================================
 Usage: ./Reolink2Exacq CameraIP UserName Password ExacqIP ExacqPort

 Example: ./Reolink2Exacq 10.10.10.32 admin pAsswOrd 10.10.10.19 1235

 http://10.10.10.32/api.cgi?cmd=GetAiState&user=admin&password=pAsswOrd
 ============================================================================
 */
using namespace std;

#include <string>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>


void WriteToLog(string log_message);
int CheckCameraMotionStatus(string CameraIP, string CameraUserName, string CameraPassword);
int CheckCameraTypeOfMotion(string CameraIP, string CameraUserName, string CameraPassword);
void SendDataToExacqServer(char* ExacqIP, int port, char* Message);
void ParceCommandLineArguments(string CameraIP, string CameraUserName, string CameraPassword, string ExacqIP, string ExacqPort);



struct curl_slist *headers = NULL;

    string CameraIP;
	string CameraUserName;
    string CameraPassword;
    char* ExacqIP;
    int ExacqPort;
    int PID;
    string PID_string;
    string Instance_Name;
    string AIO = " ";
    bool Logging = true;

int main(int argc, char *argv[]) {

	    if (argc < 2)
	    {
	    	cout << "Reolink2Exacq V1.001 By Mark Meadows (c)2024"<<endl;
	    	return EXIT_SUCCESS;

	    }


	    CameraIP = argv[1];
	    CameraUserName = argv[2];
	    CameraPassword = argv[3];
	    ExacqIP = argv[4];
	    ExacqPort = std::stoi( argv[5]);
	    Instance_Name = argv[6];
	    Logging = argv[7];


	    if (argc > 8)
	    {
	    	cout << argv[8];
	    	AIO = argv[8];
	    }

	PID = getpid();
	cout << PID << endl;
    PID_string = to_string(PID);

	WriteToLog(PID_string + " " + Instance_Name + " Reolink2Exacq has started..." );

	if(argc < 7){
		cout<< " " << endl;
		cout<<"Please check command line arguments" << endl;
	    cout<<"Example: ./Reolink2Exacq 10.10.10.32 admin pAsswOrd 10.10.10.19 1235 Cam1 AIO" << endl;
	    cout<< " " << endl;
	    WriteToLog(PID_string + " " + Instance_Name + " Reolink2Eacq Stoped with Incomplete arguments error...");
		return(2);
	}


    while(true)
    {
    usleep(50000); //sleep for 500ms
    int CameraMotion = CheckCameraMotionStatus(CameraIP, CameraUserName, CameraPassword);
     }


    WriteToLog(PID_string + " " + Instance_Name + "Reolink2Exacq has Stoped ...");
	return EXIT_SUCCESS;
}


/////////////////////////////////////////     Functions   //////////////////////////////////////////////////

void ParceCommandLineArguments(string CameraIP, string CameraUserName, string CameraPassword, string ExacqIP, string ExacqPort )
{
	cout<< CameraIP << " " << CameraUserName << " " << CameraPassword <<" " << ExacqIP << " " << ExacqPort << endl;
    string CommandLineArguments = CameraIP + " " + CameraUserName + " " + CameraPassword + " " + ExacqIP + " " + ExacqPort;

	WriteToLog(CommandLineArguments);

	return;
}


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int  CheckCameraMotionStatus(string CameraIP, string CameraUserName, string CameraPassword)
{

	//http://10.10.10.32/api.cgi?cmd=GetMdState&channel=0&user=admin&password=pAsswOrd

	string CameraURL = "http://" + CameraIP + "/api.cgi?cmd=GetMdState&channel=0&user=" + CameraUserName + "&password=" + CameraPassword ;

	//string CameraURL = "http://10.10.10.32/api.cgi?cmd=GetMdState&channel=0&user=admin&password=pAsswOrd";


	std::string readBuffer;

	CURL *curl;
				   CURLcode res;
				   curl_global_init(CURL_GLOBAL_DEFAULT);
				   curl = curl_easy_init();

				  // WriteToLog(CameraURL);

				   curl_easy_setopt(curl, CURLOPT_URL, CameraURL.c_str());
				   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
				   curl_easy_setopt(curl, CURLOPT_NOSIGNAL,  1);

				   res = curl_easy_perform(curl); //<<<Execute curl>>>

				   //cout<<res<<endl;

				   if(res == '7')
				   {
					WriteToLog (PID_string + " " + Instance_Name + "Unable to Connect to Camera ...");
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			        curl_easy_cleanup(curl);
			        curl_global_cleanup();
			        return(0);
				   }



				  if (readBuffer.length() < 10)
				   {
					WriteToLog(PID_string + " " + Instance_Name + " Camera Reply Error");
					curl_easy_cleanup(curl);
					curl_global_cleanup();
					return(0);
				   }


				  string StatusReturnedValue = readBuffer.substr(90, 1); //Result from camera

				//? std::system("clear"); //Clear the screen
				 //cout<<readBuffer<<endl;
				 //**cout<<StatusReturnedValue<<endl;

				 if(StatusReturnedValue == "0")
				 {
					 curl_easy_cleanup(curl);
					 curl_global_cleanup();
					 return (0);
				 }

				 WriteToLog(PID_string + " " + Instance_Name + " Motion Detected");
                 char* Message = "Motion Detected\n";

                 if (AIO != "AIO") //AI only if true will not send motion data just AI data **************************************
                 {
                 SendDataToExacqServer(ExacqIP, ExacqPort, Message);
                 }

				 CheckCameraTypeOfMotion(CameraIP, CameraUserName, CameraPassword);

				 curl_easy_cleanup(curl);
				 curl_global_cleanup();
		         return (1);
}


int CheckCameraTypeOfMotion(string CameraIP, string CameraUserName, string CameraPassword)
{
	//http://10.10.10.32/api.cgi?cmd=GetMdState&channel=0&user=admin&password=pAsswOrd

		string CameraURL = "http://" + CameraIP + "/api.cgi?cmd=GetAiState&user=" + CameraUserName + "&password=" + CameraPassword ;

		//http://10.10.10.32/api.cgi?cmd=GetAiState&user=admin&password=pAsswOrd


		std::string readBuffer;

		CURL *curl;
					   CURLcode res;
					   curl_global_init(CURL_GLOBAL_DEFAULT);
					   curl = curl_easy_init();

					  // WriteToLog(CameraURL);

					   curl_easy_setopt(curl, CURLOPT_URL, CameraURL.c_str());
					   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
					   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
					   curl_easy_setopt(curl, CURLOPT_NOSIGNAL,  1);

					   res = curl_easy_perform(curl); //<<<Execute curl>>>

					   //cout<<res<<endl;

					   if(res == '7')
					   {
						WriteToLog (PID_string + " " + Instance_Name + " Unable to Connect to Camera ...");
						fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				        curl_easy_cleanup(curl);
				        curl_global_cleanup();
				        return(0);
					   }



					  if (readBuffer.length() < 10)
					   {
						WriteToLog(PID_string + " " + Instance_Name + " Camera Reply Error");
						curl_easy_cleanup(curl);
						curl_global_cleanup();
						return(0);
					   }

					 string Animal = readBuffer.substr(146,1);
					 string Person = readBuffer.substr(326,1);
					 string Vehical = readBuffer.substr(418,1);

					 int AnimalInt = stoi(Animal);
                     if(AnimalInt == 1)
                     {
                    	 char* Message = " Animal Detected\n";
                    	 SendDataToExacqServer(ExacqIP, ExacqPort, Message);
                         WriteToLog(PID_string + " " + Instance_Name +"  Animal Detected\n");
                     }

                     int PersonInt = stoi(Person);
                     if(PersonInt == 1)
                     {
                       	 char* Message = " Person Detected\n";
                       	 SendDataToExacqServer(ExacqIP, ExacqPort, Message);
                         WriteToLog(PID_string + " " + Instance_Name + " Person Detected \n");
                     }

                     int VehicalInt = stoi(Vehical);
                     if(VehicalInt == 1)
                     {
                       	 char* Message = " Vehical Detected\n";
                       	 SendDataToExacqServer(ExacqIP, ExacqPort, Message);
                         WriteToLog(PID_string + " " + Instance_Name + " Vehical Detected\n");
                      }


					//* WriteToLog("AI Detected");
					//* SendDataToExacqServer(ExacqIP, ExacqPort);

					 curl_easy_cleanup(curl);
					 curl_global_cleanup();
			         return (1);
}


void SendDataToExacqServer(char* ExacqIP, int port, char* Message)
{

	 int CreateSocket = 0;

	 struct sockaddr_in ipOfServer;

	  if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0))< 0)
	     {
	         printf("Socket not created \n");
	         return ;
	     }
	   ipOfServer.sin_family = AF_INET;
	   //ipOfServer.sin_port = htons(1234);
	   ipOfServer.sin_port = htons(port);
	   //ipOfServer.sin_addr.s_addr = inet_addr("10.10.10.19");

	   ipOfServer.sin_addr.s_addr = inet_addr(ExacqIP);

	   if(connect(CreateSocket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer))<0)
	      {
	          WriteToLog(PID_string + " " + Instance_Name + " Connection to Exacq Server failed due to port and ip problems");
	          return ;
	      }

	       char* message = Message;

	        send(CreateSocket, Message, strlen(message), 0);
	    	close(CreateSocket);
	return;
}

void WriteToLog(string log_message)
{

            	struct timespec ts;
			    timespec_get(&ts, TIME_UTC);
			    char buff[100];
			    strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));

				 ofstream file;
				 file.open ("/var/log/Reolink2Exacq_" + Instance_Name + ".log",std::ios_base::app);

				char MyTime[100];
				sprintf(MyTime," %s.%09ld " , buff,ts.tv_nsec); //Format and apply data

				file<<MyTime;
				file<<log_message;
				file<<"\n";
				file.close();

				return;
}

//       Validate IP Address
bool validateIpAddress(const string &ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}
//      End Validate IP Address


////////////////////////////////////////   End Of Functions   //////////////////////////////////////////////
