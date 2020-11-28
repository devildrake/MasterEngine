#ifndef MODULE_TRANSFORMED_TEXTURED_TRIANGLE_EXCERCISE_H
#define MODULE_TRANSFORMED_TEXTURED_TRIANGLE_EXCERCISE_Hs
#include "../MathGeoLib/MathGeoLib.h"
#include <glew.h>
#include "../MathGeoLib/Geometry/Frustum.h"
#include "../Utilities/Globals.h"
#include "../ImGui/imgui.h"
#include "../Application.h"
#include "../Modules/ModuleTextures.h"
#include <IL/ilut.h>
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
	unsigned CreateQuadVBO();
	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);

	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);
	unsigned CreateProgram(const char* vtx_file_name, const char* frg_file_name);
	float3 trianglePos;
	ILinfo* info;

	struct ExcerciseUI {
	private:
		std::string GetImageFormatAsString(ILenum format) {

			std::string temp = "";
			switch (format) {
			case IL_COLOUR_INDEX:
				temp = "Color index";
				break;
			case IL_RGB:
				temp = "RGB";
				break;
			case IL_ALPHA:
				temp = "ALPHA";
				break;
			case IL_RGBA:
				temp = "RGBA";
				break;
			case IL_BGR:
				temp = "BGR";
				break;
			case IL_BGRA:
				temp = "BGRA";
				break;
			case IL_LUMINANCE:
				temp = "LUMINANCE";
				break;
			case IL_LUMINANCE_ALPHA:
				temp = "LUMINANCE ALPHA";
				break;
			default:
				break;
			}
			return temp;
		}
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

			ImGui::Text("Width:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", info->Width);
			ImGui::SameLine();
			ImGui::Text(" Height:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", info->Height);

			ImGui::Text("Pixel Depth:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", info->Depth);

			ImGui::Text("Format:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), GetImageFormatAsString(info->Format).c_str());

			if (ImGui::CollapsingHeader("Wrapping Options")) {
				if (ImGui::Button("REPEAT")) {
					App->textures->SetWrapMode(GL_REPEAT);
				}
				if (ImGui::Button("MIRRORED REPEAT")) {
					App->textures->SetWrapMode(GL_MIRRORED_REPEAT);

				}
				if (ImGui::Button("CLAMP TO EDGE")) {
					App->textures->SetWrapMode(GL_CLAMP_TO_EDGE);

				}
				if (ImGui::Button("CLAMP TO BORDER")) {
					App->textures->SetWrapMode(GL_CLAMP_TO_BORDER);
				}
			}

			if (ImGui::CollapsingHeader("Min Options")) {
				if (ImGui::Button("GL NEAREST")) {
					App->textures->SetMinMode(GL_NEAREST);
				}
				if (ImGui::Button("GL LINEAR")) {
					App->textures->SetMinMode(GL_LINEAR);

				}
				if (ImGui::Button("GL NEAREST MIPMAP NEAREST")) {
					App->textures->SetMinMode(GL_NEAREST_MIPMAP_NEAREST);
				}
				if (ImGui::Button("GL LINEAR MIPMAP NEAREST")) {
					App->textures->SetMinMode(GL_LINEAR_MIPMAP_NEAREST);
				}
				if (ImGui::Button("GL NEAREST MIPMAP LINEAR")) {
					App->textures->SetMinMode(GL_NEAREST_MIPMAP_LINEAR);
				}
				if (ImGui::Button("GL LINEAR MIPMAP LINEAR")) {
					App->textures->SetMinMode(GL_LINEAR_MIPMAP_LINEAR);
				}
			}

			if (ImGui::CollapsingHeader("Mag Options")) {
				if (ImGui::Button("GL NEAREST")) {
					App->textures->SetMagMode(GL_NEAREST);
				}
				if (ImGui::Button("GL LINEAR")) {
					App->textures->SetMagMode(GL_LINEAR);

				}
			}

			ImGui::End();
		}
	};
	ExcerciseUI ui;
};
#endif