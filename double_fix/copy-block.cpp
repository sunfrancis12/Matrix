#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

void read_matrix(const string& filename, vector<vector<double> >& matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file " + filename);
    }

    int size;
    if (!(file >> size)) {
        throw runtime_error("Unable to read matrix size");
    }
    cout << "Reading matrix of size: " << size << "x" << size << endl;
    matrix.resize(size, vector<double>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (!(file >> matrix[i][j])) {
                throw runtime_error("Error reading matrix element");
            }
        }
    }
}

// 複製子矩陣到臨時區域
void copy_submatrix(const vector<vector<double> >& src, vector<vector<double> >& dest, int start_row, int start_col, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (start_row + i < src.size() && start_col + j < src.size()) {
                dest[i][j] = src[start_row + i][start_col + j];
            }
        }
    }
}

// 執行子矩陣乘法
void multiply_submatrices(const vector<vector<double> >& A, const vector<vector<double> >& B, vector<vector<double> >& C, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            for (int k = 0; k < block_size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 將結果加回到 C 矩陣
void add_to_submatrix(vector<vector<double> >& C, const vector<vector<double> >& temp_C, int start_row, int start_col, int block_size) {
    for (int i = 0; i < block_size; i++) {
        for (int j = 0; j < block_size; j++) {
            if (start_row + i < C.size() && start_col + j < C.size()) {
                C[start_row + i][start_col + j] += temp_C[i][j];
            }
        }
    }
}

// 矩陣乘法
void matrix_multiply(const vector<vector<double> >& A,
                     const vector<vector<double> >& B,
                     vector<vector<double> >& C, int BLOCK_SIZE) {
    int size = A.size();
    vector<vector<double> > temp_A(BLOCK_SIZE, vector<double>(BLOCK_SIZE));
    vector<vector<double> > temp_B(BLOCK_SIZE, vector<double>(BLOCK_SIZE));
    vector<vector<double> > temp_C(BLOCK_SIZE, vector<double>(BLOCK_SIZE, 0.0));

    // 將矩陣分成區塊進行計算
    for (int i = 0; i < size; i += BLOCK_SIZE) {
        for (int j = 0; j < size; j += BLOCK_SIZE) {
            fill(temp_C.begin(), temp_C.end(), vector<double>(BLOCK_SIZE, 0.0));
            for (int k = 0; k < size; k += BLOCK_SIZE) {
                // 複製 A 和 B 的子矩陣到臨時區域
                copy_submatrix(A, temp_A, i, k, BLOCK_SIZE);
                copy_submatrix(B, temp_B, k, j, BLOCK_SIZE);

                // 在臨時區域執行子矩陣乘法
                multiply_submatrices(temp_A, temp_B, temp_C, BLOCK_SIZE);
            }
            // 將結果累加回 C
            add_to_submatrix(C, temp_C, i, j, BLOCK_SIZE);
        }
    }
    cout << "\r counting... " << flush;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw runtime_error("Usage: " + string(argv[0]) + " <matrix size>");
        }

        int size = stoi(argv[1]);
        int block_size = 32;
        string filename = to_string(size) + ".txt";

        vector<vector<double> > A, B, C(size, vector<double>(size, 0.0));
        read_matrix(filename, A);
        read_matrix(filename, B);

        cout << "------Start------" << endl;
        clock_t start = clock();
        matrix_multiply(A, B, C, block_size);
        clock_t end = clock();

        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        cout << "\n Total Time: " << cpu_time_used << " seconds" << endl;

        // 輸出結果矩陣的一小部分來驗證計算
        cout << "\nResult matrix (top-left 5x5 corner):" << endl;
        for (int i = 0; i < min(5, size); ++i) {
            for (int j = 0; j < min(5, size); ++j) {
                cout << setw(12) << fixed << setprecision(6) << C[i][j] << " ";
            }
            cout << endl;
        }

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}