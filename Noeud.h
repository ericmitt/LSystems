#pragma once
class CNoeud 
{
public:
	CNoeud();
	CNoeud(CNoeud*);
	CNoeud::CNoeud(float x, float y, float d=0.f);
	float GetDir();
	float GetX();
	float GetY();
	D2D1_POINT_2F GetPoint();
public:
	float posX,posY,dirCrt;
};


