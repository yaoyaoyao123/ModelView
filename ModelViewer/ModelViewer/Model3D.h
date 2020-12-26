#pragma once
#pragma execution_character_set("utf-8")
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#define PI 3.1415926

//����
struct Material
{
	QString mtlname;

	QVector3D Ka;
	QVector3D Kd;
	QVector3D Ks;
	QVector3D Tf;

	float Ni = 0;
	float d = 1;

	//��ͼ�������
	bool value_or_diffmap;
	bool value_or_specmap;

	//��ͼ·��
	QString map_kd_path;
	QString map_ks_path;

};

//����
class Mesh
{

public:

	Mesh() = default;

	//����
	QVector<double> vps, tps, nps;
	QVector<double> allps;

	//������
	QVector<std::tuple<LONGLONG, LONGLONG, LONGLONG>>faceindexs;

	//����
	Material mtl;

	//�������
	QOpenGLBuffer vbo;
	QOpenGLTexture *map_kd_texture;
	QOpenGLTexture *map_ks_texture;

};

//ģ��
class Model3D
{

public:

	Model3D();

	//mesh����
	QVector<Mesh>meshes;

	//���ʿ�
	QVector<Material>mtls;

	//ģ�ʹ�С
	uint mesh_count = 0;
	uint mtl_count = 0;

	//ģ�ͱ任����
	QMatrix4x4 transform_mat;

	//�任
	void setPitch(double);
	void setRoll(double);
	void setYaw(double);
	void rotate();

	void scale(double);

	void translate();

	void restoreAllTransform();

	~Model3D();

private:

	double scale_factor;

	int pitch;
	int yaw;
	int roll;

	int x_step;
	int y_step;
	int z_step;

};

