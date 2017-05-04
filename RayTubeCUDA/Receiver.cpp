#include"Receiver.h"



Receiver::Receiver(char *filePath)
{
	ifstream rxFile(filePath);
	rxFilePath = filePath;
	char x[20] = {0};
	char y[20] = {0};
	char z[20] = {0};
	if(!rxFile)
	{
		printf("rxFile error\n");
	}
	else
	{
		string line;
		while(getline(rxFile, line))
		{
			if(sscanf(line.c_str(), "%s %s %s", x, y, z) == 3)
			{
				break;
			}
		}
		rxPosition.x = atof(x);
		rxPosition.y = atof(y);
		rxPosition.z = atof(z);
	}
}

Point Receiver::GetPosition()
{
	return rxPosition;
}

void Receiver::SetPosition(Point position)
{
	rxPosition=position;
}

float Receiver::GetHeight()
{
	float transmitterHeight;
	ifstream rxFile(rxFilePath);
	string line;
	while(getline(rxFile,line))
	{
		int position;
		position=line.find("cube_size");
		if (position!=line.npos)
		{
			vector<string> temp=split(line," ");
			vector<string>::iterator it=temp.begin()+1;
			transmitterHeight=atof(it->c_str());		  
			break;
		}		
	}
	rxFile.close();	
	return transmitterHeight;
}



Point GenerateReceiver(Point tempReceiver, float height)
{
	Point receiver = tempReceiver;
	receiver.z += height;
	return receiver;
}