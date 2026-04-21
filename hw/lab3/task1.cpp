#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Point {
    double x, y;
};

double distance(Point p, double a, double b) {
    return abs(a * p.x - p.y + b) / sqrt(a * a + 1);
}

void fitLine(const vector<Point>& pts, double &a, double &b) {
    double sumX=0, sumY=0, sumXY=0, sumXX=0;
    int n = pts.size();

    for (auto &p : pts) {
        sumX += p.x;
        sumY += p.y;
        sumXY += p.x * p.y;
        sumXX += p.x * p.x;
    }

    a = (n*sumXY - sumX*sumY) / (n*sumXX - sumX*sumX);
    b = (sumY - a*sumX) / n;
}

int main() {
    srand(time(0));

    // test1
    vector<Point> points = {
        {1,2}, {2,4.1}, {3,5.9}, {4,8.2},
        {5,20}, {6,-10}
    };

    // test2
    // vector<Point> points = {
    //     {0,1}, {1,3}, {2,5}, {3,7},
    //     {10,-5}, {-5,20}
    // };

    int iterations = 100;
    double threshold = 0.5;
    int minInliers = 3;

    double bestA = 0, bestB = 0;
    int bestInliers = 0;

    for (int i = 0; i < iterations; i++) {
        int i1 = rand() % points.size();
        int i2 = rand() % points.size();
        if (i1 == i2) continue;

        Point p1 = points[i1];
        Point p2 = points[i2];

        if (abs(p2.x - p1.x) < 1e-6) continue;

        double a = (p2.y - p1.y) / (p2.x - p1.x);
        double b = p1.y - a * p1.x;

        vector<Point> inliers;

        for (auto &p : points) {
            if (distance(p, a, b) < threshold)
                inliers.push_back(p);
        }

        if (inliers.size() >= minInliers && inliers.size() > bestInliers) {
            double refinedA, refinedB;
            fitLine(inliers, refinedA, refinedB);

            bestA = refinedA;
            bestB = refinedB;
            bestInliers = inliers.size();
        }
    }

    cout << "Best line: y = " << bestA << "x + " << bestB << endl;
    cout << "Inliers: " << bestInliers << " / " << points.size() << endl;

    return 0;
}