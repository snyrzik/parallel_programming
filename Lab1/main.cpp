#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>
#include <filesystem>
#include <iomanip>


void ReadMatrix(const std::string& filename, std::vector<std::vector<double>>& matrix) {
    std::size_t N = 0;
    std::ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    if (!(file >> N)) {
        throw std::runtime_error("Cannot read matrix size");
    }

    if (N == 0) {
        throw std::runtime_error("Invalid matrix size");
    }

    matrix.resize(N);

    for (std::size_t i = 0; i < N; ++i) {
        matrix[i].assign(N, 0);
    }

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            double tmp = 0;

            if (!(file >> tmp)) {
                throw std::runtime_error("Cannot read matrix element");
            }

            matrix[i][j] = tmp;
        }
    }
}


void WriteMatrix(const std::string& filename, const std::vector<std::vector<double>>& matrix) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    file << std::setprecision(17);
    file << matrix.size() << "\n";

    for (std::size_t i = 0; i < matrix.size(); ++i) {
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            file << matrix[i][j] << " ";
        }
        file << "\n";
    }
}

void WriteResults(const std::vector<std::vector<double>>& matrix, double seconds) {
    std::filesystem::create_directory("output");
    WriteMatrix("output/outputC.txt", matrix);
    std::ofstream file("output/info.txt");

    if (!file.is_open()) {
        throw std::runtime_error("Cannot create info file");
    }

    std::size_t N = matrix.size();
    std::size_t multiplications = N * N * N;
    std::size_t additions = N * N * N;

    file << "Matrix size: " << N << "x" << N << "\n";
    file << "Multiplications: " << multiplications << "\n";
    file << "Additions: " << additions << "\n";
    file << std::setprecision(12);
    file << "Time: " << seconds << " seconds\n";
}


std::vector<std::vector<double>> operator*(
    const std::vector<std::vector<double>>& matrix1,
    const std::vector<std::vector<double>>& matrix2
    ) {
    std::vector<std::vector<double>> C;

    if (matrix1.empty() || matrix2.empty() || matrix1[0].empty() || matrix2[0].empty()) {
        throw std::invalid_argument("Empty matrix");
    }

    const std::size_t rows1 = matrix1.size();
    const std::size_t cols1 = matrix1[0].size();
    const std::size_t rows2 = matrix2.size();
    const std::size_t cols2 = matrix2[0].size();

    if (cols1 != rows2) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication");
    }

    C.resize(rows1);

    for (std::size_t i = 0; i < rows1; ++i) {
        C[i].assign(cols2, 0);
    }

    for (std::size_t i = 0; i < rows1; ++i) {
        for (std::size_t j = 0; j < cols2; ++j) {
            for (std::size_t k = 0; k < cols1; ++k) {
                C[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return C;
}

int main() {
    std::vector<std::vector<double>> A;
    std::vector<std::vector<double>> B;
    std::vector<std::vector<double>> C;

    try {
        ReadMatrix("input/inputA.txt", A);
        ReadMatrix("input/inputB.txt", B);

        std::chrono::steady_clock::time_point startTime;
        startTime = std::chrono::steady_clock::now();

        C = A * B;

        std::chrono::steady_clock::time_point endTime;
        endTime = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsedTime;
        elapsedTime = endTime - startTime;
        double seconds = elapsedTime.count();

        WriteResults(C, seconds);
    }

    catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}
