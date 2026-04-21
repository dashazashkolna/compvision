#include <ceres/ceres.h>
#include <vector>
using namespace std;

struct Data {
    double x, y;
};

struct CostFunctor {
    CostFunctor(double x, double y) : x(x), y(y) {}

    template <typename T>
    bool operator()(const T* const m,
                    T* residual) const {
        residual[0] = T(y) - (m[0] * T(x) + m[1]);
        return true;
    }

    double x, y;
};

int main()
{
    vector<Data> data = {
        {1,2}, {2,4}, {3,6},
        {5,20}, {6,-10}
    };

    vector<double> m = {0.0, 0.0};
    ceres::Problem problem;
    for (auto &d : data)
    {
        CostFunctor* functor = new CostFunctor(d.x, d.y);
        ceres::CostFunction* cost_function = new ceres::AutoDiffCostFunction<CostFunctor, 1, 2>(functor);
        problem.AddResidualBlock(cost_function, new ceres::HuberLoss(0.1), m.data());
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    cout << "Line: y = " << m[0] << "x + " << m[1] << endl;
}
