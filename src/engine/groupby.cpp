#include "../core/types.hpp"
#include "hashmap.hpp"

K* distinct(K* list) {
    if (list->t != KI) return nullptr;

    IntHashMap map(1024);

    for (int64_t i = 0; i < list->n; ++i) {
        map.get(list->I[i]);
    }

    std::vector<int64_t> unique_keys = map.keys();

    K* res = ktn(KI, unique_keys.size());
    for (size_t i = 0; i < unique_keys.size(); ++i) {
        res->I[i] = unique_keys[i];
    }

    return res;
}
