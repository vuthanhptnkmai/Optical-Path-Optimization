/**
dco/c++ v3.8.1
    -- Algorithmic Differentiation by Operator Overloading in C++

COPYRIGHT 2022
The Numerical Algorithms Group Limited and
Software and Tools for Computational Engineering @ RWTH Aachen University

This file is part of dco/c++.
**/

#pragma once

#include <cmath>
#include "../ad_data_view.hpp"
#include "../lapack.hpp"

namespace pde {

template <typename ATYPE> class CrankNicholson {
public:
  const ACTIVE_INPUTS<ATYPE>& X;
  const PASSIVE_INPUTS& XP;
  std::vector<ATYPE> LHSj_dl, LHSj_d, LHSj_du, Vprev, Vcurr;
  ATYPE* RHSj;
  ATYPE xmin, xmax, tmax;
  double tmin;
  CrankNicholson(const ACTIVE_INPUTS<ATYPE>& X, const PASSIVE_INPUTS& XP)
      : X(X), XP(XP), LHSj_dl(XP.N - 1), LHSj_d(XP.N), LHSj_du(XP.N - 1), Vprev(XP.N + 2), Vcurr(XP.N + 2) {}
  void swapVs() { std::swap(Vcurr, Vprev); }

private:
  CrankNicholson();
  CrankNicholson(const CrankNicholson&);
  CrankNicholson operator=(const CrankNicholson&);

public:
  void setBoundaryConditions(int ts, std::vector<ATYPE>& V);
  void prepareLHS(int ts);
  void prepareRHS(int);
  void solveTridiagonalSystem();
};

template <typename ATYPE> void CrankNicholson<ATYPE>::setBoundaryConditions(int ts, std::vector<ATYPE>& V) {
  const ATYPE dx = (xmax - xmin) / (XP.N + 1);
  const ATYPE dt = X.T / (XP.M + 1);

  if (ts == XP.M + 1) {
    for (int i = 1; i < XP.N + 1; i++) {
      const ATYPE xi = xmin + i * dx;
      const ATYPE Si = exp(xi);
      if (Si - X.K > 0.0)
        V[i] = Si - X.K;
      else
        V[i] = 0.0;
    }
  }
  V[0] = 0.0;
  const ATYPE Smax = exp(xmax);
  const ATYPE t_j = ts * dt;
  V[XP.N + 1] = Smax - exp(-X.r * (X.T - t_j)) * X.K;
}

template <typename ATYPE> void CrankNicholson<ATYPE>::prepareLHS(const int j_time) {
  const int j = j_time;
  const ATYPE dt = (tmax - tmin) / (XP.M + 1);
  const ATYPE t = tmin + j * dt;
  const ATYPE dx = (xmax - xmin) / (XP.N + 1);
  const ATYPE alpha = dt / (dx * dx);
  const ATYPE z = 1.0 + X.r * dt;

  int row = 0;
  ATYPE x = xmin + (row + 1) * dx;
  ATYPE u_ij, l_ij, c_ij;
  ATYPE vhat = X.sigmaSq(x, t);
  u_ij = 0.5 * alpha * (vhat + dx * (X.r - 0.5 * vhat));
  c_ij = -alpha * vhat;
  LHSj_du[row] = -0.5 * u_ij;
  LHSj_d[row] = z - 0.5 * c_ij;
  for (row = 1; row < XP.N - 1; row++) {
    x = xmin + (row + 1) * dx;
    vhat = X.sigmaSq(x, t);
    u_ij = 0.5 * alpha * (vhat + dx * (X.r - 0.5 * vhat));
    l_ij = 0.5 * alpha * (vhat - dx * (X.r - 0.5 * vhat));
    c_ij = -alpha * vhat;
    LHSj_dl[row - 1] = -0.5 * l_ij;
    LHSj_d[row] = z - 0.5 * c_ij;
    LHSj_du[row] = -0.5 * u_ij;
  }
  row = XP.N - 1;
  x = xmin + (row + 1) * dx;
  vhat = X.sigmaSq(x, t);
  l_ij = 0.5 * alpha * (vhat - dx * (X.r - 0.5 * vhat));
  c_ij = -alpha * vhat;
  LHSj_dl[row - 1] = -0.5 * l_ij;
  LHSj_d[row] = z - 0.5 * c_ij;
}

template <typename ATYPE> void CrankNicholson<ATYPE>::prepareRHS(const int j_time) {
  const int j = j_time;
  const ATYPE dt = (tmax - tmin) / (XP.M + 1);
  const ATYPE t = tmin + j * dt;
  const ATYPE dx = (xmax - xmin) / (XP.N + 1);
  const ATYPE alpha = dt / (dx * dx);
  std::vector<ATYPE> RHSj_d(XP.N), RHSj_du(XP.N - 1), RHSj_dl(XP.N - 1);

  int row = 0;
  ATYPE c_ij, u_ij, l_ij;
  ATYPE x = xmin + (row + 1) * dx;
  ATYPE vhat = X.sigmaSq(x, t);
  u_ij = 0.5 * alpha * (vhat + dx * (X.r - 0.5 * vhat));
  c_ij = -alpha * vhat;
  RHSj_du[row] = 0.5 * u_ij;
  RHSj_d[row] = 0.5 * c_ij + 1.0;
  for (row = 1; row < XP.N - 1; row++) {
    x = xmin + (row + 1) * dx;
    vhat = X.sigmaSq(x, t);
    u_ij = 0.5 * alpha * (vhat + dx * (X.r - 0.5 * vhat));
    l_ij = 0.5 * alpha * (vhat - dx * (X.r - 0.5 * vhat));
    c_ij = -alpha * vhat;
    RHSj_dl[row - 1] = 0.5 * l_ij;
    RHSj_d[row] = 0.5 * c_ij + 1.0;
    RHSj_du[row] = 0.5 * u_ij;
  }
  row = XP.N - 1;
  x = xmin + (row + 1) * dx;
  vhat = X.sigmaSq(x, t);
  l_ij = 0.5 * alpha * (vhat - dx * (X.r - 0.5 * vhat));
  c_ij = -alpha * vhat;
  RHSj_dl[row - 1] = 0.5 * l_ij;
  RHSj_d[row] = 0.5 * c_ij + 1.0;

  row = 0;
  RHSj[row] = RHSj_d[row] * Vprev[1] + RHSj_du[row] * Vprev[2];
  x = xmin + (row + 1) * dx;
  vhat = X.sigmaSq(x, t);
  l_ij = 0.5 * alpha * (vhat - dx * (X.r - 0.5 * vhat));
  RHSj[row] += l_ij * 0.5 * (Vcurr[0] + Vprev[0]);
  for (row = 1; row < XP.N - 1; row++)
    RHSj[row] = RHSj_dl[row - 1] * Vprev[row] + RHSj_d[row] * Vprev[row + 1] + RHSj_du[row] * Vprev[row + 2];

  row = XP.N - 1;
  x = xmin + (row + 1) * dx;
  RHSj[row] = RHSj_dl[row - 1] * Vprev[row] + RHSj_d[row] * Vprev[row + 1];
  vhat = X.sigmaSq(x, t);
  u_ij = 0.5 * alpha * (vhat + dx * (X.r - 0.5 * vhat));
  RHSj[row] += u_ij * 0.5 * (Vcurr[XP.N + 1] + Vprev[XP.N + 1]);
}

template <typename ATYPE> void CrankNicholson<ATYPE>::solveTridiagonalSystem() {
  std::vector<int> LHSj_ipiv(XP.N);
  std::vector<ATYPE> LHSj_du2(XP.N - 2);

  int info = 0;
  dgttrf(XP.N, LHSj_dl.data(), LHSj_d.data(), LHSj_du.data(), LHSj_du2.data(), LHSj_ipiv.data(), &info);
  dgttrs("N", XP.N, 1, LHSj_dl.data(), LHSj_d.data(), LHSj_du.data(), LHSj_du2.data(), LHSj_ipiv.data(), RHSj, XP.N,
         &info);
}

template <typename ATYPE>
void price(const ACTIVE_INPUTS<ATYPE>& X, const PASSIVE_INPUTS& XP, ACTIVE_OUTPUTS<ATYPE>& Y) {
  CrankNicholson<ATYPE> CN(X, XP);
  const double tmin = 0.0;
  CN.tmin = tmin;
  const ATYPE tmax = X.T;
  CN.tmax = tmax;
  const ATYPE logS0 = log(X.S0);
  const ATYPE atmvol = sqrt(X.T * X.sigmaSq(logS0, X.T));

  const int C = 10;
  const ATYPE dx = 2 * C * atmvol / (XP.N + 1);
  const ATYPE xmin = logS0 - XP.N / 2 * dx;
  CN.xmin = xmin;
  const ATYPE xmax = xmin + (XP.N + 1) * dx;
  CN.xmax = xmax;

  CN.setBoundaryConditions(XP.M + 1, CN.Vprev);
  for (int j = XP.M; j >= 0; j--) {
    CN.setBoundaryConditions(j, CN.Vcurr);
    CN.prepareLHS(j);
    CN.RHSj = &CN.Vcurr[1];
    CN.prepareRHS(j);
    CN.solveTridiagonalSystem();
    CN.swapVs();
  }
  Y.V = CN.Vprev[XP.N / 2];
}

} // namespace pde
