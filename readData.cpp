// Program Information /////////////////////////////////////////////////////////
/**
  * @file readData.cpp
  *
  * @brief implements functions that read data in     
  * 
  * @details reads in configData and metaData
  *
  * @version 1.00 Carli DeCapito
  *			 Original ( 9/12/17 )
  *
  * @note None
  */
//header file
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <queue>
#include "data.h"

//global constants
static const int STR_LEN = 100;
static const int STR_LEN_LNG = 400;

//Function Definitions ////////////////////////////////////
bool getConfigData( char filename[],
					configData &cdata );

void removeSpace( char string[] );

bool isEmpty( char string[] );

bool getMetaData( char filename[], 
				  vector<metaData> &mData );

void parseString( char string[],
				  vector<metaData> &mData );

metaData parseData( int &index, 
					char string[] );

bool logFileValid( char string[] );

//Function Implementation /////////////////////////////////
/**
 * @brief getConfigData
 *
 * @details reads configuration data from file
 *          
 * @pre file exists
 *
 * @post file is read into system
 *
 * @par Algorithm 
 *      Parse through file, read in data using file input
 *      
 * @exception None
 *
 * @param [in] filename provides name that pass passed 
 *				through command line arguments
 *
 * @param [out] None
 *
 * @return bool if file was read, return true, otherwise false
 *
 * @note None
 */
bool getConfigData( char filename[], 
					configData &cdata )
{
	ifstream fin;
	char temp[ STR_LEN ];
	bool empty;
	int tempInt;

	fin.open( filename );

	//check that file exists
	if( !fin.good() )
	{
		cout << "Error. Invalid File Name." << endl;
		return false;
	}

	//get first line
	fin.getline( temp, STR_LEN, '\n' );

	//check if first lines says file is config file
	if( strcmp( temp, "Start Simulator Configuration File") != 0 )
	{
		//string is not equal to temp
		cout << "Error. File is empty or is not Configuration File" << endl;
		return false;
	}

	//loop until get all data
	while( fin.good() )
	{
		fin.getline( temp, STR_LEN, ':');
		//remove spaces from temp
		removeSpace( temp );
		
		//Read in Version/Phase
		if ( strcmp( temp, "Version/Phase") == 0 )
		{
			//get info until new line
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				strcpy( cdata.version, temp );
			}
		}
		//read in file path
		else if ( strcmp( temp, "File Path") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if( !logFileValid( temp ) )
			{
				cout << "Error. Invalid Meta-Data File Extension." << endl;
				return false;
			}

			if ( !empty )
			{
				strcpy( cdata.filePath, temp );
			}
		}
		//read in processor CT
		else if ( strcmp( temp, "Processor cycle time (msec)" ) == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.processorCT = atoi( temp );
			}
		}
		//read in Monitor DT
		else if ( strcmp( temp, "Monitor display time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.monitorDT = atoi( temp );
			}
		} 
		//read in HD CT
		else if ( strcmp( temp, "Hard drive cycle time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.hardDriveCT = atoi( temp );
			}
		}
		//read in printer CT
		else if ( strcmp( temp, "Printer cycle time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.printerCT = atoi( temp );
			}
		}
		//read in keyboard CT
		else if ( strcmp( temp, "Keyboard cycle time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.keyboardCT = atoi( temp );
			}
		}
		//read in Memory CT
		else if ( strcmp( temp, "Memory cycle time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.memoryCT = atoi( temp );
			}
		}
		//read in mouse CT
		else if ( strcmp( temp, "Mouse cycle time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.mouseCT = atoi( temp );
			}
		}
		// read in speaker CT
		else if ( strcmp( temp, "Speaker cycle time (msec)") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				//char to int conversion
				cdata.speakerCT = atoi( temp );
			}
		}
		//read in log
		else if ( strcmp( temp, "Log") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				strcpy( cdata.logInfo, temp );
			}
		}
		// read in log file path
		else if ( strcmp( temp, "Log File Path") == 0 )
		{
			//get rest of line, remove front spaces
			//check if data is there
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				strcpy( cdata.logFilePath, temp );
			}
		}
		//check if kbytes,Mbytes, Gbytes
		else if ( strcmp( temp, "System memory (kbytes)" ) == 0 )
		{
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if( !empty )
			{
				cdata.sysMemory = atoi( temp );
			}
		}
		else if ( strcmp( temp, "System memory (Mbytes)" ) == 0 )
		{
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if( !empty )
			{
				tempInt = atoi( temp );
				tempInt *= 1000;
				cdata.sysMemory = tempInt;
			}
		}
		else if ( strcmp( temp, "System memory (Gbytes)" ) == 0 )
		{
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if( !empty )
			{
				tempInt = atoi( temp );
				tempInt *= 1000000;
				cdata.sysMemory = tempInt;
			}
		}
		else if ( strcmp( temp, "Memory block size (kbytes)" ) == 0 )
		{
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if( !empty )
			{
				tempInt = atoi( temp );
				cdata.memoryBlockSize = tempInt;
			}
		}
		else if ( strcmp( temp, "Printer quantity" ) == 0 )
		{
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if( !empty )
			{
				tempInt = atoi( temp );
				cdata.numPrinter = tempInt - 1;
			}
		}
		else if ( strcmp( temp, "Hard drive quantity" ) == 0 )
		{
			fin.getline( temp, STR_LEN, '\n' );
			removeSpace( temp );
			empty = isEmpty( temp );

			if ( !empty )
			{
				tempInt = atoi( temp );
				cdata.numHardDrive = tempInt - 1;
			}
		}
		else //end of file
		{
			if ( strcmp( temp, "End Simulator Configuration File\n") == 0 )
			{
				return true;
			}
		}
	}

	fin.close();
	return true;
}

/**
 * @brief removeSpace
 *
 * @details removes spaces at beginning of the string
 *          
 * @pre string exists
 *
 * @post spaces are removed from beginning
 *
 * @par Algorithm 
 *      Traversing string
 *      
 * @exception None
 *
 * @param [in] string contains char of a line from file
 *
 * @param [out] none
 *
 * @return none
 *
 * @note None
 */
void removeSpace( char string [] )
{
	int index = 0;
	//check if first index is space or newline
	while( string[0] == ' ' || string[0] == '\n' )
	{
		//while it is not the end of the string
		while ( string[ index ] != '\0' )
		{
			//shift every char down one
			string[ index ] = string [ index + 1 ];
			index++;
		}
	}
}

/**
 * @brief isEmpty
 *
 * @details returns if a string is empty
 *          
 * @pre string exists
 *
 * @post if string is empty, return true, otherwise false
 *
 * @par Algorithm 
 *      if, else statement with checking first element
 *      
 * @exception None
 *
 * @param [in] string provides array of char to check if empty
 *
 * @param [out] None
 *
 * @return bool: if string is empty, return false, otherwise true
 *
 * @note None
 */
bool isEmpty( char string[] )
{
	//if first element is null
	if ( string[ 0 ] == '\0')
	{
		//string is empty
		return true;
	}
	//otherwise it is not empty
	return false;
}


/**
 * @brief getMetaData
 *
 * @details reads meta data from file and stores in class
 *          
 * @pre filename exists, mData has been created
 *
 * @post mData is set
 *
 * @par Algorithm 
 *      use ifstream library function to read metadata
 *      
 * @exception None
 *
 * @param [in] filename provides string that contains metadata
 *
 * @param [out] mData provides metaData class vector
 *
 * @return bool, true if successful, false if not
 *
 * @note None
 */
bool getMetaData( char filename[],
				  vector<metaData> &mData )
{
	//declare and initalize variables
	char temp[ STR_LEN_LNG ];
	ifstream fin;

	//open file
	fin.open ( filename );

	//if file does not exist or failed when open
	if ( !fin.good() )
	{
		cout << "Error: Invalid Meta-Data filename. " << endl;
		return false;
	}

	fin.getline( temp, STR_LEN, '\n' );

	//if start of program does not say this, then it is wrong file
	if ( strcmp( temp, "Start Program Meta-Data Code:" ) != 0 ) 
	{
		cout << "Error: Invalid Meta-Data File." << endl;
	}

	while ( fin.good() )
	{
		fin.getline( temp, STR_LEN_LNG, '\n' );

		//end of file 
		if ( strcmp( temp, "End Program Meta-Data Code." ) == 0 )
		{
			return true;
		}
		//otherwise parse line of meta-data
		parseString( temp, mData);
	}

	return true;
}

/**
 * @brief parseString
 *
 * @details parses metaData line by line
 *          
 * @pre string exists and mData exists
 *
 * @post mData is set
 *
 * @par Algorithm 
 *      parse through string
 *      
 * @exception None
 *
 * @param [in] string provides mData to parse
 *
 * @param [out] mData provides metaData
 *
 * @return None
 *
 * @note None
 */
void parseString( char string[], 
				  vector<metaData> &mData )
{
	//initalize/declare variables
	int index = 0;
	metaData temp; 
	int strLen = strlen( string );
	
	while ( index < strLen - 2 )
	{
		//save meta data
		temp = parseData( index, string );

		//store data in vector
		mData.push_back( temp );
	}
}

/**
 * @brief parseData
 *
 * @details parses through the string and saves metaData
 *          
 * @pre string exists
 *
 * @post metadata is saved
 *
 * @par Algorithm 
 *      increment index until find specific symbols
 *      
 * @exception None
 *
 * @param [in] string provides string read from file that is being parsed
 *
 * @param [out] index provides index in string that 
 *
 * @return metaData
 *
 * @note None
 */
metaData parseData( int &index, 
					char string[] )
{
	//declare and initalize variables
	char strTemp[ STR_LEN ];
	int tempIndex = 0;
	metaData metaTemp; 

	while ( string [ index ] != 'S' && 	//uppercase
			string [ index ] != 'A' &&	
			string [ index ] != 'P' &&
			string [ index ] != 'I' &&
			string [ index ] != 'O' &&
			string [ index ] != 'M' &&
			string [ index ] != 'a' &&	//lowercase (ERROR)
			string [ index ] != 'i' &&
			string [ index ] != 'o' &&
			string [ index ] != 'm' &&
			string [ index ] != '(' )	//missing operation (ERROR)
	{
		//increment until find valid operation
		index++;
	}

	//check that '(' is not the code, when reach code, save
	if( string [ index ] != '(' )
	{
		metaTemp.metaCode = string[ index ];
		index++;
	}

	//find meta descriptor
	while ( string [ index ] != '(' )
	{
		//increment until find 
		index++;
	}
	//increment past '('
	index++;

	//increment past spaces
	while( string[ index ] == ' ' )
	{
		index++;
	}

	while ( string[ index ] != ')' )
	{
		//if ( string[ index ] != ' ' )
		{
			//copy data into temp
			strTemp[ tempIndex ] = string [ index ];
			tempIndex++;
		}
		//increment index
		index++;
	}

	//increment past ')'
	index++;

	//put null at end of temp
	strTemp[ tempIndex ] = '\0';

	//copy temp data to temp meta
	strcpy( metaTemp.metaDescriptor, strTemp );
	
	//check for spaces
	while ( string[ index ] == ' ' )
	{
		index++;
	}

	//reset tempIndex
	tempIndex = 0;

	//digits
	while ( string[ index ] == '0' ||
			string[ index ] == '1' ||
			string[ index ] == '2' ||
			string[ index ] == '3' ||
			string[ index ] == '4' ||
			string[ index ] == '5' ||
			string[ index ] == '6' ||
			string[ index ] == '7' ||
			string[ index ] == '8' ||
			string[ index ] == '9' ||
			string[ index ] == '-' ) 	//negative nums (ERROR)
	{
		strTemp[ tempIndex ] = string[ index ];
		tempIndex++;
		index++;
	}

	//skip spaces
	while ( string[ index ] == ' ' )
	{
		index++;
	}

	//put null at end
	strTemp[ tempIndex ] = '\0';

	//store cycles, convert char* to int
	metaTemp.cycles = atoi( strTemp );

	return metaTemp;
}


/**
 * @brief logFileValid
 *
 * @details checks that log file has extension .mdf
 *          
 * @pre string exists
 *
 * @post returns true if extensions is .mdf, otherwise false
 *
 * @par Algorithm 
 *      check through string, checking for 3 consecutive mdf
 *      
 * @exception None
 *
 * @param [in] string provides string for file name
 *
 * @param [out] 
 *
 * @return bool
 *
 * @note None
 */
bool logFileValid( char string[] )
{
	int index = 0;
	while( string[ index ] != '\0')
	{
		if ( string[ index ] == '.' )
		{
			if( string[ index + 1 ] == 'm' &&
				string[ index + 2 ] == 'd' &&
				string[ index + 3 ] == 'f' )

			{
				return true;
			}
		}
		index++;
	}
	return false;
}