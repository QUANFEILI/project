#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <iomanip>

constexpr double G = 6.674e-11; // Gravitational constant
constexpr double SOFTENING = 1e-9; // Softening factor to prevent singularities

struct Particle {
    double mass;
    double x, y, z;
    double vx, vy, vz;
    double fx, fy, fz;
};

class NBodySimulation {
private:
    std::vector<Particle> particles;
    double dt;
    size_t steps;
    size_t output_interval;

public:     

    // Constructor to initialize simulation parameters
    NBodySimulation(size_t num_particles, double time_step, size_t num_steps, size_t interval)
        : dt(time_step), steps(num_steps), output_interval(interval) {
        initialize_particles(num_particles);
    }

    // Initialize particles with random properties
    void initialize_particles(size_t num_particles) {
        std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<double> pos_dist(-1e11, 1e11);
        std::uniform_real_distribution<double> vel_dist(-1e3, 1e3);
        std::uniform_real_distribution<double> mass_dist(1e20, 1e30);

        particles.resize(num_particles);
        for (auto& p : particles) {
            p.mass = mass_dist(gen);
            p.x = pos_dist(gen);
            p.y = pos_dist(gen);
            p.z = pos_dist(gen);
            p.vx = vel_dist(gen);
            p.vy = vel_dist(gen);
            p.vz = vel_dist(gen);
            p.fx = p.fy = p.fz = 0.0;
        }
    }

    // Compute gravitational forces between particles
    void compute_forces() {
        for (auto& p : particles) {
            p.fx = p.fy = p.fz = 0.0;
        }

        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                double dx = particles[j].x - particles[i].x;
                double dy = particles[j].y - particles[i].y;
                double dz = particles[j].z - particles[i].z;
                double dist_sq = dx * dx + dy * dy + dz * dz + SOFTENING;
                double dist = std::sqrt(dist_sq);
                double force = G * particles[i].mass * particles[j].mass / dist_sq;
                double fx = force * dx / dist;
                double fy = force * dy / dist;
                double fz = force * dz / dist;

                particles[i].fx += fx;
                particles[i].fy += fy;
                particles[i].fz += fz;
                particles[j].fx -= fx;
                particles[j].fy -= fy;
                particles[j].fz -= fz;
            }
        }
    }

    // Update velocities and positions using numerical integration
    void integrate() {
        for (auto& p : particles) {
            double ax = p.fx / p.mass;
            double ay = p.fy / p.mass;
            double az = p.fz / p.mass;
            
            p.vx += ax * dt;
            p.vy += ay * dt;
            p.vz += az * dt;
            
            p.x += p.vx * dt;
            p.y += p.vy * dt;
            p.z += p.vz * dt;
        }
    }

    // Save current state to output file
    void save_state(std::ofstream& file) {
        file << particles.size();
        for (const auto& p : particles) {
            file << "\t" << p.mass << "\t" << p.x << "\t" << p.y << "\t" << p.z
                 << "\t" << p.vx << "\t" << p.vy << "\t" << p.vz
                 << "\t" << p.fx << "\t" << p.fy << "\t" << p.fz;
        }
        file << "\n";
    }

    // Run the simulation and save state at defined intervals
    void run_simulation(const std::string& output_file) {
        std::ofstream file(output_file);
        if (!file) {
            std::cerr << "Error: Unable to open output file!" << std::endl;
            return;
        }
        
        for (size_t step = 0; step < steps; ++step) {
            compute_forces();
            integrate();
            if (step % output_interval == 0) {
                save_state(file);
            }
        }
        file.close();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <num_particles> <dt> <steps> <output_interval> <output_file>" << std::endl;
        return 1;
    }
    
    size_t num_particles = std::stoul(argv[1]);
    double dt = std::stod(argv[2]);
    size_t steps = std::stoul(argv[3]);
    size_t output_interval = std::stoul(argv[4]);
    std::string output_file = argv[5];
    
    NBodySimulation simulation(num_particles, dt, steps, output_interval);
    simulation.run_simulation(output_file);
    
    return 0;
}
