#include "Bunny.h"

Bunny::Bunny()
{
  position.X = position.Y = position.Z = 0;
  direction.X = 0.0;
  direction.Y = direction.Z = 0;
  stopped = hit = false;

}

Bunny::Bunny(float px, float pz, float dx, float dz)
{
  /*
  AABBmin.X = px - 2;
  AABBmax.X = px + 2;
  AABBmin.Y = -2;
  AABBmax.Y = 2;
  AABBmin.Z = pz - 2;
  AABBmax.Z = pz + 2;

  position.X = px;
  position.Y = 0.0; 
  position.Z = pz;
  direction.X = dx;
  direction.Y = 0.0;
  direction.Z = dz;
  */
  AABBmin.X = -1;
  AABBmax.X = 1;
  AABBmin.Y = -1;
  AABBmax.Y = 1;
  AABBmin.Z = -1;
  AABBmax.Z = 1;

  position.X = 0;
  position.Y = 0.0; 
  position.Z = 0;
  direction.X = 0;
  direction.Y = 0.0;
  direction.Z = 0;
  stopped = hit = false;
}

Bunny::~Bunny()
{
}

void Bunny::step(float delta)
{
  //if(!stopped && !hit)
  //{
    position.X += direction.X * delta;
    AABBmin.X += direction.X * delta;
    AABBmax.X += direction.X * delta;
    /*
    if(position.X > 10.0){
      position.X = 10.0;
      //direction.X *= -1.0;
    }

    if(position.X < -10.0){
      position.X = -10.0;
      //direction.X *= -1.0;
    }
    */

    position.Z += direction.Z * delta;
    AABBmin.Z += direction.Z * delta;
    AABBmax.Z += direction.Z * delta;
    if(position.Z > 10.0){
       printf("hey3\n");
      position.Z = 10.0;
      direction.Z *= -1.0;
    }
    if(position.Z < -10.0){
       printf("hey4\n");
      position.Z = -10.0;
      direction.Z *= -1.0;
    }
  //}
}

void Bunny::draw(CShader * Shader, GLuint PositionBufferHandle, GLuint ColorBufferHandle, int TriangleCount)
{
		// Shader context works by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(* Shader);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);

		glPushMatrix();
		glTranslatef(position.X, position.Y, position.Z);
		//glRotatef(Rotation.Z, 0, 0, 1);
		//glRotatef(Rotation.Y, 0, 1, 0);
		//glRotatef(Rotation.X, 1, 0, 0);
		//glScalef(Scale.X, Scale.Y, Scale.Z);

      glutSolidSphere(0.5, 10, 10);
		//glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);

		glPopMatrix();
}
