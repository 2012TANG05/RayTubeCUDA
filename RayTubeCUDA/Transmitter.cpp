#include "Transmitter.h"




Transmitter::Transmitter(char* filePath)
{
	ifstream txFile(filePath);
	txFilePath=filePath;
	char x[20]={0};
	char y[20]={0};
	char z[20]={0};
	if(!txFile)
	{
		printf("txfile error");
	}
	else
	{
		string line;
		while(getline(txFile,line))
		{
			if(3==sscanf(line.c_str(),"%s %s %s",x,y,z))
			{
				break;
			}
		}
		//transmitterPosition=CPoint(atof(x),atof(y),atof(z));
		txPosition.x=atof(x);
		txPosition.y=atof(y);
		txPosition.z=atof(z);
	}
	txFile.close();	
}

	Point Transmitter::GetPosition()
	{
		return txPosition;
	}
	void Transmitter::SetPosition(Point position)
	{
		txPosition=position;
	}
	float Transmitter::GetHeight()
	{
		float transmitterHeight;
		ifstream txFile(txFilePath);
		string line;
		while(getline(txFile,line))
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
		txFile.close();	
		return transmitterHeight;
	}
	


void GenerateOriginalRays(Point txPosition,TriangleRayTube *originalRayTubes,const int tessellationFrequency,float height)
{

	Point originalVertexes[12];
	TriangleRayTube *p_originalRayTubes=originalRayTubes;
	Point originalPoint;
	originalPoint.x=txPosition.x;
	originalPoint.y=txPosition.y;
	originalPoint.z=txPosition.z+height;
	CalculateAllVertexesOfIcosahedron(originalPoint,originalVertexes);

	//计算顶部五个面
	for(int i=1;i<6;i++)
	{
		if(i<5)
		{
			GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[0],originalVertexes[i],originalVertexes[i+1],originalPoint,tessellationFrequency);
		}
		else
		{
			GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[0],originalVertexes[5],originalVertexes[2],originalPoint,tessellationFrequency);
		}
	}

	//计算中部十个面

	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[1],originalVertexes[10],originalVertexes[6],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[2],originalVertexes[6],originalVertexes[7],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[3],originalVertexes[7],originalVertexes[8],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[4],originalVertexes[8],originalVertexes[9],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[5],originalVertexes[9],originalVertexes[10],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[6],originalVertexes[1],originalVertexes[2],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[7],originalVertexes[2],originalVertexes[3],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[8],originalVertexes[3],originalVertexes[4],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[9],originalVertexes[4],originalVertexes[5],originalPoint,tessellationFrequency);
	GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[10],originalVertexes[5],originalVertexes[1],originalPoint,tessellationFrequency);

	//for(int i=1;i<6;i++)
	//{

	//	if(i<5)
	//	{
	//		GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[i],originalVertexes[i+5],originalVertexes[i+1],originalPoint,tessellationFrequency);
	//		GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[i+5],originalVertexes[i+1],originalVertexes[i+6],originalPoint,tessellationFrequency);
	//	}
	//	else
	//	{
	//		GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[10],originalVertexes[5],originalVertexes[1],originalPoint,tessellationFrequency);
	//		GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[6],originalVertexes[1],originalVertexes[2],originalPoint,tessellationFrequency);
	//	}
	//}

	//计算底部五个面
	for(int i=6;i<11;i++)
	{
		if(i<10)
		{
			GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[11],originalVertexes[i],originalVertexes[i+1],originalPoint,tessellationFrequency);
		}
		else
		{
			GetAllRayTubesOnSingleFace(&p_originalRayTubes,originalVertexes[11],originalVertexes[10],originalVertexes[7],originalPoint,tessellationFrequency);
		}
	}
}

//根据三个顶点得到三角形射线管
TriangleRayTube GetTriangleRayTube(Point vertex1,Point vertex2,Point vertex3,Point originalPoint)
{
	TriangleRayTube rayTube;
	rayTube.ray1.originalPoint=originalPoint;
	rayTube.ray2.originalPoint=originalPoint;
	rayTube.ray3.originalPoint=originalPoint;
	UitizeVector(&vertex1.x,&vertex1.y,&vertex1.z);
	UitizeVector(&vertex2.x,&vertex2.y,&vertex2.z);
	UitizeVector(&vertex3.x,&vertex3.y,&vertex3.z);
	rayTube.ray1.direction=vertex1;
	rayTube.ray2.direction=vertex2;
	rayTube.ray3.direction=vertex3;
	rayTube.path.nodeLevel=0;
	return rayTube;
}

//计算正二十面体十二个顶点的坐标
void CalculateAllVertexesOfIcosahedron(Point centerPoint,Point *originalVertexes)
{
	float theta=AngleToRad(63.4649488229220);
	float phi=AngleToRad(72);
	float x=0;
	float y=0;
	float z=0;

	originalVertexes[0].x=x;
	originalVertexes[0].y=y;
	originalVertexes[0].z=z+1;
	originalVertexes[11].x=x;
	originalVertexes[11].y=y;
	originalVertexes[11].z=z-1;
	for(int i=1;i<6;i++)
	{
		originalVertexes[i].x=x+sin(theta)*cos(phi*(i-1));
		originalVertexes[i].y=y+sin(theta)*sin(phi*(i-1));
		originalVertexes[i].z=z+cos(theta);
		//cout<<originalVertexes[i].z<<endl;
	}
	for(int i=6;i<11;i++)
	{
		originalVertexes[i].x=x+sin(theta)*cos(phi*(i-6));
		originalVertexes[i].y=y+sin(theta)*sin(phi*(i-6));
		originalVertexes[i].z=z-cos(theta);
		//cout<<originalVertexes[i].z<<endl;
	}
}

//得到每条棱上的顶点
void GetVertexesOnEdge(Point vertex1,Point vertex2,int tessellationFrequency,Point *vertexes)
{
	float x=(vertex2.x-vertex1.x)/tessellationFrequency;
	float y=(vertex2.y-vertex1.y)/tessellationFrequency;
	float z=(vertex2.z-vertex1.z)/tessellationFrequency;
	for(int n=0;n<=tessellationFrequency;n++)
	{
		int i=(n*n+n)/2;
		vertexes[i].x=vertex1.x+x*n;
		vertexes[i].y=vertex1.y+y*n;
		vertexes[i].z=vertex1.z+z*n;
	}
}

//得到条线上的顶点
void GetVertexesOnEachLine(Point startVertex,Point direction,int tessellationFrequency,int floor,Point *vertexes)
{
	int i=(floor*floor+floor)/2+1; 
	int num=1;
	for(;i<(floor*floor+3*floor)/2+1;i++)
	{		
		vertexes[i].x=startVertex.x+num*direction.x;
		vertexes[i].y=startVertex.y+num*direction.y;
		vertexes[i].z=startVertex.z+num*direction.z;
		num++;
	}
}

//得到正二十面体某个面的所有顶点
void GetAllVertexesOnSigleFace(Point vertex1,Point vertex2,Point vertex3,Point txPosition,int tessellationFrequency,Point *vertexes)
{
	Point direction;
	direction.x=(vertex3.x-vertex2.x)/tessellationFrequency;
	direction.y=(vertex3.y-vertex2.y)/tessellationFrequency;
	direction.z=(vertex3.z-vertex2.z)/tessellationFrequency;
	GetVertexesOnEdge(vertex1,vertex2,tessellationFrequency,vertexes);
	for(int n=1;n<=tessellationFrequency;n++)
	{
		GetVertexesOnEachLine(vertexes[(n*n+n)/2],direction,tessellationFrequency,n,vertexes);
	}
}

//得到正二十面体某个面的所有射线管
void GetAllRayTubesOnSingleFace(TriangleRayTube **sp_rayTubes,Point vertex1,Point vertex2,Point vertex3,Point originalPoint,const int tessellationFrequency)
{
	Point *vertexes=new Point[((tessellationFrequency+1)*(tessellationFrequency+1)+tessellationFrequency+1)/2];
	//Point *vertexes=new Point[561];
	TriangleRayTube *p_rayTubes=*sp_rayTubes;
	GetAllVertexesOnSigleFace(vertex1,vertex2,vertex3,originalPoint,tessellationFrequency,vertexes);

	for(int n=1;n<=tessellationFrequency;n++)
	{
		GetRayTubesOnEachFloor(p_rayTubes,vertexes,n,originalPoint);		
	}
	*sp_rayTubes+=tessellationFrequency*tessellationFrequency;
}

//得到正二十面体某个面某层的所有射线管
void GetRayTubesOnEachFloor(TriangleRayTube *rayTubes,Point *vertexes,int floor,Point originalPoint)
{
	int i=(floor*floor-floor)/2;
	int j=(floor*floor+floor)/2+1;
	int num=(floor-1)*(floor-1);
	for(;i<(floor*floor+floor)/2;i++,j++)
	{
		rayTubes[num]=GetTriangleRayTube(vertexes[i],vertexes[i+floor],vertexes[i+floor+1],originalPoint);
		num++;
		if(j<(floor*floor+3*floor)/2)
		{
			rayTubes[num]=GetTriangleRayTube(vertexes[j],vertexes[j-floor],vertexes[j-floor-1],originalPoint);
			num++;
		}
	}
}