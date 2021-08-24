#include <ViewProjPoint.h>

void ViewProjPoint::setInitgp_Pnt(Standard_Real x, Standard_Real y , Standard_Real z)
{
    gpf=gp_Pnt(x,y,z);
}

void ViewProjPoint::setInitgp_Pnt(gp_Pnt gp)
{
    gpf=gp;
}

void ViewProjPoint::setUpdategp_Pnt(Standard_Real x, Standard_Real y, Standard_Real z)
{
    gps=gp_Pnt(x,y,z);
}

void ViewProjPoint::setUpdategp_Pnt(gp_Pnt gp)
{
    gps=gp;
}

gp_Pnt ViewProjPoint::getFirstgp_Pnt()
{
    return gpf;
}

gp_Pnt ViewProjPoint::getSecondgp_Pnt()
{
    return gps;
}
///////
void ViewProjPoint::setInitgp_Dir(Standard_Real x, Standard_Real y , Standard_Real z)
{
    gpdf=gp_Dir(x,y,z);
}

void ViewProjPoint::setInitgp_Dir(gp_Dir gp)
{
    gpdf=gp;
}

void ViewProjPoint::setUpdategp_Dir(Standard_Real x, Standard_Real y, Standard_Real z)
{
    gpds=gp_Dir(x,y,z);
}

void ViewProjPoint::setUpdategp_Dir(gp_Dir gp)
{
    gpds=gp;
}

gp_Dir ViewProjPoint::getFirstgp_Dir()
{
    return gpdf;
}

gp_Dir ViewProjPoint::getSecondgp_Dir()
{
    return gpds;
}

///////
ViewProjPoint::ViewProjPoint(/* args */)
{
}

ViewProjPoint::~ViewProjPoint()
{
}
