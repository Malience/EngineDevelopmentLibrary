#include <unordered_map>

#include "cglm/cglm.h"

#include "ecs_type.h"

class ComponentTable abstract {
public:
	const uint32_t size;
	
	table_id getTableID(const object_id oid) { return o_to_t[oid]; }
	object_id getObjectID(const table_id tid) { return t_to_o[tid]; }

	ComponentTable(const uint32_t size) : size(size), o_to_t(size), t_to_o(size) {}
	//virtual ~ComponentTable() {}

protected:
	std::unordered_map<object_id, table_id> o_to_t;
	std::unordered_map<table_id, object_id> t_to_o;
};