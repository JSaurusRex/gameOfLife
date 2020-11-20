#version 330


// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;


// Input uniform values
uniform sampler2D texture0;


// Output fragment color
out vec4 finalColor;



vec2 Position (float x, float y)
{
    return vec2(x / textureSize(texture0, 0).x, y / textureSize(texture0, 0).y);
}

int checkNeighbour (int x, int y)
{
    vec2 pos = Position(gl_FragCoord.x - x,gl_FragCoord.y -y);
    vec4 color = texture(texture0, pos);
    int returnValue = 0;
    if(color.x != 0) returnValue = 1;
    return returnValue;
}

void main()
{
    int neighbours = 0;
	vec2 pos = Position(gl_FragCoord.x, gl_FragCoord.y);
	finalColor = texture(texture0, pos);
	bool alive = false;
	if(finalColor.x != 0) alive = true;
	vec4 white = vec4(1,1,1,1);
	finalColor = vec4(0,0,0,1);

	//1 1
	neighbours += checkNeighbour(1,1);
	neighbours += checkNeighbour(0,1);
	neighbours += checkNeighbour(-1,1);

	neighbours += checkNeighbour(1,0);
	neighbours += checkNeighbour(-1,0);

	neighbours += checkNeighbour(1,-1);
	neighbours += checkNeighbour(0,-1);
	neighbours += checkNeighbour(-1,-1);




	if((neighbours == 2 || neighbours == 3) && alive) finalColor = white;
	if(neighbours == 3 && !alive) finalColor = white;
}
