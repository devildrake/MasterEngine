#ifndef MODULE_RENDER_H
#define MODULE_RENDER_H
#include "Module.h"
#include <SDL.h>
#include <Globals.h>
#include "../MathGeoLib/Math/float3.h"
#include <list>
class Shader;
class Model;

class ModuleRender : public Module {
private:

	void* context;
	float3 bgColor;
	std::list<Model*> models;
	SDL_GLContext glcontext;
	Shader* unlitShader;
	Shader* defaultShader;
	bool faceCulling;
	bool depthTest;
	bool wireFramePolygonMode;

	Shader* quadShader;
	unsigned int quadVAO, quadVBO;
	unsigned int cubeVAO, cubeVBO;
	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;



public:

	ModuleRender();
	~ModuleRender();
	bool Init() override;
	bool Start() override;
	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;
	bool CleanUp() override;
	void MainWindowResized(unsigned width, unsigned height) override;
	SDL_GLContext GetContext();
	const unsigned& GetDefaultShaderID()const;
	const unsigned& GetUnlitShaderID()const;
	const unsigned& GetCubeVAO()const;
	void AddModel(Model* m);
	void RemoveModel(Model* m);
	void ToggleFaceCulling()const;
	void ToggleDepthTest()const;
	void ToggleWireFrameMode()const;
	void RegenerateRenderBuffer();
	unsigned GetRenderTextureID()const;
	//void SceneWindowResized(unsigned, unsigned)override;
public:
	friend class ConfigWindow;
	friend class SceneWindow;
};
#endif