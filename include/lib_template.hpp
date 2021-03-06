#ifndef LIB_TEMPLATE_HPP
#define LIB_TEMPLATE_HPP

#include "build.hpp"

class LIB_TEMPLATE_API LibTemplate
{

public:
	
    LibTemplate();
    
    void setX(const int x);
    
    void setY(const int y);
    
    int getSum() const;
	
	
private:
    
    int m_x;
    int m_y;
	
};

#endif