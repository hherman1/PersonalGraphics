#include "Fire.h"

#define SIM_DIM 100
#define N SIM_DIM 
#define ARR_DIM (N+2)
#define IX(i,j) ((i)+(N+2)*(j))
#define S_IX(sparse) IX((sparse).x,(sparse).y)
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}


struct sparse_coords {
	int x;
	int y;
};

using namespace glm;
using namespace std;
namespace simulator {
	static const int size = (N + 2) * (N + 2);
	static float u[size], v[size], u_prev[size], v_prev[size] = { };
	static float dens[size], dens_prev[size] = {};
	static float temp[size], temp_prev[size] = {};
	static float react[size], react_prev[size] = {};

	static vector<sparse_coords> generators_coords(size);
	static int generators[size]= {};

	static int objects[size] = { 0 };

	static float u_in[size], v_in[size];
	static float dens_in[size];
	static float temp_in[size];

	static const float visc = .001;
	static const float diff = 0.00005;
	static const float temperatureVelocityMultiplier = 10;
	static const float temperatureHorizontalMultiplier = 60;
	static const float reactDeterioration = 2;

	static const float genMultiplier = 0.2;

	static float h = 1.f / N;

	static const float inMag = 10000;
	static const float velMag = 200;

	int getSize() {
		return size;
	}
	int getDim() {
		return N+2;
	}
	float* getDens() {
		return dens;
	}
	float* getReact() {
		return react;
	}
	int* getGens() {
		return generators;
	}
	int* getObjs() {
		return objects;
	}

	
	void set_bnd(int b, float * x);
	void simulate(float dt);
	void addTemperatureVelocity(float * T, float * v,float * u, float dt);
	void add_source(float * x, float * s, float dt);
	void diffuse(int b, float * x, float * x0, int * objs, float diff, float dt);
	void advect(int b, float * d, float * d0, float * u, float * v, float dt);
	void dens_step(float * x, float * x0, float * u, float * v, float diff, float dt);
	void obj_step(int * objs, float * u, float * v, float * u0, float * v0);
	void vel_step(float * u, float * v, float * u0, float * v0, float visc, float dt);
	void gen_step(vector<sparse_coords> coords, float * target, float coeff);
	void project(float * u, float * v, float * p, float * div);
	void react_step(float * r, float dt);

	void set_vect(vec2 vec, int x, int y) {
		u_in[IX(x, y)] = vec.x;
		v_in[IX(x, y)] = vec.y;
	}

	void inputDens(vec2 coords) {
		int x_coord = floor(coords.x*ARR_DIM);
		int y_coord = ARR_DIM - floor(coords.y*ARR_DIM);
		dens_in[IX(x_coord, y_coord)] = inMag;
	}
	void inputTemp(vec2 coords) {
		int x_coord = floor(coords.x*ARR_DIM);
		int y_coord = ARR_DIM - floor(coords.y*ARR_DIM);
		temp_in[IX(x_coord, y_coord)] = inMag;
	}
	void inputVel(vec2 coords, vec2 vel) {
		int x_coord = floor(coords.x*ARR_DIM);
		int y_coord = ARR_DIM - floor(coords.y*ARR_DIM);
		set_vect(velMag * vec2(1, -1) *	vel, x_coord, y_coord);
	}
	void inputObjs(vec2 coords) {
		int x_coord = floor(coords.x*ARR_DIM);
		int y_coord = ARR_DIM - floor(coords.y*ARR_DIM);
		objects[IX(x_coord, y_coord)] = 1;//!objects[IX(x_coord, y_coord)];
	}
	void inputGens(vec2 coords) {
		int x_coord = floor(coords.x*ARR_DIM);
		int y_coord = ARR_DIM - floor(coords.y*ARR_DIM);
		generators[IX(x_coord, y_coord)] = 1;//!objects[IX(x_coord, y_coord)];
		bool found = false;
		for (int i = 0; i < generators_coords.size(); i++) {
			if (generators_coords[i].x == x_coord && generators_coords[i].y == y_coord) {
				found = true;
			}
		}
		if (!found) {
			generators_coords.push_back({ x_coord,y_coord });
		}
	}

	void apply_inputs(float dt) {
		std::fill(dens_prev, dens_prev + size, 0);
		std::fill(u_prev, u_prev + size, 0);
		std::fill(v_prev, v_prev + size, 0);
		std::fill(temp_prev, temp_prev + size, 0);
		std::fill(react_prev, react_prev + size, 0);
		add_source(v_prev, v_in, dt);
		add_source(u_prev, u_in, dt);
		add_source(dens_prev, dens_in, dt);
		add_source(temp_prev, temp_in, dt);

	}
	void clear_inputs() {
		std::fill(dens_in, dens_in + size, 0);
		std::fill(temp_in, temp_in + size, 0);
		std::fill(u_in, u_in + size, 0);
		std::fill(v_in, v_in + size, 0);
	}
	void simulate(float dt) {
		for (int i = 20; i < 40; i++) {
			for (int j = 20; j < 40; j++) {
				//objects[IX(i, j)] = true;
			}
		}
		apply_inputs(dt);
		addTemperatureVelocity(temp, v_prev,u_prev, dt);
		vel_step( u, v, u_prev, v_prev, visc, dt);
		obj_step(objects, u, v, u_prev, v_prev);
		gen_step(generators_coords, temp_prev, 10);
		gen_step(generators_coords, dens_prev, 10);
		gen_step(generators_coords, react_prev, 20);
		//gen_step(objects, u, v, u_prev, v_prev);
		react_step(react,dt);
		dens_step(dens, dens_prev, u, v, diff, dt);
		dens_step(temp, temp_prev, u, v, 0, dt);
		dens_step(react, react_prev, u, v, 0, dt);
		clear_inputs();
		//draw_dens( dens);
	}
	void react_step(float * r, float dt) {
		for (int i = 0; i < ARR_DIM; i++) {
			for (int j = 0; j < ARR_DIM; j++) {
				r[IX(i, j)] -= dt * reactDeterioration;
				r[IX(i, j)] *= r[IX(i, j)] >= 0;
			}
		}
	}

	void addTemperatureVelocity(float * T, float * v,float * u, float dt) {
		for (int i = 0; i < ARR_DIM; i++) {
			for (int j = 0; j < ARR_DIM; j++) {
				v[IX(i, j)] += T[IX(i, j)] * temperatureVelocityMultiplier * dt;
				float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				u[IX(i, j)] += T[IX(i, j)] * temperatureHorizontalMultiplier* dt*(r*2-1);
			}
		}
	}

	void add_source(float * x, float * s, float dt)
	{
		int i;
		for (i = 0; i<size; i++) x[i] += dt*s[i];
	}
	void diffuse( int b, float * x, float * x0,int * objs, float diff, float dt)
	{
		int i, j, k;
		float a = dt*diff*N*N;
		for (k = 0; k<20; k++) {
			for (i = 1; i <= N; i++) {
				for (j = 1; j <= N; j++) {
					x[IX(i, j)] = !objs[IX(i,j)] * (x0[IX(i, j)] 
						+ a*
						(!objs[IX(i-1,j)] * x[IX(i - 1, j)] 
							+ !objs[IX(i + 1, j)] * x[IX(i + 1, j)]
							+ !objs[IX(i, j-1)] * x[IX(i, j - 1)]
							+ !objs[IX(i, j+1)] * x[IX(i, j + 1)])
						+ a*(objs[IX(i - 1, j)] + objs[IX(i, j + 1)] + objs[IX(i + 1, j)] + objs[IX(i, j - 1)])*x0[IX(i, j)])
						/ (1 + 4*a);
				}
			}
			set_bnd(b, x);
		}
	}

	void zeroObjects(float * x, int * objs) {
		for (int i = 0; i < ARR_DIM; i++) {
			for (int j = 0; j < ARR_DIM; j++) {
				x[IX(i, j)] *= !(objs[IX(i-1, j)] || objs[IX(i+1, j)] || objs[IX(i, j-1)] || objs[IX(i, j+1)]);
			}
		}
	}
	void advect( int b, float * d, float * d0, float * u, float * v, float dt)
	{
		int i, j, i0, j0, i1, j1;
		float x, y, s0, t0, s1, t1, dt0;
		dt0 = dt*N;
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				x = i - dt0*u[IX(i, j)]; y = j - dt0*v[IX(i, j)];
				if (x < 0.5) x = 0.5; if (x > N + 0.5) x = N + 0.5; i0 = (int)x; i1 = i0 + 1;
				if (y < 0.5) y = 0.5; if (y > N + 0.5) y = N + 0.5; j0 = (int)y; j1 = j0 + 1;
				s1 = x - i0; s0 = 1 - s1; t1 = y - j0; t0 = 1 - t1;
				d[IX(i, j)] = s0*(t0*d0[IX(i0, j0)] + t1*d0[IX(i0, j1)]) +
					s1*(t0*d0[IX(i1, j0)] + t1*d0[IX(i1, j1)]);
			}
		}
		set_bnd(b, d);
	}
	void dens_step(float * x, float * x0, float * u, float * v, float diff,
		float dt)
	{
		add_source( x, x0, dt);
		SWAP(x0, x); diffuse(0, x, x0, objects, diff, dt);
		SWAP(x0, x); advect(0, x, x0, u, v, dt);
	}

	void obj_step(int* objs, float * u, float * v, float * u0, float * v0) {
		zeroObjects(u, objs);
		zeroObjects(v, objs);
		//	project(u, v, u0, v0);
	}
	void gen_step(vector<sparse_coords> coords, float * target,float coeff) {
		for (int i = 0; i < coords.size(); i++) {
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			target[S_IX(coords[i])] += coeff * r;
		}
	}

	void vel_step(float * u, float * v, float * u0, float * v0,
		float visc, float dt)
	{
		add_source( u, u0, dt); add_source( v, v0, dt);
		SWAP(u0, u); diffuse( 1, u, u0, objects, visc, dt);
		SWAP(v0, v); diffuse( 2, v, v0, objects, visc, dt);
		project( u, v, u0, v0);
		SWAP(u0, u); SWAP(v0, v);
		advect( 1, u, u0, u0, v0, dt); advect( 2, v, v0, u0, v0, dt);
		project( u, v, u0, v0);
	}

	void project( float * u, float * v, float * p, float * div)
	{
		int i, j, k;
		float h;
		h = 1.0 / N;
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				div[IX(i, j)] = -0.5*h*(u[IX(i + 1, j)] - u[IX(i - 1, j)] +
					v[IX(i, j + 1)] - v[IX(i, j - 1)]);
				p[IX(i, j)] = 0;
			}
		}
		set_bnd(0, div); set_bnd( 0, p);
		for (k = 0; k<20; k++) {
			for (i = 1; i <= N; i++) {
				for (j = 1; j <= N; j++) {
					p[IX(i, j)] = (div[IX(i, j)] + p[IX(i - 1, j)] + p[IX(i + 1, j)] +
						p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
				}
			}
			set_bnd(0, p);
		}
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				u[IX(i, j)] -= 0.5*(p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
				v[IX(i, j)] -= 0.5*(p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
			}
		}
		set_bnd( 1, u); set_bnd( 2, v);
	}

	void set_bnd(int b, float * x)
	{
		int i;
		for (i = 1; i <= N; i++) {
			x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
			x[IX(N + 1, i)] = b == 1 ? -x[ IX(N, i) ] : x[IX(N, i)];
			x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
			x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
		}
		x[IX(0, 0)] = 0.5*(x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N + 1)] = 0.5*(x[IX(1, N + 1)] + x[IX(0, N)]);
		x[IX(N + 1, 0)] = 0.5*(x[IX(N, 0)] + x[IX(N + 1, 1)]);
		x[IX(N + 1, N + 1)] = 0.5*(x[IX(N, N + 1)] + x[IX(N + 1, N)]);
	}





}

FireRender::FireRender()
{
	GLuint quadGrid = utils::makeQuadGrid(simulator::getDim());
	Indices quadGridIndices = utils::makeQuadGridIndices(simulator::getDim());

	EBO = quadGridIndices.EBO;
	VBO = quadGrid;
	elements = quadGridIndices.elements;

	glGenBuffers(1, &DBO);
	glBindBuffer(GL_ARRAY_BUFFER, DBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*simulator::getSize(), simulator::getDens(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &RBO);
	glBindBuffer(GL_ARRAY_BUFFER, RBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*simulator::getSize(), simulator::getReact(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &OBO);
	glBindBuffer(GL_ARRAY_BUFFER, OBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*simulator::getSize(), simulator::getObjs(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &GBO);
	glBindBuffer(GL_ARRAY_BUFFER, GBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLint)*simulator::getSize(), simulator::getGens(), GL_DYNAMIC_DRAW);


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, DBO);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 1, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, RBO);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 1, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, OBO);
	glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(GLint) * 1, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, GBO);
	glVertexAttribPointer(4, 1, GL_INT, GL_FALSE, sizeof(GLint) * 1, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FireRender::update() {
	glBindBuffer(GL_ARRAY_BUFFER, DBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * simulator::getSize(), simulator::getDens());
	glBindBuffer(GL_ARRAY_BUFFER, RBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * simulator::getSize(), simulator::getReact());
	glBindBuffer(GL_ARRAY_BUFFER, OBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLint) * simulator::getSize(), simulator::getObjs());
	glBindBuffer(GL_ARRAY_BUFFER, GBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLint) * simulator::getSize(), simulator::getGens());
}
FireRender::~FireRender()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &DBO);
	glDeleteBuffers(1, &RBO);
	glDeleteBuffers(1, &OBO);
	glDeleteBuffers(1, &GBO);
	glDeleteVertexArrays(1, &VAO);
}

void FireRender::draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
}

