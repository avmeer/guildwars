#include "Color.h"

Color::Color()
{
	red = 1;
	green = 1;
	blue = 1;
}

Color::Color( float initRed, float initGreen, float initBlue ){
	red = fmax(0, fmin(1, initRed));
	green = fmax(0, fmin(1, initGreen));
	blue = fmax(0, fmin(1, initBlue));
}


void Color::setColor() 
{
	glColor3f( red, green, blue );
}

float Color::get_red()
{
	return( red );
}
		
float Color::get_green(){
	return( green );
}

float Color::get_blue()
{
	return( blue );
}

void Color::set_red( float newRed )
{
	red = fmax(0, fmin(1, newRed));
}

void Color::set_green( float newGreen )
{
	green = fmax(0, fmin(1, newGreen));
}

void Color::set_blue( float newBlue )
{
	blue = fmax(0, fmin(1, newBlue));
}