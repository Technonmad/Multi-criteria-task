#include "pareto.h"

Pareto::Pareto()
{

}

void Pareto::compare(std::vector<int> alternative1, std::vector<int> alternative2)
{
    for ( std::size_t i = 0; i < alternative1.size(); ++i )
    {
        if ( alternative1[i] > alternative2[i] )
            more++;
        else if ( alternative1[i] < alternative2[i] )
            less++;
        else
            equal++;
    }
}

bool Pareto::moreOrEqual()
{
    if (more >= 0 && less == 0)
        return true;
    else
        return false;
}

void Pareto::deleteDominated(std::vector<std::vector<int>> y)
{
    for (std::vector<std::vector<int>>::iterator yi = y.begin(); yi != y.end(); ++yi)
    {
        for (std::vector<std::vector<int>>::iterator gj = y.begin(); gj != y.end(); ++gj)
        {
            if ( !(*gj == *yi) )
            {
                compare( *gj, *yi );

                if ( moreOrEqual() )
                {
                    y.erase(yi);
                    deleteDominated(y);
                    return;
                }

                compare(*yi, *gj);

                if ( moreOrEqual() )
                {
                    y.erase(gj);
                    deleteDominated(y);
                    return;
                }
            }
        }
    }
}

std::vector<std::vector<int>> Pareto::getParetoList(std::vector<std::vector<int>> y)
{
    deleteDominated(y);
    return y;
}
