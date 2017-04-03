
a=10;
c=2.67;

lorenz(dt,r) = (x,y,z)
letrec
{

'x = x + ((y*a)-(x*a))*dt;
'y = y + ((x*r)-y-(x*z))*dt;
'z = z + ((x*y)-(z*c))*dt;

};

process = _,_: lorenz;
