namespace solver
{
    double derive(double (*F)(double), double val)
    {
        double h = 0.00001;
        return (F(val + h) - F(val)) / h;
    }

    double integrate(double (*F)(double), double a, double b)
    {
        double h = 0.00001;
        double sum = 0;
        for(double i = a; i <= b; i += h)
        {
            sum += (F(i) * h);
        }
        return sum;
    }
};
