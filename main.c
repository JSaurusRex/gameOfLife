#include <stdio.h>
//#include <stdlib.h>
#include "raylib.h"

#define GLSL_VERSION            330
int main (int argc, char **argv)
{
	
	if(argc < 2)
	{
		printf("Missing image\n");
		return 0;
	}
	int limit = 1;
	char * p;
	if(argc == 3) limit = strtol(argv[2], &p, 10);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "Game Of Life");
	//rlDisableDepthTest();
	//rlDisableBackfaceCulling();
	//rlDisableScissorTest();

	Texture2D tex = LoadTexture(argv[1]);
	

	Shader shad = LoadShader(0, "fragment.shader");
	SetTargetFPS(20);
	Vector2 mousePos;
	RenderTexture2D renTex = LoadRenderTexture(tex.width,tex.height);
	RenderTexture2D ren2Tex = LoadRenderTexture(tex.width,tex.height);
	RenderTexture2D rentmp;
	BeginDrawing();
		ClearBackground(BLACK);
		BeginTextureMode(ren2Tex);
			BeginShaderMode(shad);
				DrawTextureRec(tex, (Rectangle){ 0, 0, tex.width, tex.height }, (Vector2){ 0, 0 }, WHITE);
			EndShaderMode();
		EndTextureMode();
	EndDrawing();
	float size = 1;
	float x = 0, y = 0;
	bool fpsLimit = true;
	int totalCycles = 0;
	
	while (!WindowShouldClose()) {
		int WIDTH = GetScreenWidth();
		int HEIGHT = GetScreenHeight();
		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			x -= (GetMousePosition().x - mousePos.x) * 0.4 / size;
			y -= (GetMousePosition().y - mousePos.y) * 0.4 / size;
			if(x < 0) x = 0;
			if(x > tex.width - (tex.width / size)) x = tex.width - (tex.width / size);
			if(y < 0) y = 0;
			if(y > tex.height - (tex.height / size)) y = tex.height - (tex.height / size);
			//printf("\n%f %f", x, y);
		}
		mousePos = GetMousePosition();
		float oldSize = size;
		size += (float)GetMouseWheelMove() / 10;
		

		if(size != oldSize)
		{
			if(size < 1) size = 1;
			x -= (((float)WIDTH / size) - ((float)WIDTH / oldSize))  / 5;
			y -= (((float)HEIGHT / size) - ((float)HEIGHT / oldSize))  / 5;
			if(x < 0) x = 0;
			if(x > tex.width - (tex.width / size)) x = tex.width - (tex.width / size);
			if(y < 0) y = 0;
			if(y > tex.height - (tex.height / size)) y = tex.height - (tex.height / size);
		}
		if(IsKeyPressed(KEY_SPACE)) 
		{
			fpsLimit = !fpsLimit;
			SetTargetFPS(20 * fpsLimit);
			//0 is no limit
		}

		if(IsKeyPressed(KEY_MINUS)) {limit /= 10; }
		if(IsKeyPressed(KEY_EQUAL)) {limit *= 10; }
		if(limit != 1 && fpsLimit) {SetTargetFPS(0); fpsLimit = false;}
		printf("\nCycles done so far = %i		per second: %i", totalCycles, (int)(totalCycles/GetTime()));
		//rentmp = renTex;
		//renTex = ren2Tex;
		//ren2Tex = rentmp;
		
		//ClearBackground(BLACK);
		//while((frameTime - clock()) / CLOCKS_PER_SEC < 0.05)
		BeginDrawing();
		for(int i = limit; i > 0; i--)
		{
			totalCycles++;
			BeginTextureMode(ren2Tex);
				BeginShaderMode(shad);
					DrawTextureRec(ren2Tex.texture, (Rectangle){ 0, 0, tex.width, tex.height }, (Vector2){ 0, 0 }, WHITE);
				EndShaderMode();
			EndTextureMode();
			rentmp = renTex;
			renTex = ren2Tex;
			ren2Tex = rentmp;
		}
			//ClearBackground(WHITE);
			//DrawTextureRec(ren2Tex.texture, (Rectangle){ 0 , 0 , (renTex.texture.width ) / size, -((renTex.texture.height) / size) }, (Vector2){ x, y }, WHITE);
		
			DrawTexturePro(ren2Tex.texture, (Rectangle){x, y, (tex.width) / size, (tex.height) / size}, (Rectangle){0, 0, WIDTH, HEIGHT}, (Vector2){0,0}, 0, WHITE);
			DrawFPS(5,5);
		EndDrawing();
	}

	UnloadTexture(tex);
	UnloadShader(shad);
	CloseWindow();
	return 0;

}
