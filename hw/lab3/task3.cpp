#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <random>

using namespace std;
using namespace cv;

double reprojectionError(Point2f p1, Point2f p2) {
    return norm(p1 - p2);
}

int main() {
    vector<Point3f> objectPoints = {
        {0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
        {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}
    };

    Mat cameraMatrix = (Mat_<double>(3,3) <<
        800, 0, 320,
        0, 800, 240,
        0, 0, 1
    );

    Mat distCoeffs = Mat::zeros(4,1,CV_64F);

    Mat trueRvec = (Mat_<double>(3,1) << 0.3, 0.2, 0.1);
    Mat trueTvec = (Mat_<double>(3,1) << 0, 0, 5);

    vector<Point2f> imagePoints;
    projectPoints(objectPoints, trueRvec, trueTvec,
                  cameraMatrix, distCoeffs, imagePoints);

    // шум
    for (auto &p : imagePoints) {
        p.x += (rand() % 5 - 2) * 0.1;
        p.y += (rand() % 5 - 2) * 0.1;
    }

    // аутлаєри
    imagePoints[2] = Point2f(1000, 1000);
    imagePoints[5] = Point2f(-500, 200);

    int iterations = 100;
    double threshold = 5.0;

    Mat bestRvec, bestTvec;
    int bestInliersCount = 0;

    for (int i = 0; i < iterations; i++) {

        vector<Point3f> objSample;
        vector<Point2f> imgSample;

        set<int> indices;
        while (indices.size() < 4)
            indices.insert(rand() % objectPoints.size());

        for (int idx : indices) {
            objSample.push_back(objectPoints[idx]);
            imgSample.push_back(imagePoints[idx]);
        }

        Mat rvec, tvec;
        bool success = solvePnP(objSample, imgSample,
                                cameraMatrix, distCoeffs,
                                rvec, tvec, false, SOLVEPNP_P3P);

        if (!success) continue;

        vector<Point2f> projectedPoints;
        projectPoints(objectPoints, rvec, tvec,
                      cameraMatrix, distCoeffs,
                      projectedPoints);

        int inliersCount = 0;

        for (int j = 0; j < imagePoints.size(); j++) {
            double error = reprojectionError(imagePoints[j], projectedPoints[j]);

            if (error < threshold)
                inliersCount++;
        }

        if (inliersCount > bestInliersCount) {
            bestInliersCount = inliersCount;
            bestRvec = rvec.clone();
            bestTvec = tvec.clone();
        }
    }

    cout << "Best inliers: " << bestInliersCount << endl;
    cout << "Rotation:\n" << bestRvec << endl;
    cout << "Translation:\n" << bestTvec << endl;

    return 0;
}