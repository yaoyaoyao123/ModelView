#include "ModelLoder.h"
#include<QFileInfo>


ModelLoder::ModelLoder()
{
}

void ModelLoder::run()
{

	loadObjFile(obj_file_path);

	if (mtl_file_path.isEmpty())
	{
		qDebug() << "none mtllib";
		return;
	}

	loadMtlFile(mtl_file_path);

}

bool ModelLoder::loadObjFile(QString filename)
{

	QVector<double>vertexPoints, texturePoints, normalPoints;

	if (filename.mid(filename.lastIndexOf('.')) != ".obj")
	{
		qDebug() << "file is not a obj file";
		return false;

	}

	QFile objfile(filename);

	if (objfile.isOpen()) objfile.close();

	if (!objfile.open(QIODevice::ReadOnly))
	{
		qDebug() << "open" << filename << "filed";
		return false;
	}
	else
		qDebug() << "open sucess";

	while (!objfile.atEnd())
	{
		QByteArray linedate = objfile.readLine();

		linedate = linedate.trimmed();

		if (linedate == "")
			continue;

		QList<QByteArray> str_values = linedate.split(' ');
		str_values.removeAll("");
		QString data_type = str_values.takeFirst();

		if (data_type == "mtllib")
		{
			QFileInfo mtl(filename);
			mtl_file_path = mtl.path() + "/" + str_values[0];
		}

		else if(data_type == "v")
		{
			for (int i = 0; i < 3; i++)
			{
				if (str_values[i] != "")
					vertexPoints.push_back(str_values[i].toDouble());
			}
		}
		else if (data_type == "vt")
		{
			for (int i = 0; i < 2; i++)
			{
				if (str_values[i] != "")
					texturePoints.push_back(str_values[i].toDouble());
			}
		}
		else if (data_type == "vn")
		{
			for (int i = 0; i < 3; i++)
			{
				if (str_values[i] != "")
					normalPoints.push_back(str_values[i].toDouble());
			}
		}
		else if (data_type == "usemtl")
		{
			model_3d.mesh_count++;
			Mesh meshi;
			model_3d.meshes << meshi;
			model_3d.meshes[model_3d.mesh_count - 1].mtl.mtlname = str_values[0];

		}
		else if (data_type == "f")
		{

			if (str_values.size() % 3 != 0)
			{
				if (str_values.size() == 4)
				{
					str_values.push_back(str_values.at(0));
					str_values.push_back(str_values.at(2));
				}
				else if (str_values.size() == 5)
				{
					str_values.insert(1, str_values.at(4));
					str_values.insert(2, str_values.at(2));
					str_values.insert(3, str_values.at(1));
					str_values.insert(5, str_values.at(5));
				}
				else if (str_values.size() == 7)
				{
					str_values.insert(3, str_values.at(1));
					str_values.insert(4, str_values.at(2));

				}
				else if (str_values.size() == 8)
				{
					str_values.insert(3, str_values.at(1));
					str_values.insert(4, str_values.at(2));
					str_values.insert(9, str_values.at(6));
					str_values.insert(10, str_values.at(8));
				}
				else
				{
					qDebug() << str_values.size() << str_values;
					continue;
				}
			}

			if (model_3d.meshes.count() <= 0)
			{
				return false;
			}

			std::transform(str_values.begin(), str_values.end(), std::back_inserter(model_3d.meshes[model_3d.mesh_count - 1].faceindexs), [](QByteArray &str)
			{

				QList<QByteArray>intStr = str.split('/');
				return std::make_tuple(intStr.at(0).toLongLong(), intStr.at(1).toLongLong(), intStr.at(2).toLongLong());

			});

		}
		else
			continue;

	}

	for (int i = 0; i < model_3d.mesh_count; ++i)
	{

		for (auto &verFaceInfo:model_3d.meshes[i].faceindexs)
		{
			LONGLONG vIndexs = std::get<0>(verFaceInfo);
			LONGLONG tIndexs = std::get<1>(verFaceInfo);
			LONGLONG nIndexs = std::get<2>(verFaceInfo);

			model_3d.meshes[i].vps << vertexPoints.at(vIndexs * 3 - 3);
			model_3d.meshes[i].vps << vertexPoints.at(vIndexs * 3 - 2);
			model_3d.meshes[i].vps << vertexPoints.at(vIndexs * 3 - 1);

			if (texturePoints.size() != 0 && tIndexs !=0)
			{
				model_3d.meshes[i].tps << texturePoints.at(tIndexs * 2 - 2);
				model_3d.meshes[i].tps << texturePoints.at(tIndexs * 2 - 1);
			}

			model_3d.meshes[i].nps << normalPoints.at(nIndexs * 3 - 3);
			model_3d.meshes[i].nps << normalPoints.at(nIndexs * 3 - 2);
			model_3d.meshes[i].nps << normalPoints.at(nIndexs * 3 - 1);

		}

		model_3d.meshes[i].allps << model_3d.meshes[i].tps << model_3d.meshes[i].nps << model_3d.meshes[i].vps;

	}

	vertexPoints.clear();
	texturePoints.clear();
	normalPoints.clear();
	objfile.close();
	return true;

}


bool ModelLoder::loadMtlFile(QString filename)
{
	QVector<double>vertexPoints, texturePoints, normalPoints;

	if (filename.mid(filename.lastIndexOf('.')) != ".mtl")
	{
		qDebug() << "file is not a mtl file";
		return false;
	}

	QFile mtlfile(filename);

	if (mtlfile.isOpen()) mtlfile.close();

	if (!mtlfile.open(QIODevice::ReadOnly))
	{
		qDebug() << "open" << filename << "filed";
		return false;
	}
	else
		qDebug() << "open sucess";

	QFileInfo pic(filename);

	while (!mtlfile.atEnd())
	{
		QByteArray linedate = mtlfile.readLine();

		linedate = linedate.trimmed();

		if (linedate == "")
			continue;

		QList<QByteArray> str_values = linedate.split(' ');
		str_values.removeAll("");
		QString data_type = str_values.takeFirst();

		if (data_type == "newmtl")
		{
			model_3d.mtl_count++;
			Material mtl;
			model_3d.mtls << mtl;
			model_3d.mtls[model_3d.mtl_count - 1].mtlname = str_values[0];
		}

		else if (data_type == "map_Kd")
		{
			model_3d.mtls[model_3d.mtl_count - 1].map_kd_path = pic.path() + "/" + str_values[0];
		}
		else if (data_type == "map_Ks")
		{
			model_3d.mtls[model_3d.mtl_count - 1].map_ks_path = pic.path() + "/" + str_values[0];
		}

		else if (data_type == "Kd")
		{
			model_3d.mtls[model_3d.mtl_count - 1].Kd.setX(str_values[0].toDouble());
			model_3d.mtls[model_3d.mtl_count - 1].Kd.setY(str_values[1].toDouble());
			model_3d.mtls[model_3d.mtl_count - 1].Kd.setZ(str_values[2].toDouble());
		}
		else if (data_type == "Ka")
		{
			model_3d.mtls[model_3d.mtl_count - 1].Ka.setX(str_values[0].toDouble());
			model_3d.mtls[model_3d.mtl_count - 1].Ka.setY(str_values[1].toDouble());
			model_3d.mtls[model_3d.mtl_count - 1].Ka.setZ(str_values[2].toDouble());
		}
		else if (data_type == "Ks")
		{
			model_3d.mtls[model_3d.mtl_count - 1].Ks.setX(str_values[0].toDouble());
			model_3d.mtls[model_3d.mtl_count - 1].Ks.setY(str_values[1].toDouble());
			model_3d.mtls[model_3d.mtl_count - 1].Ks.setZ(str_values[2].toDouble());
		}

		else
			continue;

	}


	mtlfile.close();
	for (int i = 0; i < model_3d.mesh_count; i++)
	{

		for (int j = 0; j < model_3d.mtl_count; j++)
		{

			if (model_3d.meshes[i].mtl.mtlname == model_3d.mtls[j].mtlname)
			{
				model_3d.meshes[i].mtl.Ka = model_3d.mtls[j].Ka;
				model_3d.meshes[i].mtl.Kd = model_3d.mtls[j].Kd;
				model_3d.meshes[i].mtl.Ks = model_3d.mtls[j].Ks;

				if (QFile::exists(model_3d.mtls[j].map_kd_path))
				{
					model_3d.meshes[i].mtl.map_kd_path = model_3d.mtls[j].map_kd_path;
					model_3d.meshes[i].mtl.value_or_diffmap = true;
				}
				else
				{
					model_3d.meshes[i].mtl.value_or_diffmap = false;
				}

				if (QFile::exists(model_3d.mtls[j].map_ks_path))
				{
					model_3d.meshes[i].mtl.map_ks_path = model_3d.mtls[j].map_ks_path;
					model_3d.meshes[i].mtl.value_or_specmap = true;
				}
				else
				{
					model_3d.meshes[i].mtl.value_or_specmap = false;
				}

			}

		}

	}

	return true;

}


void ModelLoder::setFileName(QString filename)
{
	obj_file_path = filename;
}

Model3D ModelLoder::getloadedModel()
{
	return model_3d;
}


ModelLoder::~ModelLoder()
{ 
}
