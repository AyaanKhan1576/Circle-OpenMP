#include <iostream>
#include <omp.h>
#include <cmath>
#include <vector>
#include <GL/glut.h>

using namespace std;

const int radius = 150;
const int originX = 300;
const int originY = 300;
const double angle_step = 1.0; 
const int num_points = static_cast<int>(360.0 / angle_step) + 1;

vector<pair<float, float>> circle_points_parallel;
vector<pair<float, float>> circle_points_serial;

double degree_to_radians(double degree) {
    return (degree / 180.0) * 3.1415926535;
}

// ---------- Serial Taylor Functions ----------
double serial_cos(double radians) {
    double sum = 0.0;
    for (int i = 0; i < 50; i++) {
        double term = pow(-1, i) * pow(radians, 2 * i) / tgamma(2 * i + 1);
        sum += term;
    }
    return sum;
}

double serial_sin(double radians) {
    double sum = 0.0;
    for (int i = 0; i < 50; i++) {
        double term = pow(-1, i) * pow(radians, 2 * i + 1) / tgamma(2 * i + 2);
        sum += term;
    }
    return sum;
}

// ---------- Parallel Taylor Functions ----------
double parallel_cos(double radians) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 15; i++) {
        double term = pow(-1, i) * pow(radians, 2 * i) / tgamma(2 * i + 1);
        sum += term;
    }
    return sum;
}

double parallel_sin(double radians) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < 15; i++) {
        double term = pow(-1, i) * pow(radians, 2 * i + 1) / tgamma(2 * i + 2);
        sum += term;
    }
    return sum;
}

// ---------- Parametric Functions ----------
float serial_parametric_x(int r, double rad, int j) {
    return static_cast<float>(r * serial_cos(rad) + j);
}

float serial_parametric_y(int r, double rad, int k) {
    return static_cast<float>(r * serial_sin(rad) + k);
}

float parallel_parametric_x(int r, double rad, int j) {
    return static_cast<float>(r * parallel_cos(rad) + j);
}

float parallel_parametric_y(int r, double rad, int k) {
    return static_cast<float>(r * parallel_sin(rad) + k);
}

// ---------- Serial Circle Computation ----------
void compute_circle_points_serial() {
    circle_points_serial.resize(num_points);
    double start = omp_get_wtime();

    for (int i = 0; i < num_points; ++i) {
        double angle = i * angle_step;
        double radians = degree_to_radians(angle);
        float x = serial_parametric_x(radius, radians, originX - 10);
        float y = serial_parametric_y(radius, radians, originY - 10);
        circle_points_serial[i] = { x, y };
    }

    double end = omp_get_wtime();
    cout << "Serial computation time:   " << (end - start) << " seconds\n";
}

// ---------- Parallel Circle Computation ----------
void compute_circle_points_parallel() {
    circle_points_parallel.resize(num_points);
    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < num_points; ++i) {
        double angle = i * angle_step;
        double radians = degree_to_radians(angle);
        float x = parallel_parametric_x(radius, radians, originX);
        float y = parallel_parametric_y(radius, radians, originY);
        circle_points_parallel[i] = { x, y };
    }

    double end = omp_get_wtime();
    cout << "Parallel computation time: " << (end - start) << " seconds\n";
}

// ---------- OpenGL Drawing ----------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw serial circle (Red, shifted left/down)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (const auto& pt : circle_points_serial) {
        glVertex2f(pt.first, pt.second);
    }
    glEnd();

    // Draw parallel circle (Green)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (const auto& pt : circle_points_parallel) {
        glVertex2f(pt.first, pt.second);
    }
    glEnd();

    glFlush();
}

void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_LINE_SMOOTH); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 640, 0, 640);
}

int main(int argc, char** argv) {
    omp_set_num_threads(4);
    int max_threads = omp_get_max_threads();
    cout << "OpenMP max threads: " << max_threads << "\n";

    compute_circle_points_serial();
    compute_circle_points_parallel();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Smooth Circle: Serial (Red) vs Parallel (Green)");

    initOpenGL();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
