#pragma once

#include <QObject>
#include <QStringList>
#include "mayaMVG/core/MVGProject.h"
#include "mayaMVG/qt/QObjectListModel.h"

namespace mayaMVG {
	
class MVGProjectWrapper : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString projectDirectory READ getProjectDirectory WRITE setProjectDirectory NOTIFY projectDirectoryChanged);
	Q_PROPERTY(QObjectListModel* cameraModel READ getCameraModel NOTIFY cameraModelChanged);
	Q_PROPERTY(QStringList visiblePanelNames READ getVisiblePanelNames NOTIFY panelModelChanged);
	Q_PROPERTY(QString currentContext READ getCurrentContext WRITE setCurrentContext NOTIFY currentContextChanged);

public:
	MVGProjectWrapper();
	~MVGProjectWrapper();

public slots:
	const QString getProjectDirectory() const;
	void setProjectDirectory(const QString& directory);
	QObjectListModel* getCameraModel() { return &_cameraList; }
	const QStringList& getVisiblePanelNames() const { return _visiblePanelNames; }
	const QString getCurrentContext() const;
	void setCurrentContext(const QString&);
	
signals:
	void projectDirectoryChanged();
	void cameraModelChanged();
	void panelModelChanged();
	void currentContextChanged();
	void isPointCloudDisplayedChanged(QString panel);

public:
	void selectItems(const QList<QString>& cameraNames) const;
	Q_INVOKABLE void selectCameras(const QStringList& cameraNames) const;
	Q_INVOKABLE QString openFileDialog() const;
	Q_INVOKABLE void activeSelectionContext() const;
	Q_INVOKABLE void activeMVGContext();
	Q_INVOKABLE void loadExistingProject();
	Q_INVOKABLE void loadNewProject(const QString& projectDirectoryPath);
	Q_INVOKABLE void setCameraToView(QObject* camera, const QString& viewName, bool rebuildCache=true) const;
	Q_INVOKABLE bool isPointCloudDisplayed(const QString panel);
	Q_INVOKABLE void displayPointCloud(const QString panel, const bool display);
	void clear();

private:
	void reloadMVGCamerasFromMaya();

private:
	QObjectListModel _cameraList;
	MVGProject _project;
	QStringList _allPanelNames;
	QStringList _visiblePanelNames;
	QString _currentContext;
};

} // mayaMVG
