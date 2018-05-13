#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <ctime>

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
int getdir( string dir, vector<string> &files );
double stringToDouble( string str );
int executeConsoleCommand( string command, string* result );
const vector<string> explode(const string& s, const char& c);
int validateDropboxAnswer( const string& dropboxAnswer );
void uploadFile( const string& fileName, const structSettings& settings, bool deleteLocal=true );
void uploadDirectory( const structSettings& settings );
void takeSnapshot( const structSettings& settings );
string getTimeStampFilename( const structSettings& settings );
string getNextTime( const structSettings& settings );


int main()
{
	//--------------------------------------
	//Get Settings
	//--------------------------------------
	structSettings settings;
	if( readSettings( &settings ) != _OK )
	{
		cout << "[ERROR] Reading Settings" << endl;
		return _ERROR;
	}
	
	//--------------------------------------
	//Prepare Variables
	//--------------------------------------
	cout << "Hello Rasprive!\n\n" << endl;
	
	cout << "Seconds " << settings.seconds << endl;
	cout << "localFolder " << settings.localFolder << endl;
	cout << "remoteFolder " << settings.remoteFolder << endl;
	
	int seconds = (int)stringToDouble( settings.seconds );
	string nextTime;
	
	while(true)
	{
		//Take Snapshot
		takeSnapshot( settings );
		
		//Upload Folder
		uploadDirectory( settings );
		
		//Wait to the next time
		nextTime.clear();
		nextTime = getNextTime( settings );
		cout << "Next snapshot at " << nextTime << endl;
		sleep( seconds );
	}	
}

void takeSnapshot( const structSettings& settings )
{
	//raspistill -o ./tmpSnapshots/tmpImg.RGB888 -n -q 100 -gc -ifx colourbalance -ifx denoise -t 3000 -w 3240 -h 2464 -awb auto -ex auto
	string commandResult;
	string outputFile = getTimeStampFilename( settings );		
	string snapshotCommand;
	snapshotCommand.clear();
	snapshotCommand.append("raspistill -o ");
	snapshotCommand.append(outputFile);
	snapshotCommand.append(" -n -q 100 -gc -ifx colourbalance -ifx denoise -t 3000 -w 3240 -h 2464 -awb auto -ex auto");	
	executeConsoleCommand( snapshotCommand, &commandResult );
	//cout << "snapshotCommand: " << snapshotCommand << endl;
}

string getNextTime( const structSettings& settings )
{
	//Timestamp
	time_t now = time(0) + (int)stringToDouble(settings.seconds);
	tm *ltm = localtime(&now);
	//Filename based on time stamp
	string tmpName;
	tmpName.clear();
	if( ltm->tm_mday < 10 )tmpName.append("0");
	tmpName.append( std::to_string(ltm->tm_mday) );	
	tmpName.append( "_" );
	if( ltm->tm_mon < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (1+ltm->tm_mon) ) );
	tmpName.append( "_" );
	tmpName.append( std::to_string( (1900+ltm->tm_year) ) );
	tmpName.append( "_" );
	if( ltm->tm_hour < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (ltm->tm_hour) ) );
	tmpName.append( ":" );
	if( ltm->tm_min < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (ltm->tm_min) ) );
	tmpName.append( ":" );
	if( ltm->tm_sec < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (ltm->tm_sec) ) );
	//Finish
	return tmpName;
}

string getTimeStampFilename( const structSettings& settings )
{
	//Timestamp
	time_t now = time(0);
	tm *ltm = localtime(&now);
	//Filename based on time stamp
	string tmpName;
	tmpName.append( settings.localFolder );
	if( ltm->tm_mday < 10 )tmpName.append("0");
	tmpName.append( std::to_string(ltm->tm_mday) );	
	tmpName.append( "_" );
	if( ltm->tm_mon < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (1+ltm->tm_mon) ) );
	tmpName.append( "_" );
	tmpName.append( std::to_string( (1900+ltm->tm_year) ) );
	tmpName.append( "_" );
	if( ltm->tm_hour < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (ltm->tm_hour) ) );
	tmpName.append( ":" );
	if( ltm->tm_min < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (ltm->tm_min) ) );
	tmpName.append( ":" );
	if( ltm->tm_sec < 10 )tmpName.append("0");
	tmpName.append( std::to_string( (ltm->tm_sec) ) );
	tmpName.append( ".png" );
	//tmpName.append( ".RGB888" );
	//Finish
	return tmpName;
}

void uploadDirectory( const structSettings& settings )
{
	string dir = string( settings.localFolder );
    vector<string> files = vector<string>();
    files.clear();
	getdir(dir,files);
	for( unsigned int i=0; i<files.size(); i++ )
	{
		uploadFile( files[i], settings );        
	}
}

void uploadFile( const string& fileName, const structSettings& settings, bool deleteLocal )
{
	string tmpCommand;
	string commandResult;
	tmpCommand.clear();
	commandResult.clear();
	tmpCommand.append("/home/pi/Documents/Rasprive/Dropbox-Uploader/dropbox_uploader.sh upload ");
	tmpCommand.append(settings.localFolder);
	tmpCommand.append(fileName);
	tmpCommand.append(" ");
	tmpCommand.append(settings.remoteFolder);
	tmpCommand.append(fileName);
	cout << "tmpCommand: " << tmpCommand << endl;
	executeConsoleCommand( tmpCommand, &commandResult );
	if( validateDropboxAnswer( commandResult ) == _OK )
	{		
		if( deleteLocal == true )
		{
			cout << "[UPDATED] Deleting..." << fileName << endl;
			commandResult.clear();
			tmpCommand.clear();
			tmpCommand.append("rm -f ");
			tmpCommand.append(settings.localFolder);
			tmpCommand.append(fileName);
			executeConsoleCommand( tmpCommand, &commandResult );
		}
		else
		{
			cout << "[UPDATED] A copy of \'" << fileName << "\' will be preserved locally" << endl;
		}
	}
	else
	{
		cout << fileName << " not updated, keeped till next time" << endl;
	}
	
}

int validateDropboxAnswer( const string& dropboxAnswer )
{
	vector<string> resSlides = explode(dropboxAnswer,' ');	
	if( 
		(
			resSlides[1] == "Skipping" 	&&
			resSlides[2] == "file" 		&&
			resSlides[5] == "exists" 	&&
			resSlides[8] == "same" 		&&
			resSlides[9] == "hash\n" 
		) || 
		(
			resSlides[1] == "Uploading" &&
			resSlides[5] == "DONE\n"
		)
	){	
		return _OK;
	}
	return _FAIL;
}



const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

int executeConsoleCommand( string command, string* result )
{
	//Execute Console Command
	int frameBodyLen = 1024;
	char bufferComm[frameBodyLen];
	result->clear();
	FILE* pipe = popen(command.c_str(), "r");
	try
	{
		while(!feof(pipe))
		{
			if(fgets(bufferComm, frameBodyLen, pipe) != NULL)
			{
				result->append( bufferComm );
			}
		}
	} 
	catch(...)
	{
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return _OK;
}

double stringToDouble( string str )
{
	double numb;
	istringstream( str ) >> numb;
	return numb;
}

int getdir( string dir, vector<string> &files )
{
    DIR *dp;
    struct dirent *dirp;
    if( (dp  = opendir(dir.c_str())) == NULL )
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return _ERROR;
    }


	string strDefa1(".");
	string strDefa2("..");
    while( (dirp = readdir(dp)) != NULL )
    {
		if( 
				dirp->d_name != strDefa1	&&
				dirp->d_name != strDefa2
		){		
			files.push_back(string(dirp->d_name));
		}
    }
    closedir(dp);
    return _OK;
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
