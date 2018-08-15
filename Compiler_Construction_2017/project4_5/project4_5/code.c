#include "code.h"
extern FILE* code;

int spflag = 1;
int compaflag =1;


void emit_one(int labelNum)
{

fprintf(code, "label%d:\n", labelNum);
}


void spset()
{
	spflag=1;
}


char* change(int x)
{

	switch(x)
	{

		case 2:
			if(spflag)return "v0";
		
			else spset(); return "0";
		case 8:
			if(spflag)return "t0";

			else spset(); return "1";
		case 10: 
			if(spflag)return "t2";
			else spset(); return "2";

		case 9: 
			if(spflag)return "t1";
			else spset(); return "3";

		case 21:
			if(spflag)return "s6";
			else spset(); return "4";
		case 29:
			if(spflag)return "sp";
			else spset(); return "5";

		case 20:
			if(spflag)return "s5";
			else spset(); return "6";

		case 11:
			if(spflag)return "t3";
			else spset(); return "7";

		case 30:
			if(spflag)return "fp";
			else spset(); return "8";


		case 31:
			if(spflag)return "ra";
			else spset(); return "9";

		default:
			break;

	}


	return 0;
}
void emit_two(char *oper, vn a, vn b, int ty, char *ment)
{

	if(ty ==1)
	{
	if(spflag ==1)
			fprintf(code, "\t%5s %s%d	", oper,a.name,b.val);

	else if(spflag ==0)
		fprintf(code, "\t%5s %s%d	",oper, b.name, a.val);

	}
	if(ty ==2)
	{
		if(spflag ==1)
			fprintf(code, "\t%5s $%s,%d"	,oper, change(a.val),b.val);

		else if(spflag ==0)
			fprintf(code, "\t%5s $%s,%d"	,oper, change(b.val),a.val);
	}

	if(ty==3)
	fprintf(code, "\t%5s %s" , oper, a.name);
	
	if(ty==4) 
	fprintf(code, "\t%5s $%s, $%s",	oper, change(a.val), change(b.val));

	if(ty==5)
	fprintf(code, "\t%5s $%s,%s"	,oper,change(a.val), b.name);

	if(g_traceCode) fprintf(code, "\t\t\t\t#%s", ment);
	fprintf(code,"\n");
}



void Compoundemit()
{

	if(compaflag ==0)
	{
		if(g_traceCode && spflag)
			emitComment("!!!Start!!! <compound> !!!Start!!! ");
	}

	if(compaflag==1)
	{
	if(g_traceCode && spflag) emitComment("!!!End!!!  <compound> !!!End!!! ");
	compaflag=0;
	}

}

void Selemit()
{
	if(g_traceCode && spflag) emitComment("!!!Start!!! <if> !!!Start!!!");
}

void Selendemit()
{
	if(g_traceCode && spflag) emitComment("!!!End!!! <if> !!!End!!!");


}


void Iteremit()
{

	if(g_traceCode && spflag) emitComment("!!!Start!!! <while> !!!Start!!!");
}

void Iterendemit()
{
	if(g_traceCode && spflag) emitComment("!!!End!!! <while> !!!End!!!");

}

void Retemit()
{

	if(returnflag==0)
	{

 	if(g_traceCode && spflag)emitComment("!!!Start!!! <return> !!!Start!!!");
	
	returnflag =1;
	}

	if(returnflag==1)
	{
	if(g_traceCode && spflag)emitComment("!!!End!!!  <return>  !!!End!!!");
	
	returnflag = 0;
	}

}



void Callemit()
{



	if(callflag == 0)
	{
	if(g_traceCode && spflag) emitComment("!!!Start!!! <expression> !!!Start!!!");
	callflag =1;

	}


	if(callflag == 1)
	{
	if(g_traceCode && spflag) emitComment("!!!End!!!   <expression>  !!!End!!!");
	
	callflag =0;
	}
}

void Expemit()
{

	if(expflag==0)
	{
	if(g_traceCode)emitComment("!!!Start!!! <expression> !!!Start!!!");
	
	expflag = 1;
	}
	
	if(expflag ==1)
	{
	if(g_traceCode && spflag) emitComment("!!!End!!!   <expression> !!!End!!!");
	
	expflag =0;
	}
}

void Factemit()
{
	
	if(factflag==0)
	{
	if(g_traceCode)emitComment("!!!Start!!! <const> !!!Start!!!");
	factflag = 1;

	}
	
	if(factflag==1){
	if(g_traceCode)emitComment("!!!End!!! <const> !!!End!!!");
	
	factflag =0;
	}
}






void emitComment( char * c )
{ if (g_traceCode) fprintf(code,"\t\t\t\t#%s\n",c);}




void funcemit()
{
	if(g_traceCode)emitComment("!!!End!!! <function> !!!End!!!");

}

void emitComments(char * c, char *s)
{
	if (g_traceCode) fprintf(code, "\t\t\t\t#%s, name : %s\n", c,s);
}


void emit_global(char* str, int type)
{
	if (type == 0)fprintf(code,"%s",str);
	else if (type == -1)fprintf(code,"%s: .word 0\n",str);
	else fprintf(code, "%s: .word 0:%d\n", str,type);
}


void emit_function(char* name)
{
	fprintf(code, "%s:\n", name);
}



void emit_three(char *op, vn x, vn y, vn z, int type, char *c)
{
	if (type == 0)fprintf(code, "\t%5s  $%s,$%s,$%s", op,  change(x.val), change(y.val), change(z.val));
	if (type == 1)fprintf(code, "\t%5s  $%s,%s%d"	, op, change(x.val), y.name, z.val);
	if (type == 2)fprintf(code, "\t%5s  $%s,%d($%s)", op, change(x.val), y.val, change(z.val));//sw, lw
	if (type == 3)fprintf(code, "\t%5s  $%s,$%s,%d" , op, change(x.val), change(y.val), z.val);//addi $t2, $fp, imm
	if (type == 4)fprintf(code, "\t%5s  $%s,$%s,$%s", op, change(x.val), change(y.val), change(z.val));//add $ac, $t2, $ac 

	if (type == 5)fprintf(code, "\t%5s  $%s,$%s,%d", op, change(x.val), y.name, z.val);

	if (g_traceCode) fprintf(code, "\t\t\t\t#%s", c);
	fprintf(code, "\n");
}


