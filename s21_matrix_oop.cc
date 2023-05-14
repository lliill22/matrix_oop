#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() : rows_(2), cols_(2) { Allocate(*this); }

S21Matrix::S21Matrix(int rows, int cols) {
  if (rows < 1 || cols < 1) {
    throw std::out_of_range("wrong matrix size");
  }
  rows_ = rows;
  cols_ = cols;
  Allocate(*this);
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  Allocate(*this);
  Copy(other);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.cols_ = 0;
  other.rows_ = 0;
  other.matrix_ = nullptr;
}

S21Matrix::~S21Matrix() { Destruct(*this); }

bool S21Matrix::EqMatrix(const S21Matrix& other) const noexcept {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++)
      if (abs(matrix_[i][j] - other.matrix_[i][j]) > 1e-7) return false;
  return true;
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (cols_ == other.cols_ && rows_ == other.rows_) {
    for (int i = 0; i < rows_; i++)
      for (int j = 0; j < cols_; j++) matrix_[i][j] += other.matrix_[i][j];
  } else
    throw std::logic_error("matrix must be same size");
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (cols_ == other.cols_ && rows_ == other.rows_) {
    for (int i = 0; i < rows_; i++)
      for (int j = 0; j < cols_; j++) matrix_[i][j] -= other.matrix_[i][j];
  } else
    throw std::logic_error("matrix must be same size");
}

void S21Matrix::MulNumber(const double num) noexcept {
  if (matrix_) {
    for (int i = 0; i < rows_; i++)
      for (int j = 0; j < cols_; j++) matrix_[i][j] *= num;
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ == other.rows_) {
    S21Matrix res(rows_, other.cols_);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < other.cols_; j++) {
        for (int k = 0; k < cols_; k++) {
          res.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
        }
      }
    }
    *this = res;
  } else
    throw std::logic_error(
        "rows of one matrix are not equal to columns of another matrix");
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix temp(cols_, rows_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) temp.matrix_[j][i] = matrix_[i][j];
  return temp;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (cols_ != rows_) throw std::logic_error("matrix must be squere");
  S21Matrix temp(cols_, rows_);
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) {
      S21Matrix minor(cols_ - 1, rows_ - 1);
      double det = 0;
      minor.FillMinor(*this, i, j);
      det = minor.Determinant();
      temp.matrix_[i][j] = pow(-1, i + j) * det;
    }
  return temp;
}

double S21Matrix::Determinant() const {
  double res = 0;
  if (rows_ == cols_) {
    if (rows_ == 1) return matrix_[0][0];
    S21Matrix temp(rows_ - 1, cols_ - 1);
    for (int i = 0; i < cols_; i++) {
      temp.FillMinor(*this, 0, i);
      if (i % 2)
        res -= matrix_[0][i] * temp.Determinant();
      else
        res += matrix_[0][i] * temp.Determinant();
    }
    return res;
  } else
    throw std::logic_error("matrix must be squere");
}

S21Matrix S21Matrix::InverseMatrix() const {
  S21Matrix transpose(rows_, cols_);
  double det = Determinant();
  if (rows_ == cols_) {
    if (rows_ == 1) {
      S21Matrix temp(1, 1);
      if (abs(det) < 1e-7) throw std::logic_error("determinant is zero");
      temp.matrix_[0][0] = 1.0 / det;
      return temp;
    } else {
      det = Determinant();
      if (abs(det) > 1e-7) {
        transpose = CalcComplements().Transpose();
        transpose.MulNumber(1.0 / det);
      } else
        throw std::logic_error("determinant is zero");
    }
  }
  return transpose;
}

int S21Matrix::GetRows() const noexcept { return rows_; }

int S21Matrix::GetCols() const noexcept { return cols_; }

void S21Matrix::SetRows(int rows) {
  if (rows < 1) {
    throw std::out_of_range("Incorrect input");
  } else {
    S21Matrix temp(rows, cols_);
    if (rows > rows_) {
      for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
          temp.matrix_[i][j] = matrix_[i][j];
        }
      }
    } else if (rows < rows_) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols_; j++) {
          temp.matrix_[i][j] = matrix_[i][j];
        }
      }
    }
    *this = temp;
  }
}

void S21Matrix::SetCols(int cols) {
  if (cols < 1) {
    throw std::out_of_range("Incorrect input");
  } else {
    S21Matrix temp(rows_, cols);
    if (cols > cols_) {
      for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
          temp.matrix_[i][j] = matrix_[i][j];
        }
      }
    } else if (cols < cols_) {
      for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols; j++) {
          temp.matrix_[i][j] = matrix_[i][j];
        }
      }
    }
    *this = temp;
  }
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix temp(*this);
  temp.SumMatrix(other);
  return temp;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix temp(*this);
  temp.SubMatrix(other);
  return temp;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix temp(*this);
  temp.MulMatrix(other);
  return temp;
}

S21Matrix operator*(const double num, const S21Matrix& other) {
  S21Matrix temp(other);
  temp.MulNumber(num);
  return temp;
}

S21Matrix operator*(const S21Matrix& other, const double num) {
  S21Matrix temp(other);
  temp.MulNumber(num);
  return temp;
}

bool S21Matrix::operator==(const S21Matrix& other) const noexcept {
  return EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  Destruct(*this);
  rows_ = other.rows_;
  cols_ = other.cols_;
  Allocate(*this);
  Copy(other);
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double other) {
  MulNumber(other);
  return *this;
}

double& S21Matrix::operator()(int i, int j) const {
  if (i < 0 || j < 0 || i >= rows_ || j >= cols_)
    throw std::out_of_range("out of range");
  return matrix_[i][j];
}

void S21Matrix::Allocate(S21Matrix& other) const {
  other.matrix_ = new double*[other.rows_];
  for (int i = 0; i < other.rows_; i++) {
    other.matrix_[i] = new double[other.cols_];
    for (int j = 0; j < other.cols_; j++) other.matrix_[i][j] = 0;
  }
}

void S21Matrix::Destruct(S21Matrix& other) const {
  if (other.matrix_) {
    for (int i = 0; i < other.rows_; i++) delete[] other.matrix_[i];
    delete[] other.matrix_;
  }
}

void S21Matrix::Copy(const S21Matrix& other) {
  for (int i = 0; i < rows_; i++)
    for (int j = 0; j < cols_; j++) matrix_[i][j] = other.matrix_[i][j];
}

void S21Matrix::FillMinor(const S21Matrix& other, int rows, int cols) const {
  int m_rows = 0;
  int m_cols = 0;
  for (int i = 0; i < other.rows_; i++) {
    if (i == rows) continue;
    m_cols = 0;
    for (int j = 0; j < other.cols_; j++) {
      if (j == cols) continue;
      matrix_[m_rows][m_cols] = other.matrix_[i][j];
      m_cols++;
    }
    m_rows++;
  }
}
