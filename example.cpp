#include "NelderMead.hpp"

#include <iostream>
#include <Eigen/Dense>

class Example : public NelderMead<Eigen::VectorXd, double> {
    double f(const Eigen::VectorXd& ys){
        double res = 0.0;
        double m = 1.0;
        for(int i=0; i<ys.size(); i++){
            res += m*std::abs(ys[i]);
            m*=10000.0;
        }
        return res;
    }
};


int main(){
    std::vector<Eigen::VectorXd> xs;
        
    xs.push_back(Eigen::Vector2d(1, 101));
    xs.push_back(Eigen::Vector2d(2, 100));
    xs.push_back(Eigen::Vector2d(3, 101));

    Example q;
    q.setSimplex(xs);
    for(int i=0; i<200; i++){
        q.iterate();
        std::cout<<q.current().first<<"\n";
    }
}
