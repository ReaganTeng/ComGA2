#include "SceneA2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"
#include "Utility.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "LoadTGA.h"

#include <sstream>

//#include <random>
#include <ctime>
#include <iostream>

SceneA2::SceneA2()
{
}

SceneA2::~SceneA2()
{
}

void SceneA2::Init()
{
	srand((unsigned)time(NULL));

	//RANDOM GENERATION FOR WOOD
	{
		limit = 10;
		int random_generator;
		for (int i = 0; i < limit; )
		{
			if (i == 0)
			{
				random_generator = rand() % 10 + (-5);
				coordinates_x[i] = random_generator;
				i++;
			}
			else
			{
				random_generator = rand() % 10 + (-5);

				for (int j = i; j > 0; j--)
				{
					if (coordinates_x[i - j] != random_generator)
					{
						random_generator = rand() % 10 + (-5);
						coordinates_x[i] = random_generator;
						i++;
					}
					else
					{
						continue;
					}
				}
			}
		}

		for (int i = 0; i < limit;)
		{
			if (i == 0)
			{
				random_generator = rand() % 10 + (-5);
				coordinates_z[i] = random_generator;
				i++;
			}
			else
			{
				random_generator = rand() % 10 + (-5);

				for (int j = i; j > 0; j--)
				{
					if (coordinates_z[i - j] != random_generator)
					{
						random_generator = rand() % 10 + (-5);
						coordinates_z[i] = random_generator;
						i++;
					}
					else
					{
						continue;
					}
				}
			}
		}
	}
	
	{
		range = 15;
		nrange = -15;
		
		health = 1000;
		monster1_die = false;
		monster2_die = false;
		monster3_die = false;
		monster4_die = false;
		game_over = false;
		you_win = false;
		timer_scene_1 = 0;
		timer_scene_2 = 0;
		timer_scene_3 = 0;
		translate_x = 0;
		translate_z = 0;
		monster1_health = 100;
		monster2_health = 100;
		monster3_health = 100;
		monster4_health = 100;
		pick_up_chest_mode = false;
		pick_up_tent_mode = false;
		pick_up_bench_mode = false;
		wood1_disappear = false;
		wood2_disappear = false;
		wood3_disappear = false;
		wood4_disappear = false;
		wood5_disappear = false;
		wood6_disappear = false;
		wood7_disappear = false;
		wood8_disappear = false;
		wood9_disappear = false;
		wood10_disappear = false;
		can_pick_wood1 = false;
		can_pick_wood2 = false;
		can_pick_wood3 = false;
		can_pick_wood4 = false;
		can_pick_wood5 = false;
		can_pick_wood6 = false;
		can_pick_wood7 = false;
		can_pick_wood8 = false;
		can_pick_wood9 = false;
		can_pick_wood10 = false;
		minigame_1 = true;
		minigame_2 = false;
		minigame_3 = false;
 	}
	// Init VBO here
	
		Mtx44 projection;

		projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
		projectionStack.LoadMatrix(projection);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
		
		camera.Init(Vector3(0, 60, 88), Vector3(0, 60, 0), Vector3(0, 1, 0));

		
		//LIGHTBALLS
		{
			
				light[0].type = Light::LIGHT_POINT;
				light[0].position.Set(0, 90, -100);
				light[0].color.Set(1, 1, 1);
				light[0].power = 5;
				light[0].kC = 1.f;
				light[0].kL = 0.01f;
				light[0].kQ = 0.001f;
				light[0].cosCutoff = cos(Math::DegreeToRadian(45));
				light[0].cosInner = cos(Math::DegreeToRadian(30));
				light[0].exponent = 3.f;
				light[0].spotDirection.Set(0.f, 1.f, 0.f);
			

			
				light[1].type = Light::LIGHT_SPOT;
				light[1].position.Set(0, 100, 0);
				light[1].color.Set(0, 1, 0);
				light[1].power = 3;
				light[1].kC = 1.f;
				light[1].kL = 0.01f;
				light[1].kQ = 0.001f;
				light[1].cosCutoff = cos(Math::DegreeToRadian(45));
				light[1].cosInner = cos(Math::DegreeToRadian(30));
				light[1].exponent = 3.f;
				light[1].spotDirection.Set(0.f, 1.f, 0.f);
			
			//set background color
			glClearColor(0.0f, 1.0f, 0.0f, 0.0f);

			//generate default VAO for now
			glGenVertexArrays(1, &m_vertexArrayID);
			glBindVertexArray(m_vertexArrayID);

			//load vertex and fragment shaders
			m_programID = LoadShaders("Shader//Texture.vertexshader",
				"Shader//Text.fragmentshader");

			
				m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
				m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
				m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
				m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
				m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
			

			
				m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
				m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
				m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
				m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
				m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
			

			m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
			m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
			m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
			m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
			m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
			m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
			m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
			m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

			m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
				m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
				m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
				m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
				m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
				m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
			

			
				m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
				m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
				m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
				m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
				m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
				m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
			

			m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

			// Get a handle for our "colorTexture" uniform
			m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
			m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");


			m_parameters[U_TEXT_ENABLED] =
				glGetUniformLocation(m_programID, "textEnabled");
			m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
				"textColor");




			//use our shader
			glUseProgram(m_programID);

			// Make sure you pass uniform parameters after glUseProgram()
				glUniform1i(m_parameters[U_NUMLIGHTS], 2);
			

				
					//light[0].type = Light::LIGHT_DIRECTIONAL;
					glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
					glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
					glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

					//light[0].type = Light::LIGHT_POINT;
					glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
					glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
					glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);

					//light[0].type = Light::LIGHT_SPOT;
					glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
					glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
					glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
				


				
					//light[0].type = Light::LIGHT_DIRECTIONAL;
					glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
					glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
					glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

					//light[0].type = Light::LIGHT_POINT;
					glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
					glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
					glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);

					//light[0].type = Light::LIGHT_SPOT;
					glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
					glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
					glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
				
		}



		
		//Enable depth test
		glEnable(GL_DEPTH_TEST);

		//Get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	

	//ALL GEOMETRIES
	{


		//ENVIRONMENT
		{
			//SKYBOX
			{
				meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
				meshList[GEO_FRONT]->textureID = LoadTGA("Image//Skybox//Plants//negx.tga");

				meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
				meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Skybox//Plants//posz.tga");

				meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
				meshList[GEO_LEFT]->textureID = LoadTGA("Image//Skybox//Plants//negz.tga");

				meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
				meshList[GEO_BACK]->textureID = LoadTGA("Image//Skybox//Plants//posx.tga");

				meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
				meshList[GEO_TOP]->textureID = LoadTGA("Image//Skybox//Plants//posy.tga");

				meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
				meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Skybox//Plants//negy.tga");
			}

			//GROUND
			meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("GROUND", Color(1, 1, 1), 1.f);
			meshList[GEO_GROUND]->textureID = LoadTGA("Image//grass.tga");
			meshList[GEO_GROUND]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_GROUND]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			meshList[GEO_GROUND]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_GROUND]->material.kShininess = 1.f;
		}

		//LIGHTING
		{
			meshList[GEO_LIGHTBALL_1] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 50, 50, 1.f);
			meshList[GEO_LIGHTBALL_2] = MeshBuilder::GenerateSphere("STAR", Color(0, 1, 0), 50, 50, 1.f);
		}
		meshList[GEO_GRASS] = MeshBuilder::GenerateOBJMTL("grass",
			"OBJ//SurvivalModels//grass.obj", "OBJ//SurvivalModels//grass.mtl");
		meshList[GEO_ROCKS] = MeshBuilder::GenerateOBJMTL("rocks",
			"OBJ//SurvivalModels//rockA.obj", "OBJ//SurvivalModels//rockA.mtl");
		meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("tree",
			"OBJ//SurvivalModels//tree.obj", "OBJ//SurvivalModels//tree.mtl");

		meshList[GEO_PLAYERSPHERE] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 50, 50, 1.0f);


		//CHARACTER
		{
			//GenerateSphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_HEAD] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 30, 30, 2);
			meshList[GEO_HEAD]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_HEAD]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			meshList[GEO_HEAD]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_HEAD]->material.kShininess = 1.f;


			//static Mesh*GenerateCylinder(const std::string & meshName, Color color, unsigned numSlices = 10, unsigned height = 2, float radius = 1.f);
			meshList[GEO_LIMBS] = MeshBuilder::GenerateCylinder("STAR", Color(0, 0, 0), 30, 10, 5);
			meshList[GEO_LIMBS]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_LIMBS]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			meshList[GEO_LIMBS]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_LIMBS]->material.kShininess = 1.f;

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_PALM] = MeshBuilder::GenerateSphere("STAR", Color(1, 1, 1), 30, 10, 0);


			// MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
			meshList[GEO_FEET] = MeshBuilder::GenerateCube("STAR", Color(0.545098039215686, 0.270588235294117, 0.074509803921568), 1);
			meshList[GEO_FEET]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_FEET]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			meshList[GEO_FEET]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_FEET]->material.kShininess = 1.f;

			//GenerateTorus(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices, float outerradius, float innerradius)
			meshList[GEO_EYES] = MeshBuilder::GenerateTorus("STAR", Color(0, 0, 0), 20, 20, 3, 2);
			meshList[GEO_EYES]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_EYES]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			meshList[GEO_EYES]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_EYES]->material.kShininess = 1.f;

			meshList[GEO_MOUTH] = MeshBuilder::GenerateHalfTorus("STAR", Color(0, 0, 0), 20, 20, 3, 2);
			meshList[GEO_MOUTH]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_MOUTH]->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
			meshList[GEO_MOUTH]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
			meshList[GEO_MOUTH]->material.kShininess = 1.f;
		}

		//ALL MONSTERS
		{
			meshList[GEO_MONSTERS1] = MeshBuilder::GenerateOBJ("Scarecrow", "OBJ//scarecrow_obj//Scarecrow.obj");
			meshList[GEO_MONSTERS1]->textureID = LoadTGA("Image//diffuse.tga");
			meshList[GEO_MONSTERS1]->position.Set(0, 0, -100);


			meshList[GEO_MONSTERS2] = MeshBuilder::GenerateOBJ("Scarecrow", "OBJ//scarecrow_obj//Scarecrow.obj");
			meshList[GEO_MONSTERS2]->textureID = LoadTGA("Image//diffuse.tga");
			meshList[GEO_MONSTERS2]->position.Set(0, 0, -100);

			meshList[GEO_MONSTERS3] = MeshBuilder::GenerateOBJ("Scarecrow", "OBJ//scarecrow_obj//Scarecrow.obj");
			meshList[GEO_MONSTERS3]->textureID = LoadTGA("Image//diffuse.tga");
			meshList[GEO_MONSTERS3]->position.Set(0, 0, -100);


			meshList[GEO_MONSTERS4] = MeshBuilder::GenerateOBJ("Scarecrow", "OBJ//scarecrow_obj//Scarecrow.obj");
			meshList[GEO_MONSTERS4]->textureID = LoadTGA("Image//diffuse.tga");
			meshList[GEO_MONSTERS4]->position.Set(0, 0, -100);

		}

		//ALL THE WOOD
		{
			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD1] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD1] ->position.Set(coordinates_x[0], 1, coordinates_z[0]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD2] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD2]->position.Set(coordinates_x[1], 1, coordinates_z[1]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD3] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD3]->position.Set(coordinates_x[2], 1, coordinates_z[2]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD4] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD4]->position.Set(coordinates_x[3], 1, coordinates_z[3]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD5] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD5]->position.Set(coordinates_x[4], 1, coordinates_z[4]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD6] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD6]->position.Set(coordinates_x[5], 1, coordinates_z[5]);


			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD7] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD7]->position.Set(coordinates_x[6], 1, coordinates_z[6]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD8] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD8]->position.Set(coordinates_x[7], 1, coordinates_z[7]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD9] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD9]->position.Set(coordinates_x[8], 1, coordinates_z[8]);

			//GenerateSphere(const std::string & meshName, Color color, unsigned numStacks, unsigned numSlices = 10, float radius = 1.f);
			meshList[GEO_WOOD10] = MeshBuilder::GenerateOBJMTL("wood",
				"OBJ//SurvivalModels//resourceWood.obj", "OBJ//SurvivalModels//resourceWood.mtl");
			meshList[GEO_WOOD10]->position.Set(coordinates_x[9], 1, coordinates_z[9]);
		}

		

		//FURNITURE
		{
			meshList[GEO_BENCH] = MeshBuilder::GenerateOBJMTL("bench",
				"OBJ//SurvivalModels//workbench.obj", "OBJ//SurvivalModels//workbench.mtl");
			meshList[GEO_BENCH]->position.Set(-60, 0, 0);

			meshList[GEO_CHEST] = MeshBuilder::GenerateOBJMTL("chest",
				"OBJ//SurvivalModels//chest.obj", "OBJ//SurvivalModels//chest.mtl");
			meshList[GEO_CHEST]->position.Set(0, 0, -60);

			meshList[GEO_TENT] = MeshBuilder::GenerateOBJMTL("tent",
				"OBJ//SurvivalModels//tentClosed.obj", "OBJ//SurvivalModels//tentClosed.mtl");
			meshList[GEO_TENT]->position.Set(60, 0, 0);
		}


		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16,
			16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//Exported.tga");
	}

	Mesh::SetMaterialLoc(
		m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}



void SceneA2::Update(double dt)
{

	if (Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	

	
	
	
	
	
	if (Application::IsKeyPressed('A'))
	{

		translate_x -= (float)(80 * dt);


		if (pick_up_chest_mode == true)
		{
			meshList[GEO_CHEST]->position.x -= (float)(80 * dt);
		}
		if (pick_up_tent_mode == true)
		{
			meshList[GEO_TENT]->position.x -= (float)(80 * dt);
		}
		if (pick_up_bench_mode == true)
		{
			meshList[GEO_BENCH]->position.x -= (float)(80 * dt);
		}
	}
	if (Application::IsKeyPressed('D'))
	{
		translate_x += (float)(80 * dt);

		if (pick_up_chest_mode == true)
		{
			meshList[GEO_CHEST]->position.x += (float)(80 * dt);
		}
		if (pick_up_tent_mode == true)
		{
			meshList[GEO_TENT]->position.x += (float)(80 * dt);
		}
		if (pick_up_bench_mode == true)
		{
			meshList[GEO_BENCH]->position.x += (float)(80 * dt);
		}
	}
	if (Application::IsKeyPressed('W'))
	{
		translate_z -= (float)(80 * dt);

		if (pick_up_chest_mode == true)
		{
			meshList[GEO_CHEST]->position.z -= (float)(80 * dt);
		}
		if (pick_up_tent_mode == true)
		{
			meshList[GEO_TENT]->position.z -= (float)(80 * dt);
		}
		if (pick_up_bench_mode == true)
		{
			meshList[GEO_BENCH]->position.z -= (float)(80 * dt);
		}
	}
	if (Application::IsKeyPressed('S'))
	{
		translate_z += (float)(80 * dt);

		if (pick_up_chest_mode == true)
		{
			meshList[GEO_CHEST]->position.z += (float)(80 * dt);
		}
		if (pick_up_tent_mode == true)
		{
			meshList[GEO_TENT]->position.z += (float)(80 * dt);
		}
		if (pick_up_bench_mode == true)
		{
			meshList[GEO_BENCH]->position.z += (float)(80 * dt);
		}
	}

	//LIGHTING
	{
		float LSPEED = 30.0f;
		//MOVE LIGHT TO NEGATIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('I'))
			light[0].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('K'))
			light[0].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('J'))
			light[0].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('L'))
			light[0].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('O'))
			light[0].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('P'))
			light[0].position.y += (float)(LSPEED * dt);

		if (Application::IsKeyPressed('5'))
		{
			//to do: switch light type to POINT and pass the information to shader
			light[0].type = Light::LIGHT_POINT;
		}
		else if (Application::IsKeyPressed('6'))
		{
			//to do: switch light type to DIRECTIONAL and pass the information to shader
			light[0].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (Application::IsKeyPressed('7'))
		{
			//to do: switch light type to SPOT and pass the information to shader
			light[0].type = Light::LIGHT_SPOT;
		}



		if (Application::IsKeyPressed('T'))
			light[1].position.z -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF Z AXIS
		if (Application::IsKeyPressed('F'))
			light[1].position.z += (float)(LSPEED * dt);
		//MOVE LIGHT TO NEGATIVE SIDE OF x AXIS
		if (Application::IsKeyPressed('Y'))
			light[1].position.x -= (float)(LSPEED * dt);
		//MOVE LIGHT TO POSITIVE SIDE OF X AXIS
		if (Application::IsKeyPressed('G'))
			light[1].position.x += (float)(LSPEED * dt);
		//GO DOWN
		if (Application::IsKeyPressed('U'))
			light[1].position.y -= (float)(LSPEED * dt);
		//GO UP
		if (Application::IsKeyPressed('H'))
			light[1].position.y += (float)(LSPEED * dt);



		if (Application::IsKeyPressed('8'))
		{
			//to do: switch light type to POINT and pass the information to shader
			light[1].type = Light::LIGHT_POINT;
		}
		else if (Application::IsKeyPressed('9'))
		{
			//to do: switch light type to DIRECTIONAL and pass the information to shader
			light[1].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (Application::IsKeyPressed('0'))
		{
			//to do: switch light type to SPOT and pass the information to shader
			light[1].type = Light::LIGHT_SPOT;
		}



	}


	

	if (Application::IsKeyPressed('R'))
	{
		timer_scene_1 = 0;
		timer_scene_2 = 0;
		timer_scene_3 = 0;

		meshList[GEO_BENCH]->position.x = -60;
		meshList[GEO_BENCH]->position.z = 0;
		meshList[GEO_CHEST]->position.x = 0;
		meshList[GEO_CHEST]->position.z = -60;
		meshList[GEO_TENT]->position.x = 60;
		meshList[GEO_TENT]->position.z = 0;


		minigame_1 = true;
		minigame_2 = false;
		minigame_3 = false;
		you_win = false;
		game_over = false;

		health = 1000;
		monster1_health = 100;
		monster2_health = 100;
		monster3_health = 100;
		monster4_health = 100;
		
		monster1_die = false;
		monster2_die = false;
		monster3_die = false;
		monster4_die = false;

		pick_up_chest_mode = false;
		pick_up_tent_mode = false;
		pick_up_bench_mode = false;

		wood1_disappear = false;
		wood2_disappear = false;
		wood3_disappear = false;
		wood4_disappear = false;
		wood5_disappear = false;
		wood6_disappear = false;
		wood7_disappear = false;
		wood8_disappear = false;
		wood9_disappear = false;
		wood10_disappear = false;

		can_pick_wood1 = false;
		can_pick_wood2 = false;
		can_pick_wood3 = false;
		can_pick_wood4 = false;
		can_pick_wood5 = false;
		can_pick_wood6 = false;
		can_pick_wood7 = false;
		can_pick_wood8 = false;
		can_pick_wood9 = false;
		can_pick_wood10 = false;

	}

	
	if (minigame_1 == true)
	{
		timer_scene_1 += (float)(5 * dt);

		//WOOD
		{
			if (wood1_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD1]->position.z < range && camera.position.z - meshList[GEO_WOOD1]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD1]->position.x < range && camera.position.x - meshList[GEO_WOOD1]->position.x > nrange))
				{
					can_pick_wood1 = true;
				}
				else
				{
					can_pick_wood1 = false;
				}
			}
			if (wood2_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD2]->position.z < range && camera.position.z - meshList[GEO_WOOD2]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD2]->position.x < range && camera.position.x - meshList[GEO_WOOD2]->position.x > nrange))
				{
					can_pick_wood2 = true;
				}
				else
				{
					can_pick_wood2 = false;
				}
			}
			if (wood3_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD3]->position.z < range && camera.position.z - meshList[GEO_WOOD3]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD3]->position.x < range && camera.position.x - meshList[GEO_WOOD3]->position.x > nrange))
				{
					can_pick_wood3 = true;
				}
				else
				{
					can_pick_wood3 = false;
				}
			}
			if (wood4_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD4]->position.z < range && camera.position.z - meshList[GEO_WOOD4]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD4]->position.x < range && camera.position.x - meshList[GEO_WOOD4]->position.x > nrange))
				{
					can_pick_wood4 = true;
				}
				else
				{
					can_pick_wood4 = false;
				}
			}
			if (wood5_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD5]->position.z < range && camera.position.z - meshList[GEO_WOOD5]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD5]->position.x < range && camera.position.x - meshList[GEO_WOOD5]->position.x > nrange))
				{
					can_pick_wood5 = true;
				}
				else
				{
					can_pick_wood5 = false;
				}
			}
			if (wood6_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD6]->position.z < range && camera.position.z - meshList[GEO_WOOD6]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD6]->position.x < range && camera.position.x - meshList[GEO_WOOD6]->position.x > nrange))
				{
					can_pick_wood6 = true;
				}
				else
				{
					can_pick_wood6 = false;
				}
			}
			if (wood7_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD7]->position.z < range && camera.position.z - meshList[GEO_WOOD7]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD7]->position.x < range && camera.position.x - meshList[GEO_WOOD7]->position.x > nrange))
				{
					can_pick_wood7 = true;
				}
				else
				{
					can_pick_wood7 = false;
				}
			}
			if (wood8_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD8]->position.z < range && camera.position.z - meshList[GEO_WOOD8]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD8]->position.x < range && camera.position.x - meshList[GEO_WOOD8]->position.x > nrange))
				{
					can_pick_wood8 = true;
				}
				else
				{
					can_pick_wood8 = false;
				}
			}
			if (wood9_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD9]->position.z < range && camera.position.z - meshList[GEO_WOOD9]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD9]->position.x < range && camera.position.x - meshList[GEO_WOOD9]->position.x > nrange))
				{
					can_pick_wood9 = true;
				}
				else
				{
					can_pick_wood9 = false;
				}
			}
			if (wood10_disappear == false)
			{
				if ((camera.position.z - meshList[GEO_WOOD10]->position.z < range && camera.position.z - meshList[GEO_WOOD10]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_WOOD10]->position.x < range && camera.position.x - meshList[GEO_WOOD10]->position.x > nrange))
				{
					can_pick_wood10 = true;
				}
				else
				{
					can_pick_wood10 = false;
				}
			}
		}

		
		if (Application::IsKeyPressed('E'))
		{
			if (can_pick_wood1 == true)
			{
				wood1_disappear = true;
			}
			else
			{
				wood1_disappear = false;
			}

			if (can_pick_wood2 == true)
			{
				wood2_disappear = true;

			}
			else
			{
				wood2_disappear = false;
			}

			if (can_pick_wood3 == true)
			{
				wood3_disappear = true;

			}
			else
			{
				wood3_disappear = false;
			}

			if (can_pick_wood4 == true)
			{
				wood4_disappear = true;

			}
			else
			{
				wood4_disappear = false;
			}

			if (can_pick_wood5 == true)
			{
				wood5_disappear = true;
			}
			else
			{
				wood5_disappear = false;
			}

			if (can_pick_wood6 == true)
			{
				wood6_disappear = true;
			}
			else
			{
				wood6_disappear = false;
			}

			if (can_pick_wood7 == true)
			{
				wood7_disappear = true;
			}
			else
			{
				wood7_disappear = false;
			}

			if (can_pick_wood8 == true)
			{
				wood8_disappear = true;
			}
			else
			{
				wood8_disappear = false;
			}

			if (can_pick_wood9 == true)
			{
				wood9_disappear = true;
			}
			else
			{
				wood9_disappear = false;
			}

			if (can_pick_wood10 == true)
			{
				wood10_disappear = true;
			}
			else
			{
				wood10_disappear = false;
			}
		}

		if (wood1_disappear == true && wood2_disappear == true && wood3_disappear == true && wood4_disappear == true && wood5_disappear == true
			&& wood6_disappear == true && wood7_disappear == true && wood8_disappear == true && wood9_disappear == true && wood10_disappear == true)
		{
			minigame_1 = false;
			minigame_2 = true;
		}
		
	}

	

	if (minigame_2 == true)
	{
		timer_scene_2 += (float)(5 * dt);

		if (health <= 0)
		{
			minigame_2 = false;
			game_over = true;
		}

		if (monster1_health <= 0)
		{
			monster1_die = true;
		}
		if (monster2_health <= 0)
		{
			monster2_die = true;
		}
		if (monster3_health <= 0)
		{
			monster3_die = true;
		}
		if (monster4_health <= 0)
		{
			monster4_die = true;
		}

		if (timer_scene_2 >= 20)
		{
			health -= (float)(1 * dt);
			camera.position.z = -50;
			camera.target.z = -120;

		}
		//KILL THE MONSTERS
		if (Application::IsKeyPressed('X'))
		{
			if (monster4_die == false
				&& monster2_die == false
				&& monster1_die == false
				&& monster3_die == false)
			{
				monster1_health -= (float)(10 * dt);

			}
			if (monster4_die == false
				&& monster2_die == false
				&& monster1_die == true
				&& monster3_die == false)
			{
				monster2_health -= (float)(10 * dt);
			}
			if (monster4_die == false
				&& monster2_die == true
				&& monster1_die == true
				&& monster3_die == false)
			{
				monster3_health -= (float)(10 * dt);
			}
			if (monster4_die == false
				&& monster2_die == true
				&& monster1_die == true
				&& monster3_die == true)
			{
				monster4_health -= (float)(10 * dt);
			}
		}

		//WINNING CONDITION
		if (monster2_die == true
			&& monster1_die == true
			&& monster3_die == true
			&& monster4_die == true)
		{
			minigame_2 = false;
			minigame_3 = true;
		}

	}
	

	if (minigame_3 == true)
	{
		timer_scene_3 += (float)(5 * dt);

		//FUNCTIONS TO PICK
		{
			//PICKING UP CHEST
			{
				if ((camera.position.z - meshList[GEO_CHEST]->position.z < range && camera.position.z - meshList[GEO_CHEST]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_CHEST]->position.x < range && camera.position.x - meshList[GEO_CHEST]->position.x > nrange))
				{
					if (Application::IsKeyPressed('E'))
					{
						pick_up_chest_mode = true;
					}

				}
				if (Application::IsKeyPressed('X'))
				{
					pick_up_chest_mode = false;
				}
			}

			//PICKING UP BENCH
			{
				if ((camera.position.z - meshList[GEO_BENCH]->position.z < range && camera.position.z - meshList[GEO_BENCH]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_BENCH]->position.x < range && camera.position.x - meshList[GEO_BENCH]->position.x > nrange))
				{
					if (Application::IsKeyPressed('E'))
					{
						pick_up_bench_mode = true;
					}

				}
				if (Application::IsKeyPressed('X'))
				{
					pick_up_bench_mode = false;
				}
			}

			//PICKING UP TENT
			{
				if ((camera.position.z - meshList[GEO_TENT]->position.z < range && camera.position.z - meshList[GEO_TENT]->position.z > nrange)
					&& (camera.position.x - meshList[GEO_TENT]->position.x < range && camera.position.x - meshList[GEO_TENT]->position.x > nrange))
				{
					if (Application::IsKeyPressed('E'))
					{
						pick_up_tent_mode = true;
					}

				}
				if (Application::IsKeyPressed('X'))
				{
					pick_up_tent_mode = false;
				}
			}




			if ((meshList[GEO_BENCH]->position.x != -60 && pick_up_bench_mode == false)
				||(meshList[GEO_BENCH]->position.z != 0 && pick_up_bench_mode == false))
			{
				if ((meshList[GEO_CHEST]->position.x != 0 && pick_up_chest_mode == false)
					|| (meshList[GEO_CHEST]->position.z != -60 && pick_up_chest_mode == false))
				{
					if ((meshList[GEO_TENT]->position.x != 60 && pick_up_tent_mode == false)
						|| (meshList[GEO_TENT]->position.z != 0 && pick_up_tent_mode == false))
					{
						minigame_3 = false;
						you_win = true;
					}
				}
			}
		}
	}




	if (game_over == true)
	{
		if (Application::IsKeyPressed('X'))
		{
			health = 1000;
			monster1_health = 100;
			monster2_health = 100;
			monster3_health = 100;
			monster4_health = 100;


			timer_scene_2 = 0;
			game_over = false;
			minigame_2 = true;
			monster1_die = false;
			monster2_die = false;
			monster3_die = false;
			monster4_die = false;

		}
	}

	
	camera.Update(dt);
}


void SceneA2::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 2);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}


	mesh->Render(); //this line should only be called once

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneA2::RenderSkybox()
{
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 0, -500 );
	modelStack.Scale(1000, 1000, 0);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 0, 500 );
	modelStack.Scale(1000, 1000, 0);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(-500, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(500, 0, 0 );
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, 500, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, -500, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void SceneA2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	//glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f
		//is the spacing of each character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST);
}

void SceneA2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen


	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);


	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);		//is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}



void  SceneA2::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//Set view matrix using camera settings
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();

	{
		if (light[0].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[0].position.x,
				light[0].position.y, light[0].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() *
				lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightDirection_cameraspace.x);
		}
		else if (light[0].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() *
				light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
				&spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
				&lightPosition_cameraspace.x);

		}





		if (light[1].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[1].position.x,
				light[1].position.y, light[1].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() *
				lightDir;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
				&lightDirection_cameraspace.x);
		}
		else if (light[1].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
				&lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() *
				light[1].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1,
				&spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() *
				light[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
				&lightPosition_cameraspace.x);

		}

		//Render the lightball
		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x, light[0].position.y + 10,
			light[0].position.z);
		modelStack.Scale(10, 10,
			10);
		RenderMesh(meshList[GEO_LIGHTBALL_1], false);
		modelStack.PopMatrix();

		//Render the lightball
		modelStack.PushMatrix();
		modelStack.Translate(light[1].position.x, light[1].position.y + 10,
			light[1].position.z);
		modelStack.Scale(10, 10,
			10);
		RenderMesh(meshList[GEO_LIGHTBALL_2], false);
		modelStack.PopMatrix();
	}


	//MAKE THE SKYBOX TRANSLATE CORRECTLY WITH CAMERA
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	RenderSkybox();
	RenderMesh(meshList[GEO_PLAYERSPHERE], true);
	modelStack.PopMatrix();

	
	//STATIC ENVIRONMENT
	{
		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(0, 10, 0);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(1000, 1000, 0);
		RenderMesh(meshList[GEO_GROUND], false);
		modelStack.PopMatrix();

		//TREES
		{
			float radianPerSlice = Math::TWO_PI / 30;
			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(100 / 4 + (250 * cosf(i * radianPerSlice)), 10, 150 / 4 + (250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}

			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(140 / 4 + (250 * cosf(i * radianPerSlice)), 10, 100 / 4 + (250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}

			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(170 / 4 + (250 * cosf(i * radianPerSlice)), 10, 150 / 4 + (250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}

			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(200 / 4 + (250 * cosf(i * radianPerSlice)), 10, 150/4 +(250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}

			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(240 / 4 + (250 * cosf(i * radianPerSlice)), 10, 100 / 4 + (250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}


			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();

				modelStack.Translate(280 / 4 + (250 * cosf(i * radianPerSlice)), 10, 50 / 4 + (250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}


			for (int i = 0; i < 360; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(320 / 4 + (250 * cosf(i * radianPerSlice)), 10, 25 / 4 + (250 * sinf(i * radianPerSlice)));
				modelStack.Scale(20, 20, 20);
				RenderMesh(meshList[GEO_TREE], true);
				modelStack.PopMatrix();
			}
		}

		//ALL THE GRASS
		{
			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(5, 10, 150);
			modelStack.Scale(15, 15, 15);
			RenderMesh(meshList[GEO_GRASS], true);
			modelStack.PopMatrix();


			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(150, 10, 15);
			modelStack.Scale(15, 15, 15);
			RenderMesh(meshList[GEO_GRASS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(30, 10, 100);
			modelStack.Scale(15, 15, 15);
			RenderMesh(meshList[GEO_GRASS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(0, 10, 100);
			modelStack.Scale(15, 15, 15);
			RenderMesh(meshList[GEO_GRASS], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(30, 10, 30);
			modelStack.Scale(15, 15, 15);
			RenderMesh(meshList[GEO_GRASS], true);
			modelStack.PopMatrix();
		}

		//ALL THE ROCKS
		{
			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(15, 0, 200);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_ROCKS], true);
			modelStack.PopMatrix();


			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Translate(-80, 0, -90);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_ROCKS], true);
			modelStack.PopMatrix();
		}

		//MY CHARACTER
		{
			//HEAD
			modelStack.PushMatrix();
			modelStack.Translate(0, 65 , 0 );
			
			//torso
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, -15, 0);
				modelStack.Scale(0.5, 2.5, 0.5);

				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();
			}

			//LEFT ARM
			{
				modelStack.PushMatrix();
				modelStack.Translate(6, -10, 0);
				modelStack.Rotate(/*rotate_left_arm_X*/ 0, /*rotate_left_arm_Y*/ 0, 0/*rotate_left_arm_Z*/, 1);


				//left for arm
				modelStack.PushMatrix();
				modelStack.Translate(2, 0, 10);
				modelStack.Rotate(90, 90, 0, 1);
				modelStack.Scale(0.5, 1.5, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();

				//left back arm
				modelStack.PushMatrix();
				modelStack.Rotate(90, -1.5, 0, 1);
				modelStack.Scale(0.5, 1, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();


				RenderMesh(meshList[GEO_PALM], true);
				modelStack.PopMatrix();
			}

			//RIGHT ARM
			{
				modelStack.PushMatrix();
				modelStack.Translate(-6, -10, 0);
				modelStack.Rotate(/*rotate_right_arm_X*/0, 0 /*rotate_right_arm_Y*/, 0/*rotate_right_arm_Z*/, 1);

				//right for arm
				modelStack.PushMatrix();
				modelStack.Translate(-2, 0, 10);
				modelStack.Rotate(90, 90, 0, 1);
				modelStack.Scale(0.5, 1.5, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();

				//right back arm
				modelStack.PushMatrix();
				modelStack.Rotate(90, 1.5, 0, 1);
				modelStack.Scale(0.5, 1, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();


				RenderMesh(meshList[GEO_PALM], true);
				modelStack.PopMatrix();
			}


			//ENTIRE RIGHT LEG
			{
				//right thigh joint
				modelStack.PushMatrix();
				modelStack.Translate(-4, -5, 0);
				modelStack.Rotate(/*-rotate_right_leg_X*/0, 0/*rotate_right_leg_Y*/, 0, 1);


				//right thigh
				modelStack.PushMatrix();
				modelStack.Translate(0, -25, 0);
				modelStack.Scale(0.5, 1, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();


				//right lower leg joint
				modelStack.PushMatrix();
				modelStack.Translate(0, -5, 0);
				modelStack.Rotate(30 /*+ rotate_right_front_leg_X*/, 30 /*+ rotate_right_front_leg_Y*/, 0, 1);

				//right lower leg
				modelStack.PushMatrix();
				modelStack.Translate(0, -30, 10);
				modelStack.Scale(0.5, 1.5, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();

				//right feet
				modelStack.PushMatrix();
				modelStack.Translate(0, -35, 15);
				modelStack.Scale(5, 2.5, 10);
				RenderMesh(meshList[GEO_FEET], true);
				modelStack.PopMatrix();


				RenderMesh(meshList[GEO_PALM], true);
				modelStack.PopMatrix();


				RenderMesh(meshList[GEO_PALM], true);
				modelStack.PopMatrix();
			}


			//ENTIRE LEFT LEG
			{
				//left thigh joint
				modelStack.PushMatrix();
				modelStack.Translate(4, -5, 0);
				modelStack.Rotate(/*-rotate_left_leg_X*/ 0, 0 /*rotate_left_leg_Y*/, 0, 1);

				//left thigh
				modelStack.PushMatrix();
				modelStack.Translate(0, -25, 0);
				modelStack.Scale(0.5, 1, 0.5);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();


				//left lower leg joint
				modelStack.PushMatrix();
				modelStack.Translate(0, -5, 0);
				modelStack.Rotate(30 /*+ rotate_left_front_leg_X*/, 30 /*+ rotate_left_front_leg_Y*/, 0, 1);

				//left lower leg
				modelStack.PushMatrix();
				modelStack.Translate(0, -30, 10);
				modelStack.Scale(0.5, 1.5, 0.5);
				//modelStack.Rotate(rotate_left_leg, 30, 0, 1);
				RenderMesh(meshList[GEO_LIMBS], true);
				modelStack.PopMatrix();

				//left feet
				modelStack.PushMatrix();
				modelStack.Translate(0, -35, 15);
				modelStack.Scale(5, 2.5, 10);
				RenderMesh(meshList[GEO_FEET], true);
				modelStack.PopMatrix();


				RenderMesh(meshList[GEO_PALM], true);
				modelStack.PopMatrix();




				RenderMesh(meshList[GEO_PALM], true);
				modelStack.PopMatrix();
			}


			modelStack.Rotate(-30, 0, 30, 1);
			

			//ENTIRE EYES
			{
				modelStack.PushMatrix();
				modelStack.Translate(-5, 4, 5);
				modelStack.Scale(1, 1, 1);
				modelStack.Rotate(90, 0, 0, 1);
				RenderMesh(meshList[GEO_EYES], true);
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(5, 4, 5);
				modelStack.Scale(1, 1, 1);
				modelStack.Rotate(90, 0, 0, 1);
				RenderMesh(meshList[GEO_EYES], true);
				modelStack.PopMatrix();
			}


			//ENTIRE MOUTH
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 7);
				modelStack.Rotate(-90, -180, 360, 1);
				modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_MOUTH], true);
				modelStack.PopMatrix();
			}

			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_HEAD], true);
			modelStack.PopMatrix();
		}


	}
	

	//SCENE 1
	if (minigame_1 == true)
	{
		if (timer_scene_1 < 10)
		{
			camera.Reset();
			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 20, 0);
			RenderText(meshList[GEO_TEXT], "We should collect",
				Color(1, 1, 1));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 18, 0);
			RenderText(meshList[GEO_TEXT], "firewood",
				Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if (timer_scene_1 < 20)
		{
			camera.Reset();
			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 20, 0);
			RenderText(meshList[GEO_TEXT], "10 should be enough",
				Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else
		{
			if (wood1_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD1]->position.x, meshList[GEO_WOOD1] -> position.y, meshList[GEO_WOOD1]->position.z);
				RenderMesh(meshList[GEO_WOOD1], true);
				modelStack.PopMatrix();

			}

			if (wood2_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD2]->position.x, meshList[GEO_WOOD2]->position.y, meshList[GEO_WOOD2]->position.z);
				RenderMesh(meshList[GEO_WOOD2], true);
				modelStack.PopMatrix();
			}

			if (wood3_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD3]->position.x, meshList[GEO_WOOD3]->position.y, meshList[GEO_WOOD3]->position.z);
				RenderMesh(meshList[GEO_WOOD3], true);
				modelStack.PopMatrix();
			}

			if (wood4_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD4]->position.x, meshList[GEO_WOOD4]->position.y, meshList[GEO_WOOD4]->position.z);
				RenderMesh(meshList[GEO_WOOD4], true);
				modelStack.PopMatrix();
			}
	
			if (wood5_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD5]->position.x, meshList[GEO_WOOD5]->position.y, meshList[GEO_WOOD5]->position.z);
				RenderMesh(meshList[GEO_WOOD5], true);
				modelStack.PopMatrix();
			}

			if (wood6_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD6]->position.x, meshList[GEO_WOOD6]->position.y, meshList[GEO_WOOD6]->position.z);
				RenderMesh(meshList[GEO_WOOD6], true);
				modelStack.PopMatrix();
			}

			if (wood7_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD7]->position.x, meshList[GEO_WOOD7]->position.y, meshList[GEO_WOOD7]->position.z);
				RenderMesh(meshList[GEO_WOOD7], true);
				modelStack.PopMatrix();
			}

			if (wood8_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD8]->position.x, meshList[GEO_WOOD8]->position.y, meshList[GEO_WOOD8]->position.z);
				RenderMesh(meshList[GEO_WOOD8], true);
				modelStack.PopMatrix();
			}

			if (wood9_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD9]->position.x, meshList[GEO_WOOD9]->position.y, meshList[GEO_WOOD9]->position.z);
				RenderMesh(meshList[GEO_WOOD9], true);
				modelStack.PopMatrix();
			}

			if (wood10_disappear == false)
			{
				modelStack.PushMatrix();
				modelStack.Scale(20, 20, 20);
				modelStack.Translate(meshList[GEO_WOOD10]->position.x, meshList[GEO_WOOD10]->position.y, meshList[GEO_WOOD10]->position.z);
				RenderMesh(meshList[GEO_WOOD10], true);
				modelStack.PopMatrix();
			}

			if (can_pick_wood1 == true || can_pick_wood2 == true || can_pick_wood3 == true || can_pick_wood4 == true || can_pick_wood5 == true
				|| can_pick_wood6 == true || can_pick_wood7 == true || can_pick_wood8 == true || can_pick_wood9 == true || can_pick_wood10 == true)
			{

				RenderTextOnScreen(meshList[GEO_TEXT], "press E to pick \n up wood",
					Color(1, 1, 1), 4, 10, 10);
			}
		}

	}


	//SCENE 2
	if (minigame_2 == true)
	{

		if (timer_scene_2 < 10)
		{
			camera.Reset();
			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 20, 0);
			RenderText(meshList[GEO_TEXT], "Good job!",
				Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if (timer_scene_2 < 20)
		{
			camera.Reset();
			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 20, 0);
			RenderText(meshList[GEO_TEXT], "Oh no, monsters coming!",
				Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else
		{
			

			if (monster4_die == false
				&& monster2_die == false
				&& monster1_die == false
				&& monster3_die == false)
			{
				modelStack.PushMatrix();
				//scale, translate, rotate
				modelStack.Translate(meshList[GEO_MONSTERS1]->position.x, 10, meshList[GEO_MONSTERS1]->position.z);
				modelStack.Scale(15, 15, 15);
				RenderMesh(meshList[GEO_MONSTERS1], true);
				modelStack.PopMatrix();

				std::ostringstream monst_health;
				monst_health.str("");
				monst_health << monster1_health;

				modelStack.PushMatrix();
				modelStack.Scale(4, 4, 4);
				modelStack.Translate(0, 15, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Monster 1 health: " + monst_health.str(),
					Color(1, 0, 0), 3, 0, 20);
				modelStack.PopMatrix();
			}

			if (monster4_die == false
				&& monster2_die == false
				&& monster1_die == true
				&& monster3_die == false)
			{
				modelStack.PushMatrix();
				//scale, translate, rotate
				modelStack.Translate(meshList[GEO_MONSTERS2]->position.x, 10, meshList[GEO_MONSTERS2]->position.z);
				modelStack.Scale(15, 15, 15);
				RenderMesh(meshList[GEO_MONSTERS2], true);
				modelStack.PopMatrix();


				std::ostringstream monst_health;
				monst_health.str("");
				monst_health << monster2_health;

				modelStack.PushMatrix();
				modelStack.Scale(4, 4, 4);
				modelStack.Translate(0, 15, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Monster 2 health: " + monst_health.str(),
					Color(1, 0, 0), 3, 0, 20);
				modelStack.PopMatrix();
			}

			if (monster4_die == false
				&& monster2_die == true
				&& monster1_die == true
				&& monster3_die == false)
			{
				modelStack.PushMatrix();
				//scale, translate, rotate
				modelStack.Translate(meshList[GEO_MONSTERS3]->position.x, 10, meshList[GEO_MONSTERS3]->position.z);
				modelStack.Scale(15, 15, 15);
				RenderMesh(meshList[GEO_MONSTERS3], true);
				modelStack.PopMatrix();

				std::ostringstream monst_health;
				monst_health.str("");
				monst_health << monster3_health;

				modelStack.PushMatrix();
				modelStack.Scale(4, 4, 4);
				modelStack.Translate(0, 15, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Monster 3 health: " + monst_health.str(),
					Color(1, 0, 0), 3, 0, 20);
				modelStack.PopMatrix();
			}

			if (monster4_die == false
				&& monster2_die == true
				&& monster1_die == true
				&& monster3_die == true)
			{
				modelStack.PushMatrix();
				//scale, translate, rotate
				modelStack.Translate(meshList[GEO_MONSTERS4]->position.x, 10, meshList[GEO_MONSTERS4]->position.z);
				modelStack.Scale(15, 15, 15);
				RenderMesh(meshList[GEO_MONSTERS4], true);
				modelStack.PopMatrix();

				std::ostringstream monst_health;
				monst_health.str("");
				monst_health << monster4_health;

				modelStack.PushMatrix();
				modelStack.Scale(4, 4, 4);
				modelStack.Translate(0, 15, 0);
				RenderTextOnScreen(meshList[GEO_TEXT], "Monster 4 health: " + monst_health.str(),
					Color(1, 0, 0), 3, 0, 20);
				modelStack.PopMatrix();
			}


			std::ostringstream your_health;
			your_health.str("");
			your_health << health;

			modelStack.PushMatrix();
			RenderTextOnScreen(meshList[GEO_TEXT], "Your health: " + your_health.str(),
				Color(0, 1, 0), 3, 0, 10);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			RenderTextOnScreen(meshList[GEO_TEXT], "Hold X to attack",
				Color(1, 1, 1), 3, 0, 4);
			modelStack.PopMatrix();
		}

		
	}

	
	if (minigame_3 == true)
	{

		if (timer_scene_3 < 10)
		{
			camera.Reset();
			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 20, 0);
			RenderText(meshList[GEO_TEXT], "That was close",
				Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else if (timer_scene_3 < 20)
		{
			camera.Reset();
			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-10, 20, 0);
			RenderText(meshList[GEO_TEXT], "Let's start to move",
				Color(1, 1, 1));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Scale(4, 4, 4);
			modelStack.Translate(-11, 18, 0);
			RenderText(meshList[GEO_TEXT], "furniture!",
				Color(1, 1, 1));
			modelStack.PopMatrix();
		}
		else
		{ 
			if ((camera.position.z - meshList[GEO_BENCH]->position.z < range && camera.position.z - meshList[GEO_BENCH]->position.z > nrange
				&& camera.position.x - meshList[GEO_BENCH]->position.x < range && camera.position.x - meshList[GEO_BENCH]->position.x > nrange)
				||
				(camera.position.z - meshList[GEO_TENT]->position.z < range && camera.position.z - meshList[GEO_TENT]->position.z > nrange
					&& camera.position.x - meshList[GEO_TENT]->position.x < range && camera.position.x - meshList[GEO_TENT]->position.x > nrange)
				||
				(camera.position.z - meshList[GEO_CHEST]->position.z < range && camera.position.z - meshList[GEO_CHEST]->position.z > nrange
					&& camera.position.x - meshList[GEO_CHEST]->position.x < range && camera.position.x - meshList[GEO_CHEST]->position.x > nrange))
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "press E to pick up furniture",
					Color(1, 1, 1), 3, 10, 10);
			}



			//PICKING UP FURNITURE
			{

				if (pick_up_chest_mode == true)
				{

					modelStack.PushMatrix();
					modelStack.Translate(camera.position.x, 0, camera.position.z);

					modelStack.PushMatrix();
					modelStack.Translate(0, 50, -30);
					//modelStack.Rotate(rotate, 0, 1, 0);
					modelStack.Scale(15, 15, 15);
					RenderMesh(meshList[GEO_CHEST], true);
					modelStack.PopMatrix();

					RenderMesh(meshList[GEO_PLAYERSPHERE], true);
					modelStack.PopMatrix();

					RenderTextOnScreen(meshList[GEO_TEXT], "press X to drop furniture",
						Color(1, 1, 1), 3, 10, 10);
				}
				else
				{
					modelStack.PushMatrix();
					modelStack.Translate(meshList[GEO_CHEST]->position.x, 30, meshList[GEO_CHEST]->position.z);
					//modelStack.Rotate(rotate, 0, 1, 0);
					modelStack.Scale(15, 15, 15);
					RenderMesh(meshList[GEO_CHEST], true);
					modelStack.PopMatrix();

				}

				if (pick_up_tent_mode == true)
				{
					modelStack.PushMatrix();
					modelStack.Translate(camera.position.x, 0, camera.position.z);

					modelStack.PushMatrix();
					modelStack.Translate(0, 50, -30);
					modelStack.Scale(15, 15, 15);
					RenderMesh(meshList[GEO_TENT], true);
					modelStack.PopMatrix();

					RenderMesh(meshList[GEO_PLAYERSPHERE], true);
					modelStack.PopMatrix();

					RenderTextOnScreen(meshList[GEO_TEXT], "press X to drop furniture",
						Color(1, 1, 1), 3, 10, 10);
				}
				else
				{
					modelStack.PushMatrix();
					modelStack.Translate(meshList[GEO_TENT]->position.x, 30, meshList[GEO_TENT]->position.z);
					//modelStack.Rotate(rotate, 0, 1, 0);
					modelStack.Scale(15, 15, 15);
					RenderMesh(meshList[GEO_TENT], true);
					modelStack.PopMatrix();

				}


				if (pick_up_bench_mode == true)
				{
					modelStack.PushMatrix();
					modelStack.Translate(camera.position.x, 0, camera.position.z);

					modelStack.PushMatrix();
					modelStack.Translate(0, 50, -30);
					modelStack.Scale(15, 15, 15);
					RenderMesh(meshList[GEO_BENCH], true);
					modelStack.PopMatrix();

					RenderMesh(meshList[GEO_PLAYERSPHERE], true);
					modelStack.PopMatrix();

					RenderTextOnScreen(meshList[GEO_TEXT], "press X to drop furniture",
						Color(1, 1, 1), 3, 10, 10);
				}
				else
				{
					modelStack.PushMatrix();
					modelStack.Translate(meshList[GEO_BENCH]->position.x, 30, meshList[GEO_BENCH]->position.z);
					//modelStack.Rotate(rotate, 0, 1, 0);
					modelStack.Scale(15, 15, 15);
					RenderMesh(meshList[GEO_BENCH], true);
					modelStack.PopMatrix();

				}
			}
		}
	}

	if (you_win == true)
	{
		camera.Reset();
		RenderTextOnScreen(meshList[GEO_TEXT], "You win",
			Color(1, 1, 1), 4, 10, 20);


		camera.Reset();
		RenderTextOnScreen(meshList[GEO_TEXT], "Press R to replay",
			Color(1, 1, 1), 4, 5, 10);
	}

	if (game_over == true)
	{
		camera.Reset();
		RenderTextOnScreen(meshList[GEO_TEXT], "Game Over",
			Color(1, 0, 0), 4, 10, 20);

		RenderTextOnScreen(meshList[GEO_TEXT], "tap X to try again",
			Color(1, 0, 0), 4, 10, 15);
	}
}

void  SceneA2::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}