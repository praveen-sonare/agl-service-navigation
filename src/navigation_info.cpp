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