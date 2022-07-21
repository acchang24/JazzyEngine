#pragma once

class RenderObj;
class Mesh;

class ModelImporter
{
public:
	ModelImporter();
	~ModelImporter();

	ModelImporter(const ModelImporter&) = delete;
	ModelImporter& operator=(const ModelImporter&) = delete;

	Mesh* CreateModel(const std::string& fileName);
private:
};

