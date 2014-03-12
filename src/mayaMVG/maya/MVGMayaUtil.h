#pragma once

#include <maya/MString.h>
#include <maya/MStatus.h>

class QWidget;
class MDagPath;
class M3dView;

namespace mayaMVG {

class MVGMenu;
class MVGCamera;

struct MVGMayaUtil {
	// window
	static MStatus createMVGWindow();
	static MStatus deleteMVGWindow();
	static QWidget* getMVGWindow();
	// window menu
	static QWidget* getMVGMenuLayout();
	// viewports
	static QWidget* getMVGLeftViewportLayout();
	static QWidget* getMVGRightViewportLayout();
	static MStatus setFocusOnLeftView();
	static MStatus setFocusOnRightView();
	static bool isMVGView(const M3dView & view);
	static bool isActiveView(const M3dView & view);
	static bool mouseUnderView(const M3dView & view);
	// context
	static MStatus createMVGContext();
	static MStatus deleteMVGContext();
	static MStatus activeContext(); 
	// cameras
	static MStatus getMVGLeftCamera(MDagPath& path);
	static MStatus setMVGLeftCamera(const MVGCamera& camera);
	static MStatus getMVGRightCamera(MDagPath& path);
	static MStatus setMVGRightCamera(const MVGCamera& camera);
	// maya selection
	static MStatus addToMayaSelection(MString objectName);
	static MStatus clearMayaSelection();
};

} // mayaMVG
