#include "3rd\glew\include\GL\glew.h"
#include "stdafx.h"
#include "Zombie.h"
#include "Effect.cpp"
#include "Content.h"
#include "3rd\GLFW\include\glfw3.h"
#include "3rd\glm\glm.hpp"
#include "main.h"

Zombie::Zombie(void)
{
}

Zombie::~Zombie(void)
{

}

void Zombie::Init(void)
{
	zombieModel2 = Content::LoadModel("../resources/zombie2plus.3ds");
	myTex2 = Content::LoadTexture("../resources/gun.png");
	posXzombie = 0;
	posYzombie = 0;

	float LO = 0.0005, HI = 0.0010;
	zombiespeed = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));

	for (int i = 0; i < zombieModel2->Elements(); i++)
	{
		zombieModel2->GetMesh(i)->GetMaterial().Color = glm::vec3(1, 1, 0);
		zombieModel2->GetMesh(i)->GetMaterial().texture = myTex2;
	}
	shaderColor2 = new Effect("shaders/color");
	shaderColor2->CreateShader();
}

void Zombie::Update(Model* zombieModel3, float posXbullet, float posYbullet, float posX, float posY)
{
	int i = 0;
	anglezombie = atan2(posYzombie - posY, posXzombie - posX) * (180 / 3.14);
	shaderColor2->GetParameter("matColor")->SetValue(zombieModel2->GetMesh(i)->GetMaterial().Color);

	if (zombieModel2->GetMesh(i)->GetMaterial().texture != NULL)
	{
		shaderColor2->GetParameter("mode")->SetValue(1.0f);
		shaderColor2->GetParameter("tex")->SetValue(*zombieModel2->GetMesh(i)->GetMaterial().texture);
	}
	else
	{
		shaderColor2->GetParameter("mode")->SetValue(-1.0f);
	}
	int losujx, losujy;
	for (int i = 0; i < zombieModel2->Elements(); i++)
	{
		if (respawn == 0)
		{
			losujx = -20 + (rand() % static_cast<int>(20 - -20 + 1));
			if (losujx>10||losujx<-10)
			{
				posXzombie = losujx;
			}
			losujy = -20 + (rand() % static_cast<int>(20 - -20 + 1));
			if (losujy>10 || losujy<-10)
			{
				posYzombie = losujy;
			}
		}

		posXzombie = posXzombie - zombiespeed * (float)cos(anglezombie * 3.14 / 180.0f);
		posYzombie = posYzombie - zombiespeed * (float)sin(anglezombie * 3.14 / 180.0f);

		shaderColor2->GetParameter("World")->SetValue(
			zombieModel2->GetMesh(i)->getLocalWorld() *
			glm::translate(glm::mat4(1.0f), glm::vec3(posXzombie, posYzombie, 1)) *
			glm::rotate(glm::mat4(1.0f), anglezombie - 90.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(0.02, 0.02, 0.02))
		);

		stop = clock();
		if ((start2)+1000<stop)
		{
			start2 = clock();
			respawn = 1;
		}
		if (respawn==1)
		{
			zombieModel2->GetMesh(i)->Draw();
			if (posXbullet >= posXzombie - 0.2 && posXbullet <= posXzombie + 0.2)
			{
				if (posYbullet >= posYzombie - 0.2 && posYbullet <= posYzombie + 0.2)
				{
					respawn = 0;
					kills += 1;
				}
			}

			if (posX >= posXzombie - 0.2 && posX <= posXzombie + 0.2)
			{
				if (posY >= posYzombie - 0.2 && posY <= posYzombie + 0.2)
				{
					respawn = 0;
					if (kills > 11)
					{
						glfwSetWindowShouldClose(glfwGetCurrentContext(), GL_TRUE);
					}
				}
			}
		}
	}
}
