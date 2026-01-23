#pragma once

#include "../core/types.hpp"

void add_I(int64_t* res, const int64_t* a, const int64_t* b, int32_t n);
void add_F(double* res, const double* a, const double* b, int32_t n);

inline K* add(K* a, K* b) {
    // validation
    if (a->n != b->n || a->t != b->t) {
        throw std::runtime_error("length_error");
    }
    
    // New return K object
    K* r = ktn(a->t, a->n);

    switch (a->t) {
        case KI: // int
            if (b->t == KI) {
                add_I(r->I, a->I, b->I, a->n);
            }
            break;
            
        case KF: // float
            if (b->t == KF) {
                add_F(r->F, a->F, b->F, a->n);
            }
            break;
        
        // Add more later

        default:
            // Handle error or other types
            r0(r);
            return nullptr;
    }

    return r;
}
