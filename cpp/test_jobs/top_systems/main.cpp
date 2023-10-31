#include<iostream>
#include<vector>
#include<initializer_list>
#include<cassert>
#include<limits>
#include<cmath>
#include<memory>
#include<string>
#include<stdexcept>
#include<sstream>

/**
 * /brief user-defined function compare 2 float points 
 */
template<typename T>
bool is_equal(T a, T b)
{
	return std::fabs(a - b) < std::numeric_limits<T>::epsilon();
}
/**
 * /breif Abstract class figure
 */
template<typename T>
class figure
{
	protected:
		std::vector<T> m_coords;
	public:
		figure(std::initializer_list<T> l) : m_coords(l){}
		figure(const figure& f) : m_coords(f.m_coords){}
		figure(figure&& f) : m_coords( std::move(f.m_coords) ){}

		figure() = delete;
		figure& operator=(const figure&) = delete;
		figure& operator=(figure&&) = delete;

		virtual void paint() const = 0;
		virtual ~figure(){}
};

/**
 *  /brief parse coords to string '{x1, y1, x2, y2 ...}'
 *
 */
template<typename Container>
std::string coords2string(const Container& c)
{
	if(c.size() == 0)
	{
		return std::string("{empty}");
	}

	std::stringstream s;
	s<<"{";
	for(auto it = c.begin(); it < c.end(); ++it)
	{
		s<<(*it)<<( (it+1) == c.end() ? "}" : "," );
	}

	return s.str();
}

/**
* /brief triangle
*/
template<typename T>
class triangle : public figure<T>
{
	public:
		triangle(std::initializer_list<T> l) : figure<T>(l){ check_triangle(); }
		triangle() = delete; ///> disable cnt empty triangle
		triangle( const triangle& t ) : figure<T>( t ){ check_triangle(); }
		triangle( triangle&& t) : figure<T>( std::move(t) ){ check_triangle(); }
		triangle& operator=(const triangle& t)
		{ 
			(*this).m_coords = t.m_coords;
			check_triangle();
			return *(this);
		}

		triangle& operator=(triangle&& t)
		{ 
			(*this).m_coords = std::move(t.m_coords);
			check_triangle();
			return *(this);
		}

		virtual void paint() const override
		{
			std::cout<<"triangle.paint("<< coords2string( (*this).m_coords)<<")\n";
		}
		virtual ~triangle(){}

	private:
		void check_triangle()
		{
			if( (*this).m_coords.size() != 6 || ///> triangle has 3 2D-point

					is_equal<T>( (*this).m_coords[0], (*this).m_coords[2]) || ///> check x1,x2,x3
					is_equal<T>( (*this).m_coords[0], (*this).m_coords[4]) ||
					is_equal<T>( (*this).m_coords[2], (*this).m_coords[4]) ||

					is_equal<T>( (*this).m_coords[1], (*this).m_coords[3]) || ///> check y1,y2,y3
					is_equal<T>( (*this).m_coords[1], (*this).m_coords[5]) ||
					is_equal<T>( (*this).m_coords[3], (*this).m_coords[5]) ) 
			{
				throw( std::out_of_range( std::string("check coords triangle: ") + coords2string( (*this).m_coords ) )); 
			}

		}
};

/**
 * /brief rectangle
 */
template<typename T>
class rectangle : public figure<T>
{
	public:
		rectangle(std::initializer_list<T> l) : figure<T>(l){ check_rectangle(); }
		rectangle() = delete; ///> disable cnt empty rectangle
		rectangle(const rectangle& r ) : figure<T>( r ){ check_rectangle(); }
		rectangle( rectangle&& r) : figure<T>( std::move(r) ){ check_rectangle(); }
		rectangle& operator=(const rectangle& r)
		{ 
			(*this).m_coords = r.m_coords;
			check_rectangle();
			return *(this);
		}

		rectangle& operator=(rectangle&& r)
		{ 
			(*this).m_coords = std::move(r.m_coords);
			check_rectangle();
			return *(this);
		}

		virtual void paint() const override
		{
			std::cout<<"rectangle.paint("<< coords2string( (*this).m_coords )<<")\n";
		}
		virtual ~rectangle(){}
	private:
		void check_rectangle(){
			if( (*this).m_coords.size() != 4 ||  ///> rectangle should has 2 2D-point: lower-left corner & upper-right corner

					is_equal<T>( (*this).m_coords[0], (*this).m_coords[2] ) || ///> check x1,x2
					std::isgreater( (*this).m_coords[0], (*this).m_coords[2]) ||

					is_equal<T>( (*this).m_coords[1], (*this).m_coords[3] ) || ///> check y1,y2
					std::isgreater( (*this).m_coords[1], (*this).m_coords[3]) ) 
			{
				throw( std::out_of_range( std::string("check coords rectangle: ") + coords2string( (*this).m_coords ) )); 
			}
		}
};

template<typename T>
std::shared_ptr< figure<T> > make_shared_figure(std::string stype, std::initializer_list<T> l)
{
	if( "triangle" == stype)
	{
		return std::make_shared< triangle<T> >(l);
	}
	else if( "rectangle" == stype)
	{
		return std::make_shared< rectangle<T> >(l);
	}
	else
	{
		throw( std::out_of_range( std::string("Unknown type figures is ") + stype ) );
	}

	return nullptr;
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";
	
	///> check point epsilon
	assert( is_equal(1.0, 1.0 + 0.000'000'000'000'000'1) );	
	
	using shared_ptr_figure = std::shared_ptr< figure<double> >;
	std::vector< shared_ptr_figure > vec;

	///> create two figures
	vec.push_back( make_shared_figure<double>( "triangle", {1,2, 3,4, 5,6}) );
	vec.push_back( make_shared_figure<double>( "rectangle", {3,4, 7,8}) );
	//vec.push_back( make_shared_figure<double>( "unknown", {}) );

	///> try create third figure - bad rectangle: x1 > x2
	try
	{
		vec.push_back( make_shared_figure<double>( "rectangle", {9,4,7,8} ) );
	}	
	catch(const std::exception& e)
	{
		std::cout<<"Bad figure: "<<e.what()<<"\n";
	}

	///> check point size collection
	assert( vec.size() == 2 );

	///> polimorfizm - each figure self paint()
	std::cout<<"I have collection from "<<vec.size()<<" figures are:\n";
	for(const auto& ptr: vec )
	{
		(*ptr).paint();
	}	
	
	triangle<double> tr1({0,0, 1,1, 2,2});
	std::cout<<"tr1:";
	tr1.paint();
	triangle tr2(tr1);
	std::cout<<"tr2:";
	tr2.paint();
	tr2 = {3,3, 4,4, 5,5};
	std::cout<<"update tr2, tr2:";
	tr2.paint();
	triangle tr3( std::move(tr2) );
	std::cout<<"move(tr2), tr2:";
	tr2.paint();
	std::cout<<"tr3:";
	tr3.paint();
	/*triangle tr4(tr2); ///> here abort programm, tr2 is empty!
	std::cout<<"tr4:";
	tr4.paint();*/

	std::cout<<"End\n";

	return 0;
}
