#include <stddef.h>
#include "common_defs.h"

#pragma once

//��������� ������

struct SnakeParams{

	float alpha;
	float beta;
	float gamma;
	float kappa;
	int window;

	SnakeParams()
	{
		alpha=SP_ALPHA;
		beta=SP_BETA;
		gamma=SP_GAMMA;
		kappa=SP_KAPPA;

		window = 2;
	}
};

struct TSnakeParameters
{
	float alpha, beta, gamma, kappa; 
	int iterations_number;
	int window;

	TSnakeParameters(): alpha(1.0f), beta(2.0f), gamma(15.0f), kappa(4.0f), iterations_number(30), window (2)  {}
};


class ShaderSnake
{
public:
	
	// Performs initialization
	bool Init();

	// Prepares snake to work
	bool GetReady();

	// Sets initial snake position(replaces old with a new if called multiple times)
	void AddSeed(int x, int y);

	// Sets snake params and image to work with
	bool FixParams(int h_image, int width, int height, int components, TSnakeParameters& params);

	// Returns pointer to algorithm output
	Texture2D* Output() const;

	// Performs an iteration
	int Iterate();

	// Checks if all initializations has been done successfully
	bool IsReady() const;

	int GetSize();

	ShaderSnake();
	~ShaderSnake();

private:
	// Snake points
	Texture2D* m_points_1;
	Texture2D* m_points_2;
	int m_size;
	bool m_odd_iter;

	// Shader stuff
	ShaderManager* m_shader;
	FrameBuffer *m_FBO_1;
	FrameBuffer *m_FBO_2;

	// Params
	TSnakeParameters m_params;

	// Image
	Texture2D* m_image;

	void Cleanup();
	void Resample();

	void Reset();
};