#include "mayaMVG/core/MVGGeometryUtil.hpp"
#include "mayaMVG/core/MVGPointCloud.hpp"
#include "mayaMVG/core/MVGProject.hpp"
#include "mayaMVG/core/MVGCamera.hpp"
#include "mayaMVG/core/MVGLog.hpp"
#include "mayaMVG/core/MVGPlaneKernel.hpp"
#include <maya/MPointArray.h>
#include <maya/M3dView.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>
#include <maya/MMatrix.h>

namespace mayaMVG
{

void MVGGeometryUtil::viewToCameraSpace(M3dView& view, const MPoint& viewPoint, MPoint& cameraPoint)
{
    double portHeight = (double)view.portHeight();
    double portWidth = (double)view.portWidth();
    MDagPath dagPath;
    view.getCamera(dagPath);
    MVGCamera camera(dagPath);
    // center
    cameraPoint.x = (viewPoint.x / portWidth) - 0.5;
    cameraPoint.y = (viewPoint.y / portWidth) - 0.5 - 0.5 * (portHeight / portWidth - 1.0);
    cameraPoint.z = 0.;
    // zoom
    cameraPoint = cameraPoint * camera.getHorizontalFilmAperture() * camera.getZoom();
    // pan
    cameraPoint.x += camera.getHorizontalPan();
    cameraPoint.y += camera.getVerticalPan();
}

MPoint MVGGeometryUtil::viewToCameraSpace(M3dView& view, const MPoint& viewPoint)
{
    MPoint point;
    viewToCameraSpace(view, viewPoint, point);
    return point;
}

void MVGGeometryUtil::viewToCameraSpace(M3dView& view, const MPointArray& viewPoints,
                                        MPointArray& cameraPoints)
{
    cameraPoints.setLength(viewPoints.length());
    for(size_t i = 0; i < viewPoints.length(); ++i)
        viewToCameraSpace(view, viewPoints[i], cameraPoints[i]);
}

MPointArray MVGGeometryUtil::viewToCameraSpace(M3dView& view, const MPointArray& viewPoints)
{
    MPointArray points;
    viewToCameraSpace(view, viewPoints, points);
    return points;
}

void MVGGeometryUtil::cameraToViewSpace(M3dView& view, const MPoint& cameraPoint, MPoint& viewPoint)
{
    MDagPath dagPath;
    view.getCamera(dagPath);
    MVGCamera camera(dagPath);
    float x = cameraPoint.x;
    float y = cameraPoint.y;
    // pan
    x -= camera.getHorizontalPan();
    y -= camera.getVerticalPan();
    // zoom
    x /= (camera.getHorizontalFilmAperture() * camera.getZoom());
    y /= (camera.getHorizontalFilmAperture() * camera.getZoom());
    // center
    viewPoint.x = round((x + 0.5) * view.portWidth());
    viewPoint.y = round((y + 0.5 + 0.5 * (view.portHeight() / (float)view.portWidth() - 1.0)) *
                        view.portWidth());
}

MPoint MVGGeometryUtil::cameraToViewSpace(M3dView& view, const MPoint& cameraPoint)
{
    MPoint point;
    cameraToViewSpace(view, cameraPoint, point);
    return point;
}

void MVGGeometryUtil::cameraToViewSpace(M3dView& view, const MPointArray& cameraPoints,
                                        MPointArray& viewPoints)
{
    viewPoints.setLength(cameraPoints.length());
    for(size_t i = 0; i < viewPoints.length(); ++i)
        cameraToViewSpace(view, cameraPoints[i], viewPoints[i]);
}

MPointArray MVGGeometryUtil::cameraToViewSpace(M3dView& view, const MPointArray& cameraPoints)
{
    MPointArray points;
    cameraToViewSpace(view, cameraPoints, points);
    return points;
}

void MVGGeometryUtil::worldToViewSpace(M3dView& view, const MPoint& worldPoint, MPoint& viewPoint)
{
    // don't use M3dView::worldToView() because of the cast to short values
    MStatus status;
    MMatrix modelViewMatrix, projectionMatrix;
    CHECK(view.modelViewMatrix(modelViewMatrix))
    CHECK(view.projectionMatrix(projectionMatrix))
    MPoint point = worldPoint * (modelViewMatrix * projectionMatrix);
    unsigned int viewportX, viewportY, viewportWidth, viewportHeight;
    view.viewport(viewportX, viewportY, viewportWidth, viewportHeight);
    viewPoint.x =
        static_cast<int>(static_cast<double>(viewportWidth) * (point.x / point.w + 1.0) / 2.0);
    viewPoint.y =
        static_cast<int>(static_cast<double>(viewportHeight) * (point.y / point.w + 1.0) / 2.0);
    viewPoint.z = 0.0;
}

MPoint MVGGeometryUtil::worldToViewSpace(M3dView& view, const MPoint& worldPoint)
{
    MPoint point;
    worldToViewSpace(view, worldPoint, point);
    return point;
}

void MVGGeometryUtil::worldToViewSpace(M3dView& view, const MPointArray& worldPoints,
                                       MPointArray& viewPoints)
{
    viewPoints.setLength(worldPoints.length());
    for(size_t i = 0; i < worldPoints.length(); ++i)
        worldToViewSpace(view, worldPoints[i], viewPoints[i]);
}

MPointArray MVGGeometryUtil::worldToViewSpace(M3dView& view, const MPointArray& worldPoints)
{
    MPointArray points;
    worldToViewSpace(view, worldPoints, points);
    return points;
}

void MVGGeometryUtil::viewToWorldSpace(M3dView& view, const MPoint& viewPoint, MPoint& worldPoint)
{
    MPoint worldDir;
    CHECK(view.viewToWorld(viewPoint.x, viewPoint.y, worldPoint, worldDir))
}

MPoint MVGGeometryUtil::viewToWorldSpace(M3dView& view, const MPoint& viewPoint)
{
    MPoint point;
    viewToWorldSpace(view, viewPoint, point);
    return point;
}

void MVGGeometryUtil::viewToWorldSpace(M3dView& view, const MPointArray& viewPoints,
                                       MPointArray& worldPoints)
{
    worldPoints.setLength(viewPoints.length());
    MPoint worldDir;
    for(size_t i = 0; i < viewPoints.length(); ++i)
        CHECK(view.viewToWorld(viewPoints[i].x, viewPoints[i].y, worldPoints[i], worldDir))
}

MPointArray MVGGeometryUtil::viewToWorldSpace(M3dView& view, const MPointArray& viewPoints)
{
    MPointArray points;
    viewToWorldSpace(view, viewPoints, points);
    return points;
}

void MVGGeometryUtil::worldToCameraSpace(M3dView& view, const MPoint& worldPoint,
                                         MPoint& cameraPoint)
{
    viewToCameraSpace(view, worldToViewSpace(view, worldPoint), cameraPoint);
}

MPoint MVGGeometryUtil::worldToCameraSpace(M3dView& view, const MPoint& worldPoint)
{
    MPoint point;
    viewToCameraSpace(view, worldToViewSpace(view, worldPoint), point);
    return point;
}

void MVGGeometryUtil::worldToCameraSpace(M3dView& view, const MPointArray& worldPoints,
                                         MPointArray& cameraPoints)
{
    cameraPoints.setLength(worldPoints.length());
    for(size_t i = 0; i < worldPoints.length(); ++i)
        viewToCameraSpace(view, worldToViewSpace(view, worldPoints[i]), cameraPoints[i]);
}

MPointArray MVGGeometryUtil::worldToCameraSpace(M3dView& view, const MPointArray& worldPoints)
{
    MPointArray points;
    viewToCameraSpace(view, worldToViewSpace(view, worldPoints), points);
    return points;
}

void MVGGeometryUtil::cameraToWorldSpace(M3dView& view, const MPoint& cameraPoint,
                                         MPoint& worldPoint)
{
    viewToWorldSpace(view, cameraToViewSpace(view, cameraPoint), worldPoint);
}

MPoint MVGGeometryUtil::cameraToWorldSpace(M3dView& view, const MPoint& cameraPoint)
{
    MPoint point;
    cameraToWorldSpace(view, cameraPoint, point);
    return point;
}

void MVGGeometryUtil::cameraToWorldSpace(M3dView& view, const MPointArray& cameraPoints,
                                         MPointArray& worldPoints)
{
    worldPoints.setLength(cameraPoints.length());
    for(size_t i = 0; i < worldPoints.length(); ++i)
        cameraToWorldSpace(view, cameraPoints[i], worldPoints[i]);
}

MPointArray MVGGeometryUtil::cameraToWorldSpace(M3dView& view, const MPointArray& cameraPoints)
{
    MPointArray points;
    cameraToWorldSpace(view, cameraPoints, points);
    return points;
}

void MVGGeometryUtil::cameraToImageSpace(MVGCamera& camera, const MPoint& cameraPoint,
                                         MPoint& imagePoint)
{
    MFnDagNode fnImage(camera.getImagePath());
    const double width = fnImage.findPlug("coverageX").asDouble();
    const double height = fnImage.findPlug("coverageY").asDouble();
    assert(camera.getHorizontalFilmAperture() != 0.0);
    MPoint pointCenteredNorm = cameraPoint / camera.getHorizontalFilmAperture();
    const double verticalMargin = (width - height) / 2.0;
    imagePoint.x = (pointCenteredNorm.x + 0.5) * width;
    imagePoint.y = (-pointCenteredNorm.y + 0.5) * width - verticalMargin;
}

MPoint MVGGeometryUtil::cameraToImageSpace(MVGCamera& camera, const MPoint& cameraPoint)
{
    MPoint point;
    cameraToImageSpace(camera, cameraPoint, point);
    return point;
}

void MVGGeometryUtil::cameraToImageSpace(MVGCamera& camera, const MPointArray& cameraPoints,
                                         MPointArray& imagePoints)
{
    imagePoints.setLength(cameraPoints.length());
    for(size_t i = 0; i < cameraPoints.length(); ++i)
        cameraToImageSpace(camera, cameraPoints[i], imagePoints[i]);
}

MPointArray MVGGeometryUtil::cameraToImageSpace(MVGCamera& camera, const MPointArray& cameraPoints)
{
    MPointArray points;
    cameraToImageSpace(camera, cameraPoints, points);
    return points;
}

bool MVGGeometryUtil::projectPointsOnPlane(M3dView& view, const MPointArray& toProjectCSPoints,
                                           const MPointArray& faceWSPoints,
                                           MPointArray& projectedWSPoints)
{
    if(faceWSPoints.length() < 3)
        return false;
    // compute plane
    openMVG::Mat facePointsMat(3, faceWSPoints.length());
    for(size_t i = 0; i < faceWSPoints.length(); ++i)
        facePointsMat.col(i) = TO_VEC3(faceWSPoints[i]);
    PlaneKernel kernel(facePointsMat);
    double outlierThreshold = std::numeric_limits<double>::infinity();
    PlaneKernel::Model model;
    openMVG::robust::LeastMedianOfSquares(kernel, &model, &outlierThreshold);
    // compute camera center
    MDagPath cameraPath;
    view.getCamera(cameraPath);
    MVGCamera camera(cameraPath);
    MPoint cameraCenter = TO_MPOINT(camera.getPinholeCamera()._C);
    MVGPointCloud cloud(MVGProject::_CLOUD);
    MMatrix inclusiveMatrix = MMatrix::identity;
    if(cloud.isValid())
        inclusiveMatrix = cloud.getDagPath().inclusiveMatrix();
    cameraCenter *= inclusiveMatrix;
    // project moved point on computed plane
    MPoint projectedWSPoint;
    for(size_t i = 0; i < toProjectCSPoints.length(); ++i)
    {
        plane_line_intersect(model, cameraCenter,
                             MVGGeometryUtil::cameraToWorldSpace(view, toProjectCSPoints[i]),
                             projectedWSPoint);
        projectedWSPoints.append(projectedWSPoint);
    }
    return true;
}

bool MVGGeometryUtil::projectPointOnPlane(M3dView& view, const MPoint& toProjectCSPoint,
                                          const MPointArray& faceWSPoints, MPoint& projectedWSPoint)
{
    MPointArray toProjectCSPoints;
    MPointArray projectedWSPoints;
    toProjectCSPoints.append(toProjectCSPoint);
    if(projectPointsOnPlane(view, toProjectCSPoints, faceWSPoints, projectedWSPoints))
    {
        assert(projectedWSPoints.length() == 1);
        projectedWSPoint = projectedWSPoints[0];
        return true;
    }
    return false;
}

} // namespace
