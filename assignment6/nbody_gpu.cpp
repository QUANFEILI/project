#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <string>
#include <cuda_runtime.h>

double G = 6.674*std::pow(10,-11);
//double G = 1;

__constant__ double G_device = 6.674e-11;

struct simulation {
  size_t nbpart;
  
  std::vector<double> mass;

  //position
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;

  //velocity
  std::vector<double> vx;
  std::vector<double> vy;
  std::vector<double> vz;

  //force
  std::vector<double> fx;
  std::vector<double> fy;
  std::vector<double> fz;

  simulation(size_t nb)
    :nbpart(nb), mass(nb),
     x(nb), y(nb), z(nb),
     vx(nb), vy(nb), vz(nb),
     fx(nb), fy(nb), fz(nb) 
  {}
};

void random_init(simulation& s) {
  std::random_device rd;  
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dismass(0.9, 1.);
  std::normal_distribution<double> dispos(0., 1.);
  std::normal_distribution<double> disvel(0., 1.);

  for (size_t i = 0; i<s.nbpart; ++i) {
    s.mass[i] = dismass(gen);

    s.x[i] = dispos(gen);
    s.y[i] = dispos(gen);
    s.z[i] = dispos(gen);
    s.z[i] = 0.;
    
    s.vx[i] = disvel(gen);
    s.vy[i] = disvel(gen);
    s.vz[i] = disvel(gen);
    s.vz[i] = 0.;
    s.vx[i] = s.y[i]*1.5;
    s.vy[i] = -s.x[i]*1.5;
  }

  return;
  //normalize velocity (using normalization found on some physicis blog)
  double meanmass = 0;
  double meanmassvx = 0;
  double meanmassvy = 0;
  double meanmassvz = 0;
  for (size_t i = 0; i<s.nbpart; ++i) {
    meanmass += s.mass[i];
    meanmassvx += s.mass[i] * s.vx[i];
    meanmassvy += s.mass[i] * s.vy[i];
    meanmassvz += s.mass[i] * s.vz[i];
  }
  for (size_t i = 0; i<s.nbpart; ++i) {
    s.vx[i] -= meanmassvx/meanmass;
    s.vy[i] -= meanmassvy/meanmass;
    s.vz[i] -= meanmassvz/meanmass;
  }
}

void init_solar(simulation& s) {
  enum Planets {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, MOON};
  s = simulation(10);

  s.mass[SUN] = 1.9891 * std::pow(10, 30);
  s.mass[MERCURY] = 3.285 * std::pow(10, 23);
  s.mass[VENUS] = 4.867 * std::pow(10, 24);
  s.mass[EARTH] = 5.972 * std::pow(10, 24);
  s.mass[MARS] = 6.39 * std::pow(10, 23);
  s.mass[JUPITER] = 1.898 * std::pow(10, 27);
  s.mass[SATURN] = 5.683 * std::pow(10, 26);
  s.mass[URANUS] = 8.681 * std::pow(10, 25);
  s.mass[NEPTUNE] = 1.024 * std::pow(10, 26);
  s.mass[MOON] = 7.342 * std::pow(10, 22);

  double AU = 1.496 * std::pow(10, 11);

  s.x = {0, 0.39*AU, 0.72*AU, 1.0*AU, 1.52*AU, 5.20*AU, 9.58*AU, 19.22*AU, 30.05*AU, 1.0*AU + 3.844*std::pow(10, 8)};
  s.y = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  s.z = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  s.vx = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  s.vy = {0, 47870, 35020, 29780, 24130, 13070, 9680, 6800, 5430, 29780 + 1022};
  s.vz = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

void dump_state(simulation& s) {
  std::cout<<s.nbpart<<'\t';
  for (size_t i=0; i<s.nbpart; ++i) {
    std::cout<<s.mass[i]<<'\t';
    std::cout<<s.x[i]<<'\t'<<s.y[i]<<'\t'<<s.z[i]<<'\t';
    std::cout<<s.vx[i]<<'\t'<<s.vy[i]<<'\t'<<s.vz[i]<<'\t';
    std::cout<<s.fx[i]<<'\t'<<s.fy[i]<<'\t'<<s.fz[i]<<'\t';
  }
  std::cout<<'\n';
}

void load_from_file(simulation& s, std::string filename) {
  std::ifstream in (filename);
  size_t nbpart;
  in>>nbpart;
  s = simulation(nbpart);
  for (size_t i=0; i<s.nbpart; ++i) {
    in>>s.mass[i];
    in >>  s.x[i] >>  s.y[i] >>  s.z[i];
    in >> s.vx[i] >> s.vy[i] >> s.vz[i];
    in >> s.fx[i] >> s.fy[i] >> s.fz[i];
  }
  if (!in.good())
    throw "kaboom";
}

__global__ void compute_forces_kernel(
  size_t nbpart,
  const double* mass,
  const double* x, const double* y, const double* z,
  double* fx, double* fy, double* fz
) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i >= nbpart) return;

  double xi = x[i], yi = y[i], zi = z[i];
  double m_i = mass[i];
  double fxi = 0., fyi = 0., fzi = 0.;

  for (int j = 0; j < nbpart; ++j) {
    if (i == j) continue;
    double dx = x[j] - xi;
    double dy = y[j] - yi;
    double dz = z[j] - zi;
    double dist_sq = dx*dx + dy*dy + dz*dz + 0.1;
    double dist = sqrt(dist_sq);
    double F = G_device * m_i * mass[j] / dist_sq;

    fxi += F * dx / dist;
    fyi += F * dy / dist;
    fzi += F * dz / dist;
  }

  fx[i] = fxi;
  fy[i] = fyi;
  fz[i] = fzi;
}

__global__ void update_positions_kernel(
  size_t nbpart,
  double dt,
  const double* mass,
  double* x, double* y, double* z,
  double* vx, double* vy, double* vz,
  const double* fx, const double* fy, const double* fz
) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i >= nbpart) return;

  vx[i] += fx[i] / mass[i] * dt;
  vy[i] += fy[i] / mass[i] * dt;
  vz[i] += fz[i] / mass[i] * dt;

  x[i] += vx[i] * dt;
  y[i] += vy[i] * dt;
  z[i] += vz[i] * dt;
}

int main(int argc, char* argv[]) {
  if (argc != 6) {
    std::cerr
      <<"usage: "<<argv[0]<<" <input> <dt> <nbstep> <printevery> <blocksize>\n"
      <<"input can be:\n"
      <<"a number (random initialization)\n"
      <<"planet (initialize with solar system)\n"
      <<"a filename (load from file in singleline tsv)\n";
    return -1;
  }

  double dt = std::atof(argv[2]); 
  size_t nbstep = std::atol(argv[3]);
  size_t printevery = std::atol(argv[4]);
  int blockSize = std::atoi(argv[5]);

  simulation s(1);
  {
    size_t nbpart = std::atol(argv[1]);
    if ( nbpart > 0) {
      s = simulation(nbpart);
      random_init(s);
    } else {
      std::string inputparam = argv[1];
      if (inputparam == "planet") {
        init_solar(s);
      } else{
        load_from_file(s, inputparam);
      }
    }    
  }

  size_t N = s.nbpart, size = N * sizeof(double);
  double *d_mass, *d_x, *d_y, *d_z, *d_vx, *d_vy, *d_vz, *d_fx, *d_fy, *d_fz;

  cudaMalloc(&d_mass, size);
  cudaMalloc(&d_x, size); cudaMalloc(&d_y, size); cudaMalloc(&d_z, size);
  cudaMalloc(&d_vx, size); cudaMalloc(&d_vy, size); cudaMalloc(&d_vz, size);
  cudaMalloc(&d_fx, size); cudaMalloc(&d_fy, size); cudaMalloc(&d_fz, size);

  cudaMemcpy(d_mass, s.mass.data(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_x, s.x.data(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_y, s.y.data(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_z, s.z.data(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_vx, s.vx.data(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_vy, s.vy.data(), size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_vz, s.vz.data(), size, cudaMemcpyHostToDevice);

  int gridSize = (N + blockSize - 1) / blockSize;

  for (size_t step = 0; step < nbstep; ++step) {
    compute_forces_kernel<<<gridSize, blockSize>>>(N, d_mass, d_x, d_y, d_z, d_fx, d_fy, d_fz);
    update_positions_kernel<<<gridSize, blockSize>>>(N, dt, d_mass, d_x, d_y, d_z, d_vx, d_vy, d_vz, d_fx, d_fy, d_fz);

    if (step % printevery == 0) {
      cudaMemcpy(s.x.data(), d_x, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.y.data(), d_y, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.z.data(), d_z, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.vx.data(), d_vx, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.vy.data(), d_vy, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.vz.data(), d_vz, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.fx.data(), d_fx, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.fy.data(), d_fy, size, cudaMemcpyDeviceToHost);
      cudaMemcpy(s.fz.data(), d_fz, size, cudaMemcpyDeviceToHost);
      dump_state(s);
    }
  }

  cudaFree(d_mass); cudaFree(d_x); cudaFree(d_y); cudaFree(d_z);
  cudaFree(d_vx); cudaFree(d_vy); cudaFree(d_vz);
  cudaFree(d_fx); cudaFree(d_fy); cudaFree(d_fz);

  return 0;
}
