//#include<set>
class A{
public:
	int _a;
        A() : _a(0) { Printf("A constructor\n"); }
	~A(){Printf("A destructor\n");}
};
class AA{
public:
	int _a;
        AA() : _a(0) { Printf("AA constructor\n"); }
	~AA(){Printf("AA destructor\n");}
};
class B : public A{
public:
	int _b;
        B() : _b(0) { Printf("B constructor\n"); }
	~B(){Printf("B destructor\n");}
};
class C : public A, public AA{
public:
	int _c;
        C() : _c(0) { Printf("C constructor\n"); }
	~A(){Printf("C destructor\n");}
};

void TEST()
{
	gSystem->Load("StMyLibrary");
	B b;
	b._a = 0;
        //std::set<int> ss;
        //ss.insert(b._a);
        C c;
        Printf("A: %d AA: %d\n", c.A::_a, c.AA::_a);
}
