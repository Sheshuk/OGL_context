
class taylor {
private:
	/* data */

public:
	taylor(unsigned n);
	///--------------------------------------------------------
	taylor(const taylor &t);
    ~taylor();
    void upd(double dt);
    double &operator[](unsigned n);
    void print();
    void stop();
    bool is_moving();//{for(int n=1;n<N;++n)if(c[n]!=0)return true; return false;}
    //dynamic cast
    operator double (){return c[0];}
    operator float () {return c[0];}
public:
    double* c = 0;
    unsigned N = 0;
};