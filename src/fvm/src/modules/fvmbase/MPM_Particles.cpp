#include "MPM_Particles.h"

#include "StorageSite.h"
#include "CRConnectivity.h"



typedef MPM::VecD3 VecD3;
typedef MPM::VecD3Array VecD3Array;

MPM::MPM(string fileName):
  _particles(0),
  _coordinates()
{
  char* file;
  file = &fileName[0];

  // MPM::setandwriteParticles(file);
  //get coordinate
  const shared_ptr<VecD3Array> MPM_Coordinates = MPM::readCoordinates(file);

  //get velocity
  const shared_ptr<VecD3Array> MPM_Velocities = MPM::readVelocities(file);
  
  //get type
  const shared_ptr<Array<int> > MPM_Types = MPM::readTypes(file);

  //store all the information in MPM class
  MPM::Init(MPM_Coordinates, MPM_Velocities, MPM_Types);}

MPM::MPM( ):
  _particles(0),
  _coordinates()
{}


MPM::~MPM() { }


void MPM::setandwriteParticles(const char *file)
{
 //here, we want to build up a solid circle 
 //the number of solid points and coordiantes are written to file
    FILE *fp;
           
    VecD3 center;
    center[0]=0.;
    center[1]=0.;
    center[2]=0.0;

    int count=0;   
    double radius = 0.20;
   
#if 1
    //set up particle cartesian coordinate
    VecD3 temp;
    
    int nX=101, nY=21, nZ=1;
    double gapX=35e-6/(nX-1), gapY=1e-6/(nY-1), gapZ=0.0/nZ;
    VecD3 solidPoint[nX*nY*nZ];
    VecD3 solidVelocity[nX*nY*nZ];
    int type[nX*nY*nZ];


    for(int i=0; i<nX; i++){
      for(int j=0; j<nY; j++){
	for(int k=0; k<nZ; k++){
	  temp[0]=i*gapX-17.5e-6;
	  temp[1]=j*gapY-0.5e-6;
	  temp[2]=k*gapZ;
	  //VecD3 dr = temp-center;
	  //if(mag2(dr)<=radius*radius){
	    solidPoint[count][0]=temp[0];
	    solidPoint[count][1]=temp[1];
	    solidPoint[count][2]=temp[2];
	    type[count] = 1;  //internal particles
	    if(i==0||i==nX-1||j==0||j==nY-1||k==0||k==nZ-1){
	      type[count] = 1;       //surface particles	   
	    }
	    count+=1;
	    //}
	}
      }
    }

#endif

#if 0
    int nX=51, nY=1000, nZ=1;
    double radius1=0., radius2=0.2;
    double gapR=(radius2-radius1)/nX, gapAngle=2*3.1415926/nY, gapZ=1.0/nZ;
    VecD3 solidPoint[nX*nY*nZ];
    VecD3 solidVelocity[nX*nY*nZ];
    int type[nX*nY*nZ];

    //polar coordinate 
     for(int i=0; i<nX; i++){
      for(int j=0; j<nY; j++){
	for(int k=0; k<nZ; k++){	 
	  solidPoint[count][0]=(radius1+i*gapR)*(cos(j*gapAngle))+center[0];
	  solidPoint[count][1]=(radius1+i*gapR)*(sin(j*gapAngle))+center[1];
	  solidPoint[count][2]=k*gapZ+center[2];
	  
	  if(i!=(nX-1)) type[count] = 0;  //internal particles
	  if(i==(nX-1)){
	    type[count] = 1;       //surface particles	   
	  }
	  count+=1;
	}
      }
    }

     /* 
    radius1=0.9, radius2=1.5;
    gapR=(radius2-radius1)/nX, gapAngle=2*3.1415926/nY, gapZ=1.0/nZ;

      //polar coordinate 
     for(int i=0; i<nX; i++){
      for(int j=0; j<nY; j++){
	for(int k=0; k<nZ; k++){	 
	  solidPoint[count][0]=(radius1+i*gapR)*(cos(j*gapAngle))+center[0];
	  solidPoint[count][1]=(radius1+i*gapR)*(sin(j*gapAngle))+center[1];
	  solidPoint[count][2]=k*gapZ+center[2];
	  
	  if(i!=0) type[count] = 0;  //internal particles
	  if(i==0){
	    type[count] = 1;       //surface particles	   
	  }
	  count+=1;
	}
      }
    }
     */
#endif
#if 0
     int nX=10,  nY=50, nZ=50;
  const double pi = atan(1.0)*4.0;
    double radius1=0., radius2=10.0;
    double gapR=(radius2-radius1)/nX, gapAlfa=pi/nY, gapBeta=2*pi/nZ;
    const int nMPM = nX*nY*nZ;
    Array<VecD3> solidPoint(nMPM);
    Array<VecD3> solidVelocity(nMPM);
    Array<int> type(nMPM);

    //polar coordinate 
     for(int i=1; i<=nX; i++){
      for(int j=0; j<nY; j++){
	for(int k=0; k<nZ; k++){	 
	  solidPoint[count][0]=(radius1+i*gapR)*sin(j*gapAlfa)*(cos(k*gapBeta))+center[0];
	  solidPoint[count][1]=(radius1+i*gapR)*sin(j*gapAlfa)*(sin(k*gapBeta))+center[1];
	  solidPoint[count][2]=(radius1+i*gapR)*cos(j*gapAlfa)+center[2];
	  
	  type[count] = 1;  //internal particles
	  if(i==nX || i==nX-1){
	    type[count] = 1;       //surface particles	   
	  }
	  count+=1;
	}
      }
    }

#endif
    //set up particle velocity
#if 1
    for(int p=0; p<count; p++){
      solidVelocity[p][0]=0.001;
      solidVelocity[p][1]=0.0;
      solidVelocity[p][2]=0.0;
    }
#endif 


#if 0
    //set up rotating cylinder velcity
    const double angV = 1;
    for (int p=0; p<count; p++){
      double r = mag(solidPoint[p]-center);
      double angle = atan2(solidPoint[p][1]-center[1],solidPoint[p][0]-center[0]);
      solidVelocity[p][0] = -angV*r*sin(angle);
      solidVelocity[p][1] = angV*r*cos(angle);
      solidVelocity[p][2] = 0.0;
    }

#endif

    cout<<"count of particles is "<<count<<endl;
    //write out coordinate and velocity and particle type into file
    fp=fopen(file,"w");
    fprintf(fp,"%i\n",count);
    for(int p=0; p<count; p++){
      fprintf(fp, "%e\t%e\t%e\n", solidPoint[p][0],solidPoint[p][1],solidPoint[p][2]);
    } 
    for(int p=0; p<count; p++){
      fprintf(fp, "%e\t%e\t%e\n", solidVelocity[p][0],solidVelocity[p][1],solidVelocity[p][2]);
    }    
    for(int p=0; p<count; p++){
      fprintf(fp, "%i\n", type[p]);
    } 
    fclose(fp);
}


const shared_ptr<Array<VecD3> > MPM::readCoordinates(const char *file)

{
    FILE *fp;
    int nMPM;
    double x=0, y=0, z=0;
   
    fp=fopen(file,"r");
    fscanf(fp,"%i\n",&nMPM);
    cout<<"number of particles is"<<nMPM<<endl;
    
    shared_ptr<Array<VecD3> > MPM_Points ( new Array<VecD3> (nMPM));
    //read in coordinate
    for(int i=0; i<nMPM; i++){
      fscanf(fp,"%lf\t%lf\t%lf\n", &x, &y, &z);
      (*MPM_Points)[i][0]=x;
      (*MPM_Points)[i][1]=y;
      (*MPM_Points)[i][2]=z;
    }
    fclose(fp);   
    return (MPM_Points);
}

const shared_ptr<Array<VecD3> > MPM::readVelocities(const char *file)

{
    FILE *fp;
    int nMPM;
    double vx=0, vy=0, vz=0;
    double x=0, y=0, z=0;
    fp=fopen(file,"r");
    fscanf(fp,"%i\n",&nMPM);
    
    shared_ptr<Array<VecD3> > MPM_Points ( new Array<VecD3> (nMPM));
    //read in cooridnate and skip
    for(int i=0; i<nMPM; i++){
      fscanf(fp,"%lf\t%lf\t%lf\n", &x, &y, &z);      
    }
    //read in velocity
    for(int i=0; i<nMPM; i++){
      fscanf(fp,"%lf\t%lf\t%lf\n", &vx, &vy, &vz);
      (*MPM_Points)[i][0]=vx;
      (*MPM_Points)[i][1]=vy;
      (*MPM_Points)[i][2]=vz;
    }
    fclose(fp);   
    return (MPM_Points);
}


const shared_ptr<Array<int> > MPM::readTypes(const char *file)

{
    FILE *fp;
    int nMPM;
    double vx=0, vy=0, vz=0;
    double x=0, y=0, z=0;
    int t=0;
    fp=fopen(file,"r");
    fscanf(fp,"%i\n",&nMPM);
    
    shared_ptr<Array<int> > MPM_Points ( new Array<int> (nMPM));
    //read in cooridnate and skip
    for(int i=0; i<nMPM; i++){
      fscanf(fp,"%lf\t%lf\t%lf\n", &x, &y, &z);      
    }
    //read in velocity and skip
    for(int i=0; i<nMPM; i++){
      fscanf(fp,"%lf\t%lf\t%lf\n", &x, &y, &z);      
    }
    //read in type
    for(int i=0; i<nMPM; i++){
      fscanf(fp,"%i\n", & t);
      (*MPM_Points)[i]=t;
    }
    fclose(fp);   
    return (MPM_Points);
}


void MPM::Init(const shared_ptr<Array<VecD3> > coordinates,
	       const shared_ptr<Array<VecD3> > velocities,
	       const shared_ptr<Array<int> > types)
{

  const int n = (*coordinates).getLength();  //number of particles
  _particles.setCount(n);
  
  MPM::setCoordinates(coordinates);
  MPM::setVelocities(velocities);
  MPM::setTypes(types);

}

