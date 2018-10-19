//#include<set>
class A{
public:
	int _a;
	~A(){Printf("A destructor\n");}
};
class B : public A{
public:
	int _b;
	~B(){Printf("B destructor\n");}
};

void TEST()
{
	gSystem->Load("StMyLibrary");
	B b;
	b._a = 0;
        std::set<int> ss;
        ss.insert(b._a);
}
