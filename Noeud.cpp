#include "pch.h"
#include "Noeud.h"


CNoeud::CNoeud()
{
	posX = 0.f;
	posY = 0.f;
	dirCrt = 0.f;
}

CNoeud::CNoeud(CNoeud* n)
{
	posX = n->posX;
	posY = n->posY;
	dirCrt = n->dirCrt;
}

CNoeud::CNoeud(float x, float y, float d)
{
	posX = x;
	posY = y;
	dirCrt = d;
}
	
float CNoeud::GetDir()
{
	return dirCrt;
}
float CNoeud::GetX()
{
	return posX;
}

float CNoeud::GetY()
{
	return posY;
}

D2D1_POINT_2F CNoeud::GetPoint()
{
	D2D1_POINT_2F p ;
	p.x = posX;
	p.y = posY;
	return p;
}


