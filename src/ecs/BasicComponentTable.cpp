#include "BasicComponentTable.h"

template<typename T> BasicComponentTable<T>::BasicComponentTable(const size_t size) : ComponentTable{ size } {
	table = (T*) malloc(size * sizeof(T));
}
template<typename T> BasicComponentTable<T>::~BasicComponentTable() { 
	//for (int i = --next; i>=0; i--) delete stack[i];
	delete table; 
}

template<typename T> T& BasicComponentTable<T>::create(object_id oid) {
	table_id tid = next++; //Iterates AFTER tid is assigned

	o_to_t[oid] = tid;
	t_to_t[tid] = oid;

	return &(table[tid]);
}

template<typename T> T& BasicComponentTable<T>::get(object_id oid) {
	return &(table[o_to_t[oid]]);
}
template<typename T> T& BasicComponentTable<T>::operator[](object_id oid) {
	return &(table[o_to_t[oid]]);
}

template<typename T> void BasicComponentTable<T>::remove(object_id oid) {
	table_id tid = o_to_t[oid];

	//delete stack[tid]; //delete the old

	table[tid] = table[--next]; //Swap with the last slot

	o_to_t.erase(oid); //Clear the current oid
	oid = t_to_o[next]; //Get the oid of the last slot
	t_to_o.erase(next); //Clear last slot
	o_to_t[oid] = tid; //Re make mapping after swap;
	t_to_o[tid] = oid;
}
