#pragma once

class RenderObj;

class ModelImporter
{
public:
	ModelImporter();
	~ModelImporter();

	ModelImporter(const ModelImporter&) = delete;
	ModelImporter& operator=(const ModelImporter&) = delete;

	RenderObj* CreateModel(const std::string& fileName);
private:
};

