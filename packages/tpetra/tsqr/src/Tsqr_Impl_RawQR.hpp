#ifndef TSQR_IMPL_RAWQR_HPP
#define TSQR_IMPL_RAWQR_HPP

namespace TSQR {
namespace Impl {

/// \brief "Raw" local QR factorization interface.
///
/// Subclass and specialize this interface as needed.
///
/// The methods are instance methods so that subclass instances may
/// have state.  For example, a cuSOLVER implementation would have a
/// CUDA stream instance (cudaStream_t) and a cuSOLVER handle
/// (cusolverDnHandle_t).
///
/// WORK size query ("LWORK query") happens as in LAPACK, by passing
/// in lwork = -1.  A cuSOLVER Implementation would just check if
/// lwork is -1, and call cusolverDn?geqrf_bufferSize in that case
/// (replace the question mark with S, D, C, or Z as appropriate for
/// the Scalar type).
///
/// Methods are virtual because they are meant to be called from host.
/// (For the CUDA case, we plan to make cuSOLVER calls from host; we
/// don't need to call QR from device.)
template<class Scalar>
class RawQR {
public:
  using value_type = Scalar;

  virtual ~RawQR() = default;

  //! Compute QR factorization of a general m by n matrix A.
  virtual void
  compute_QR(const int m, const int n,
             value_type A[], const int lda,
             value_type TAU[],
             value_type WORK[], const int lwork) const = 0;

  /// \brief Apply Householder reflectors.
  ///
  /// Overwrite the general complex m by n matrix C with the product
  /// of Q and C, where Q is the product of k elementary (Householder)
  /// reflectors as returned by GEQRF.
  ///
  /// This corresponds to LAPACK's _UNMQR for complex value_type types,
  /// and to LAPACK's _ORMQR for real value_type types.
  virtual void
  apply_Q_factor(const char SIDE, const char TRANS,
                 const int m, const int n, const int k,
                 const value_type A[], const int lda,
                 const value_type TAU[],
                 value_type C[], const int ldc,
                 value_type WORK[], const int lwork) const = 0;

  /// \brief Compute explicit QR factor from QR factorization (GEQRF).
  ///
  /// Generate the m by n matrix Q with orthonormal (or unitary, if
  /// value_type is complex) columns corresponding to the first n columns
  /// of a product of k elementary reflectors of order m, as returned
  /// by GEQRF.
  ///
  /// This corresponds to LAPACK's _UNGQR for complex value_type types,
  /// and to LAPACK's _ORGQR for real value_type types.
  virtual void
  compute_explicit_Q(const int m, const int n, const int k,
                     value_type A[], const int lda,
                     const value_type TAU[],
                     value_type WORK[], const int lwork) const = 0;
};

} // namespace Impl
} // namespace TSQR

#endif // TSQR_IMPL_RAWQR_HPP
