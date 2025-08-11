#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <iomanip>
#include <chrono>

constexpr double G = 6.674e-11; // Gravitational constant
constexpr double SOFTENING = 1e-9; // Softening factor to prevent singularities

// Structure to represent a particle
struct Particle {
    double mass;    // Mass
    double x, y, z; // Position coordinates
    double vx, vy, vz; // Velocity components
    double fx, fy, fz; // Force components
};

class NBodySimulation {
private:
    std::vector<Particle> particles; // List of particles
    double dt;  // Time step size
    size_t steps; // Number of simulation steps
    size_t output_interval; // Interval for saving simulation state

public:
    // Constructor
    NBodySimulation(size_t num_particles, double time_step, size_t num_steps, size_t interval)
        : dt(time_step), steps(num_steps), output_interval(interval) {
        if (num_particles > 0) {
            initialize_particles(num_particles); // Random initialization
        }
    }

    // Load particles from a file
    void load_particles_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        size_t num_particles;
        file >> num_particles; // Read number of particles
        particles.resize(num_particles);

        for (auto& p : particles) {
            file >> p.mass >> p.x >> p.y >> p.z >> p.vx >> p.vy >> p.vz;
            p.fx = p.fy = p.fz = 0.0; // Initialize forces to zero
        }
    }

    // Initialize particles with random properties
    void initialize_particles(size_t num_particles) {
        std::mt19937 gen(std::random_device{}()); // Random number generator
        std::uniform_real_distribution<double> pos_dist(-1e11, 1e11); // Position distribution
        std::uniform_real_distribution<double> vel_dist(-1e3, 1e3); // Velocity distribution
        // Mass distribution, ranging from 1e20 to 1e30
        // I think these are the true masses of some planets, specific values may be larger, I'm not sure :)
        std::uniform_real_distribution<double> mass_dist(1e20, 1e30); 
        

        particles.resize(num_particles);
        for (auto& p : particles) {
            p.mass = mass_dist(gen); // Random mass
            p.x = pos_dist(gen);     // Random position
            p.y = pos_dist(gen);
            p.z = pos_dist(gen);
            p.vx = vel_dist(gen);    // Random velocity
            p.vy = vel_dist(gen);    
            p.vz = vel_dist(gen);
            p.fx = p.fy = p.fz = 0.0; // Initialize forces to zero
        }
    }

    // Compute gravitational forces between all particles
    void compute_forces() {
        for (size_t i = 0; i < particles.size(); ++i) {
            particles[i].fx = particles[i].fy = particles[i].fz = 0.0; // Reset forces

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

    // Update particle positions and velocities
    void integrate() {
        for (auto& p : particles) {
            double inv_mass = 1.0 / p.mass; // Precompute inverse mass for efficiency
            p.vx += p.fx * inv_mass * dt;
            p.vy += p.fy * inv_mass * dt;
            p.vz += p.fz * inv_mass * dt;
            p.x += p.vx * dt;
            p.y += p.vy * dt;
            p.z += p.vz * dt;
        }
    }

    // Save simulation state to file
    void save_state(std::ofstream& file) {
        if (!file) {
            std::cerr << "Error: Unable to write output file." << std::endl;
            return;
        }

        // Start each line with the number of particles
        file << particles.size();

        // Separate particle data with tabs
        for (const auto& p : particles) {
            file << "\t" << p.mass
                 << "\t" << p.x << "\t" << p.y << "\t" << p.z
                 << "\t" << p.vx << "\t" << p.vy << "\t" << p.vz
                 << "\t" << p.fx << "\t" << p.fy << "\t" << p.fz;
        }
        file << "\n";
    }

    //Run the simulation
    void run_simulation(const std::string& output_file) {
        std::ofstream file(output_file);
        if (!file) {
            std::cerr << "Error: Unable to open output file: " << output_file << std::endl;
            return;
        }

        auto start_time = std::chrono::high_resolution_clock::now(); // Record start time

        for (size_t step = 0; step < steps; ++step) {
            compute_forces();
            integrate();
            if (step % output_interval == 0) {
                save_state(file); // Save state at regular intervals
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now(); // Record end time
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Simulation completed in " << duration.count() << " milliseconds." << std::endl;
    }
};

// Main function
int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <num_particles> <dt> <steps> <output_interval> <output_file>" << std::endl;
        return 1;
    }

    size_t num_particles = std::stoul(argv[1]);
    double dt = std::stod(argv[2]);
    size_t steps = std::stoul(argv[3]);
    size_t output_interval = std::stoul(argv[4]);
    std::string output_file = argv[5];

    NBodySimulation simulation(num_particles, dt, steps, output_interval);

    // If num_particles is 0, load data from file
    if (num_particles == 0) {
        simulation.load_particles_from_file("solar.tsv");
    }

    simulation.run_simulation(output_file);

    return 0;
}
