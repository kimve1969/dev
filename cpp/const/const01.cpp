#include<iostream>

void foo(const int *a, int *b)
{
	return;
}

void bar(const int * const* a, int R, int C)
{
	//a[0][0] = 0; error assigned read-only (int *)*
	//a[1] = &a[0][0]; error ssigned read-onle *(a + 8) 

	// A[R][C]
	for(int i=0; i<R; ++i)
	{
		for(int j=0; j<C; ++j)
		{
			std::cout<<"a["<<i<<"]["<<j<<"] = "<<a[i][j]<< ((j == C-1) ? "\n" : ", ");
		}
	}
	std::cout<<"\n";
	return;
}

int main(int argc, char* argv[])
{
	std::cout<<"Start\n";

	const int cnst_a{1};
	int b{2};
	// cnst_a = 3; compiline error - assigned read-only variable

	foo( &cnst_a, &b ); // Ok
	foo( &b, const_cast<int*>(&cnst_a) /* I know what I do! */ );

	int A1D[10];
	for(int i=0; i<10; ++i)
	{
		A1D[i] = i;
	}
	int *A2D[2]{ & A1D[0], & A1D[5] }; // A2D[i][j], i = 0-1, j=0-4
	
	bar( const_cast<const int* const *>(A2D), 2, 5);

	std::cout<<"End\n";
	return 0;
}
