#include "Fire.h"

#define SIM_DIM 20
#define N SIM_DIM 
#define IX(i,j) ((i)+(N+2)*(j))


namespace simulator {
	static const int size = (N + 2) * (N + 2);
	static int u[size], v[size], u_prev[size], v_prev[size];
	static int dens[size], dens_prev[size];

	static float h = 1.f / N;

	void simulate(float dt) {

	}
	void add_source(float * x, float * s, float dt)
	{
		int i;
		for (i = 0; i<size; i++) x[i] += dt*s[i];
	}	void diffuse_bad(int b, float * x, float * x0, float diff, float dt)
	{
		int i, j;
		float a = dt*diff*N*N;
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				x[IX(i, j)] = x0[IX(i, j)] + a*(x0[IX(i - 1, j)] + x0[IX(i + 1, j)] +
					x0[IX(i, j - 1)] + x0[IX(i, j + 1)] - 4 * x0[IX(i, j)]);
			}
		}
		set_bnd(N, b, x);
	}

}

void simulate(float dt) {



}

Fire::Fire()
{
}


Fire::~Fire()
{
}
