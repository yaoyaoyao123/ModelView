#pragma once
#include<Model3D.h>
#include <QThread>

class ModelLoder : public QThread
{
	Q_OBJECT


public:

	ModelLoder();
	void setFileName(QString);

	bool loadObjFile(QString);
	bool loadMtlFile(QString);
	
	Model3D getloadedModel();

	~ModelLoder();

private:

	QString obj_file_path;
	QString mtl_file_path;

	Model3D model_3d;

protected:

	void run();

};
