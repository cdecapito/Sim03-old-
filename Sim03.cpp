// Program Information /////////////////////////////////////////////////////////
/**
  * @file Sim02.cpp
  *
  * @brief main driver     
  * 
  * @details calls implementation
  *
  * @version 1.01 Carli DeCapito
  * 		 Additional Processing and Threading ( 10/7/17)
  *
  *			 1.00 Carli DeCapito
  * 		 Original (9/12/17)
  *
  * @note None
  */

//global constants
static const int NEW = 0;
static const int READY = 1;
static const int RUNNING = 2;
static const int WAITING = 3;
static const int TERMINATED = 4;

//header files
#include <iostream>
#include <vector>
#include "data.h"
#include "process.h"
#include "readData.cpp"
#include "simFuncs.cpp"

using namespace std;

//main driver functions
bool configFileValid( char string[] );

//main driver implmentation
int main( int argc, char* argv[] )
{
	//declare and initalize variables
	configData cdata;
	vector<metaData> mdata;
	vector<process> pdata;
	
	//check for command line arguments
	if( argc != 2 )
	{
		cout << "Error: Invalid Number of Command Line Arguements." << endl;
        return 0;
	}

	//check that command line argument file ends in .conf
	if( !configFileValid( argv[ 1 ] ))
	{
		cout << "Error. Invalid Configuration Data File Extension." << endl;
		return 0;
	} 

	//get config data
	if ( !getConfigData( argv[1], cdata ) )
	{
		return 0;
	}
	
	//get metadata
	if( !getMetaData( cdata.filePath, mdata ) )
	{
		return 0;
	}

	
	//split data into processes
	splitMetaData( pdata, mdata );

	//output data to file and/or monitor
	logData( cdata, pdata );
	return 0;
}


/**
 * @brief 
 *
 * @details 
 *          
 * @pre 
 *
 * @post 
 *
 * @par Algorithm 
 *      
 *      
 * @exception None
 *
 * @param [in] 
 *
 * @param [out] 
 *
 * @return None
 *
 * @note None
 */
bool configFileValid( char string[] )
{
	int index = 0;
	while( string[ index ] != '\0')
	{
		if ( string[ index ] == '.' )
		{
			if( string[ index + 1 ] == 'c' &&
				string[ index + 2 ] == 'o' &&
				string[ index + 3 ] == 'n' &&
				string[ index + 4 ] == 'f' )

			{
				return true;
			}
		}
		index++;
	}
	return false;
}
