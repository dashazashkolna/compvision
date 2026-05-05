#define GLOG_USE_GLOG_EXPORT
#include <ceres/ceres.h>

// f(x, y, z) = (x - 2)^2 + (y + 1)^2 + (z - 4)^2
// Мінімум в точці (2, -1, 4)
struct ThreeVariablesSumOfSquares
{
    template <typename T>
    bool operator()(const T* const vars, T* residuals) const
    {
        residuals[0] = vars[0] - T(2.0);   // (x - 2)
        residuals[1] = vars[1] + T(1.0);   // (y + 1)  
        residuals[2] = vars[2] - T(4.0);   // (z - 4)
        
        return true;
    }
};

void testThreeVariablesMinimization()
{
    double x = 0.0, y = 0.0, z = 0.0;
    std::vector<double> vars = {x, y, z};
    
    ceres::Problem problem;

    ThreeVariablesSumOfSquares* functor = new ThreeVariablesSumOfSquares();
    ceres::CostFunction* cost_function = new ceres::AutoDiffCostFunction<ThreeVariablesSumOfSquares, 3, 3>(functor);
    problem.AddResidualBlock(cost_function, nullptr, vars.data());

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    options.max_num_iterations = 100;
    
    ceres::Solver::Summary summary;
    Solve(options, &problem, &summary);

    std::cout << summary.BriefReport() << "\n";
    std::cout << "minimum of f(x,y,z) = (x-2)^2 + (y+1)^2 + (z-4)^2:\n";
    std::cout << "x = " << vars[0] << ", y = " << vars[1] << ", z = " << vars[2] << "\n";

    double f_value = std::pow(vars[0] - 2.0, 2) + std::pow(vars[1] + 1.0, 2) + std::pow(vars[2] - 4.0, 2);
    std::cout << "f(x,y,z): " << f_value << "\n";
}

int main()
{
    testThreeVariablesMinimization();
    
    return 0;
}