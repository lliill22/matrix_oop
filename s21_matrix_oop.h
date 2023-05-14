#ifndef S21MATRIXPLUS_SRC_S21_MATRIX_OOP_H_
#define S21MATRIXPLUS_SRC_S21_MATRIX_OOP_H_
#include <cmath>
#include <iostream>

class S21Matrix {
 public:
  S21Matrix();
  explicit S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  ~S21Matrix();

  bool EqMatrix(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;

  int GetRows() const noexcept;
  int GetCols() const noexcept;
  void SetRows(int rows);
  void SetCols(int cols);

  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other) const;
  friend S21Matrix operator*(const double num, const S21Matrix& other);
  friend S21Matrix operator*(const S21Matrix& other, const double num);
  bool operator==(const S21Matrix& other) const noexcept;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double other);
  double& operator()(int i, int j) const;

 private:
  void Allocate(S21Matrix& other) const;
  void Destruct(S21Matrix& other) const;
  void Copy(const S21Matrix& other);
  void FillMinor(const S21Matrix& other, int rows, int cols) const;

  int rows_, cols_;
  double** matrix_;
};
#endif  //  S21MATRIXPLUS_SRC_S21_MATRIX_OOP_H_
