#include "SimpleAlgorithm.h"

static int sep_radius = 40;
static int alignment_radius = 200;
static float collisionCutOff = 10.0;
static float collisionBreakOne = 1000.0;

namespace
{

double wrapAround(double value, double min, double max)
{
    double range = max - min;
    double wrappedValue = value;

    while (wrappedValue < min)
    {
        wrappedValue += range;
    }

    while (wrappedValue >= max)
    {
        wrappedValue -= range;
    }

    return wrappedValue;
}

void boids_algorithm(std::vector<Bird>& birds, std::vector<Obstacle>& obs, double separation_factor = 1,
                     double alignment_factor = 1, double cohesion_factor = 1)
{
    size_t num_boids = birds.size();

    // Rule 1: Separation - steer to avoid crowding nearby boids
    std::vector<Vec> separation_vectors(num_boids, Vec());
    std::vector<Vec> alignment_vectors(num_boids, Vec());
    std::vector<Vec> cohesion_vectors(num_boids, Vec());
    std::vector<Vec> obstacle_avoidance_vectors(num_boids, Vec());
    for (size_t i = 0; i < num_boids; ++i)
    {
        Vec average_align_velocity;
        int align_neighbours_count = 0;
        Vec average_position;
        for (size_t j = 0; j < num_boids; ++j)
        {
            if (i != j)
            {
                Vec diff = birds[i].position - birds[j].position;
                double distance = diff.length();
                if (distance < sep_radius)
                    separation_vectors[i] = separation_vectors[i] + (diff.normalized() * (sep_radius - distance));
                if (distance < alignment_radius)
                {
                    average_align_velocity = average_align_velocity + birds[j].velocity;
                    average_position = average_position + birds[j].position;
                }
            }
        }
        if (align_neighbours_count != 0)
        {
            average_align_velocity = average_align_velocity * (1.0 / align_neighbours_count);
            average_position = average_position * (1.0 / align_neighbours_count);
        }
        alignment_vectors[i] = (average_align_velocity - birds[i].velocity);
        alignment_vectors[i].normalize();
        cohesion_vectors[i] = (average_position - birds[i].position);
        cohesion_vectors[i].normalize();
        separation_vectors[i].normalize();
        int no_of_close_obs = 0;
        for (int j = 0; j < obs.size(); j++)
        {
            Obstacle& obstacle = obs[j];
            Vec dir = obstacle.center - birds[i].position;
            float distLength = obstacle.distanceTo(birds[i].position);

            if (distLength != 0 && distLength < collisionCutOff)
            {
                dir.toLength(-collisionBreakOne / distLength);
                obstacle_avoidance_vectors[i] += dir;
            }
        }
        if (no_of_close_obs != 0)
        {
            obstacle_avoidance_vectors[i] = obstacle_avoidance_vectors[i] * (1.0 / no_of_close_obs);
            obstacle_avoidance_vectors[i].normalize();
        }
    }

    
    float total = separation_factor + alignment_factor + cohesion_factor;
    // Update velocities and positions
    for (size_t i = 0; i < num_boids; ++i)
    {
        birds[i].velocity = birds[i].velocity + (separation_vectors[i] * (separation_factor / total)) +
                            (alignment_vectors[i] * (alignment_factor / total)) +
                            (cohesion_vectors[i] * (cohesion_factor / total)) + obstacle_avoidance_vectors[i];
        birds[i].velocity.toLength(4);
        birds[i].position = birds[i].position + birds[i].velocity;

        birds[i].angle = birds[i].velocity.angle();
    }
}
} // namespace

SimpleAlgorithm::SimpleAlgorithm(std::vector<Bird> const& birds, std::vector<Obstacle> const& obs)
    : birds(birds), obstacles(obs)
{
}

std::vector<Bird> const& SimpleAlgorithm::update()
{
    boids_algorithm(birds, obstacles);
    return birds;
}