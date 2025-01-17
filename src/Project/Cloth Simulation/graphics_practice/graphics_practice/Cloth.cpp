#include "Cloth.h"
#include "main.h"
#include "SOIL.h"
#include <ctime>
#include "Control.h"
Cloth::Cloth(float _x, float _y, int _numX, int _numY)
{
	x = _x;
	y = _y;
	numX = _numX;
	numY = _numY;
	numTriangle = 0;
	texture[0] = SOIL_load_OGL_texture
	(
		"towel_color.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	texture[1] = SOIL_load_OGL_texture
		(
		"towel_normal.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
	
	for (int j = 0; j <= numY; j++)
	{
		for (int i = 0; i <= numX; i++)
		{
			//particles: (numX + 1) * (numY + 1) 
			particles.push_back(vec4(
				((-y/2)*j + (y / 2)*(numY-j)) / numY,
				((-x / 2)*i + (x / 2)*(numX - i)) / numX, 
				1,
				1.0));
			particleNormals.push_back(vec4(0.0, 0.0, 0.0, 1.0));
			particleTextures.push_back(vec2((float)i/numX, (float)j/numY));
			shadows.push_back(vec4(0, 0, 0, 0));
		}
	}

	
	for (int j = 0; j <= numY - 1; j++)
	{
		for (int i = 0; i <= numX - 1; i++)
		{
			int offset = (numX + 1) * j;

			if ((i % 2 == 0) && (j % 2 == 0) || (i % 2 == 1) && (j % 2 == 1))
			{
				//upper
				indices.push_back(i + offset);
				indices.push_back(i + 1 + offset);
				indices.push_back(i + numX + 1 + offset);
				numTriangle++;

				//lower
				indices.push_back(i + 1 + offset);
				indices.push_back(i + numX + 1 + offset);
				indices.push_back(i + numX + 2 + offset);
				numTriangle++;
			}

			else
			{
				//upper
				indices.push_back(i + offset);
				indices.push_back(i + 1 + offset);
				indices.push_back(i + numX + 2 + offset);
				numTriangle++;

				//lower
				indices.push_back(i + offset);
				indices.push_back(i + numX + 1 + offset);
				indices.push_back(i + numX + 2 + offset);
				numTriangle++;
			}
		}
	}
}

void Cloth::draw()
{
	static GLint previousInput = 0;
	static GLint currentInput = 1;
	static GLint currentOutput = 2;

	mvstack.push(model_view);

	if (controller.doAnimation)
	{
		computeShader.Use();
		{
			glUniform1f(computeShader("kfr"), controller.kfr);
			glUniform1i(computeShader("isWind"), controller.isWind);
			glUniform1i(computeShader("isPin"), controller.isPin);
			glUniformMatrix4fv(computeShader("sphere"), 1, GL_TRUE, controller.sphere * Translate(controller.sphereX, 0, 0));
			glUniform1f(computeShader("sphereX"), controller.sphereX);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, g_verticesBuffer[previousInput]);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, g_verticesBuffer[currentInput]);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, g_verticesBuffer[currentOutput]);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, g_normalsBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, g_shadowBuffer);

			// Process all vertices.
			glDispatchCompute(1, 1, 1);

			// Make sure, all vertices and normals are written.
			glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, 0);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, 0);
		}
		computeShader.UnUse();
	}
	program.Use();
	{
		glUniform1i(program("isWireframe"), controller.isWireframe);

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(program("TextureColor"), 0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(program("TextureNormal"), 1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferCloth);
		glEnableVertexAttribArray(program["vTexCoord"]);
		glVertexAttribPointer(program["vTexCoord"], 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		
		glBindBuffer(GL_ARRAY_BUFFER, g_normalsBuffer);
		glVertexAttribPointer(program["v_normal"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(program["v_normal"]);
		
		glUniformMatrix4fv(program("ModelView"), 1, GL_TRUE, model_view);
		
		// Draw Lines
		glBindBuffer(GL_ARRAY_BUFFER, g_verticesBuffer[currentOutput]);
		glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		for (int i = 0; i < numTriangle; i++)
			glDrawElements(controller.isWireframe?GL_LINE_LOOP:GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &(indices[i * 3]));

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		// Draw Points
		glBindBuffer(GL_ARRAY_BUFFER, g_verticesBuffer[currentOutput]);
		glVertexAttribPointer(program["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, (numX + 2)*(numY + 1));
	}
	program.UnUse();

	shadowShader.Use();
	{
		glActiveTexture(GL_TEXTURE10);
		glEnable(GL_TEXTURE_2D);
		glUniform1i(shadowShader("TextureColor"), 10);
		glBindTexture(GL_TEXTURE_2D, shadowTexture);

		glBindBuffer(GL_ARRAY_BUFFER, g_shadowBuffer);
		glVertexAttribPointer(shadowShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shadowShader["vPosition"]);

		glUniformMatrix4fv(shadowShader("ModelView"), 1, GL_TRUE, model_view);

		// Draw Lines
		glBindBuffer(GL_ARRAY_BUFFER, g_shadowBuffer);
		glVertexAttribPointer(shadowShader["vPosition"], 4, GL_FLOAT, GL_FALSE, 0, 0);
		
		if (controller.isWireframe == false)
		{
			for (int i = 0; i < numTriangle; i++)
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, &(indices[i * 3]));
		}

		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	shadowShader.UnUse();

	if (controller.doAnimation)
	{
		previousInput = (previousInput + 1) % 3;
		currentInput = (currentInput + 1) % 3;
		currentOutput = (currentOutput + 1) % 3;
	}
	model_view = mvstack.pop();
}

void Cloth::reset()
{
	controller.doAnimation = false;
	controller.isWireframe = false;
	controller.isPin = true;
	controller.isWind = false;
	for (int j = 0; j <= numY; j++)
	{
		for (int i = 0; i <= numX; i++)
		{
			const int size = particles.size() * sizeof(vec4);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[0]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[1]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_verticesBuffer[2]);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, &particles[0], GL_DYNAMIC_DRAW);

			const int shadowSize = shadows.size() * sizeof(vec4);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_shadowBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, shadowSize, &shadows[0], GL_DYNAMIC_DRAW);

			const int normalSize = particleNormals.size() * sizeof(vec4);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, g_normalsBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, normalSize, &particleNormals[0], GL_DYNAMIC_DRAW);
		}
	}
}