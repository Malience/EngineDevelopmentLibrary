#include <unordered_map>

#include "cglm/cglm.h"

#include "ecs_type.h"
#include "ComponentTable.h"

class TransformComponentTable : public ComponentTable {
public:

	vec3* position_table;
	vec3* scale_table;
	vec4* rotation_table; //Quaternion

	mat4* transform_table;

	TransformComponentTable(const size_t size);
	~TransformComponentTable();

	table_id create(object_id oid);
	table_id ccreate(object_id oid);
	void remove(object_id oid);

	vec3& position(object_id oid);
	vec3& scale(object_id oid);
	vec4& rotation(object_id oid);

	mat4& transform(object_id oid);


	void get(object_id oid, vec3* pos, vec3* scale, vec4* rot, mat4* transform);

	void flag(object_id oid);
	void integrate();

private:
	table_id next = 0;
	table_id dirty = 0; //The transform needs to be remade
};