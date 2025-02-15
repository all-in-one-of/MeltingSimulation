#ifndef MATHFUNCTIONS
#define MATHFUNCTIONS

#include <vector>
#include <iostream>

#include <ngl/Vec3.h>
#include <ngl/Mat3.h>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/IterativeLinearSolvers>
#include <eigen3/Eigen/QR>
#include <eigen3/Eigen/SVD>

#include <omp.h>



//------------------------------------------------------------------------------------------------------------------------------------------------------
/// @file MathFunctions.h
/// @brief Structure which contains general mathematical calculations. Most of these will be using the Eigen library.
/// @author Ina M. Sorensen
/// @version 1.0
/// @date 27.06.16
///
/// @todo
//------------------------------------------------------------------------------------------------------------------------------------------------------


struct MathFunctions
{
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get vector index from (i,j,k) cell index
  /// @param [in] Cell index in 3d (i,j,k)
  /// @param [in] _noCells is the total number of cells in grid
  //----------------------------------------------------------------------------------------------------------------------
  static int getVectorIndex(int i, int j, int k, int _noCells);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Get cell index of a particle
  /// @param [in] Position of particle
  //----------------------------------------------------------------------------------------------------------------------
  static Eigen::Vector3i getParticleGridCell(Eigen::Vector3f _particlePosition, float _cellSize, Eigen::Vector3f _gridEdgeOrigin);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns value of cubic B-spline
  /// @param [in] Position in a single direction, x
  //----------------------------------------------------------------------------------------------------------------------
  static float calcCubicBSpline(float _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns value of differentiated cubic B-spline
  /// @param [in] Position in a single direction, x
  /// @todo Doesn't make sure that weight is positive. Is this important?
  //----------------------------------------------------------------------------------------------------------------------
  static float calcCubicBSpline_Diff(float _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns value of integrated cubic B-spline over values x1 and x0
  /// @param [in] Position to be integrated over in a single direction, x1 and x0
  //----------------------------------------------------------------------------------------------------------------------
  static float calcCubicBSpline_Integ(int _faceDirection, int _iIndexIncrement, int _jIndexIncrement, int _kIndexIncrement);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns value of tight quadratic stencil
  /// @param [in] Position in a single direction, x
  /// @todo Doesn't make sure that weight is positive. Is this important?
  //----------------------------------------------------------------------------------------------------------------------
  static float calcTightQuadraticStencil(float _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns value of differentiated tight quadratic stencil
  /// @param [in] Position in a single direction, x
  /// @todo Doesn't make sure that weight is positive. Is this important?
  //----------------------------------------------------------------------------------------------------------------------
  static float calcTightQuadraticStencil_Diff(float _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Solves Ax=B using MINRES method written based on Stanford code
  /// @param [in] _A and _B are the matrices in the problem
  /// @param [in] _preconditioner is the preconditioner matrix which must be positive definite. If no preconditioner, pass nullptr
  /// @param [in] _shift is a value other than zero if want to solve (A-shift*I)x=b instead
  /// @param [in] _maxLoops is the max number of loops the method will do
  /// @param [in] _tolerance is the value below which the function will exit.
  /// @param[out] o_x is the solution
  /// @todo Need to work out how to apply a preconditioner
  //----------------------------------------------------------------------------------------------------------------------
  static void MinRes(const Eigen::MatrixXf &_A, const Eigen::VectorXf &_B, Eigen::VectorXf &io_x, const Eigen::MatrixXf &_preconditioner, float _shift, float _maxLoops, float _tolerance, bool _show);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Solves Ax=B using conjugate gradient method. Only works for square matrix A
  /// @param [in] _A and _B which are a 2 and 1 dimensional matrix respectively.
  /// @param [in] _maxLoops is the max number of loops the method will do unless _minResidual is met first.
  /// @param [in] _x0 is a 1 dimensional vector giving the first guess at the solution
  /// @param[out] o_x is the solution
  //----------------------------------------------------------------------------------------------------------------------
  static void conjugateGradient(const Eigen::SparseMatrix<double> &_A, const Eigen::VectorXd &_B, Eigen::VectorXd &o_x, float _maxLoops, float _minResidual);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Solves Ax=B for all possible matrices A. Will use a method in Eigen that is slow, so only used for small matrices
  /// @param [in] _A and _B which are a 2 and 1 dimensional matrix respectively.
  /// @param[out] o_x is the solution
  //----------------------------------------------------------------------------------------------------------------------
  static void linearSystemSolve(const Eigen::Matrix3f &_A, const Eigen::Vector3f &_B, Eigen::Vector3f &o_x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Polar decomposition
  /// @param [in] _decomposeMatrix is the matrix to be polar decomposed
  /// @param [out] o_R is the rotation matrix, o_S is the stretch matrix
  //----------------------------------------------------------------------------------------------------------------------
  static void polarDecomposition(const Eigen::Matrix3f &_decomposeMatrix, Eigen::Matrix3f &o_R, Eigen::Matrix3f &o_S);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Singular value decomposition
  /// @param [in] _decomposeMatrix is the matrix to be decomposed
  /// @param [out] o_U and o_V are the left and right singular vectors respectively
  /// @param [out] o_singularValues is a diagonal matrix containing the singular values.
  //----------------------------------------------------------------------------------------------------------------------
  static void singularValueDecomposition(const Eigen::Matrix3f &_decomposeMatrix, Eigen::Matrix3f &o_U, Eigen::Matrix3f &o_singularValues, Eigen::Matrix3f &o_V);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculates the central difference gradient
  //----------------------------------------------------------------------------------------------------------------------
  static void centralDifferenceGradient();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Sign function. Gives sign of input value and zero if input is zero. Used for differentiating |x|
  //----------------------------------------------------------------------------------------------------------------------
  static float signFunction(float _x);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Find minimum above zero in vector
  //----------------------------------------------------------------------------------------------------------------------
  static int findMinVectorValue(const std::vector<int> &_vectorList);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A:B - Multiply every element in A with every element in B
  /// NB! For 3x3 matrices only
  //----------------------------------------------------------------------------------------------------------------------
  static Eigen::Matrix3f matrixElementMultiplication(const Eigen::Matrix3f &_A, const Eigen::Matrix3f &_B);

};

#endif // MATHFUNCTIONS

