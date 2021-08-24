cmake_minimum_required(VERSION 3.0.0)
project(NQml VERSION 0.1.0 LANGUAGES CXX)

set(QML_IMPORT_PATH "${QML_DIRS}"
                    CACHE STRING "Qt Creator 4.1 extra qml import paths"
                    FORCE
                    )
set(Qt5_DIR "/home/cqwuxialong/Qt/5.15.2/gcc_64/lib/cmake/Qt5")

include_directories("./src"
                    "/home/cqwuxiaolong/OCCT/build/include/opencascade"
                    )
link_directories("/home/cqwuxiaolong/OCCT/build/lin64/gcc/lib/")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

FIND_PACKAGE(Qt5 COMPONENTS Core Quick Widgets Gui Svg Xml LinguistTools QuickControls2 WebEngine WebChannel REQUIRED)
file(GLOB NQml_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "./src/*.*")

add_executable(NQml ${NQml_SRC})
target_link_libraries(NQml TKernel TKMath TKG2d TKG3d TKGeomBase TKBRep TKGeomAlgo TKTopAlgo TKPrim TKBO TKShHealing TKBool TKHLR TKFillet TKOffset TKFeat TKMesh TKXMesh TKService TKV3d TKOpenGl TKMeshVS TKCDF TKLCAF TKCAF TKBinL TKXmlL TKBin TKXml TKStdL TKStd TKTObj TKBinTObj TKXmlTObj TKVCAF TKXSBase TKSTEPBase TKSTEPAttr TKSTEP209 TKSTEP TKIGES TKXCAF TKXDEIGES TKXDESTEP TKSTL TKVRML TKXmlXCAF TKBinXCAF TKRWMesh TKDraw TKTopTest TKOpenGlTest TKViewerTest TKXSDRAW TKDCAF TKXDEDRAW TKTObjDRAW TKQADraw
    TKViewerTest
    libtcl8.6.so
    )


target_link_libraries(NQml Qt5::Quick Qt5::Widgets Qt5::QuickControls2 Qt5::Svg Qt5::Xml Qt5::WebEngine Qt5::WebChannel)


add_executable(OcctColors ./OcctColors/OcctColors.cxx)
target_link_libraries(OcctColors TKernel)
target_link_libraries(OcctColors Qt5::Quick Qt5::Widgets)
# set_target_properties(application PROPERTIES
#     QT_QML_MODULE_VERSION 1.0
#     QT_QML_MODULE_URI Example
# )
# qt6_qml_type_registration(application)
