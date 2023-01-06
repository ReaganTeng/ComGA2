#ifndef SCENEA2_H
#define SCENEA2_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class SceneA2 : public Scene
{

	MS modelStack, viewStack, projectionStack;

	enum GEOMETRY_TYPE
	{
		GEO_AXES,

		GEO_LIGHTBALL_1,
		GEO_LIGHTBALL_2,

		//skyboxes
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//body
		GEO_HEAD,
		GEO_LIMBS,
		GEO_PALM,
		GEO_FEET,
		GEO_BLADE,
		GEO_HANDLE,
		GEO_EYES,
		GEO_MOUTH,

		GEO_TREE,
		GEO_GRASS,
		GEO_ROCKS,

		GEO_WOOD1,
		GEO_WOOD2,
		GEO_WOOD3,
		GEO_WOOD4,
		GEO_WOOD5,
		GEO_WOOD6,
		GEO_WOOD7,
		GEO_WOOD8,
		GEO_WOOD9,
		GEO_WOOD10,

		GEO_CAMPFIRE,
		GEO_BENCH,
		GEO_TENT,
		GEO_CHEST,

		GEO_MONSTERS1,
		GEO_MONSTERS2,
		GEO_MONSTERS3,
		GEO_MONSTERS4,

		GEO_PLAYERSPHERE,

		GEO_TEXT,

		GEO_GROUND,

		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,


		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHTENABLED,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,


		U_TEXT_ENABLED,
		U_TEXT_COLOR,


		U_TOTAL
	};
public:
	SceneA2();
	~SceneA2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	void RenderSkybox();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color
		color, float size, float x, float y);

	float translate_x;
	float translate_z;

	float shooting_time;
	Vector3 position;

	float timer_scene_1;
	float timer_scene_2;
	float timer_scene_3;


	//ALL THE WOOD PROPERTIES
		int coordinates_x[10];
		int coordinates_z[10];

		bool wood1_disappear;
		bool wood2_disappear;
		bool wood3_disappear;
		bool wood4_disappear;
		bool wood5_disappear;
		bool wood6_disappear;
		bool wood7_disappear;
		bool wood8_disappear;
		bool wood9_disappear;
		bool wood10_disappear;

		bool can_pick_wood1;
		bool can_pick_wood2;
		bool can_pick_wood3;
		bool can_pick_wood4;
		bool can_pick_wood5;
		bool can_pick_wood6;
		bool can_pick_wood7;
		bool can_pick_wood8;
		bool can_pick_wood9;
		bool can_pick_wood10;
		

		
	//ALL THE MONSTER PROPERTIES
	bool monster1_die;
	bool monster2_die;
	bool monster3_die;
	bool monster4_die;
	int monster1_health;
	int monster2_health;
	int monster3_health;
	int monster4_health;

	int limit;
	
	float range;
	float nrange;

	int health;
	

	bool minigame_1;
	bool minigame_2;
	bool minigame_3;

	bool game_over;

	bool pick_up_chest_mode;
	bool pick_up_bench_mode;
	bool pick_up_tent_mode;

	bool you_win;


	

	Camera3 camera;
	Light light[2];

	
};

#endif