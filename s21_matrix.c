#include "s21_matrix.h"

/**
 * @brief Создает нулевую матрицу размерности rows * columns.
 *
 * @return int OK/INCORRECT_MATRIX
 */
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int mem_flg = 1;
  int res = INCORRECT_MATRIX;
  if (rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix) {
      for (rows--; rows >= 0 && mem_flg; rows--) {
        result->matrix[rows] = (double *)calloc(columns, sizeof(double));
        if (!result->matrix[rows]) mem_flg = 0;
      }
    }
    if (mem_flg) res = OK;
  }
  return res;
}

/**
 * @brief Очищает матрицу A
 *
 * @param A matrix_t type
 */
void s21_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) {
    if (A->matrix[i]) {
        free(A->matrix[i]);
        A->matrix[i] = NULL;
    }
  }
  if (A->matrix) free(A->matrix);
  A->matrix = NULL;
  A->columns = 0;
  A->rows = 0;
}

/**
 * @brief Возвращает результат сравнения матриц A и B.
 *
 * @return int SUCCESS/FAILURE
 */
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (!check_matrix(A) && !check_matrix(B) && !matrix_size_eq(A, B)) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
          res = FAILURE;
        }
      }
    }
  } else {
    res = FAILURE;
  }
  return res;
}

/**
 * @brief Cложения матриц A и B.
 *
 * @return int OK/INCORRECT_MATRIX/CALCULATION_ERROR
 */
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (!check_matrix(A) && !check_matrix(B)) {
    if (!matrix_size_eq(A, B)) {
      res = s21_create_matrix(A->rows, A->columns, result);
      if (!res) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
          }
        }
      }
    } else {
      res = CALCULATION_ERROR;
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Вычитания матриц A и B. 
 *
 * @return int OK/INCORRECT_MATRIX/CALCULATION_ERROR
 */
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (!check_matrix(A) && !check_matrix(B)) {
    if (!matrix_size_eq(A, B)) {
      res = s21_create_matrix(A->rows, A->columns, result);
      if (!res) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
          }
        }
      }
    } else {
      res = CALCULATION_ERROR;
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Умножения матрицы A на число number.
 *
 * @return int OK/INCORRECT_MATRIX
 */
int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int res = OK;
  if (!check_matrix(A)) {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (!res) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
        }
      }
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Умножения матриц A и B. 
 *
 * @return int OK/INCORRECT_MATRIX/CALCULATION_ERROR
 */
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int res = OK;
  if (!check_matrix(A) && !check_matrix(B)) {
    if (A->columns == B->rows) {
      res = s21_create_matrix(A->rows, B->columns, result);
      if (!res) {
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < B->columns; j++) {
            for (int k = 0; k < B->rows; k++) {
              result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
            }
          }
        }
      }
    } else {
      res = CALCULATION_ERROR;
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Транспонирование матрицы A. 
 * 
 * @return int OK/INCORRECT_MATRIX
 */
int s21_transpose(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (!check_matrix(A)) {
    res = s21_create_matrix(A->columns, A->rows, result);
    if (!res) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Алгебраическое дополнение матрицы A.
 * 
 *
 * @return int OK/INCORRECT_MATRIX
 */
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int res = OK;
if (!check_matrix(A)) {
    if (A->rows == A->columns && A->rows > 1) {
      s21_create_matrix(A->rows, A->rows, result);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->rows; j++) {
          double determinant = 0.0;
          matrix_t minor;
          s21_create_matrix(A->rows - 1, A->rows - 1, &minor);
          get_minor(A, &minor, i, j);
          s21_determinant(&minor, &determinant);
          result->matrix[i][j] = pow(-1, i + j) * determinant;
          s21_remove_matrix(&minor);
        }
      }
    } else {
      res = INCORRECT_MATRIX;
    }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}
/**
 * @brief Определитель матрицы A.
 * 
 * @return int OK/INCORRECT_MATRIX/CALCULATION_ERROR
 */
int s21_determinant(matrix_t *A, double *result) {
  int res = OK;
  if (!check_matrix(A)) {
      if (A->rows == A->columns) {
          *result = 0.0;
          if (A->rows == 1) {
              *result = A->matrix[0][0];
          } else {
            for (int i = 0; i < A->rows; i++) {
              double determinant = 0.0;
              matrix_t minor;
              s21_create_matrix(A->rows - 1, A->rows - 1, &minor);
              get_minor(A, &minor, i, 0);
              s21_determinant(&minor, &determinant);
              determinant *= A->matrix[i][0];
              if (i % 2) {
                *result -= determinant;
              } else {
                *result += determinant;
              }
              s21_remove_matrix(&minor);
            }
          }
      } else {
        res = CALCULATION_ERROR;
      }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Обратная матрица A.
 *
 * @return int OK/INCORRECT_MATRIX/CALCULATION_ERROR
 */
int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int res = OK;
  if (!check_matrix(A)) {
      if (A->rows == A->columns) {
        double determinant = 0.0;
        s21_determinant(A, &determinant);
        if (fabs(determinant) > EPS) {
          matrix_t complement;
          matrix_t transpose;
          s21_calc_complements(A, &complement);
          s21_transpose(&complement, &transpose);
          s21_mult_number(&transpose, 1.0 / determinant, result);
          s21_remove_matrix(&complement);
          s21_remove_matrix(&transpose);
        } else {
          res = CALCULATION_ERROR;
        }
      } else {
        res = CALCULATION_ERROR;
      }
  } else {
    res = INCORRECT_MATRIX;
  }
  return res;
}

/**
 * @brief Проверяет корректность матрицы A.
 *
 *
 * @return int OK/INCORRECT_MATRIX
 */
int check_matrix(matrix_t *A) {
  int err = INCORRECT_MATRIX;
  if ((A != NULL) && (A->matrix != NULL)
                        && (A->rows > 0) && (A->columns > 0)) {
    err = OK;
  }
  return err;
}

/**
 * @brief Минор матрицы A в координатах row и col.
 *
 */
void get_minor(matrix_t *A, matrix_t *result, int a, int b) {
  for (int i = 0, n = 0; i < A->rows; i++) {
    if (i != a) {
      for (int j = 0, m = 0; j < A->columns; j++) {
        if (j != b) {
          result->matrix[n][m] = A->matrix[i][j];
          m++;
        }
      }
      n++;
    }
  }
}

/**
 * @brief Проверка матриц на идентичный размер
 *
 * @return int 0 идентичный, 1 неидентичный
 */
int matrix_size_eq(matrix_t *A, matrix_t *B) {
  int err = INCORRECT_MATRIX;
  if (A->rows == B->rows && A->columns == B->columns) {
    err = OK;
  }
  return err;
}

