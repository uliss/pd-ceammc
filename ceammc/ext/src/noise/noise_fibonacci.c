/*
ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
Fibonacci - © Andr[e Sier 2002

revision 09.14.2002
	f0,f1 -- 1
	tst ()


output fibonacci series of numbers

modos:	raw (calcular a iteracao da sequencia fibo n)
		lookup table (calcular n valores da sequencia e armazenar numa tabela)		
ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ
*/

#include "ext.h"

#define MAXSIZE 256
//#define MOD 10000000


typedef struct _something_shines_every_day_twelve_oh_five {
	t_object 			x_ob;
	long 				x_a;				// nada == swim / double.v
	long				x_iter[MAXSIZE];	//a iteracao a calcular
	Boolean			x_fire;			//bang on calc?
	Boolean			x_rep;			
	void 			       *x_outlet;
	long				x_fill;										
} Fibo;


void *Fibo_new 		(long arg );
void  Fibo_assist		(Fibo *x, void *b, long m, long a, char *s);			
void  Fibo_bang		(Fibo *x);												
void  Fibo_int		(Fibo *x, long n);											
void  Fibo_fire		(Fibo *x, long n);											
void  Fibo_rep		(Fibo *x, long n);											
void  Fibo_free		(Fibo *x);											
void  Fibo_tavle		(Fibo *x, long n);											
void  Fibo_filltavle	(Fibo *x, long n);											
void  Fibo_trust		(Fibo *x, long n);											
int	Fibonacci		(int n);
void *Fibo_class		; 					

	// 0 @ 47
long fivo []=   { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,
		     89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765,
		     10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040,
		     1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169, 63245986, 102334155,
		     165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, -13233752223}; 


void  Fibo_free(Fibo *x)             {notify_free((t_object *)x);}												
void  Fibo_fire(Fibo *x, long n) { x->x_fire = n;}												
void  Fibo_rep(Fibo *x, long n)  { x->x_rep = n;}												

void  Fibo_int(Fibo *x, long n) //output n'th iteration , the further the more expensive
{
	x->x_a = Fibonacci(n);	
	if (x->x_fire) Fibo_bang(x);
}

void  Fibo_bang (Fibo *x)
{	
	outlet_int (x->x_outlet,   x->x_a);	
}

void  Fibo_tavle	(Fibo *x, long n)
{
	x->x_a = x->x_iter[n];
	if (x->x_fire) Fibo_bang(x);
}

void  Fibo_filltavle	(Fibo *x, long n)
{
	register short int i;
	for (i=x->x_fill; i<(n+x->x_fill); i++) 
		x->x_iter[i] = Fibonacci(i);
	x->x_fill += n;
	if(x->x_rep)post("fill is %ld",x->x_fill,0);
}

void  Fibo_trust	(Fibo *x, long n)
{
	x->x_a = fivo[n];
	if(x->x_rep)post("&fill is %07x",&fivo[n],0);
	if (x->x_fire) Fibo_bang(x);
}


int Fibonacci (int n)
{
	if (n<2) {
		if (n==0||n==1) return 1; //f0,f1 =1
		else return (n);// no mirror
		}
	else
		return (Fibonacci(n-1)+Fibonacci(n-2));
}

void Fibo_assist(Fibo *x, void *b, long m, long a, char *s)		
{
 if ((m==1)&&(a==0)) 		sprintf(s,"(int), tavle, bang");
 else if ((m==2)&&(a==0)) 	sprintf(s,"(int)fibo out");
}

void *Fibo_new ( long arg)
{
	register short i;

	Fibo *x;
	x = (Fibo *)newobject(Fibo_class);

	x->x_a = 0;
	x->x_fill = 0;
	
	for (i=0; i<MAXSIZE; i++) // tavula redonda	
		x->x_iter[i] = 0;

	if (arg) Fibo_filltavle(x,arg);	
	x->x_outlet = intout(x);	
	x->x_fire = 1;
	x->x_rep = 0;
	return (x);
} 

void main(void) /* ANSI C requires main to return int. */
{
	long int tick = gettime();
	setup((t_messlist **)&Fibo_class, (method)Fibo_new,\
	(method)Fibo_free, (short)sizeof(Fibo), 0L,A_DEFLONG,  0); 
	addbang((method)Fibo_bang);
	addint((method)Fibo_int);
	addmess((method)Fibo_fire,"fire", A_LONG, 0);
	addmess((method)Fibo_tavle,"tavle", A_LONG, 0);
	addmess((method)Fibo_trust,"tst", A_LONG, 0);
	addmess((method)Fibo_filltavle,"fill", A_LONG, 0);
	addmess((method)Fibo_rep,"rep", A_LONG, 0);
	addmess((method)Fibo_assist,"assist",A_CANT,0);
	post("A-Chaos Lib :: a-fibonacci  " __DATE__" "__TIME__"                                   ©   a n d r Ž s i e r   2 0 0 4   all rights reserved",tick, 0);
}