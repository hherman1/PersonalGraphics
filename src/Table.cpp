#include "Table.h"



using namespace basicgraphics;
using namespace std;

Table::Table()
{
	trump_texture.bind();
	trump_texture.loadImage("trump.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	trump_texture.unbind();
	white_texture.bind();
	white_texture.whiteTexture();
}


Table::~Table()
{
}

void Table::draw(basicgraphics::GLSLProgram & shader)
{
	drawWall(shader);
	standard_shader::setTexture(shader, white_texture);
	drawBoard(shader);
	drawNet(shader);
	drawLegs(shader);
}
void Table::trump() {
	_trump = !_trump;
}
void Table::drawLegs(basicgraphics::GLSLProgram & shader) {

	shared_ptr<ArrayMesh> cube = utils::getCube();
	standard_shader::setMaterial(shader, {
	vec3(0.19225),
	vec3(0.50754),
	vec3(0.508273),
	128 * 0.4f
	});
	//front right
	mat4 model;
	model = mat4(1.f);
	model = translate(model, vec3(TABLE_RIGHT - TABLE_LEG_SIZE, TABLE_HEIGHT / 2, TABLE_FRONT - TABLE_LEG_SIZE));
	model = scale(model, vec3(TABLE_LEG_SIZE / 2, TABLE_HEIGHT / 2, TABLE_LEG_SIZE / 2));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);
	//front left
	model = mat4(1.f);
	model = translate(model, vec3(TABLE_LEFT + TABLE_LEG_SIZE, TABLE_HEIGHT / 2, TABLE_FRONT - TABLE_LEG_SIZE));
	model = scale(model, vec3(TABLE_LEG_SIZE / 2, TABLE_HEIGHT / 2, TABLE_LEG_SIZE / 2));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);
	//back right
	model = mat4(1.f);
	model = translate(model, vec3(TABLE_RIGHT - TABLE_LEG_SIZE, TABLE_HEIGHT / 2, TABLE_BACK + TABLE_LEG_SIZE));
	model = scale(model, vec3(TABLE_LEG_SIZE / 2, TABLE_HEIGHT / 2, TABLE_LEG_SIZE / 2));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);
	//back left
	model = mat4(1.f);
	model = translate(model, vec3(TABLE_LEFT + TABLE_LEG_SIZE, TABLE_HEIGHT / 2, TABLE_BACK + TABLE_LEG_SIZE));
	model = scale(model, vec3(TABLE_LEG_SIZE / 2, TABLE_HEIGHT / 2, TABLE_LEG_SIZE / 2));
	standard_shader::setModel(shader, model);
	standard_shader::drawArrayMesh(*cube);
}


void Table::drawBoard(basicgraphics::GLSLProgram & shader)
{
	shared_ptr<ArrayMesh> cube = utils::getCube();
	//table
	mat4 model;
	model = translate(model, vec3(0, TABLE_HEIGHT, 0));
	model = scale(model, vec3(TABLE_WIDTH / 2, TABLE_DEPTH / 2, TABLE_LENGTH / 2));
	standard_shader::setModel(shader, model);
	standard_shader::setMaterial(shader, table_mat);
	standard_shader::drawArrayMesh(*cube);
}

void Table::drawWall(basicgraphics::GLSLProgram & shader)
{

	if (_trump) {
		standard_shader::setTexture(shader, trump_texture);
	}
	else {
		standard_shader::setTexture(shader, white_texture);
		standard_shader::setMaterial(shader, table_mat);
	}
    //ping_pong::drawFloor(shader);
    shared_ptr<ArrayMesh> wall = utils::getCube();
    mat4 model;
    model = translate(model, vec3(0, TABLE_WALL_HEIGHT / 2 + TABLE_TOP, TABLE_BACK));
    model = scale(model, vec3(TABLE_WIDTH / 2, TABLE_WALL_HEIGHT / 2, 0.001f));
    standard_shader::setModel(shader, model);
    standard_shader::drawArrayMesh(*wall);
}

void Table::drawNet(basicgraphics::GLSLProgram & shader)
{
	mat4 net = mat4(1.f);
	net = translate(net, vec3(0.0f, TABLE_TOP + NET_DEPTH / 2, 0.0f));
	vec3 diffuse;
	shared_ptr<ArrayMesh> cube;

	for (int i = 0; i < N_VSTRINGS; i++) {
		mat4 net_string = mat4(1.f);

		float dx = (TABLE_WIDTH + 2 * NET_OVERHANG) / (N_VSTRINGS - 1);

		net_string = translate(net_string, vec3(TABLE_LEFT - NET_OVERHANG + i*dx, 0, 0.0f));
		net_string = scale(net_string, vec3(0.00125f, NET_DEPTH / 2, 0.00125f));
		standard_shader::setModel(shader, net * net_string);
		if (i == 0 || i == N_VSTRINGS - 1) {
			diffuse = vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			diffuse = vec3(1.0f, 1.0f, 1.0f);
		}

		standard_shader::setMaterial(shader, {
			diffuse,
			diffuse,
			vec3(0.01),
			0.5f * 128
		});
		cube = utils::getCube();
		standard_shader::drawArrayMesh(*cube);
	}
	for (int i = 0; i < N_HSTRINGS; i++) {
		float dy = (float)NET_DEPTH / (float)(N_HSTRINGS - 1);
		float y = dy * i - NET_DEPTH / 2;
		mat4 model_make = mat4(1.f);
		model_make = translate(model_make, vec3(0.0f, y, 0.0f));
		model_make = scale(model_make, vec3((TABLE_WIDTH / 2) + NET_OVERHANG, 0.00125f, 0.00125f));
		standard_shader::setModel(shader, net * model_make);

		if (i == 0 || i == N_HSTRINGS - 1) {
			diffuse = vec3(0.0f, 0.0f, 0.0f);
		}
		else {
			diffuse = vec3(1.0f, 1.0f, 1.0f);
		}
		standard_shader::setMaterial(shader, {
			diffuse,
			diffuse,
			vec3(0.01),
			0.5f * 128
		});
		cube = utils::getCube();
		standard_shader::drawArrayMesh(*cube);
	}
}

