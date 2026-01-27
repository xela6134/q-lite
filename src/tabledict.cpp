// main.cpp
#include "core/types.hpp"

int main() {
    int32_t rows = 5;

    // 1. Create Data Columns (The "Values")
    K* time_col = ktn(KI, rows);
    K* price_col = ktn(KF, rows);
    for (int i=0; i<rows; ++i) {
        time_col->I[i] = 1000 + i;
        price_col->F[i] = 10.5 + i;
    }

    // 2. Create "Values" List (A generic list holding the columns)
    K* values = ktn(KK, 2); 
    values->k[0] = time_col;
    values->k[1] = price_col;

    // 3. Create "Keys" List (Column Names)
    K* keys = ktn(KS, 2);
    keys->S[0] = (char*)"time";
    keys->S[1] = (char*)"price";

    // 4. Create Dictionary (Map Keys -> Values)
    K* dict = xD(keys, values);

    // 5. Flip to Table
    K* table = xT(dict);

    std::cout << "Table Created Successfully!" << std::endl;
    // In a real database, 'show(table)' would iterate keys/values and print a grid.

    std::cout << "\nTable:" << std::endl;
    show(table);

    std::cout << "\nDict:" << std::endl;
    show(dict);

    std::cout << "\nTime Col:" << std::endl;
    show(time_col);

    std::cout << "\nPrice Col:" << std::endl;
    show(price_col);

    // Cleanup (Cascades down: Table -> Dict -> Lists -> Columns)
    r0(table);

    return 0;
}
