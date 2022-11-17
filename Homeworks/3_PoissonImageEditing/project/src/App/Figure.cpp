#include"Figure.h"

Figure::~Figure() {

}

void Figure::set_start_point_(QPoint a)
{
	start_point_ = a;
}

void Figure::set_end_point_(QPoint b)
{
	end_point_ = b;
}