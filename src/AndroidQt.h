// Copyright (c) 2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef ANDROIDQT_H
#define ANDROIDQT_H

#include <OpenGl_Context.hxx>

#include <Standard_WarningsDisable.hxx>
// workaround broken definitions in Qt
#define GLdouble GLdouble
#include <AIS_ViewCube.hxx>
#include <QMutex>
#include <QtQuick/qquickwindow.h>
#include <QtQuick/QQuickItem>
#include <AIS_RubberBand.hxx>
#undef GLdouble
#include <Standard_WarningsRestore.hxx>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include "AndroidQt_TouchParameters.h"
#include <AIS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <tcl.h>
#include <iostream>
#include <Draw_Interpretor.hxx>
#include <Draw.hxx>
#include <DBRep.hxx>
#include <DrawTrSurf.hxx>
#include <ViewerTest.hxx>
#include <TCollection_AsciiString.hxx>
#include <OSD_File.hxx>
#include <NOcaf_Application.h>
#include <ViewProjPoint.h>
//#include <Ntcl.h>
#include <DDocStd.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Line.hxx>
#include <Geom_Plane.hxx>
#include <GeomAPI_IntCS.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <NCollection_Sequence.hxx>
#include <NCollection_Vector.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TDF_Tool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <QStringList>
#include <AIS_Manipulator.hxx>
class AIS_AnimationCamera;
class AIS_InteractiveObject;
class AIS_InteractiveContext;
class AIS_Point;
class AIS_RubberBand;
class AIS_XRTrackedDevice;
class Graphic3d_Camera;
class SelectMgr_EntityOwner;
class V3d_View;
class WNT_HIDSpaceMouse;


enum CurrentAction3d
{
    CurAction3d_Nothing,
    CurAction3d_DynamicZooming,
    CurAction3d_WindowZooming,
    CurAction3d_DynamicPanning,
    CurAction3d_DynamicRotation
};
enum EditState
{
    DrawNothing,
    DrawLine,
    DrawPolyLine,
    DrawArc,
    DrawCircle,
    DrawRectangle
};
Standard_IMPORT Draw_Interpretor theCommands;

static void ReadInitFile (const TCollection_AsciiString& theFileName, Draw_Interpretor& theDI)
{
  TCollection_AsciiString aPath = theFileName;

  char* com = new char [aPath.Length() + strlen ("source ") + 2];
  Sprintf (com, "source %s", aPath.ToCString());
  theDI.Eval (com);
  delete [] com;
}


//! QML item with embedded OCCT viewer.
class AndroidQt : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(double transParency READ transParency WRITE setTransParency NOTIFY transParencyChanged)
    Q_PROPERTY(QString commandLine READ commandLine WRITE setCommandLine NOTIFY commandLineChanged)
    Q_PROPERTY(QString occtColor READ occtColor WRITE setOcctColor NOTIFY occtColorChanged)

    QML_ELEMENT
public:
    //! Default constructor.
    AndroidQt();
    double transParency();
    void setTransParency(const double &value);

    QString commandLine();//get result
    void setCommandLine(const QString &cmdline);

    QString occtColor();
    void setOcctColor(const QString &cmdline);
    Q_INVOKABLE void setOcctColorComplete();

    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();

    Q_INVOKABLE void setAISDisplayMode(bool);
    Q_INVOKABLE void unSetAISDisplayMode();
    Q_INVOKABLE void setRaytracing(bool);
    Q_INVOKABLE void setRaytracedShadows(bool);
    Q_INVOKABLE void setRaytracedReflections(bool);
    Q_INVOKABLE void setRaytracedAntialiasing(bool);
    //! Display shape from file.
    Q_INVOKABLE bool readShapeFromFile(QString theFilePath);
    //! Handle touch event.
    Q_INVOKABLE void initTouch(const double theX,
                               const double theY);
    //! Handle touch event.
    Q_INVOKABLE void updateTouch(const double theX,
                                 const double theY);

    //mouse control
    Q_INVOKABLE void leftButtonPressed();
    Q_INVOKABLE void rightButtonPressed();
    Q_INVOKABLE void leftRightButtonPressed();
    Q_INVOKABLE void middleButtonPressed();
    Q_INVOKABLE void mouseButtonReleased();
    Q_INVOKABLE void mouseDoubleClicked();
    // Q_INVOKABLE void PositionChanged(bool);
    // Q_INVOKABLE void LeftButtonReleased();
    // Q_INVOKABLE void RightButtonReleased();
    // Q_INVOKABLE void LeftRightButtonReleasedd();
    // Q_INVOKABLE void MiddleButtonReleasedd();

    //select control
    Q_INVOKABLE void selectVertices();
    Q_INVOKABLE void selectEdges();
    Q_INVOKABLE void selectFaces();
    Q_INVOKABLE void selectWires();
    Q_INVOKABLE void selectShapes();
    Q_INVOKABLE void selectShelles();
    Q_INVOKABLE void selectSolid();
    Q_INVOKABLE void selectCompsolid();
    Q_INVOKABLE void selectCompound();

    // view control
    Q_INVOKABLE void hlr(bool);
    Q_INVOKABLE void shading();
    Q_INVOKABLE void wireframe();
    Q_INVOKABLE void fitAll();
    Q_INVOKABLE void top();
    Q_INVOKABLE void bottom();
    Q_INVOKABLE void left();
    Q_INVOKABLE void right();
    Q_INVOKABLE void front();
    Q_INVOKABLE void back();
    Q_INVOKABLE void axo();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void rotation(bool);
    Q_INVOKABLE void pan(bool);
    Q_INVOKABLE void zoom(const int angleDelta ,bool );

    //DrawEdit 
    Q_INVOKABLE void drawLine(bool);
    Q_INVOKABLE void drawPolyLine(bool);
    Q_INVOKABLE void drawArc(bool);
    Q_INVOKABLE void drawCircle(bool);
    Q_INVOKABLE void drawRectangle(bool);
    Q_INVOKABLE void confirmPolyLine();
    
    //
    gp_Pnt mouse2ProjIntersector(Standard_Integer theX,Standard_Integer theY);



    //! make test shape
    Q_INVOKABLE void makeBox(void);
    Q_INVOKABLE void makeCone(void);
    Q_INVOKABLE void makeSphere(void);
    Q_INVOKABLE void makeCylinder(void);
    Q_INVOKABLE void makeTorus(void);
    Q_INVOKABLE void makeFillet(void);
    Q_INVOKABLE void makeChamfer(void);
    Q_INVOKABLE void makeExtrude(void);
    Q_INVOKABLE void makeRevol(void);
    Q_INVOKABLE void makeLoft(void);
    Q_INVOKABLE void testCut(void);
    Q_INVOKABLE void testFuse(void);
    Q_INVOKABLE void testCommon(void);
    Q_INVOKABLE void testHelix(void);
    Q_INVOKABLE void makeToroidalHelix(void);
    Q_INVOKABLE void makeConicalHelix(void);
    Q_INVOKABLE void makeCylindricalHelix(void);


public slots:

    //! Handle OpenGL context creation and window resize events.
    void sync();

    //! Redraw OCCT viewer and handle pending viewer events in rendering thread.
    void paint();

//    void stdout(QString str);
signals:
    void transParencyChanged();
    void commandLineChanged();
    void occtColorChanged();

private slots:

    //! Handle window change event.
    void handleWindowChanged(QQuickWindow *theWin);

private:
    //! (Re-)initialize viewer on OpenGL context change.
    //! \param theWin handle to GUI window
    bool initViewer(Aspect_Drawable theWin);
    void SetSelectionMode(const Standard_Integer theMode);

    //! Close viewer
    void release();

private:
    Handle(V3d_Viewer) myViewer;              //!< 3D viewer
    Handle(V3d_View) myView;                  //!< 3D view
    Handle(AIS_InteractiveContext) myContext; //!< interactive context
    Graphic3d_Vec2i myWinTopLeft;             //!< cached window position (top-left)
    Graphic3d_Vec2i myWinSize;                //!< cached window width-height

    QMutex myMutex;                         //!< mutex for interconnection with rendering thread
    QSize myViewportSize;                   //!< QML item size
    AndroidQt_TouchParameters myTouchPoint; //!< cached state of touch event
    Handle(AIS_ViewCube) H_AisViewCube;
    int myCurrentAction3d;
    int myAngleDelta;
    bool myLeftButtomClick;


    double myTransParency;
    QString myCommandLine;
    QString myColor;


    PressedMouseButton myPressedMouseButton;
    EditState   myEditState;
    double clickedX,clickedY;
    Draw_Interpretor& aDI=theCommands;
    Handle(NOcaf_Application)       myOcaf_app;

    
    bool clickedfirst =false;

    bool  allowZoom;
    bool  mouseMoved;
    gp_Ax2 anAxis;

    ViewProjPoint myViewProjPoint;
    Handle(AIS_Shape) tmpAISShape;
    TopoDS_Edge tmpEdge;
    
    NCollection_Sequence<gp_Pnt> aPolyLineCollection;
    NCollection_Vector<Handle(AIS_InteractiveObject)> myObject3d;
    NCollection_Vector<Handle(AIS_InteractiveObject)> myColorAIS;
    NCollection_Vector<Handle(TPrsStd_AISPresentation)> myPresentations;
    NCollection_List<TCollection_AsciiString> myEntrys;
    bool clickedfirstPL;

    bool mRedo,mUndo;
    bool mSetColor,mSetColorAIS;

    Handle(AIS_Manipulator) aManipulator;
    
};

#endif // ANDROIDQT_H
