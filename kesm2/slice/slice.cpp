#include <iostream>
#include <fstream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <time.h>

#include <aerotype.h>
#include <build.h>
#include <wapi.h>

using namespace std;

void displaystatus(void);



#define SCREEN_HEIGHT		30
#define DYNAMIC_SLICE_STEP	0.0005
#define FIXED				0
#define RANDOM				1
#define ALTERNATING			2


//cutting parameters
int CuttingMode = RANDOM;
double YSliceSize		=	0.3;		//slice width (.625 / 1.2 optimal)
double ZSliceSize		=	0.002;		//slice thickness (0.0003 optimal)
double AverageCuttingSpeed		=	15;
double Deviation	=	0.5;
double SlicesPerBlock	=	100;
int StairBlockDepth		=	1000;		//number of blocks deep for stair stepping
int StairBlockWidth		=	25;			//number of blocks wide for stair stepping

//camera fire parameters
double FIRE_STEP = 0.0003;
double FIRE_PULSE_WIDTH = 0.1;

double LiftHeight		=	0.1;
double ReturnSpeed		=	21;
double XSliceSize		=	8;

//cutting session variables
double TotalZ			=	0;
double TotalSlices		=	0;

int ThreadNum=0;
bool abort_session=false;


int i=0;
int k=0;
int j=0;


void SendCommand(char *command)
{
	long error=0;
	LPSTR psErr=new char[100];
	//if(WAPICommReady () == 0)
	//printf("\nBuffer full, Wait before sending command");

	while(WAPICommReady() == 0);	//if the buffer is full, do nothing

	error = WAPIAerSend ( command );
	//printf( "COMMAND: \"%s\" RETURN CODE: %li\n", command , error );
	//displaystatus();
	//error = WAPIErrorToAscii (error, psErr);
	//printf("\n %s\n", psErr);
}



void SetFire(double firestep, double pulsewidth)
{
	char firecommand[80];
	char temp[100];
	strcpy(firecommand, "fire");
	sprintf(temp, " X%f", firestep);
	strcat(firecommand, temp);

	strcat(firecommand, " PULSE");
	
	sprintf(temp, " %f", pulsewidth);
	strcat(firecommand, temp);

	strcat(firecommand, " ABS ON");
	cout<<firecommand<<endl;
	//int test;
	//cin>>test;
	SendCommand(firecommand);
}



double getSpeed()
//retrieves a cutting speed based on set parameters
{
	switch(CuttingMode)
	{
	case FIXED:
		return AverageCuttingSpeed;
		break;
	case RANDOM:
		return (((double)rand()/(double)RAND_MAX)*2*Deviation) - Deviation + AverageCuttingSpeed;
		break;
	case ALTERNATING:
		break;
	default:
		break;
	}
	return 0.0;
}

void AbortThread(void *myID)
{
	char keypress;
	while(keypress != '0')
		keypress=(char)_getch();
	abort_session=true;
}

void ClearScreen()
{
	for(int i=0; i<SCREEN_HEIGHT; i++)
		cout<<endl;
}


int Min(int a, int b)
{
	if(a < b)
		return a;
	else
		return b;
}


void Exit(int code)
{
	cout<<"Goodbye"<<endl;
	exit(0);
}

void AbortStage()
{
	WAPIAerAbort();
}



void ShutdownStage()
{
	WAPIAerClose(0);
}

bool IncrementStage(double x, double y, double z, double f)
{

	char keypress;
	if(abort_session)
	{
		cout<<"Cutting Paused..."<<endl;
		cout<<"(0) Abort immediately, any other key to continue"<<endl;
		keypress=(char)_getch();
		if(keypress=='0')
		{
			AbortStage();
			return false;
		}
		else
		{
			cout<<"Resuming..."<<endl;
			ThreadNum++;
			_beginthread(AbortThread, 0, &ThreadNum);
			abort_session=false;

		}
	}

	char movecommand[80];
	char temp[100];
	strcpy(movecommand, "LINEAR");

	if(x != 0)
	{
		sprintf(temp, " X%f", x);
		strcat(movecommand, temp);
	}
	if(y != 0)
	{
		sprintf(temp, " Y%f", y);
		strcat(movecommand, temp);
	}
	if(z != 0)
	{
		sprintf(temp, " Z%f", z);
		strcat(movecommand, temp);
	}
	if(f != 0)
	{
		sprintf(temp, " F%f", f);
		strcat(movecommand, temp);
	}

	if( (x == 0) && (y == 0) && (z == 0))
		return false;
	else
		SendCommand(movecommand);

	return true;
}

void ChangeCuttingOptionsMenu(char menu)
{
	switch(menu)
	{
	case '1':
		cout<<"Enter new Z Slice Size ("<<ZSliceSize<<"mm): ";
		cin>>ZSliceSize;
		cout<<endl;
		break;
	case '2':
		cout<<"Enter new Y Slice Size ("<<YSliceSize<<"mm): ";
		cin>>YSliceSize;
		cout<<endl;
		break;
	case '3':
		cout<<"Enter new Cutting Speed ("<<AverageCuttingSpeed<<"mm/s): ";
		cin>>AverageCuttingSpeed;
		cout<<endl;
		break;
	case '4':
		cout<<"Enter new Slices Per Block ("<<SlicesPerBlock<<"): ";
		cin>>SlicesPerBlock;
		cout<<endl;
		break;
	case '5':
		cout<<"Enter new Stair Block Depth ("<<StairBlockDepth<<"): ";
		cin>>StairBlockDepth;
		cout<<endl;
		break;
	case '6':
		cout<<"Enter new Stair Block Width ("<<StairBlockDepth<<"): ";
		cin>>StairBlockWidth;
		cout<<endl;
		break;
	case '7':
		cout<<"Enter new X Slice Size ("<<XSliceSize<<"mm): ";
		cin>>XSliceSize;
		cout<<endl;
		break;
	case '8':
		cout<<"Enter new Lift Height ("<<LiftHeight<<"mm): ";
		cin>>LiftHeight;
		cout<<endl;
		break;
	case '9':
		cout<<"Enter new Return Speed ("<<ReturnSpeed<<"mm/s): ";
		cin>>ReturnSpeed;
		cout<<endl;
		break;
	default:
		break;
	}
}

void CuttingOptionsMenu(char menu)
{
	char menuoption;
	ofstream outfile;
	ifstream infile;
	switch(menu)
	{
	case '1':
		while(menuoption != '0')
		{
			ClearScreen();
			cout<<"(1) Z Slice Size     = "<<ZSliceSize<<endl;
			cout<<"(2) Y Slice Size     = "<<YSliceSize<<endl;
			cout<<"(3) Cutting Speed    = "<<AverageCuttingSpeed<<endl;
			cout<<"(4) Slices Per Block = "<<SlicesPerBlock<<endl;
			cout<<"(5) Stair Block Depth= "<<StairBlockDepth<<endl;
			cout<<"(6) Stair Block Width= "<<StairBlockWidth<<endl;
			cout<<"-------------------------------"<<endl;
			cout<<"(7) X Slice Size     = "<<XSliceSize<<endl;
			cout<<"(8) Lift Height      = "<<LiftHeight<<endl;
			cout<<"(9) Return Speed     = "<<ReturnSpeed<<endl;
			cout<<"(0) Exit"<<endl;
			menuoption=_getch();
			ChangeCuttingOptionsMenu(menuoption);
		}
		break;


	case '3':		//display cutting parameters
		ClearScreen();
		cout<<"Z Slice Size:    "<<ZSliceSize<<"mm"<<endl;
		cout<<"Y Slice Size:    "<<YSliceSize<<"mm"<<endl;
		cout<<"Cutting Speed:   "<<AverageCuttingSpeed<<"mm/s"<<endl;
		cout<<"Slices per Block:"<<SlicesPerBlock<<endl;
		cout<<"Block Thickness: "<<ZSliceSize*SlicesPerBlock<<"mm"<<endl;
		cout<<"Stair BlockDepth:"<<StairBlockDepth<<endl;
		cout<<"Stair BlockWidth:"<<StairBlockWidth<<endl;
		cout<<"-------------------------------"<<endl;
		cout<<"X Slice Size:    "<<XSliceSize<<endl;
		cout<<"Lift Height:     "<<LiftHeight<<endl;
		cout<<"Return Speed:    "<<ReturnSpeed<<endl;
		_getch();
		break;
	case '2':		//load cutting parameters
		infile.open("test.txt");
		infile>>YSliceSize;
		infile>>ZSliceSize;
		infile>>AverageCuttingSpeed;
		infile>>SlicesPerBlock;
		infile>>StairBlockDepth;
		infile>>StairBlockWidth;
		infile>>XSliceSize;
		infile>>LiftHeight;
		infile>>ReturnSpeed;

		CuttingOptionsMenu('3');
		break;
	case '4':		//save cutting parameters
		outfile.open("test.txt");
		outfile<<YSliceSize<<endl;
		outfile<<ZSliceSize<<endl;
		outfile<<AverageCuttingSpeed<<endl;
		outfile<<SlicesPerBlock<<endl;
		outfile<<StairBlockDepth<<endl;
		outfile<<StairBlockWidth<<endl;
		outfile<<XSliceSize<<endl;
		outfile<<LiftHeight<<endl;
		outfile<<ReturnSpeed<<endl;

		CuttingOptionsMenu('3');
		break;
	case '0':
		break;
	default:
		break;
	}
}

bool CutSlice(double cuttingspeed)
{
	//stage motion code for one complete slice
	//microtome startes at top of block
	if(!IncrementStage(0,0,ZSliceSize, 1))
		return false;//translate z down ZSliceSize

	int starttime = time(NULL);				//allot time for z-axis to settle
	while(time(NULL) <= starttime + 2);

	//turn camera firing on
	//SendCommand("fire ON");
	SetFire(FIRE_STEP, FIRE_PULSE_WIDTH);

	
	if(!IncrementStage(XSliceSize, 0,0,cuttingspeed))
		return false;//translate x forward XSliceSize at CuttingSpeed

	//turn camera firing off
	SendCommand("fire OFF");


	if(!IncrementStage(0,0,-LiftHeight,1))
		return false;//translate z up LiftHeight
	if(!IncrementStage(-XSliceSize, 0,0,ReturnSpeed))
		return false;//translate x backward XSliceSize at ReturnSpeed
	if(!IncrementStage(0,0,LiftHeight,1))
		return false;//translate z down LiftHeight

	return true;
}

bool CutBlock()
{

	//stage motion code for one complete block
	//microtome starts at top of block
	for(int i=0; i<SlicesPerBlock; i++)			//cut a sequence of slices
	{
		double CurrentSpeed = getSpeed();		//retrieve a valid cutting speed
		if(!CutSlice(CurrentSpeed))
		{
			cout<<"Terminating block"<<endl;
			return false;
		}
	}
	return true;
}

void SliceSession()
{
	char keypress;
	cout<<"Beginning Slice Cutting Session"<<endl;

	//set session variables
	TotalZ=0.0;
	TotalSlices=0;
	//begin cutting
	cout<<"Position Microtome..."<<endl;
	cout<<"(0) to quit, (c) to change parameters, any other key to cut"<<endl;
	while(keypress != '0')
	{
		keypress = _getch();
		if(keypress == 'c')
		{
			CuttingOptionsMenu('1');
			ClearScreen();
			cout<<"Position Microtome..."<<endl;
			cout<<"(0) to quit, (c) to change parameters, any other key to cut"<<endl;
		}
		else if(keypress == '+')
		{
			ZSliceSize+= DYNAMIC_SLICE_STEP;
			if(ZSliceSize > 0.01)
			{
				cout<<"Error, upper thickness of knife is 10 microns"<<endl;
				ZSliceSize=0.01;
			}
			else
			cout<<"Z Slice Size increased 1/2 micron: "<<ZSliceSize<<endl;
		}
		else if(keypress == '-')
		{
			ZSliceSize-= DYNAMIC_SLICE_STEP;
			if(ZSliceSize < DYNAMIC_SLICE_STEP)
			{
				cout<<"Error, Z step can't go to zero.  We'd like to get this done today, ya know..."<<endl;
				ZSliceSize=DYNAMIC_SLICE_STEP;
			}
			else
				cout<<"Z Slice Size decreased 1/2 micron: "<<ZSliceSize<<endl;
		}
		else if(keypress != '0')
		{
			double CurrentSpeed = getSpeed();		//get a valid cutting speed
			cout<<"Cutting Slice at "<<CurrentSpeed<<" mm/sec..."<<endl;

			CutSlice(CurrentSpeed);
			TotalZ+=ZSliceSize;
			TotalSlices++;
		}
			
	}//end cutting loop
	cout<<"Total Depth for this session: "<<TotalZ<<"mm"<<endl;
	keypress=_getch();


}

void BlockSession()
{
	char keypress;
	cout<<"Position Microtome..."<<endl;
	cout<<"(0) to quit, any other key to begin block"<<endl;
	keypress=(char)_getch();
	TotalZ=0.0;
	TotalSlices=0;
	if(keypress != '0')
	{
		cout<<"Cutting Block"<<endl;
		CutBlock();
		TotalZ+=ZSliceSize*SlicesPerBlock;
		TotalSlices+=SlicesPerBlock;
		cout<<"Total Cutting Depth: "<<TotalZ<<"mm"<<endl;
		keypress =(char)_getch();
	}
}

void TrimCuttingSession()
{
//	char keypress;

	double XTrimStart		=	0.0;
	double YTrimStart		=	0.0;
	double ZTrimStart		=	0.0;
	double XPosition		=	0.0;
	double YPosition		=	0.0;
	double ZPosition		=	0.0;

	int TrimBlockDepth		=	StairBlockDepth;		//probably change this later
	int TrimBlockWidth		=	StairBlockWidth;		//and this

	cout<<"Beginning Trim Cutting..."<<endl;

	ThreadNum++;
	_beginthread(&AbortThread, 0, &ThreadNum);

	for(int i=0; i<TrimBlockDepth; i++)					//for each layer
	{
		IncrementStage(0, -YPosition, 0, 5);			//move the stage back to beginning position
		YPosition = 0.0;
		for(int j=0; j<TrimBlockWidth; j++)				//for each block
		{
			if(!CutBlock())
			{
				cout<<"Terminating Stair Session..."<<endl;
				return;
			}
			ZPosition+=SlicesPerBlock*ZSliceSize;
			IncrementStage(0, 0, -ZPosition, 1);		//push the stage back down
			ZPosition=0.0;
			IncrementStage(0, YSliceSize, 0, 5);		//move the stage over for next block
			YPosition+=YSliceSize;
		}
		IncrementStage(0, -YPosition, 0, 5);		//move back to beginning of layer
		YPosition=0.0;
		IncrementStage(0, 0, SlicesPerBlock*ZSliceSize, 1);	//move the stage back up for the next layer
	}
}


void StairSteppingSession()
{
//	char keypress;
	//cout<<"Enter Desired Block Depth: "<<endl;
	cout<<"test"<<endl;
	double XStairStart		=	0.0;
	double YStairStart		=	0.0;
	double ZStairStart		=	0.0;
	double XPosition		=	0.0;
	double YPosition		=	0.0;
	double ZPosition		=	0.0;

	int blocknumber=0;
	ThreadNum++;
	//_beginthread(&AbortThread, 0, &ThreadNum);

	while(i < StairBlockDepth)
	{
		
		IncrementStage(0,0,-ZPosition, 1);		//move z -ZPosition
		IncrementStage(0,-YPosition, 0, 2);		//move y -YPosition
		ZPosition=YPosition=0.0;
		IncrementStage(0,0, ZSliceSize*SlicesPerBlock*i,1);//move down ZSliceSize*SlicesPerBlock*i
		ZPosition+=ZSliceSize*SlicesPerBlock*i;
		k=Min(i, StairBlockWidth);
		if(!CutBlock())
		{
			cout<<"Terminating Stair Session"<<endl;
			return;
		}
		
		blocknumber++;
		cout<<"Pass:     "<<i<<"Block:     "<<blocknumber<<endl;
		ZPosition+=ZSliceSize*SlicesPerBlock;
		for(j=0; j<k; j++)
		{
			IncrementStage(0, 0,-ZSliceSize*SlicesPerBlock*2,1);//move up ZSliceSize*SlicesPerBlock*2
			ZPosition-=ZSliceSize*SlicesPerBlock*2;
			IncrementStage(0,YSliceSize, 0, 2);//move over YSliceSize
			YPosition+=YSliceSize;
			if(!CutBlock())
			{
				cout<<"Terminating stair session"<<endl;
				return;
			}
			ZPosition+=ZSliceSize*SlicesPerBlock;
			blocknumber++;
			cout<<"Pass:"<<i<<"     Block:"<<blocknumber<<endl;
		}
		i++;
	}

}

void MainMenu(char menu)
{
	char menuoption;
	switch(menu)
	{
	case '1':											//cutting parameters menu
		while(menuoption !='0')
		{
			ClearScreen();
			cout<<"(1) Enter new cutting parameters"<<endl;
			cout<<"(2) Load cutting parameters"<<endl;
			cout<<"(3) Display cutting parameters"<<endl;
			cout<<"(4) Save cutting parameters"<<endl;
			cout<<"(0) Main Menu"<<endl;
			menuoption=(char)_getch();
			CuttingOptionsMenu(menuoption);
		}
		break;
	case '2':
		SliceSession();
		break;
	case '3':
		BlockSession();
		break;
	case '4':
		StairSteppingSession();
		break;
	case '5':
		TrimCuttingSession();
		break;
	case '0':
		break;
	default:
		cout<<"Error, re-enter"<<endl;
	}
}

void InitializeStage()
{
	long error ;
	LPSTR psErr=new char[100];

	error = WAPIAerOpen (0); // open board 1
	if( error != 0 )
	{
		printf("Could not open board" );
		exit(0);
	}
	
	if( WAPIAerCheckInitz ( 0 ) )        // check if board has already been initialized
	{
		error = WAPIAerInitialize ( NULL, "c:\\u500\\mmi\\u500c.jwp", "c:\\u500\\mmi\\KESM\\bts.prm");
	}
	else
	{
		error = WAPIAerSoftwareInitialize ( "c:\\u500\\mmi\\KESM\\bts.prm" ) ; // resets system software without hardware reset
		//error = WAPIAerInitialize ( NULL, "c:\\u500\\mmi\\u500c.jwp", "c:\\u500\\mmi\\bts.prm");
	}
  
	if( error ) 
	{
		printf("Initialization error %i\n", error ) ;
		error = WAPIErrorToAscii (error, psErr);
		printf("\n %s\n", psErr);
		exit(0);
	}
}


		
void main()
{
	long error=0;
	char menuoption;
	InitializeStage();

	SendCommand("enable x y z");
	SendCommand("pr me un/se");
	//SetFire(0.0006, 0.1);
	//SetFire(0.00061036, 0.1);								//10x objective
	//SetFire(0.0003, 0.1);
	//SetFire(0.00015258789, 0.1);							//40x objective
	//SetFire(0.0003, 0.1);							//40x objective
	//SendCommand("fire X0.00015258789 PULSE 0.1 ABS");		//40x objective




	while(menuoption != '0')
	{
		ClearScreen();
		cout<<"(1) Cutting parameters"<<endl;
		cout<<"(2) Cut by slice"<<endl;
		cout<<"(3) Cut by block"<<endl;
		cout<<"(4) Stair stepping"<<endl;
		cout<<"(5) Trim Cutting"<<endl;
		cout<<"(0) Exit"<<endl;
		menuoption=(char)_getch();
		MainMenu(menuoption);
	}
	Exit(0);

	ShutdownStage();

}

void displaystatus(void)
{
	long error ;
	short i ;
	error = WAPIAerCheckStatus ();
	if( !error ) 
	{
		printf("STATUS: ");
		for( i = 0 ; i < 6 ; i++ ) 
		{
			printf("%8lX,", WAPIAerReadStatus (i) );
		}
		printf("\n");
	}    
}
