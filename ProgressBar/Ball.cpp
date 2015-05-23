#include "stdafx.h"
#include "Ball.h"


Ball::Ball() :HALF_G(9.81 / 2), PI(3.14159265)
{
	currCoord = { 0, 0 };
	degree = 1;
	velocity = 0;

}
//-------------------------------------------------------------------


Ball::~Ball()
{
}
//-------------------------------------------------------------------

POINT &Ball::GetCurrCoord() 
{
	return currCoord;
}
//-------------------------------------------------------------------
void Ball::ComputeCoord(const int &t)
{
	int temp = t / 10;
	
	currCoord.x = lround(multVelosCos * temp);

	currCoord.y = lround(multVelosSin * temp - HALF_G*temp*temp);

}// void Ball::ComputeCoord(const int &t)
//-------------------------------------------------------------------

void Ball::ComputeVelosCos()
{
	multVelosCos = velocity *cos(degree * PI / 180.0);
}
//-------------------------------------------------------------------

void Ball::ComputeVelosSin()
{
	multVelosSin = velocity * sin(degree * PI / 180.0);
}
//-------------------------------------------------------------------
void Ball::SetVeloc(const double &v)
{
	velocity = v * 10;
}

void Ball::SetDegree(const int &d)
{
	degree = d;
}

double Ball::GetmultVelosCos() const
{
	return multVelosCos;

}