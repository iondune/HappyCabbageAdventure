#include "CRenderable.h"


CRenderable::CRenderable(CMesh const & Mesh)
	: Scale(1)
{
	std::vector<GLfloat> Vertices;
	std::vector<GLfloat> Colors;
	std::vector<GLushort> Indices;
	std::vector<GLfloat> TexCoords;

	for (unsigned int i = 0; i < Mesh.Vertices.size(); ++ i)
	{
		for (unsigned int j = 0; j < 3; ++ j)
			Vertices.push_back(Mesh.Vertices[i].Position[j]);
		for (unsigned int j = 0; j < 3; ++ j)
			Colors.push_back(Mesh.Vertices[i].Color[j]);
		for (unsigned int j = 0; j < 2; ++ j)
			TexCoords.push_back(Mesh.Vertices[i].TextureCoordinates[j]);
	}

	for(unsigned int i = 0; i < Mesh.Triangles.size(); ++ i) 
		for (unsigned int j = 0; j < 3; ++ j)
			Indices.push_back(Mesh.Triangles[i].Indices[j]);

	glGenBuffers(1, & PositionBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(GLfloat), & Vertices.front(), GL_STATIC_DRAW);

	glGenBuffers(1, & ColorBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, Colors.size()*sizeof(GLfloat), & Colors.front(), GL_STATIC_DRAW);

	glGenBuffers(1, & IndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size()*sizeof(GLushort), & Indices.front(), GL_STATIC_DRAW);

	glGenBuffers(1, & TexCoordBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, TexCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, TexCoords.size()*sizeof(GLfloat), & TexCoords.front(), GL_STATIC_DRAW);

	IndexCount = Indices.size();
}


SVector3 const & CRenderable::getTranslation() const
{
	return Translation;
}

SVector3 const & CRenderable::getRotation() const
{
	return Rotation;
}

SVector3 const & CRenderable::getScale() const
{
	return Scale;
}


void CRenderable::setTranslation(SVector3 const & translation)
{
	Translation = translation;
}

void CRenderable::setRotation(SVector3 const & rotation)
{
	Rotation = rotation;
}

void CRenderable::setScale(SVector3 const & scale)
{
	Scale = scale;
}

void CRenderable::draw(CShader const & Shader)
{
	CShaderContext ShaderContext(Shader);
	ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 3);
	ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);
	ShaderContext.bindBuffer(IndexBufferHandle);

	glPushMatrix();

	glTranslatef(Translation.X, Translation.Y, Translation.Z);
	glRotatef(Rotation.Z, 0, 0, 1);
	glRotatef(Rotation.Y, 0, 1, 0);
	glRotatef(Rotation.X, 1, 0, 0);
	glScalef(Scale.X, Scale.Y, Scale.Z);

	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);

	glPopMatrix();
}

void CRenderable::draw(CShader const & Shader, CTexture const & Texture)
{
	CShaderContext ShaderContext(Shader);
	ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 3);
	ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);
	ShaderContext.bindBuffer("aTexCoord", TexCoordBufferHandle, 2);
	ShaderContext.bindBuffer(IndexBufferHandle);

	ShaderContext.uniform("uTexColor", 0);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture.TextureHandle);

	glPushMatrix();

	glTranslatef(Translation.X, Translation.Y, Translation.Z);
	glRotatef(Rotation.Z, 0, 0, 1);
	glRotatef(Rotation.Y, 0, 1, 0);
	glRotatef(Rotation.X, 1, 0, 0);
	glScalef(Scale.X, Scale.Y, Scale.Z);

	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_SHORT, 0);


	glPopMatrix();
}
