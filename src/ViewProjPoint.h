#ifndef VIEWPROJPOINT
#define VIEWPROJPOINT
#include <Standard.hxx>
#include <gp_Pnt.hxx>
class ViewProjPoint
{
private:
    gp_Pnt gpf,gps;//InitTouch updateTouch
    gp_Dir gpdf,gpds;
    /* data */
public:
    void setInitgp_Pnt(Standard_Real,Standard_Real,Standard_Real);
    void setInitgp_Pnt(gp_Pnt);
    void setUpdategp_Pnt(Standard_Real,Standard_Real,Standard_Real);
    void setUpdategp_Pnt(gp_Pnt);

    void setInitgp_Dir(Standard_Real,Standard_Real,Standard_Real);
    void setInitgp_Dir(gp_Dir);
    void setUpdategp_Dir(Standard_Real,Standard_Real,Standard_Real);
    void setUpdategp_Dir(gp_Dir);

    gp_Pnt getFirstgp_Pnt();
    gp_Pnt getSecondgp_Pnt();

    gp_Dir getFirstgp_Dir();
    gp_Dir getSecondgp_Dir();
    
    ViewProjPoint(/* args */);
    ~ViewProjPoint();
};



#endif