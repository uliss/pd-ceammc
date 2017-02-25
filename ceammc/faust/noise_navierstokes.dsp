

navier(dt1,v) = (x1,x2,x3,x4,x5)
letrec
{
'x1= x1 + dt1 * (-2*x1 + 4*x2*x3 + 4*x4*x5) ;
'x2= x2 + dt1 * (-9*x2 + 3*x1*x3) ;
'x3= x3 + dt1 * (-5*x3 - 7*x1*x2 + v) ;
'x4= x4 + dt1 * (-5*x4 - x1*x5) ;
'x5= x5 + dt1 * (-x5 - 3*x1*x4) ;
	
};

process = _,_: navier;