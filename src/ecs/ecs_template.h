
#include<unordered_map>

//experimental bimap;
template<class K, class V>
class unordered_bimap {
public:

	void reserve(const size_t count) { 
		K_to_V.reserve(count); 
		V_to_K.reserve(count); 
	}

	void insert(const K key, const V val) {
		K_to_V[key] = val;
		V_to_K[val] = key;
	}

	V& getValue(const K& key) {return K_to_V[key];}
	K& getKey(const V& val) { return V_to_K[val]; }

private:
	std::unordered_map<K, V> K_to_V;
	std::unordered_map<V, K> V_to_K;
};