#ifndef PARETO_H
#define PARETO_H


#include<vector>

class Pareto
{
private:
    int more = 0;
    int less = 0;
    int equal = 0;

public:
    Pareto();

private:
    void compare(std::vector<int> alternative1, std::vector<int> alternative2);

    bool moreOrEqual();

    void deleteDominated(std::vector<std::vector<int>> y);

public:
    std::vector<std::vector<int>> getParetoList(std::vector<std::vector<int>> y);
};

#endif // PARETO_H
