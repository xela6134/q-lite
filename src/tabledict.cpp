// main.cpp
#include "core/types.hpp"

int main() {
    int32_t rows = 5;

    // Create data columns
    K* time_col = ktn(KI, rows);
    K* price_col = ktn(KF, rows);
    for (int i = 0; i < rows; ++i) {
        time_col->I[i] = 1000 + i;
        price_col->F[i] = 10.5 + i;
    }

    // 2. Create values list (A generic list holding the columns)
    K* values = ktn(KK, 2);
    values->k[0] = time_col;
    values->k[1] = price_col;

    // Create keys List (Column Names)
    K* keys = ktn(KS, 2);
    keys->S[0] = (char*)"time";
    keys->S[1] = (char*)"price";

    // Create dictionary (Map Keys -> Values)
    K* dict = xD(keys, values);
    K* table = xT(dict);

    std::cout << "Table created successfully" << std::endl;

    std::cout << "\nTable:" << std::endl;
    show(table);

    std::cout << "\nDict:" << std::endl;
    show(dict);

    std::cout << "\nTime Col:" << std::endl;
    show(time_col);

    std::cout << "\nPrice Col:" << std::endl;
    show(price_col);

    r0(table);

    return 0;
}
