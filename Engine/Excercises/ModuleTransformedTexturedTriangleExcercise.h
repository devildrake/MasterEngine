#ifndef MODULE_TRANSFORMED_TEXTURED_TRIANGLE_EXCERCISE_H
#define MODULE_TRANSFORMED_TEXTURED_TRIANGLE_EXCERCISE_Hs
#include "../Modules/Module.h"
#include "../MathGeoLib/MathGeoLib.h"
#include "glew.h"
#include "../MathGeoLib/Geometry/Frustum.h"
#include "../Utilities/Globals.h"
#include "../ImGui/imgui.h"
#include "IL/ilut.h"
class ModuleTransformedTexturedTriangleExcercise :public Module {
public:
	ModuleTransformedTexturedTriangleExcercise();
	~ModuleTransformedTexturedTriangleExcercise();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	GLuint texture;
	unsigned vbo;
	unsigned fragmentShader, vertexShader;
	unsigned shaderID;
private:
	unsigned CreeateQuadVBO();
	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);

	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	float3 trianglePos;

	ILinfo* info;

	struct ExcerciseUI {
	private:

	public:

		ExcerciseUI() {
			p_open = true;
		}

		bool p_open;



		void Draw(ILinfo* info) {
			ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);


			if (!ImGui::Begin("EXcercise UI", &p_open))
			{
				ImGui::End();
				return;
			}

			//TO DO SHOW WIDTH/HEIGHT
			ImGui::Text("Width:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", info->Width);
			ImGui::SameLine();
			ImGui::Text(" Height:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", info->Height);

			//TO DO SHOW PIXEL DEPTH
			//TO DO SHOW FORMAT
			//TO DO Allow switching options: WRAP mode, MAG/MIN filter, 



			ImGui::End();
		}
	};
	ExcerciseUI ui;
};
#endif