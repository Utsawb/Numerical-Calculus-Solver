#include <iostream>
#include <string>
#include <fstream>
#include <dlfcn.h>
#include "solver.hpp"

constexpr double pi = 3.141592653589793238463;
constexpr double e = 0.5772156649015328606065120900824024310421;
typedef double (*func_type) (double);

inline void write_file(std::string func_text)
{
    std::ofstream f("_tmp/func.cpp");
    f << "#include <cmath>\ndouble pi = M_PI;\ndouble e = M_E;\nextern \"C\" double func (double x)\n{\n    return ";
    f << func_text;
    f << ";\n}";
    f.close();
}

inline void assemble_and_link()
{
    system("g++ -fPIC -c -o _tmp/func.o _tmp/func.cpp");
    system("g++ -shared -o _tmp/libfunc.so _tmp/func.o");
}

inline void* load_function(func_type& func)
{
    void* func_lib = dlopen("_tmp/libfunc.so", RTLD_NOW);
    void* tmp = dlsym(func_lib, "func");
    func = (func_type) tmp;
    return func_lib;
}

inline double do_input(std::string num)
{
    if (num == "pi")
    {
        return pi;
    }
    else if (num == "e")
    {
        return e;
    }
    else if (num == "-pi")
    {
        return -1.0 * pi;
    }

    return std::stod(num);
}

int main (void)
{
    system("rm -rf _tmp");
    system("mkdir _tmp");
    std::string buffer;
    void* dl_handle;
    func_type func;

    std::cout << "What function would you like to derive|integrate? Please use C++ syntax, assume <cmath> is included" << std::endl;
    std::cout << "f(x) = ";
    std::getline(std::cin, buffer);
    write_file(buffer);
    assemble_and_link();
    dl_handle = load_function(func);

    std::cout << "Would you like to derive(1) or integrate(2)?: ";
    std::getline(std::cin, buffer);

    switch (std::stoi(buffer))
    {
        case 1:
            std::cout << "x = ";
            std::getline(std::cin, buffer);
            std::cout << "f'(x) = " << solver::derive(func, do_input(buffer)) << std::endl;
            break;
        case 2:
            std::cout << "a = ";
            std::getline(std::cin, buffer);
            double a = do_input(buffer);
            std::cout << "b = ";
            std::getline(std::cin, buffer);
            std::cout << "∫f(x)dx = " << solver::integrate(func, a, do_input(buffer)) << std::endl;
            break;
    }

    dlclose(dl_handle);
    system("rm -rf _tmp");
    return 0;
}
