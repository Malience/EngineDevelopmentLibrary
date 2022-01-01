#include "ecs_type.h"
#include "ComponentTable.h"

template<typename T>
class BasicComponentTable : public ComponentTable {
public:

	T* table;

	table_id getNext() { return next; }

	BasicComponentTable(const size_t size);
	~BasicComponentTable();

	T& create(object_id oid);
	T& get(object_id oid);

	T& operator[](object_id oid);

	void remove(object_id oid);

private:
	table_id next = 0;
};