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

#if defined(_WIN32)
#include <windows.h>
#endif

#include "AndroidQt.h"
#include "AndroidQt_UserInteractionParameters.h"
#include "AndroidQt_Window.h"

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Quantity_Color.hxx>
#include <Standard_ErrorHandler.hxx>
#include <TopoDS.hxx>

#include <UnitsAPI.hxx>
#include <WNT_Window.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <Xw_Window.hxx>

#include <EGL/egl.h>

#include <Standard_WarningsDisable.hxx>
#include <QFileInfo>
#include <Standard_WarningsRestore.hxx>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <BinDrivers.hxx>
#include <XmlDrivers.hxx>

#include <Ntcl.h>

// =======================================================================
// function : AndroidQt
// purpose  :
// =======================================================================

AndroidQt::AndroidQt()
{
    connect(this, SIGNAL(windowChanged(QQuickWindow *)), this, SLOT(handleWindowChanged(QQuickWindow *)));
    // set shaders location variable
    QByteArray aDataRoot = "/home/cqwuxiaolong/OCCT/src";
    qputenv("CSF_ShadersDirectory", aDataRoot + "/Shaders");

    //    aDI = theCommands;

#ifdef _WIN32
    // force Windows to report dll loading problems interactively
    ::SetErrorMode(0);
#endif

    aDI.Init();
    Tcl_Interp *interp = aDI.Interp();
    Tcl_Init(interp);
    Tcl_RegisterIO(interp);

    TCollection_AsciiString aCascadeDir = "/home/cqwuxiaolong/OCCT";
    aCascadeDir.ChangeAll('\\', '/');
    TCollection_AsciiString aDataDir = "../../../data/";

    OSD_Path anInstallFlagPath(".cadrays-installed");
    OSD_File aFile(anInstallFlagPath);

    if (aFile.Exists())
    {
        aDataDir = "data/";
        aCascadeDir = "occt-tcl/";
    }

#ifdef _WIN32
    putenv(const_cast<char *>((TCollection_AsciiString("CASROOT=") + aCascadeDir).ToCString()));
#else
    setenv("CASROOT", aCascadeDir.ToCString(), 1);
#endif
    Tcl_PutEnv((TCollection_AsciiString("CASROOT=") + aCascadeDir).ToCString());

    Tcl_PutEnv((TCollection_AsciiString("APP_DATA=") + aDataDir).ToCString());

    Draw::BasicCommands(aDI);
    DBRep::BasicCommands(aDI);
    DrawTrSurf::BasicCommands(aDI);

    TCollection_AsciiString aDefStr(getenv("CASROOT"));
    aDefStr += "/src/DrawResources/DrawDefault";
    ReadInitFile(aDefStr, aDI);

    aDI.Eval("proc reopenStdout {file} { close stdout; open $file w; }");

    aDI.Eval("pload ALL");

    TCollection_AsciiString aDefDir = aDataDir;
    TCollection_AsciiString aUserDefDir;

    aManipulator = new AIS_Manipulator();
    // aManipulator->SetPart (0, AIS_ManipulatorMode::AIS_MM_TranslationPlane, Standard_False);
    // aManipulator->SetPart (1, AIS_ManipulatorMode::AIS_MM_TranslationPlane, Standard_False);
    // aManipulator->SetPart (2, AIS_ManipulatorMode::AIS_MM_TranslationPlane, Standard_False);
     // Attach manipulator to already displayed object and manage manipulation modes
    // aManipulator->AttachToObject (anAISObject);
    // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_Translation);
    // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
    // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_Scaling);
    // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_TranslationPlane);
    //Draw::Load (aDI, "ImportExport", "DrawPlugin", aDefDir, aUserDefDir);
    //Draw::Load(aDI, "TOPTEST", "DrawPlugin1", aDefDir, aUserDefDir);
    //Draw::Load(aDI, "DCAF", "DrawPlugin2", aDefDir, aUserDefDir);
    //Draw::Load(aDI, "XSDRAW", "DrawPlugin3", aDefDir, aUserDefDir);
    //Draw::Load(aDI, "XDEDRAW", "DrawPlugin4", aDefDir, aUserDefDir);
    //Draw::Load(aDI, "AISV", "DrawPlugin5", aDefDir, aUserDefDir);
    // ncs=new NConsoleStream(std::cout,&myCommandLine);
    // ncs->registerNConsoleMessageHandler();
}

double AndroidQt::transParency()
{
    if (myContext->NbSelected())
    {
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
            return myContext->SelectedInteractive()->Transparency();
    }
    return 0;
}

void AndroidQt::setTransParency(const double &value)
{
    if (value == myTransParency)
        return;
    myTransParency = value;
    if (myContext->NbSelected())
    {
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
        {
            myContext->SetTransparency(myContext->SelectedInteractive(), myTransParency, Standard_False);
        }
    }
    else
    {
        myTransParency = -1;
    }
    emit transParencyChanged();
}

QString AndroidQt::commandLine()
{
    return myCommandLine;
}

void AndroidQt::setCommandLine(const QString &cmdline)
{
    myMutex.lock();
    std::string str = cmdline.toStdString();
    const char *ch = str.c_str();
    Standard_CString cmd = ch;
    aDI.Eval(cmd);
    myCommandLine = aDI.Result();
    emit commandLineChanged();
    myMutex.unlock();
}

QString AndroidQt::occtColor()
{
    return myColor;
}

void AndroidQt::setOcctColor(const QString &hexcolor)
{    
    myColor = hexcolor;
    mSetColorAIS=true;
    if (myContext->NbSelected())
    {
        myEntrys.Clear();
        myColorAIS.Clear();

        // Handle(Prs3d_Drawer) drowHilight = new Prs3d_Drawer();
        // drowHilight->SetColor(color);

        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
        {
            
            Handle(TPrsStd_AISPresentation) anAisPresentation = Handle(TPrsStd_AISPresentation)::DownCast(myContext->SelectedInteractive()->GetOwner());
            // myContext->SetSelectionStyle(drowHilight);
            //myContext->SetColor(myContext->SelectedInteractive(),color,Standard_False);
            myColorAIS.Append(myContext->SelectedInteractive());
            TDF_Label aLabel = anAisPresentation->Label();
            TCollection_AsciiString Entry;
            TDF_Tool::Entry(aLabel, Entry);
            myEntrys.Append(Entry);

        }
    }
    else
    {
        ;
    }
    myContext->ClearSelected(Standard_False);
    emit occtColorChanged();
}

void AndroidQt::setOcctColorComplete()
{
    mSetColor=true;
    mSetColorAIS=false;
}

void AndroidQt::undo()
{
    // aDI.Eval("Undo D");
    // aDI.Result();
    mUndo = true;
}

void AndroidQt::redo()
{
    // aDI.Eval("Redo D");
    // aDI.Result();
    mRedo = true;
}

void AndroidQt::setAISDisplayMode(bool m)
{
    if (myContext->NbSelected())
    {
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
        {
            m == true ? myContext->SetDisplayMode(myContext->SelectedInteractive(), AIS_DisplayMode::AIS_Shaded, Standard_False) : myContext->SetDisplayMode(myContext->SelectedInteractive(), AIS_DisplayMode::AIS_WireFrame, Standard_False);
        }
    }
}

void AndroidQt::unSetAISDisplayMode()
{
    if (myContext->NbSelected())
    {
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
        {
            myContext->UnsetDisplayMode(myContext->SelectedInteractive(), Standard_False);
        }
    }
}

void AndroidQt::setRaytracing(bool r)
{
    if (r)
        myView->ChangeRenderingParams().Method = Graphic3d_RM_RAYTRACING;
    else
        myView->ChangeRenderingParams().Method = Graphic3d_RM_RASTERIZATION;
}

void AndroidQt::setRaytracedShadows(bool r)
{
    myView->ChangeRenderingParams().IsShadowEnabled = r;
}

void AndroidQt::setRaytracedReflections(bool r)
{
    myView->ChangeRenderingParams().IsReflectionEnabled = r;
}

void AndroidQt::setRaytracedAntialiasing(bool r)
{
    myView->ChangeRenderingParams().IsAntialiasingEnabled = r;
}

// =======================================================================
// function : ReadShapeFromFile
// purpose  :
// =======================================================================
bool AndroidQt::readShapeFromFile(QString theFilePath)
{
    QUrl aFileUrl(theFilePath);
    QString aFilePath = theFilePath;
    if (aFileUrl.isLocalFile())
    {
        aFilePath = QUrl(theFilePath).toLocalFile();
    }

    if (!QFile(aFilePath).exists())
    {
        return false;
    }

    TopoDS_Shape aShape;
    BRep_Builder aBuildTool;
    try
    {
        OCC_CATCH_SIGNALS

        if (!BRepTools::Read(aShape, aFilePath.toStdString().c_str(), aBuildTool))
        {
            return false;
        }

        if (!myContext.IsNull())
        {
            myContext->EraseAll(Standard_False);

            Handle(AIS_Shape) aShapePrs = new AIS_Shape(aShape);
            aShapePrs->SetColor(Quantity_Color(1.0, 0.73, 0.2, Quantity_TOC_RGB));

            myContext->Display(aShapePrs, Standard_False);
            myContext->SetDisplayMode(aShapePrs, AIS_Shaded, Standard_False);
        }

        myMutex.lock();
        myMutex.unlock();

        if (window())
        {
            window()->update();
        }
    }
    catch (Standard_Failure)
    {
        return false;
    }

    return true;
}

// =======================================================================
// function : InitTouch
// purpose  :
// =======================================================================
void AndroidQt::initTouch(const double theX,
                          const double theY)
{
    myMutex.lock();
    myTouchPoint.SetStarts(theX, theY);
    myMutex.unlock();
    if (!myView.IsNull())
    {
        Standard_Real x, y, z, vx, vy, vz; // initTouch  ConvertWithProj
        myView->ConvertWithProj(theX, theY, x, y, z, vx, vy, vz);
        //myViewProjPoint.setInitgp(x, y, z);
    }
}

// =======================================================================
// function : UpdateTouch
// purpose  :
// =======================================================================
void AndroidQt::updateTouch(const double theX,
                            const double theY)
{
    myMutex.lock();
    myTouchPoint.SetEnds(theX, theY);
    mouseMoved = true;
    if (!myView.IsNull())
    {
        Standard_Real xv, yv, zv;
        myView->Proj(xv, yv, zv);
        Standard_Real xat, yat, zat;
        myView->At(xat, yat, zat);
        gp_Pln aPlane(gp_Pnt(xat, yat, zat), gp_Dir(xv, yv, zv));

        Standard_Integer aPixX, aPixY;
        Standard_Real aX, aY, aZ, aDX, aDY, aDZ;
        myView->ConvertWithProj(theX, theY, aX, aY, aZ, aDX, aDY, aDZ);
        gp_Lin aLine(gp_Pnt(aX, aY, aZ), gp_Dir(aDX, aDY, aDZ));

        // Compute intersection
        Handle(Geom_Line) aGeomLine = new Geom_Line(aLine);
        Handle(Geom_Plane) aGeomPlane = new Geom_Plane(aPlane);
        GeomAPI_IntCS anIntersector(aGeomLine, aGeomPlane);
        if (!anIntersector.IsDone() || anIntersector.NbPoints() == 0)
        {
            myViewProjPoint.setUpdategp_Pnt(gp::Origin().X(), gp::Origin().Y(), gp::Origin().Z());
        }
        myViewProjPoint.setUpdategp_Pnt(anIntersector.Point(1).X(), anIntersector.Point(1).Y(), anIntersector.Point(1).Z());
        myViewProjPoint.setUpdategp_Dir(aDX, aDY, aDZ);
    }
    myMutex.unlock();
    if (window())
        window()->update();
}

void AndroidQt::leftButtonPressed()
{
    myPressedMouseButton = Pressed_LeftButton;
    clickedX = (Standard_Integer)myTouchPoint.X().second;
    clickedY = (Standard_Integer)myTouchPoint.Y().second;
    myContext->SelectDetected();
    //drwaLine
    myViewProjPoint.setInitgp_Pnt(myViewProjPoint.getSecondgp_Pnt());
    clickedfirst = !clickedfirst ? true : false;
    if (clickedfirst && myEditState == DrawLine)
    {
        QString v1 = QString("vertex v1 %1 %2 %3").arg(myViewProjPoint.getFirstgp_Pnt().X()).arg(myViewProjPoint.getFirstgp_Pnt().Y()).arg(myViewProjPoint.getFirstgp_Pnt().Z());
        std::string str = v1.toStdString();
        const char *ch = str.c_str();
        Standard_CString cmd = ch;
        aDI.Eval(cmd);
    }
    //drawCircle
    if (clickedfirst && myEditState == DrawCircle)
    {
        anAxis.SetDirection(myViewProjPoint.getSecondgp_Dir());
        anAxis.SetLocation(gp_Pnt(myViewProjPoint.getFirstgp_Pnt().X(), myViewProjPoint.getFirstgp_Pnt().Y(), myViewProjPoint.getFirstgp_Pnt().Z()));
    }
    //drawPolyLine

    if (myEditState == DrawPolyLine)
    {
        clickedfirstPL = true;
        aPolyLineCollection.Append(myViewProjPoint.getFirstgp_Pnt());
    }

    if (aManipulator->HasActiveMode())
    {
        aManipulator->StartTransform (myTouchPoint.X().second, myTouchPoint.Y().second, myView);
    }
}

void AndroidQt::rightButtonPressed()
{
    myPressedMouseButton = Pressed_RightButton;
    if(!myContext->NbSelected())myEditState=DrawNothing;
    //drawLine
    myContext->Remove(tmpAISShape, Standard_False);
    tmpAISShape.Nullify();
    clickedfirst = false;
    //drawPolyLine
    clickedfirstPL = false;
    if (aPolyLineCollection.Length() > 1)
    {
        aPolyLineCollection.Clear();
        myContext->Remove(myObject3d.First(), Standard_False);
    }

    myContext->ClearSelected(Standard_False);
    myContext->UpdateCurrentViewer();
}

void AndroidQt::leftRightButtonPressed()
{
    myPressedMouseButton = Pressed_LeftButton_RightButton;
}

void AndroidQt::middleButtonPressed()
{
    myPressedMouseButton = Pressed_MiddleButton;
}

void AndroidQt::mouseButtonReleased()
{
    myPressedMouseButton = Pressed_Nothing;
    
}

void AndroidQt::mouseDoubleClicked()
{
    
    if (myContext->NbSelected())
    {
        for (myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected())
        {
            aManipulator->Attach (myContext->SelectedInteractive());
            // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_Translation);
            // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
            // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_Scaling);
            // aManipulator->EnableMode (AIS_ManipulatorMode::AIS_MM_TranslationPlane);
            // myContext->Display(aManipulator,Standard_False);
        }
    }
}
//select control
void AndroidQt::selectWires()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_WIRE));
}

void AndroidQt::selectShapes()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_SHAPE));
}

void AndroidQt::selectShelles()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_SHELL));
}

void AndroidQt::selectSolid()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_SOLID));
}

void AndroidQt::selectCompsolid()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_COMPSOLID));
}

void AndroidQt::selectCompound()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_COMPOUND));
}

void AndroidQt::selectVertices()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_VERTEX));
}

void AndroidQt::selectEdges()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_EDGE));
}

void AndroidQt::selectFaces()
{
    myContext->Deactivate();
    SetSelectionMode(AIS_Shape::SelectionMode(TopAbs_FACE));
}

//viwer control
void AndroidQt::fitAll()
{
    myView->FitAll();
}

void AndroidQt::top()
{
    myView->SetProj(V3d_Zpos);
    myView->FitAll();
}

void AndroidQt::bottom()
{
    myView->SetProj(V3d_Zneg);
    myView->FitAll();
}

void AndroidQt::left()
{
    myView->SetProj(V3d_Xneg);
    myView->FitAll();
}

void AndroidQt::right()
{
    myView->SetProj(V3d_Xpos);
    myView->FitAll();
}

void AndroidQt::front()
{
    myView->SetProj(V3d_Yneg);
    myView->FitAll();
}

void AndroidQt::back()
{
    myView->SetProj(V3d_Ypos);
    myView->FitAll();
}

void AndroidQt::axo()
{
    myView->SetProj(V3d_XposYnegZpos);
    myView->FitAll();
}

void AndroidQt::reset()
{
    myView->Reset();
    myView->FitAll();
}
///////////////////////////////////////////////////////////////
void AndroidQt::rotation(bool p)
{
    if (p)
    {
        myCurrentAction3d = CurAction3d_DynamicRotation;
        myTouchPoint.ClearDev();
    }
    else
    {
        myCurrentAction3d = CurAction3d_Nothing;
        myTouchPoint.ClearDev();
    }
}

void AndroidQt::pan(bool p)
{
    if (p)
    {
        myCurrentAction3d = CurAction3d_DynamicPanning;
        myTouchPoint.ClearDev();
    }
    else
    {
        myCurrentAction3d = CurAction3d_Nothing;
        myTouchPoint.ClearDev();
    }
}

void AndroidQt::zoom(const int angleDelta, bool b)
{
    myMutex.lock();
    myAngleDelta = angleDelta;
    allowZoom = b;
    myMutex.unlock();
    if (window())
        window()->update();
}

//draw
void AndroidQt::drawLine(bool b)
{
    myEditState = b ? DrawLine : DrawNothing;
    clickedfirst = false;
}

void AndroidQt::drawPolyLine(bool b)
{
    myEditState = b ? DrawPolyLine : DrawNothing;
    clickedfirst = false;
}

void AndroidQt::drawArc(bool b)
{
    myEditState = b ? DrawArc : DrawNothing;
    clickedfirst = false;
}

void AndroidQt::drawCircle(bool b)
{
    myEditState = b ? DrawCircle : DrawNothing;
    clickedfirst = false;
}

void AndroidQt::drawRectangle(bool b)
{
    myEditState = b ? DrawRectangle : DrawNothing;
    clickedfirst = false;
}

void AndroidQt::confirmPolyLine()
{
    QString s("polyline w1 ");
    if(aPolyLineCollection.Length()>1)
    {
        aPolyLineCollection.Remove(aPolyLineCollection.Length());
        for (NCollection_Sequence<gp_Pnt>::Iterator aPntIt(aPolyLineCollection); aPntIt.More(); aPntIt.Next())
        {
            QString sx = QString::number(aPntIt.Value().X());
            QString sy = QString::number(aPntIt.Value().Y());
            QString sz = QString::number(aPntIt.Value().Z());
            s.append(sx + " " + sy + " " + sz + " ");
        }
        std::string str = s.toStdString();
        const char *ch = str.c_str();
        Standard_CString cmd = ch;
        aDI.Eval(cmd);
        aDI.Eval("NewCommand D");
        aDI.Eval("XAddShape D w1");
        aDI.Eval("XShow D");
        aDI.Eval("NewCommand D");
    }
    
   
    
    //drawLine 

    if(!tmpAISShape.IsNull())
    {
        myContext->Remove(tmpAISShape, Standard_False);
        tmpAISShape.Nullify();
    }
    clickedfirst = false;
    //drawPolyLine
    clickedfirstPL = false;
    if (aPolyLineCollection.Length() > 1)
    {
        aPolyLineCollection.Clear();
        myContext->Remove(myObject3d.First(), Standard_False);
    }

}

void AndroidQt::hlr(bool switching)
{
    myMutex.lock();
    myView->SetComputedMode(switching);
    myMutex.unlock();
}

void AndroidQt::shading()
{
    myContext->SetDisplayMode(AIS_DisplayMode::AIS_Shaded, Standard_True);
}

void AndroidQt::wireframe()
{
    myContext->SetDisplayMode(AIS_DisplayMode::AIS_WireFrame, Standard_True);
}

// =======================================================================
// function : handleWindowChanged
// purpose  :
// =======================================================================
void AndroidQt::handleWindowChanged(QQuickWindow *theWin)
{
    if (theWin == NULL)
    {
        return;
    }
    connect(theWin, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);

    theWin->setClearBeforeRendering(false);
}

// =======================================================================
// function : sync
// purpose  :
// =======================================================================
void AndroidQt::sync()
{
    myViewportSize = window()->size() * window()->devicePixelRatio();

    Graphic3d_Vec2i aWinTopLeft(window()->x(), window()->y());
    Graphic3d_Vec2i aWinSize(myViewportSize.width(), myViewportSize.height());
    const bool isChangedLeft = (myWinTopLeft.x() != aWinTopLeft.x());
    const bool isChangedTop = (myWinTopLeft.y() != aWinTopLeft.y());
    myWinTopLeft = aWinTopLeft;

    if (myViewer.IsNull())
    {
        initViewer(Aspect_Drawable(window()->winId()));
        connect(window(), SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
    }
    else
    {
        Handle(OpenGl_GraphicDriver) aDriver = Handle(OpenGl_GraphicDriver)::DownCast(myViewer->Driver());
#ifdef __ANDROID__
        if (aDriver->getRawGlContext() != eglGetCurrentContext())
        {
            initViewer(Aspect_Drawable(window()->winId()));
        }
        else
#endif
        {
#ifdef __ANDROID__
            Handle(AndroidQt_Window) aWindow = Handle(AndroidQt_Window)::DownCast(myView->Window());
            aWindow->SetSize(myViewportSize.width(), myViewportSize.height());
            //myView->MustBeResized(); // can be used instead of SetWindow() when EGLsurface has not been changed

            EGLContext anEglContext = eglGetCurrentContext();
            myView->SetWindow(aWindow, (Aspect_RenderingContext)anEglContext);
#else
            if (aWinSize.x() != myWinSize.x() || aWinSize.y() != myWinSize.y())
            {
                myView->MustBeResized();
                myView->Invalidate();
            }
            else if (isChangedTop)
            {
                myView->MustBeResized();
            }
            else if (isChangedLeft)
            {
                myView->MustBeResized();
            }
#endif
        }
    }
    myWinSize = aWinSize;
}

// =======================================================================
// function : paint
// purpose  :
// =======================================================================
void AndroidQt::paint()
{
    myMutex.lock();
    ////Zoom
    if (mRedo)
    {
        aDI.Eval("Redo D");
        mRedo = False;
    }
    if (mUndo)
    {
        aDI.Eval("Undo D");
        mUndo = false;
    }
    if (allowZoom)
    {
        myView->StartZoomAtPoint((Standard_Integer)myTouchPoint.X().second, (Standard_Integer)myTouchPoint.Y().second);
        myView->ZoomAtPoint(0, 0, myAngleDelta, 0);
        myAngleDelta = 0;
        allowZoom = false;
    }

    if (mouseMoved)
    {
        myContext->MoveTo((Standard_Integer)myTouchPoint.X().second, (Standard_Integer)myTouchPoint.Y().second, myView, Standard_False);
        mouseMoved = false;
    }
    
    // int i =0;
    for(myContext->InitDetected();myContext->MoreDetected();myContext->NextDetected())
    {
        // i+=1;//交点
        // qDebug()<<i;
    }
    //setlectRectangle
    if (myEditState == DrawNothing && myPressedMouseButton == Pressed_LeftButton && (Abs(myTouchPoint.DevX()) > 50 || Abs(myTouchPoint.DevY()) > 50))
    {
        myContext->SelectRectangle(Graphic3d_Vec2i((Standard_Integer)myTouchPoint.X().first, (Standard_Integer)myTouchPoint.Y().first),
                                   Graphic3d_Vec2i((Standard_Integer)myTouchPoint.X().second, (Standard_Integer)myTouchPoint.Y().second), myView);
    }
    //qDebug() << myViewProjPoint.getSecondgp_Pnt().X() << "~~~" << myViewProjPoint.getSecondgp_Pnt().Y()<<"~~~"<<myViewProjPoint.getSecondgp_Pnt().Z();
    //drawLine
    if (myEditState == DrawLine)
    {

        if (clickedfirst)
        {
            myContext->Remove(tmpAISShape, Standard_False);
            if (myViewProjPoint.getFirstgp_Pnt().Distance(myViewProjPoint.getSecondgp_Pnt()) > 0)
            {
                tmpEdge = BRepBuilderAPI_MakeEdge(myViewProjPoint.getFirstgp_Pnt(), myViewProjPoint.getSecondgp_Pnt());

                QString v2 = QString("vertex v2 %1 %2 %3").arg(myViewProjPoint.getSecondgp_Pnt().X()).arg(myViewProjPoint.getSecondgp_Pnt().Y()).arg(myViewProjPoint.getSecondgp_Pnt().Z());
                std::string str = v2.toStdString();
                const char *ch = str.c_str();
                Standard_CString cmd = ch;

                aDI.Eval(cmd);

                if (!tmpEdge.IsNull())
                {
                    tmpAISShape = new AIS_Shape(tmpEdge);
                    myContext->Display(tmpAISShape, Standard_False);
                }
            }
        }
        else
        {
            if (!tmpAISShape.IsNull())
            {
                myContext->Remove(tmpAISShape, Standard_False);
                tmpAISShape.Nullify();

                aDI.Eval("edge ee v1 v2");
                aDI.Eval("NewCommand D");
                aDI.Eval("XAddShape D ee");
                aDI.Eval("XShow D");
                aDI.Eval("NewCommand D");
            }
        }

    } //drawLine end

    //drawCircle
    if (myEditState == DrawCircle)
    {

        if (clickedfirst)
        {
            myContext->Remove(tmpAISShape, Standard_False);
            if (myViewProjPoint.getFirstgp_Pnt().Distance(myViewProjPoint.getSecondgp_Pnt()) > 0)
            {
                QString v2 = QString("circle c1 %1 %2 %3 %4 %5 %6 %7").arg(myViewProjPoint.getFirstgp_Pnt().X()).arg(myViewProjPoint.getFirstgp_Pnt().Y()).arg(myViewProjPoint.getFirstgp_Pnt().Z()).arg(myViewProjPoint.getSecondgp_Dir().X()).arg(myViewProjPoint.getSecondgp_Dir().Y()).arg(myViewProjPoint.getSecondgp_Dir().Z()).arg(myViewProjPoint.getFirstgp_Pnt().Distance(myViewProjPoint.getSecondgp_Pnt()));
                std::string str = v2.toStdString();
                const char *ch = str.c_str();
                Standard_CString cmd = ch;
                aDI.Eval(cmd);

                gp_Circ aProfileCircle(anAxis, myViewProjPoint.getFirstgp_Pnt().Distance(myViewProjPoint.getSecondgp_Pnt()));
                tmpEdge = BRepBuilderAPI_MakeEdge(aProfileCircle);
                if (!tmpEdge.IsNull())
                {
                    tmpAISShape = new AIS_Shape(tmpEdge);
                    myContext->Display(tmpAISShape, Standard_False);
                }
            }
        }
        else
        {
            if (!tmpAISShape.IsNull())
            {
                myContext->Remove(tmpAISShape, Standard_False);
                tmpAISShape.Nullify();
                aDI.Eval("mkedge ee c1");
                aDI.Eval("NewCommand D");
                aDI.Eval("XAddShape D ee");
                aDI.Eval("XShow D");
                aDI.Eval("NewCommand D");
            }
        }

    } //drawCircle end

    //drawPolyLine
    if (myEditState == DrawPolyLine)
    {
        if (clickedfirstPL)
        {
            //myContext->Remove(tmpAISShape,Standard_False);
            if (myViewProjPoint.getFirstgp_Pnt().Distance(myViewProjPoint.getSecondgp_Pnt()) > 0)
            {
                BRepBuilderAPI_MakePolygon aTria;
                if (aPolyLineCollection.Length() < 2)
                {
                    aPolyLineCollection.Append(myViewProjPoint.getSecondgp_Pnt());
                }
                aPolyLineCollection.ChangeLast() = myViewProjPoint.getSecondgp_Pnt();
                for (NCollection_Sequence<gp_Pnt>::Iterator aPntIt(aPolyLineCollection); aPntIt.More(); aPntIt.Next())
                {
                    aTria.Add(aPntIt.Value());
                }
                if (aTria.IsDone())
                {
                    tmpAISShape = new AIS_Shape(aTria.Shape());
                    if (myObject3d.Length() > 1)
                    {
                        myContext->Remove(myObject3d.First(), Standard_False);
                        myObject3d.ChangeFirst() = myObject3d.Last();
                        myObject3d.ChangeLast() = tmpAISShape;
                    }
                    else
                    {
                        myObject3d.Append(tmpAISShape);
                    }
                    myContext->Display(tmpAISShape, 0, -1, Standard_False);
                }
            }
        }
        else
        {
        }
    } //drawPolyLine end

    //drawRectangle
    if (myEditState == DrawRectangle)
    {
        gp_Pnt firstgp;
        gp_Pnt secondgp;
        gp_Pnt thirdgp;
        gp_Pnt furthgp;
        static QString sCmd("");
        if (clickedfirst)
        {
            myContext->Remove(tmpAISShape, Standard_False);
            Standard_Real x1 = myTouchPoint.X().first;
            Standard_Real x2 = myTouchPoint.X().second;
            Standard_Real y1 = myTouchPoint.Y().first;
            Standard_Real y2 = myTouchPoint.Y().second;
            Standard_Real xMax = x1 > x2 ? x1 : x2;
            Standard_Real yMax = y1 > y2 ? y1 : y2;
            Standard_Real xMin = x1 < x2 ? x1 : x2;
            Standard_Real yMin = y1 < y2 ? y1 : y2;
            gp_Pnt firstgp = mouse2ProjIntersector(xMin, yMax);
            gp_Pnt secondgp = mouse2ProjIntersector(xMax, yMax);
            gp_Pnt thirdgp = mouse2ProjIntersector(xMax, yMin);
            gp_Pnt furthgp = mouse2ProjIntersector(xMin, yMin);
            if (myViewProjPoint.getFirstgp_Pnt().Distance(myViewProjPoint.getSecondgp_Pnt()) > 0)
            {
                BRepBuilderAPI_MakePolygon aTria;
                //aTria.Add(gp_Pnt())
                //左上   x小y大
                //右上   x大y大
                //右下   x大y小
                //左下   x小y小
                //qDebug()<< myTouchPoint.X()<<"***"<<myTouchPoint.Y()<<"***"<<myTouchPoint.GetEndx()<<"***"<<myTouchPoint.GetEndy();
                aTria.Add(firstgp);
                aTria.Add(secondgp);
                aTria.Add(thirdgp);
                aTria.Add(furthgp);
                aTria.Add(firstgp);
                sCmd.clear();
                sCmd.append("polyline w2 ");
                sCmd.append(QString::number(firstgp.X()) + " " + QString::number(firstgp.Y()) + " " + QString::number(firstgp.Z()) + " ");
                sCmd.append(QString::number(secondgp.X()) + " " + QString::number(secondgp.Y()) + " " + QString::number(secondgp.Z()) + " ");
                sCmd.append(QString::number(thirdgp.X()) + " " + QString::number(thirdgp.Y()) + " " + QString::number(thirdgp.Z()) + " ");
                sCmd.append(QString::number(furthgp.X()) + " " + QString::number(furthgp.Y()) + " " + QString::number(furthgp.Z()) + " ");
                sCmd.append(QString::number(firstgp.X()) + " " + QString::number(firstgp.Y()) + " " + QString::number(firstgp.Z()) + " ");
                if (aTria.IsDone())
                {
                    tmpAISShape = new AIS_Shape(aTria.Shape());
                    myContext->Display(tmpAISShape, Standard_False);
                }
            }
        }
        else
        {
            if (!tmpAISShape.IsNull())
            {
                myContext->Remove(tmpAISShape, Standard_False);
                tmpAISShape.Nullify();

                std::string ss = sCmd.toStdString();
                Standard_CString cmd = ss.c_str();
               // qDebug() << cmd;
                aDI.Eval(cmd);
                aDI.Eval("NewCommand D");
                aDI.Eval("XAddShape D w2");
                aDI.Eval("XShow D");
                aDI.Eval("NewCommand D");
            }
        }

    } //drawRectangle end

    //qDebug() << (Standard_Integer)myTouchPoint.X().first << "---" << (Standard_Integer)myTouchPoint.Y().first << "---" << (Standard_Integer)myTouchPoint.X().second << "---" << (Standard_Integer)myTouchPoint.Y().second << "---" << myContext->NbSelected() << "---" << myContext->MoveTo((Standard_Integer)myTouchPoint.X().second, (Standard_Integer)myTouchPoint.Y().second, myView, Standard_False);
    if(mSetColorAIS)
    {
        Handle(Prs3d_Drawer) hilightcolor=new Prs3d_Drawer();
        Quantity_Color color;
        Quantity_Color::ColorFromHex(myColor.toStdString().c_str(),color);
        hilightcolor->SetColor(color);
        for(int i=0;i<myColorAIS.Size();i++)
        {
            myContext->SetColor(myColorAIS.Value(i),color,Standard_False);
            myContext->HilightWithColor(myColorAIS.Value(i),hilightcolor,Standard_True);
        }
    }
    if(mSetColor)
    {
        if(!myEntrys.IsEmpty())
        {
           aDI.Eval("NewCommand D");
            for (NCollection_List<TCollection_AsciiString>::Iterator anEntry (myEntrys); anEntry.More(); anEntry.Next())
            {
                QString v1("XSetColor D ");
                v1.append(anEntry.Value().ToCString());
                v1.append(" ");
                v1.append(myColor);
                aDI.Eval(v1.toStdString().c_str());
                QString cmdSetColor("AISDisplay D ");
                cmdSetColor.append(anEntry.Value().ToCString());
                aDI.Eval(cmdSetColor.toStdString().c_str());
                
            }
            aDI.Eval("NewCommand D");
            
        }
        
        //aDI.Eval(cmdSetColor->toStdString().c_str());
        mSetColor=false;
    }

    // if(myPressedMouseButton==PressedMouseButton::Pressed_LeftButton)
    // {
    if (aManipulator->HasActiveMode())
    {
        aManipulator->Transform (myTouchPoint.GetEndx(), myTouchPoint.GetEndy(), myView);
        myView->Redraw();
    }
    //qDebug()<<aManipulator->HasTransformation();
   // }

    switch (myCurrentAction3d)
    {
    case CurAction3d_Nothing:
        break;
    case CurAction3d_DynamicPanning:
    {
        if (Abs(myTouchPoint.DevX()) + Abs(myTouchPoint.DevY()) > 1)
        {
            myView->Pan((Standard_Integer)myTouchPoint.X().second - (Standard_Integer)myTouchPoint.X().first, (Standard_Integer)myTouchPoint.Y().first - (Standard_Integer)myTouchPoint.Y().second);
            myTouchPoint.ClearDev();
        }
        break;
    }
    case CurAction3d_DynamicRotation:
    {
        if (Abs(myTouchPoint.DevX()) + Abs(myTouchPoint.DevY()) > 1)
        {
            myView->StartRotation((Standard_Integer)myTouchPoint.X().first, (Standard_Integer)myTouchPoint.Y().first);
            myView->Rotation((Standard_Integer)myTouchPoint.X().second, (Standard_Integer)myTouchPoint.Y().second);
            myTouchPoint.ClearDev();
        }
        break;
    }
    case CurAction3d_WindowZooming:
        break;
    default:
        break;
    }

    myMutex.unlock();
    myView->Redraw();
}

// =======================================================================
// function : initViewer
// purpose  :
// =======================================================================
bool AndroidQt::initViewer(Aspect_Drawable theWin)
{

#ifdef _WIN32
    int aWidth = 0, aHeight = 0;
    Handle(Aspect_DisplayConnection) aDisplayConnection;
#ifdef __ANDROID__
    EGLint aCfgId = 0;
    EGLDisplay anEglDisplay = eglGetCurrentDisplay();
    EGLContext anEglContext = eglGetCurrentContext();
    EGLSurface anEglSurf = eglGetCurrentSurface(EGL_DRAW);

    if (anEglDisplay == EGL_NO_DISPLAY || anEglContext == EGL_NO_CONTEXT || anEglSurf == EGL_NO_SURFACE)
    {
        release();
        return false;
    }

    eglQuerySurface(anEglDisplay, anEglSurf, EGL_WIDTH, &aWidth);
    eglQuerySurface(anEglDisplay, anEglSurf, EGL_HEIGHT, &aHeight);
    eglQuerySurface(anEglDisplay, anEglSurf, EGL_CONFIG_ID, &aCfgId);

    const EGLint aConfigAttribs[] = {EGL_CONFIG_ID, aCfgId, EGL_NONE};
    EGLint aNbConfigs = 0;
    void *anEglConfig = NULL;

    if (eglChooseConfig(anEglDisplay, aConfigAttribs, &anEglConfig, 1, &aNbConfigs) != EGL_TRUE)
    {
        Message::DefaultMessenger()->Send("Error: EGL does not provide compatible configurations", Message_Fail);
        release();
        return false;
    }

    if (!myViewer.IsNull())
    {
        Handle(OpenGl_GraphicDriver) aDriver = Handle(OpenGl_GraphicDriver)::DownCast(myViewer->Driver());
        Handle(AndroidQt_Window) aWindow = Handle(AndroidQt_Window)::DownCast(myView->Window());
        if (!aDriver->InitEglContext(anEglDisplay, anEglContext, anEglConfig))
        {
            Message::DefaultMessenger()->Send("Error: OpenGl_GraphicDriver can not be initialized", Message_Fail);
            release();
            return false;
        }

        aWindow->SetSize(aWidth, aHeight);
        myView->SetWindow(aWindow, (Aspect_RenderingContext)anEglContext);
    }

    Handle(OpenGl_GraphicDriver) aDriver = new OpenGl_GraphicDriver(NULL, Standard_False);
#elif defined(_WIN32)
    HWND aWinHandle = (HWND)theWin;
    HDC aWindowDC = wglGetCurrentDC();
    HGLRC aRendCtx = wglGetCurrentContext();
    if (aWinHandle == NULL || aWindowDC == NULL || aRendCtx == NULL)
    {
        Message::DefaultMessenger()->Send("Error: No active WGL context!", Message_Fail);
        release();
        return false;
    }

    RECT aRect;
    ::GetClientRect(aWinHandle, &aRect);
    aWidth = aRect.right - aRect.left;
    aHeight = aRect.bottom - aRect.top;
    myWinSize.x() = aWidth;
    myWinSize.y() = aHeight;
    if (!myViewer.IsNull())
    {
        Handle(WNT_Window) aWindow = new WNT_Window(aWinHandle);
        myView->SetWindow(aWindow, (Aspect_RenderingContext)aRendCtx);
        return true;
    }
    Handle(OpenGl_GraphicDriver) aDriver = new OpenGl_GraphicDriver(aDisplayConnection, Standard_False);
#endif

    aDriver->ChangeOptions().buffersNoSwap = Standard_True;
    //aDriver->ChangeOptions().glslWarnings  = Standard_True; // for GLSL shaders debug

#ifdef __ANDROID__
    if (!aDriver->InitEglContext(anEglDisplay, anEglContext, anEglConfig))
    {
        Message::DefaultMessenger()->Send("Error: OpenGl_GraphicDriver can not be initialized", Message_Fail);
        release();
        return false;
    }
#endif

    // create viewer
    myViewer = new V3d_Viewer(aDriver);
    myViewer->SetDefaultBackgroundColor(AndroidQt_UserInteractionParameters::BgColor.Name());
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    // create AIS context
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->SetDisplayMode(AIS_Shaded, false);

#ifdef __ANDROID__
    Handle(AndroidQt_Window) aWindow = new AndroidQt_Window(aWidth, aHeight);
#elif defined(_WIN32)
    Handle(WNT_Window) aWindow = new WNT_Window(aWinHandle);
#endif

    myView = myViewer->CreateView();
    myView->SetImmediateUpdate(Standard_False);

#ifdef __ANDROID__
    myView->SetWindow(aWindow, (Aspect_RenderingContext)anEglContext);
#else
    myView->SetWindow(aWindow, (Aspect_RenderingContext)aRendCtx);
#endif
    myView->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);

    return true;
#else
    Handle(Aspect_DisplayConnection) aDisp = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) aDriver = new OpenGl_GraphicDriver(aDisp, false);
    aDriver->ChangeOptions().buffersNoSwap = true;
    aDriver->ChangeOptions().sRGBDisable = true;
    aDriver->ChangeOptions().useSystemBuffer = false;
    // create viewer
    myViewer = new V3d_Viewer(aDriver);
    myViewer->SetDefaultBackgroundColor(Quantity_NOC_BLACK);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
    //myViewer->SetRectangularGridValues(0,0,1,1,0);
    myViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
    
    // create AIS context
    myContext = new AIS_InteractiveContext(myViewer);
    myContext->SetPixelTolerance(20);
    myView = myViewer->CreateView();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);
    myView->SetImmediateUpdate(false);

    // myView->ChangeRenderingParams().ToShowStats = true;
    // myView->ChangeRenderingParams().CollectedStats = Graphic3d_RenderingParams::PerfCounters_All;

    Handle(Xw_Window) aWindow = new Xw_Window(aDisp, this->window()->winId());

    Handle(OpenGl_Context) aGlCtx = new OpenGl_Context();
    bool isCoreProfile = true;
    if (!aGlCtx->Init(isCoreProfile))
    {
        return false;
    }
    myView->SetWindow(aWindow, aGlCtx->RenderingContext());
    // myOcaf_app=new NOcaf_Application;
    // Handle(TDocStd_Document) myOcafDoc;
    // myOcaf_app->NewDocument("BinOcaf", myOcafDoc);
    // BinDrivers::DefineFormat(myOcaf_app);
    // XmlDrivers::DefineFormat(myOcaf_app);

    ViewerTest::SetAISContext(myContext);
    ViewerTest::CurrentView(myView);
    Standard_CString docName = "D";
    aDI.Eval("pload ALL");
    aDI.Eval("XNewDoc D");
    //aDI.Eval("Open s.x.xbf D");
    aDI.Eval("UndoLimit D 100");

    Handle(TDocStd_Document) aDoc;
    if (!DDocStd::GetDocument(docName, aDoc))
    {
        return 1;
    }
    TDF_Label aRoot = aDoc->GetData()->Root();
    TPrsStd_AISViewer::New(aRoot, ViewerTest::GetAISContext());

    //aDI.Remove("vinit");
    // aDI.Eval("box b 10 10 10");
    // aDI.Eval("NewCommand D");
    // aDI.Eval("XAddShape D b");
    // aDI.Eval("NewCommand D");
    // aDI.Eval("XShow D");

    // aDI.Eval("box b 10 10 10 10 10 10");
    // aDI.Eval("NewCommand D");
    // aDI.Eval("XAddShape D b");
    // aDI.Eval("NewCommand D");
    // aDI.Eval("XShow D");
    // aDI.Eval("NewCommand D");

    //aDI.EvalFile("/home/cqwuxiaolong/OCCT/samples/tcl/xde.tcl");
    //qDebug() << aDI.Result();
    //this->ReadShapeFromFile("/home/cqwuxiaolong/OCCT/data/occ/CrankArm.brep");
    makeBox();
    // makeCone();
    // makeSphere();
    // makeCylinder();
    // makeTorus();
    // makeFillet();
    // makeChamfer();
    // makeExtrude();
    // makeRevol();
    // makeLoft();
    // testCut();
    // testFuse();
    // testCommon();
    // makeToroidalHelix();
    // makeConicalHelix();
    // makeCylindricalHelix();
    myView->FitAll();
    return true;
#endif
}

void AndroidQt::SetSelectionMode(const Standard_Integer theMode)
{
    myContext->Activate(theMode);
}

// =======================================================================
// function : release
// purpose  :
// =======================================================================
void AndroidQt::release()
{
    myContext.Nullify();
    myView.Nullify();
    myViewer.Nullify();
}

gp_Pnt AndroidQt::mouse2ProjIntersector(Standard_Integer theX, Standard_Integer theY)
{
    Standard_Real xv, yv, zv;
    myView->Proj(xv, yv, zv);
    Standard_Real xat, yat, zat;
    myView->At(xat, yat, zat);
    gp_Pln aPlane(gp_Pnt(xat, yat, zat), gp_Dir(xv, yv, zv));

    Standard_Integer aPixX, aPixY;
    Standard_Real aX, aY, aZ, aDX, aDY, aDZ;
    myView->ConvertWithProj(theX, theY, aX, aY, aZ, aDX, aDY, aDZ);
    gp_Lin aLine(gp_Pnt(aX, aY, aZ), gp_Dir(aDX, aDY, aDZ));

    // Compute intersection
    Handle(Geom_Line) aGeomLine = new Geom_Line(aLine);
    Handle(Geom_Plane) aGeomPlane = new Geom_Plane(aPlane);
    GeomAPI_IntCS anIntersector(aGeomLine, aGeomPlane);
    if (!anIntersector.IsDone() || anIntersector.NbPoints() == 0)
    {
        return gp::Origin();
    }
    return anIntersector.Point(1);
}
#include<QElapsedTimer>
void AndroidQt::makeBox()
{
    // TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(random(), random(), random()).Shape();
    // //Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    // QElapsedTimer time;  
    // time.start();
    // NCollection_Vector<Handle(AIS_InteractiveObject)> tmp;
    // for(int i=0;i<100000;i++)
    // {
    //     TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(random(), random(), random()).Shape();
    //     Handle(AIS_Shape) aT=new AIS_Shape(aTopoBox);
    //     tmp.Append(aT);
    //     aTopoBox.Free();
    // }
    // qDebug()<<time.elapsed()<<"create ms";  
    // time.restart();
    // for (NCollection_Vector<Handle(AIS_InteractiveObject)>::Iterator aPntIt(tmp); aPntIt.More(); aPntIt.Next())
    // {
    //     myContext->Display(aPntIt.Value(), Standard_False);
    // }
    // qDebug()<<time.elapsed()<<"show ms";  
    // qDebug()<<tmp.Size();  
    // anAisBox->SetDisplayMode(1);
    //anAisBox->SetColor(Quantity_NOC_AZURE);
    //myContext->Display(anAisBox, Standard_False);
}

void AndroidQt::makeCone()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) anAisReducer = new AIS_Shape(aTopoReducer);

    anAisReducer->SetColor(Quantity_NOC_BISQUE);

    anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCone = new AIS_Shape(aTopoCone);

    anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

    myContext->Display(anAisReducer, Standard_True);
    myContext->Display(anAisCone, Standard_True);
}

void AndroidQt::makeSphere()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 20.0, 0.0));

    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 3.0).Shape();
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);

    anAisSphere->SetColor(Quantity_NOC_BLUE1);

    myContext->Display(anAisSphere, Standard_True);
}

void AndroidQt::makeCylinder()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 30.0, 0.0));

    TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCylinder = new AIS_Shape(aTopoCylinder);

    anAisCylinder->SetColor(Quantity_NOC_RED);

    anAxis.SetLocation(gp_Pnt(8.0, 30.0, 0.0));
    TopoDS_Shape aTopoPie = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0, M_PI_2 * 3.0).Shape();
    Handle(AIS_Shape) anAisPie = new AIS_Shape(aTopoPie);

    anAisPie->SetColor(Quantity_NOC_TAN);

    myContext->Display(anAisCylinder, Standard_True);
    myContext->Display(anAisPie, Standard_True);
}

void AndroidQt::makeTorus()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 40.0, 0.0));

    TopoDS_Shape aTopoTorus = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0).Shape();
    Handle(AIS_Shape) anAisTorus = new AIS_Shape(aTopoTorus);

    anAisTorus->SetColor(Quantity_NOC_YELLOW);

    anAxis.SetLocation(gp_Pnt(8.0, 40.0, 0.0));
    TopoDS_Shape aTopoElbow = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0, M_PI_2).Shape();
    Handle(AIS_Shape) anAisElbow = new AIS_Shape(aTopoElbow);

    anAisElbow->SetColor(Quantity_NOC_THISTLE);

    myContext->Display(anAisTorus, Standard_True);
    myContext->Display(anAisElbow, Standard_True);
}

void AndroidQt::makeFillet()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeFillet MF(aTopoBox);

    // Add all the edges to fillet.
    for (TopExp_Explorer ex(aTopoBox, TopAbs_EDGE); ex.More(); ex.Next())
    {
        MF.Add(1.0, TopoDS::Edge(ex.Current()));
    }

    Handle(AIS_Shape) anAisShape = new AIS_Shape(MF.Shape());
    anAisShape->SetColor(Quantity_NOC_VIOLET);

    myContext->Display(anAisShape, Standard_True);
}

void AndroidQt::makeChamfer()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(8.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeChamfer MC(aTopoBox);
    TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;

    TopExp::MapShapesAndAncestors(aTopoBox, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);

    for (Standard_Integer i = 1; i <= aEdgeFaceMap.Extent(); ++i)
    {
        TopoDS_Edge anEdge = TopoDS::Edge(aEdgeFaceMap.FindKey(i));
        TopoDS_Face aFace = TopoDS::Face(aEdgeFaceMap.FindFromIndex(i).First());

        MC.Add(0.6, 0.6, anEdge, aFace);
    }

    Handle(AIS_Shape) anAisShape = new AIS_Shape(MC.Shape());
    anAisShape->SetColor(Quantity_NOC_TOMATO);

    myContext->Display(anAisShape, Standard_True);
}

void AndroidQt::makeExtrude()
{
    // prism a vertex result is an edge.
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(0.0, 60.0, 0.0));
    TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismVertex = new AIS_Shape(aPrismVertex);

    // prism an edge result is a face.
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(5.0, 60.0, 0.0), gp_Pnt(10.0, 60.0, 0.0));
    TopoDS_Shape aPrismEdge = BRepPrimAPI_MakePrism(anEdge, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismEdge = new AIS_Shape(aPrismEdge);

    // prism a wire result is a shell.
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(16.0, 60.0, 0.0));

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(anAxis, 3.0));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aPrismCircle = BRepPrimAPI_MakePrism(aCircleWire, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismCircle = new AIS_Shape(aPrismCircle);

    // prism a face or a shell result is a solid.
    anAxis.SetLocation(gp_Pnt(24.0, 60.0, 0.0));
    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(anAxis, 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aPrismEllipse = BRepPrimAPI_MakePrism(aEllipseFace, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismEllipse = new AIS_Shape(aPrismEllipse);

    anAisPrismVertex->SetColor(Quantity_NOC_PAPAYAWHIP);
    anAisPrismEdge->SetColor(Quantity_NOC_PEACHPUFF);
    anAisPrismCircle->SetColor(Quantity_NOC_PERU);
    anAisPrismEllipse->SetColor(Quantity_NOC_PINK);

    myContext->Display(anAisPrismVertex, Standard_True);
    myContext->Display(anAisPrismEdge, Standard_True);
    myContext->Display(anAisPrismCircle, Standard_True);
    myContext->Display(anAisPrismEllipse, Standard_True);
}

void AndroidQt::makeRevol()
{
    gp_Ax1 anAxis;

    // revol a vertex result is an edge.
    anAxis.SetLocation(gp_Pnt(0.0, 70.0, 0.0));
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(2.0, 70.0, 0.0));
    TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis);
    Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(aRevolVertex);

    // revol an edge result is a face.
    anAxis.SetLocation(gp_Pnt(8.0, 70.0, 0.0));
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(6.0, 70.0, 0.0), gp_Pnt(6.0, 70.0, 5.0));
    TopoDS_Shape aRevolEdge = BRepPrimAPI_MakeRevol(anEdge, anAxis);
    Handle(AIS_Shape) anAisRevolEdge = new AIS_Shape(aRevolEdge);

    // revol a wire result is a shell.
    anAxis.SetLocation(gp_Pnt(20.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(15.0, 70.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aRevolCircle = BRepPrimAPI_MakeRevol(aCircleWire, anAxis, M_PI_2);
    Handle(AIS_Shape) anAisRevolCircle = new AIS_Shape(aRevolCircle);

    // revol a face result is a solid.
    anAxis.SetLocation(gp_Pnt(30.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(gp_Ax2(gp_Pnt(25.0, 70.0, 0.0), gp::DZ()), 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aRevolEllipse = BRepPrimAPI_MakeRevol(aEllipseFace, anAxis, M_PI_4);
    Handle(AIS_Shape) anAisRevolEllipse = new AIS_Shape(aRevolEllipse);

    anAisRevolVertex->SetColor(Quantity_NOC_LIMEGREEN);
    anAisRevolEdge->SetColor(Quantity_NOC_LINEN);
    anAisRevolCircle->SetColor(Quantity_NOC_MAGENTA1);
    anAisRevolEllipse->SetColor(Quantity_NOC_MAROON);

    myContext->Display(anAisRevolVertex, Standard_True);
    myContext->Display(anAisRevolEdge, Standard_True);
    myContext->Display(anAisRevolCircle, Standard_True);
    myContext->Display(anAisRevolEllipse, Standard_True);
}

void AndroidQt::makeLoft()
{
    // bottom wire.
    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(0.0, 80.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);

    // top wire.
    BRepBuilderAPI_MakePolygon aPolygon;
    aPolygon.Add(gp_Pnt(-3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 83.0, 6.0));
    aPolygon.Add(gp_Pnt(-3.0, 83.0, 6.0));
    aPolygon.Close();

    BRepOffsetAPI_ThruSections aShellGenerator;
    BRepOffsetAPI_ThruSections aSolidGenerator(true);

    aShellGenerator.AddWire(aCircleWire);
    aShellGenerator.AddWire(aPolygon.Wire());

    aSolidGenerator.AddWire(aCircleWire);
    aSolidGenerator.AddWire(aPolygon.Wire());

    // translate the solid.
    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aSolidGenerator.Shape(), aTrsf);

    Handle(AIS_Shape) anAisShell = new AIS_Shape(aShellGenerator.Shape());
    Handle(AIS_Shape) anAisSolid = new AIS_Shape(aTransform.Shape());

    anAisShell->SetColor(Quantity_NOC_OLIVEDRAB);
    anAisSolid->SetColor(Quantity_NOC_PEACHPUFF);

    myContext->Display(anAisShell, Standard_True);
    myContext->Display(anAisSolid, Standard_True);
}

void AndroidQt::testCut()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 90.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCuttedShape1 = BRepAlgoAPI_Cut(aTopoBox, aTopoSphere);
    TopoDS_Shape aCuttedShape2 = BRepAlgoAPI_Cut(aTopoSphere, aTopoBox);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform1(aCuttedShape1, aTrsf);

    aTrsf.SetTranslation(gp_Vec(16.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform2(aCuttedShape2, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisCuttedShape1 = new AIS_Shape(aTransform1.Shape());
    Handle(AIS_Shape) anAisCuttedShape2 = new AIS_Shape(aTransform2.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCuttedShape1->SetColor(Quantity_NOC_TAN);
    anAisCuttedShape2->SetColor(Quantity_NOC_SALMON);

    myContext->Display(anAisBox, Standard_True);
    myContext->Display(anAisSphere, Standard_True);
    myContext->Display(anAisCuttedShape1, Standard_True);
    myContext->Display(anAisCuttedShape2, Standard_True);
}

void AndroidQt::testFuse()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 100.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aFusedShape = BRepAlgoAPI_Fuse(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aFusedShape, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisFusedShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisFusedShape->SetColor(Quantity_NOC_ROSYBROWN);

    myContext->Display(anAisBox, Standard_True);
    myContext->Display(anAisSphere, Standard_True);
    myContext->Display(anAisFusedShape, Standard_True);
}

void AndroidQt::testCommon()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 110.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCommonShape = BRepAlgoAPI_Common(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aCommonShape, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisCommonShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCommonShape->SetColor(Quantity_NOC_ROYALBLUE);

    myContext->Display(anAisBox, Standard_True);
    myContext->Display(anAisSphere, Standard_True);
    myContext->Display(anAisCommonShape, Standard_True);
}

void AndroidQt::testHelix()
{
    makeCylindricalHelix();

    makeConicalHelix();

    makeToroidalHelix();
}

void AndroidQt::makeCylindricalHelix()
{
    Standard_Real aRadius = 3.0;
    Standard_Real aPitch = 1.0;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_CylindricalSurface) aCylinder = new Geom_CylindricalSurface(gp::XOY(), aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(0.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myContext->Display(anAisHelixCurve, Standard_True);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(8.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_CORAL);
        myContext->Display(anAisPipe, Standard_True);
    }
}

/**
 * make conical helix, it is the same as the cylindrical helix,
 * the only different is the surface.
 */
void AndroidQt::makeConicalHelix()
{
    Standard_Real aRadius = 3.0;
    Standard_Real aPitch = 1.0;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_ConicalSurface) aCylinder = new Geom_ConicalSurface(gp::XOY(), M_PI / 6.0, aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myContext->Display(anAisHelixCurve, Standard_True);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(28.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_DARKGOLDENROD);
        myContext->Display(anAisPipe, Standard_True);
    }
}

void AndroidQt::makeToroidalHelix()
{
    Standard_Real aRadius = 1.0;
    Standard_Real aSlope = 0.05;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aSlope, 1.0));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_ToroidalSurface) aCylinder = new Geom_ToroidalSurface(gp::XOY(), aRadius * 5.0, aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 2.0 * M_PI / aSlope).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(45.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myContext->Display(anAisHelixCurve, Standard_True);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 0.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius * 6.0, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(60.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_CORNSILK1);
        myContext->Display(anAisPipe, Standard_True);
    }
}
