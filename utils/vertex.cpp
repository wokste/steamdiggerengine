#include "vertex.h"

Vertex::Vertex(sf::Vector3f pos, float textureX, float textureY) : x(pos.x), y(pos.y), z(pos.z), textureX(textureX), textureY(textureY)
{
};

Vertex operator+(const Vertex& left, const sf::Vector3f& right){
	return Vertex(sf::Vector3f(left.x + right.x, left.y + right.y, left.z + right.z), left.textureX, left.textureY);
};

void VertexArray::clear(){
	vertices.clear();
};

void VertexArray::createCube(sf::Vector3f pos, int frameNum, Vector2i& framesPerSheet){
	const sf::Vector3f AxisX = sf::Vector3f(1,0,0);
	const sf::Vector3f AxisY = sf::Vector3f(0,1,0);
	const sf::Vector3f AxisZ = sf::Vector3f(0,0,1);

	// Back Face
	//addSheet(pos + AxisZ, AxisX, AxisY, frameNum, framesPerSheet);

	// Front Face
	addSheet(pos        , AxisY, AxisX, frameNum, framesPerSheet);

	// Bottom Face
	addSheet(pos + AxisY, AxisZ, AxisX, frameNum, framesPerSheet);

	// Top Face
	addSheet(pos        , AxisX, AxisZ, frameNum, framesPerSheet);

	// Right face
	addSheet(pos + AxisX, AxisY, AxisZ, frameNum, framesPerSheet);

	// Left Face
	addSheet(pos + AxisY,-AxisY, AxisZ, frameNum, framesPerSheet);
}

void VertexArray::addSheet(const sf::Vector3f start, const sf::Vector3f& dir1, const sf::Vector3f& dir2, int frameNum, Vector2i& framesPerSheet){
	GLfloat texLeft = (GLfloat)(frameNum % framesPerSheet.x) / framesPerSheet.x;
	GLfloat texRight = texLeft + (1.0 / framesPerSheet.x);
	GLfloat texTop = (GLfloat)(frameNum / framesPerSheet.x) / framesPerSheet.y;
	GLfloat texBottom = texTop + (1.0 / framesPerSheet.y);

	vertices.push_back(Vertex(start              , texLeft , texTop));
	vertices.push_back(Vertex(start + dir1       , texLeft , texBottom));
	vertices.push_back(Vertex(start + dir2       , texRight, texTop));

	vertices.push_back(Vertex(start + dir1       , texLeft , texBottom));
	vertices.push_back(Vertex(start + dir1 + dir2, texRight, texBottom));
	vertices.push_back(Vertex(start + dir2       , texRight, texTop));
}

void VertexArray::render() const{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &(vertices[0].x));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &(vertices[0].textureX));
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY );
}
