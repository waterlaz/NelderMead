#include <set>
#include <vector>
#include <assert.h>

template<typename D, typename R>
class NelderMead {
private:
    class CompareFirst {
    public:
        int operator()(const std::pair<R, D>& a, 
                       const std::pair<R, D>& b){
            return a.first < b.first;
        }
    };
    std::multiset<std::pair<R, D>, CompareFirst > xs;
public:
    virtual R f(const D& x) = 0;
    R alpha = 1.0;
    R gamma = 2.0;
    R ro = 0.5;
    R delta = 0.5;
    int n;
    const std::pair<R, D>& current(){
        assert( !xs.empty() );
        return *xs.begin();
    }
    void setSimplex(const std::vector<D>& _xs){
        n =_xs.size()-1;
        assert(n>=1);
        for(auto && x:_xs){
            xs.insert( std::pair<R, D>(f(x), x) );
        }
    }
    void addToSimplex(const D& x){
        xs.insert(std::pair<R, D>( f(x), x) );
        n = xs.size();
    }
    void iterate(){
        assert( !xs.empty() );
        auto worst = xs.end();
        worst--;
        //compute centroid:
        auto it = xs.begin();
        D c = it->second;
        it++;
        while( it!=worst ){
            c += it->second;
            it++;
        }
        c /= n;
        auto secondWorst = worst;
        secondWorst--;
        auto best = xs.begin();
        //reflection:
        D r = c + alpha*( c - worst->second );
        R rv = f(r);
        if( rv<secondWorst->first && rv>=best->first ){
            xs.erase( worst );
            xs.insert( std::pair<R, D>(rv, r) );
            return;
        }
        //expansion:
        if( rv<best->first ){
            D e = c + gamma*(r-c);
            R ev = f(e);
            xs.erase(worst);
            if(ev<rv){
                xs.insert( std::pair<R, D>(ev, e) );
            } else {
                xs.insert( std::pair<R, D>(rv, r) );
            }
            return;
        }
        //contraction:
        D e = c + ro*(worst->second - c);
        R ev = f(e);
        if( ev<worst->first ){
            xs.erase(worst);
            xs.insert( std::pair<R, D>(ev, e) );
            return;
        }
        //shrink:
        it = xs.begin();
        std::multiset<std::pair<R, D>, CompareFirst > ys;
        ys.insert(*it);
        it++;
        while(it!=xs.end()){
            D x = it->second;
            x = best->second + delta*(x - best->second);
            ys.insert(std::pair<R, D>(f(x), x));
            it++;
        }
    }
};
