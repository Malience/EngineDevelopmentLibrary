
#include "TransformComponentTable.h"

TransformComponentTable::TransformComponentTable(const size_t size) : ComponentTable{ size } {
	position_table = (vec3*) malloc(size * sizeof(vec3));
	scale_table = (vec3*) malloc(size * sizeof(vec3));
	rotation_table = (vec4*) malloc(size * sizeof(vec4));

	transform_table = (mat4*) malloc(size * sizeof(mat4));
}
TransformComponentTable::~TransformComponentTable() {
	delete position_table;
	delete scale_table;
	delete rotation_table;

	delete transform_table;
}

table_id TransformComponentTable::create(object_id oid) {
	table_id tid = next++; //Iterates next AFTER assigning tid
	//TODO: Handle flaged

	o_to_t[oid] = tid;
	t_to_o[tid] = oid;

	return tid;
}
table_id TransformComponentTable::ccreate(object_id oid) {
	table_id tid = next++; //Iterates next AFTER assigning tid

	o_to_t[oid] = tid;
	t_to_o[tid] = oid;

	glm_vec3_zero(position_table[tid]);
	glm_vec3_one(scale_table[tid]);
	glm_quat_identity(rotation_table[tid]);

	glm_mat4_identity(transform_table[tid]);

	return tid;
}
void TransformComponentTable::remove(object_id oid) {
	//TODO: Implement

}

vec3& TransformComponentTable::position(object_id oid) {return position_table[o_to_t[oid]];}
vec3& TransformComponentTable::scale(object_id oid) { return scale_table[o_to_t[oid]]; }
vec4& TransformComponentTable::rotation(object_id oid) { return rotation_table[o_to_t[oid]]; }

mat4& TransformComponentTable::transform(object_id oid) { return transform_table[o_to_t[oid]]; }


void TransformComponentTable::get(object_id oid, vec3* pos, vec3* scale, vec4* rot, mat4* transform) {
	//TODO: Implement

}

void TransformComponentTable::flag(object_id oid) {
	//TODO: Implement
}
void TransformComponentTable::integrate() {
	//TODO: Implement
}