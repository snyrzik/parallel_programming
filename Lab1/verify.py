import numpy as np


def read_matrix(filename):
    with open(filename, "r") as file:
        N = int(file.readline())
        numbers = file.read().split()

    if N <= 0 or len(numbers) != N * N:
        raise ValueError("Invalid matrix: " + filename)

    matrix = np.array(numbers, dtype=float)
    return matrix.reshape(N, N)


def main():
    A = read_matrix("input/inputA.txt")
    B = read_matrix("input/inputB.txt")
    C = read_matrix("output/outputC.txt")

    expected = A @ B

    if C.shape == expected.shape and np.allclose(C, expected, rtol=1e-9, atol=1e-8):
        print("PASS: matrices match")
    else:
        print("FAIL: matrices are different")
        raise SystemExit(1)


if __name__ == "__main__":
    main()
