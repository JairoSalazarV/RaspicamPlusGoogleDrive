#include <iostream>
#include <fstream>
#include <string>

#define		_OK			1
#define		_ERROR		0
#define		_FAIL		2


using namespace std;

typedef struct structSettings
{
	string seconds;
	string localFolder;
	string remoteFolder;
}structSettings;

int readFileContain( const string &fileName, string* contain );
int readSettings( structSettings* settings );


int main()
{
	structSettings settings;
	if( readSettings( &settings ) != _OK )
	{
		cout << "[ERROR] Reading Settings" << endl;
		return _ERROR;
	}
	
	cout << "Seconds " << settings.seconds << endl;
	cout << "localFolder " << settings.localFolder << endl;
	cout << "remoteFolder " << settings.remoteFolder << endl;
	
}

int readSettings( structSettings* settings )
{
	string fileName;
	
	//Reading Seconds
	fileName = "seconds.rasprive";
	if( readFileContain( fileName, &settings->seconds ) != _OK )
	{
		cout << "[ERROR] Reading " << fileName << endl;
		return _ERROR;
	}
	
	//Reading Local Folder
	fileName = "localFolder.rasprive";
	if( readFileContain( fileName, &settings->localFolder ) != _OK )
	{
		cout << "[ERROR] Reading " << fileName << endl;
		return _ERROR;
	}
	
	//Reading Remote Folder
	fileName = "remoteFolder.rasprive";
	if( readFileContain( fileName, &settings->remoteFolder ) != _OK )
	{
		cout << "[ERROR] Reading " << fileName << endl;
		return _ERROR;
	}
	
	return _OK;
	
}



int readFileContain( const string &fileName, string* contain )
{ 
	ifstream myfile ( fileName.c_str() );
	if( myfile.is_open() )
	{
		if( !getline( myfile, *contain ) )
		{
			return _FAIL;
		}
		myfile.close();
	}
	else
	{
		return _ERROR;
	}
	return _OK;
}
