// Copyright 2019 NEDL CO.,LTD

#include<string.h>
#include "navigation_info.h"

using namespace std;

void NavigationInfo::setNaviInfoSessionHandle( char* sessionHandle )
{
	mSessionHandle = stoi(sessionHandle);
}

uint32_t NavigationInfo::getNaviInfoSessionHandle()
{
	return mSessionHandle;
}

void NavigationInfo::setNaviInfoClient( string client )
{
	mClient = client;
}

string NavigationInfo::getNaviInfoClient()
{
	return mClient;
}

void NavigationInfo::setNaviInfoCurrentLatitude( char* latitude )
{
	strcpy(mCurrentLatitude,latitude);
}

void NavigationInfo::setNaviInfoCurrentHeading( char* heading )
{
	strcpy(mCurrentHeading,heading);
}

char* NavigationInfo::getNaviInfoCurrentLatitude()
{
	return mCurrentLatitude;
}	

void NavigationInfo::setNaviInfoCurrentLongitude( char* longitude )
{
	strcpy(mCurrentLongitude,longitude);
}

char* NavigationInfo::getNaviInfoCurrentLongitude()
{
	return mCurrentLongitude;
}

char* NavigationInfo::getNaviInfoCurrentHeading()
{
	return mCurrentHeading;
}

void NavigationInfo::setNaviInfoCurrentDirState( char* state )
{
	strcpy(mCurrentState,state);
}

char* NavigationInfo::getNaviInfoCurrentDirState()
{
	return mCurrentState;
}

void NavigationInfo::setNaviInfoCurrentTotalDistance( char* totaldistance )
{
	strcpy(mCurrentTotalDistance,totaldistance);
}

int NavigationInfo::getNaviInfoCurrentTotalDistance()
{
	string totaldistance_string;
	int totaldistance_int;
	totaldistance_string = mCurrentTotalDistance;
	totaldistance_int = atoi(totaldistance_string.c_str());
	return totaldistance_int;
}

void NavigationInfo::setNaviInfoCurrentCumulativeDistance( char* cumulativedistance )
{
	strcpy(mCurrentCumulativeDistance,cumulativedistance);
}

int NavigationInfo::getNaviInfoCurrentCumulativeDistance()
{
	string cumulativedistance_string;
	int cumulativedistance_int;
	cumulativedistance_string = mCurrentCumulativeDistance;
	cumulativedistance_int = atoi(cumulativedistance_string.c_str());
	return cumulativedistance_int;
}

void NavigationInfo::setNaviInfoCurrentGuidanceState( char* state )
{
	strcpy(mGuidanceState,state);
}

char* NavigationInfo::getNaviInfoCurrentGuidanceState()
{
	return mGuidanceState;
}

void NavigationInfo::setNaviInfoAllRoutes( char* route )
{
	mRoutes = stoi(route);
}

uint32_t NavigationInfo::getNaviInfoAllRoutes()
{
	return mRoutes;
}

void NavigationInfo::setNaviInfoDestLatitude( char* latitude )
{
	strcpy(mDestLatitude,latitude);
}

char* NavigationInfo::getNaviInfoDestLatitude()
{
	return mDestLatitude;
}	

void NavigationInfo::setNaviInfoDestLongitude( char* longitude )
{
	strcpy(mDestLongitude,longitude);
}

char* NavigationInfo::getNaviInfoDestLongitude()
{
	return mDestLongitude;
}

void NavigationInfo::setNaviInfoDemoLatitude( char* latitude )
{
	strcpy(mDemoLatitude,latitude);
}

char* NavigationInfo::getNaviInfoDemoLatitude()
{
	return mDemoLatitude;
}	

void NavigationInfo::setNaviInfoDemoLongitude( char* longitude )
{
	strcpy(mDemoLongitude,longitude);
}

char* NavigationInfo::getNaviInfoDemoLongitude()
{
	return mDemoLongitude;
}

void NavigationInfo::setNaviInfoDemoDistance( char* distance )
{
	strcpy(mDemoDistance,distance);
}

char* NavigationInfo::getNaviInfoDemoDistance()
{
	return mDemoDistance;
}	

void NavigationInfo::setNaviInfoDemoDirection( char* direction )
{
	strcpy(mDemoDirection,direction);
}

char* NavigationInfo::getNaviInfoDemoDirection()
{
	return mDemoDirection;
}

void NavigationInfo::setGuidanceStartLatitude(char* latitude)
{strcpy(mGuidanceStartLatitude,latitude);
}

char* NavigationInfo::getGuidanceStartLatitude()
{
	return mGuidanceStartLatitude;
}

void NavigationInfo::setGuidanceStartLongitude(char* longitude)
{
	strcpy(mGuidanceStarLongitude,longitude);
}

char* NavigationInfo::getGuidanceStartLongitude()
{
	return mGuidanceStarLongitude;
}