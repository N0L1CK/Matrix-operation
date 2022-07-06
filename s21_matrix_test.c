#include "s21_matrix.h"

#include <check.h>

double rand_float(double low, double high);
int rand_int();

double rand_float(double low, double high) {
  double val = (double)rand() / RAND_MAX;
  return low + val * (high - low);
}

int rand_int() { return rand() % 100 + 1; }

START_TEST(test_s21_create_matrix) {
  int rows = 100, cols = 100, res;
  matrix_t A = {0};
  res = s21_create_matrix(rows, cols, &A);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = 0;
      ck_assert_ldouble_eq_tol(0, A.matrix[i][j], EPS);
    }
  }
  ck_assert_int_eq(A.rows, rows);
  ck_assert_int_eq(A.columns, cols);
  ck_assert_int_eq(res, OK);
  s21_remove_matrix(&A);

  for (rows = -100, cols = -200; rows < 200 && cols < 100; rows++, cols++) {
    if (rows > 0 && cols > 0) {
      ck_assert_int_eq(s21_create_matrix(rows, cols, &A), OK);
      s21_remove_matrix(&A);
    } else {
      ck_assert_int_eq(s21_create_matrix(rows, cols, &A), INCORRECT_MATRIX);
      s21_remove_matrix(&A);
    }
  }
}
END_TEST

START_TEST(test_s21_remove_matrix) {
  int rows = 100, cols = 100;
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  s21_remove_matrix(&A);
  ck_assert_int_eq(A.rows, FAILURE);
  ck_assert_int_eq(A.columns, FAILURE);
}
END_TEST

START_TEST(test_s21_eq_matrix) {
  int rows = rand_int(), cols = rand_int();
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  matrix_t B = {0};
  s21_create_matrix(rows, cols, &B);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double rand_val = rand_float(DBL_MIN, DBL_MAX);
      A.matrix[i][j] = rand_val;
      B.matrix[i][j] = rand_val;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double rand_val = rand_float(DBL_MIN, DBL_MAX);
      A.matrix[i][j] = rand_val * 0.56789;
      B.matrix[i][j] = rand_val;
    }
  }
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  s21_create_matrix(10, 10, &A);
  s21_create_matrix(20, 20, &B);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_sum_matrix) {
  int rows = rand_int(), cols = rand_int();
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  matrix_t B = {0};
  s21_create_matrix(rows, cols, &B);
  matrix_t check = {0};
  s21_create_matrix(rows, cols, &check);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = rand_float(DBL_MIN, DBL_MAX);
      B.matrix[i][j] = rand_float(DBL_MIN, DBL_MAX);
      check.matrix[i][j] = A.matrix[i][j] + B.matrix[i][j];
    }
  }
  matrix_t res = {0};
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  s21_create_matrix(10, 10, &A);
  s21_create_matrix(20, 20, &B);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), CALCULATION_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(test_s21_sub_matrix) {
  int rows = rand_int(), cols = rand_int();
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  matrix_t B = {0};
  s21_create_matrix(rows, cols, &B);
  matrix_t check = {0};
  s21_create_matrix(rows, cols, &check);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = rand_float(DBL_MIN, DBL_MAX);
      B.matrix[i][j] = rand_float(DBL_MIN, DBL_MAX);
      check.matrix[i][j] = A.matrix[i][j] - B.matrix[i][j];
    }
  }
  matrix_t res = {0};
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  s21_create_matrix(10, 10, &A);
  s21_create_matrix(20, 20, &B);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), CALCULATION_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(test_s21_mult_number) {
  int rows = rand_int(), cols = rand_int();
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  matrix_t check = {0};
  s21_create_matrix(rows, cols, &check);
  double num = rand_float(-10e5, 10e5);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = rand_float(DBL_MIN, DBL_MAX);
      check.matrix[i][j] = A.matrix[i][j] * num;
    }
  }
  matrix_t res = {0};
  ck_assert_int_eq(s21_mult_number(&A, num, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);
  s21_remove_matrix(&A);

  ck_assert_int_eq(s21_mult_number(&A, num, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(test_s21_mult_matrix) {
  int rows = rand_int(), cols = rand_int();
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  matrix_t B = {0};
  s21_create_matrix(cols, rows, &B);

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) A.matrix[i][j] = rand_float(-10e10, 10e10);

  for (int i = 0; i < cols; i++)
    for (int j = 0; j < rows; j++) B.matrix[i][j] = rand_float(-10e10, 10e10);
  matrix_t check = {0};
  s21_create_matrix(A.rows, B.columns, &check);

  for (int i = 0; i < A.rows; i++) {
    for (int j = 0; j < B.columns; j++) {
      for (int k = 0; k < A.columns; k++)
        check.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
    }
  }

  matrix_t res = {0};
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  s21_create_matrix(10, 10, &A);
  s21_create_matrix(20, 20, &B);
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), CALCULATION_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(test_s21_transpose) {
  int rows = rand_int(), cols = rand_int();
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);

  matrix_t check = {0};
  s21_create_matrix(cols, rows, &check);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double rand_val = rand_float(-10e10, 10e10);
      A.matrix[i][j] = rand_val;
      check.matrix[j][i] = rand_val;
    }
  }

  matrix_t res = {0};
  ck_assert_int_eq(s21_transpose(&A, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&check, &res), SUCCESS);

  s21_remove_matrix(&A);

  ck_assert_int_eq(s21_transpose(&A, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&res);
  s21_remove_matrix(&check);
}
END_TEST

START_TEST(test_s21_calc_complements) {
  int shape = 3;
  matrix_t A = {0};
  s21_create_matrix(shape, shape, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][1] = 4.0;
  A.matrix[1][2] = 2.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;

  matrix_t complement = {0};
  s21_create_matrix(shape, shape, &complement);
  complement.matrix[0][1] = 10.0;
  complement.matrix[0][2] = -20.0;
  complement.matrix[1][0] = 4.0;
  complement.matrix[1][1] = -14.0;
  complement.matrix[1][2] = 8.0;
  complement.matrix[2][0] = -8.0;
  complement.matrix[2][1] = -2.0;
  complement.matrix[2][2] = 4.0;

  matrix_t res = {0};
  ck_assert_int_eq(s21_calc_complements(&A, &res), OK);
  ck_assert_int_eq(s21_eq_matrix(&complement, &res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&complement);
  s21_remove_matrix(&res);

  int rows = 1, cols = 1;
  s21_create_matrix(rows, cols, &A);
  A.matrix[0][0] = 1;
  ck_assert_int_eq(s21_calc_complements(&A, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);

  rows = 10, cols = 20;
  s21_create_matrix(rows, cols, &A);
  ck_assert_int_eq(s21_calc_complements(&A, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);

  ck_assert_int_eq(s21_calc_complements(&A, &res), INCORRECT_MATRIX);
  s21_remove_matrix(&res);
}
END_TEST

START_TEST(test_s21_determinant) {
  int rows = 2, cols = 2;
  matrix_t A = {0};
  s21_create_matrix(rows, cols, &A);
  int val = 1;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = val++;
    }
  }
  double res = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, -2.0, EPS);
  s21_remove_matrix(&A);

  rows = 3, cols = 3;
  s21_create_matrix(rows, cols, &A);
  val = 1;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = val++;
    }
  }
  A.matrix[2][2] = 0.0;
  res = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, 27, EPS);
  s21_remove_matrix(&A);

  rows = 5, cols = 5;
  s21_create_matrix(rows, cols, &A);
  val = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = val++;
    }
  }
  res = 0.0;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, 0, EPS);
  s21_remove_matrix(&A);

  int shape = 1;
  s21_create_matrix(shape, shape, &A);
  A.matrix[0][0] = 1.0;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, 1.0, EPS);
  s21_remove_matrix(&A);

  shape = 4;
  s21_create_matrix(shape, shape, &A);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 2.0;
  A.matrix[0][3] = 2.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 1.0;
  A.matrix[1][2] = 2.0;
  A.matrix[1][3] = 2.0;
  A.matrix[2][0] = 2.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;
  A.matrix[2][3] = 2.0;
  A.matrix[3][0] = 2.0;
  A.matrix[3][1] = 2.0;
  A.matrix[3][2] = 2.0;
  A.matrix[3][3] = 1.0;
  ck_assert_int_eq(s21_determinant(&A, &res), OK);
  ck_assert_double_eq_tol(res, -7.0, EPS);
  s21_remove_matrix(&A);

  rows = 10, cols = 20;
  s21_create_matrix(rows, cols, &A);
  ck_assert_int_eq(s21_determinant(&A, &res), CALCULATION_ERROR);
  s21_remove_matrix(&A);

  ck_assert_int_eq(s21_determinant(&A, &res), INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_s21_inverse_matrix) {
  int shape = 3;
  matrix_t A = {0};
  s21_create_matrix(shape, shape, &A);

  A.matrix[0][0] = 2.0;
  A.matrix[0][1] = 5.0;
  A.matrix[0][2] = 7.0;
  A.matrix[1][0] = 6.0;
  A.matrix[1][1] = 3.0;
  A.matrix[1][2] = 4.0;
  A.matrix[2][0] = 5.0;
  A.matrix[2][1] = -2.0;
  A.matrix[2][2] = -3.0;

  matrix_t res = {0};
  ck_assert_int_eq(s21_inverse_matrix(&A, &res), OK);

  matrix_t inversed = {0};
  s21_create_matrix(shape, shape, &inversed);
  inversed.matrix[0][0] = 1.0;
  inversed.matrix[0][1] = -1.0;
  inversed.matrix[0][2] = 1.0;
  inversed.matrix[1][0] = -38.0;
  inversed.matrix[1][1] = 41.0;
  inversed.matrix[1][2] = -34.0;
  inversed.matrix[2][0] = 27.0;
  inversed.matrix[2][1] = -29.0;
  inversed.matrix[2][2] = 24.0;

  ck_assert_int_eq(s21_eq_matrix(&inversed, &res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
  s21_remove_matrix(&inversed);

  shape = 4;
  s21_create_matrix(shape, shape, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 2.0;
  A.matrix[0][3] = 2.0;
  A.matrix[1][0] = 2.0;
  A.matrix[1][1] = 1.0;
  A.matrix[1][2] = 2.0;
  A.matrix[1][3] = 2.0;
  A.matrix[2][0] = 2.0;
  A.matrix[2][1] = 2.0;
  A.matrix[2][2] = 1.0;
  A.matrix[2][3] = 2.0;
  A.matrix[3][0] = 2.0;
  A.matrix[3][1] = 2.0;
  A.matrix[3][2] = 2.0;
  A.matrix[3][3] = 1.0;

  ck_assert_int_eq(s21_inverse_matrix(&A, &res), OK);

  s21_create_matrix(shape, shape, &inversed);
  inversed.matrix[0][0] = -5.0/7.0;
  inversed.matrix[0][1] = 2.0/7.0;
  inversed.matrix[0][2] = 2.0/7.0;
  inversed.matrix[0][3] = 2.0/7.0;
  inversed.matrix[1][0] = 2.0/7.0;
  inversed.matrix[1][1] = -5.0/7.0;
  inversed.matrix[1][2] = 2.0/7.0;
  inversed.matrix[1][3] = 2.0/7.0;
  inversed.matrix[2][0] = 2.0/7.0;
  inversed.matrix[2][1] = 2.0/7.0;
  inversed.matrix[2][2] = -5.0/7.0;
  inversed.matrix[2][3] = 2.0/7.0;
  inversed.matrix[3][0] = 2.0/7.0;
  inversed.matrix[3][1] = 2.0/7.0;
  inversed.matrix[3][2] = 2.0/7.0;
  inversed.matrix[3][3] = -5.0/7.0;

  ck_assert_int_eq(s21_eq_matrix(&inversed, &res), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&res);
  s21_remove_matrix(&inversed);

  int rows = 10, cols = 20;
  s21_create_matrix(rows, cols, &A);
  ck_assert_int_eq(s21_inverse_matrix(&A, &res), CALCULATION_ERROR);
  s21_remove_matrix(&A);

  rows = 5, cols = 5;
  s21_create_matrix(rows, cols, &A);
  int val = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      A.matrix[i][j] = val++;
    }
  }

  ck_assert_int_eq(s21_inverse_matrix(&A, &res), CALCULATION_ERROR);
  s21_remove_matrix(&res);
  s21_remove_matrix(&A);

  ck_assert_int_eq(s21_inverse_matrix(&A, &res), INCORRECT_MATRIX);
}
END_TEST

Suite *s21_matrix_suite(void) {
  Suite *suite;
  TCase *core;

  suite = suite_create("s21_matrix");
  core = tcase_create("Core");

  tcase_add_test(core, test_s21_create_matrix);
  tcase_add_test(core, test_s21_remove_matrix);
  tcase_add_test(core, test_s21_eq_matrix);
  tcase_add_test(core, test_s21_sum_matrix);
  tcase_add_test(core, test_s21_sub_matrix);
  tcase_add_test(core, test_s21_mult_number);
  tcase_add_test(core, test_s21_mult_matrix);
  tcase_add_test(core, test_s21_transpose);
  tcase_add_test(core, test_s21_calc_complements);
  tcase_add_test(core, test_s21_determinant);
  tcase_add_test(core, test_s21_inverse_matrix);

  suite_add_tcase(suite, core);

  return (suite);
}

int main(void) {
  int failed = 0;
  Suite *suite;

  SRunner *runner;

  suite = s21_matrix_suite();
  runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
