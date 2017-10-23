// Program Information ////////////////////////////////////////////////////////
/**
  * @file data.h
  *
  * @brief defines classes configData and metaData   
  * 
  * @details defines member functions and data members
  *
  * @version 1.01 Carli DeCapito
  *			 Compilation of 2 classes (10/3/17)
  *
  *			 1.00 Carli DeCapito
  * 		 Original Document (9/11/17)
  *
  * @note None
  */

//precompiler directives
#ifndef DATA_H
#define DATA_H

//header files
#include <iostream>
#include <vector>

using namespace std;

//configData class definition
class configData
{
	//class constants
	static const int STR_LEN = 100;

	public:	
		//constructor
		configData();
		//destructor
		~configData();

		//data members
		char version[ STR_LEN ];
		char filePath[ STR_LEN ];
		char logInfo[ STR_LEN ];
		char logFilePath[ STR_LEN ];
		int processorCT;
		int monitorDT;
		int hardDriveCT;
		int printerCT;
		int keyboardCT;
		int memoryCT;
		int mouseCT;
		int speakerCT;
		int sysMemory;
		int memoryBlockSize;
		int numHardDrive;
		int currHardDrive;
		int numPrinter;
		int currPrinter;
		int lastAddUsed;
		bool firstAlloc;

};


//class metaData definition
class metaData
{
	//class constants
	static const int STR_LEN = 100;
	static const int CODE_ERROR = 0;
	static const int ACTION_ERROR = 1;
	static const int CYCLE_ERROR = 2;

	public:
		//constructor
		metaData();
		//destructor
		~metaData();

		//member functions
		bool isDescriptorValid( configData cData );

		void getEndString( char string[],
						   char process[],
						   char memory[] );

		void getStartString( char string[],
							 char process[] );

		bool errorCheck( char error[],
					  	 configData cData );

		//data members
		char metaCode;
		char metaDescriptor[ STR_LEN ];
		int cycles;
		int time;
		bool used;
};

#endif