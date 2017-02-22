#include "Table.h"



using namespace basicgraphics;
using namespace std;

Table::Table()
{
}


Table::~Table()
{
}

void Table::draw(basicgraphics::GLSLProgram & shader)
{
	shared_ptr<ArrayMesh> cube = utils::getCube();
	mat4 model;
	model = translate(model, vec3(0, 0, 0));
	model = scale(model, vec3(TABLE_WIDTH / 2, TABLE_DEPTH/2, TABLE_LENGTH / 2));
	standard_shader::setModel(shader, model);
	standard_shader::setMaterial(shader, {
		vec3(0.0,0.1,0.06),
		vec3(0.0,0.60980392,0.80980392),
		vec3(0.50196078,0.50196078,0.50196078),
		0.25f * 128.f
	});
	standard_shader::drawArrayMesh(*cube);
}
