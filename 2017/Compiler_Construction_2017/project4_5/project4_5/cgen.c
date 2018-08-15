#include "globals.h"
#include "code.h"
#include "cgen.h"
#include "util.h"

#define v0 2
#define t0 8
#define ac 9
#define t2 10
#define t3 11
#define s5 20
#define s6 21
#define sp 29
#define fp 30
#define ra 31

scopeHash* maintain_global;
int label_cnt;

static void cGen (AST_node *, int);
static void genStmt (AST_node *, int);
static void make_input_output ();

static void ee_push(int save_reg_num)
{
	vn A,B,C;
	emitComment("<<<<push(memory)_temp>>>>");

	B.val = sp;
	A.val = sp;
	C. val = -4;


	if(spflag)
	emit_three("addi", A, B, C, 3, "sp -= 4");

	B.val = 0;
	A.val = save_reg_num;
	C.val = sp;

	 
	
	
	emit_three("sw", A, B, C, 2, "Push temp(save_reg_num) to sp");
	emitComment("<<<<push(memory)_temp>>>>");
}
static void ee_pop(int save_reg_num)
{
	vn A, B, C;
	if(spflag)
	emitComment("********pop(memory)_temp************");
	
	B.val =0 ;
	A.val = save_reg_num;
	if(spflag)
	C.val = sp;
	

	emit_three("lw", A, B, C, 2, "pop temp(save_reg_num) to sp");

	
	B.val = sp;
	C.val = 4;
	if(spflag)
	A.val = sp;
	emit_three("addi", A, B, C, 3, "sp += 4");
	emitComment("********pop(memory)_temp************");
}


// Procedure genStmt generates code at a statement node.
static void genStmt( AST_node *tree, int array_value) {    ///< 0 - array, 1 - value.

	AST_node *p1, *p2, *p3;
	int loc,v;
	declInfo *hash_loc;
	int saveLabel1, saveLabel2;
	vn A, B, C;
	scopeHash* sc_point1;

	switch (tree->kind) {
		case COMPSTMT:  ///< C0 local_declarations, C1: statement_list.
			
			Compoundemit();
			if(spflag)
			{
			
			p2 = tree->child[1];	
			p1 = tree->child[0];
		
			compaflag=1;
			}
			
			if(p2 && compaflag) cGen(p2, 1);
			if(p1 && compaflag) cGen(p1, 1);
			Compoundemit();
			break;

		case SELSTMT:
			
			Selemit();


			if(spflag){
			p2 = tree->child[1];
			p1 = tree->child[0];
			p3 = tree->child[2];
			}

			if(p3 == NULL)
			{
				cGen(p1, 1);
				A.val = ac;
				B.name = "label";
				saveLabel1 = C.val = ++label_cnt;
				emit_three("blez", A, B, C, 1, "if - false branch if_end");
				cGen(p2, 1);
				emit_one(saveLabel1);
			}	
		
			
			else if(p3 != NULL)
			{
				cGen(p1, 1);  ///< if -  t1(9)에 p1의 결과값이 저장 되어 있다.
				A.val = ac; B.name = "label"; saveLabel1 = C.val = ++label_cnt;
				emit_three("blez", A, B, C, 1, "if/else - false branch else code");
				
				cGen(p2, 1);  ///< then.
				
				A.name = "label"; saveLabel2 = B.val = ++label_cnt;
				emit_two("j", A, B, 1, "");
				emit_one(saveLabel1);   ///< else 시작.
				cGen(p3, 1);  ///< else.
				emit_one(saveLabel2);   ///< if문 끝.
			}
		

			Selendemit();
			break;

		case ITERSTMT: 
			Iteremit();

			p1 = tree->child[0];
			p2 = tree->child[1];
			saveLabel1 = ++label_cnt;
			emit_one(label_cnt);                                            ///< while 시작.
			cGen(p1, 1);
			A.val = ac; B.name = "label"; saveLabel2 = C.val = ++label_cnt; ///< while 끝 label.
			emit_three("blez", A, B, C, 1, "");                           
			cGen(p2, 1);
			A.name = "label"; B.val = saveLabel1;   ///< j - while로 돌아감
			emit_two("j", A, B, 1, "");
			emit_one(saveLabel2);                   ///< while 끝.
			
			Iterendemit();
			break; 

		case RETSTMT: 
			
			Retemit();


			p1 =  tree->child[0];

			if(p1!=NULL)cGen(p1, 1);

			A.val = sp; B.val = fp; C.val = -4;
			emit_three("addi", A, B, C, 3, "sp = fp - 4");

			ee_pop(ra);
			ee_pop(fp);

			A.name = "$ra";
			emit_two("jr", A, B, 3, "jr ra");
			

			Retemit();
			
			break;

		case CALL:


			Callemit();

			p1 = tree->child[0];
			v=0;
			while (p1)
			{
				p2=p1->sibling;
				p1->sibling = NULL;
				if ( p1->attr.name == NULL)//상수
				{
					cGen(p1, 1);//값
				}
				else
				{
					hash_loc = searchHash(tree->scope, 1, p1->attr.name);//변수
					if (hash_loc->aux.vpInfo.isArr)//배열
					{
						if (p1->child[0] != NULL)cGen(p1, 1);// [exp]  - 값
						else cGen(p1, 0);//a[] - 주소
					}
					else cGen(p1, 1);// 값
				}
				v+=4;
				p1->sibling = p2;
				ee_push(ac);
				p1 = p1->sibling;
			}
			ee_push(fp);
			A.val = fp; B.val = sp;
			emit_two("move", A, B, 4, "");

			A.name = tree->attr.name; B.val = 0;
			emit_two("jal", A, B, 3, "call function");// return시 $pc = $ra(호출과 동시 $pc+4를 $ra에 저장)

			if(spflag)
			A.val = sp; B.val =sp; C.val =v;
			if(!spflag)
			 B.val = sp; C.val = v; A.val = sp;
			emit_three("addi", A, B, C, 3, "sp = sp + para_mem_size");
			
			
			Callemit();
			break;

		
		case EXPR:      ///< assign.
		
		
			Expemit();

			p1 = tree->child[0]; p2 = tree->child[1];

			cGen(p2, 1);
			ee_push(ac);
			cGen(p1, 0);
			ee_pop(t2);

			//left가 ac right가 t2
			A.val = t2; B.val = 0; C.val = ac;
			emit_three("sw", A, B, C, 2, "assign: push temp");

			A.val = ac; B.val = t2;
			emit_two("move",A,B,4,"ac <- t2 because x =x +1");

			Expemit();
			break;







		case FACTOR:
			
			Factemit();
			
			A.val = ac; B.val = tree->attr.val;
			emit_two("li", A, B, 2, "load const");
			Factemit();
			break;

		case VAR:  
			p1 = tree->child[0];
			if (g_traceCode && spflag)emitComment("!!!Start!!!<Variable> !!!Start!!!");
			hash_loc = searchHash(tree->scope, 1, tree->attr.name);   //자신의 scope에서 검색	
			if (p1 != NULL) cGen(p1, 1);
			if (hash_loc->loc > 0 && hash_loc->vpf!=P)//global
			{
				if (g_traceCode && spflag)emitComments("**it is global**", tree->attr.name);
				A.val = t2; B.name = tree->attr.name;
				emit_two("la", A, B, 5, "load global value base address");
			}
			else
			{
				if (hash_loc->vpf == P && spflag ) loc = searchParentFuncScope(tree->scope)->max - hash_loc->loc + 4;
				else loc = hash_loc->loc;
	
				if (g_traceCode && loc>0 && spflag )emitComments("**it is parameter**", tree->attr.name);
				if (g_traceCode && loc<0 && spflag )emitComments("**it is local_var**", tree->attr.name);
				A.val = t2; B.val = fp; C.val = loc;
				emit_three("addi", A, B, C, 3, "load local/para base address");
			}
			//base t2에 저장되어 있음
			if (hash_loc->aux.vpInfo.isArr && hash_loc->vpf == P)//check - chain처럼 연계 되도 문제 x?
			{
				A.val = t2; B.val = 0; C.val = t2;
				emit_three("lw", A, B, C, 2, "a[]of real address take");//check
			}

			if (p1 != NULL)// 뒤에 [exp]
			{
				A.val = t3; B.val = 4;//t3 = 4
				emit_two("li", A, B, 2, "load 4 of need exp*4");// t3 = [exp]의 exp(ac) * 4

				A.val = t3;  B.val = ac; C.val = t3;
				emit_three("mul", A, B, C, 0, "t3 = exp(ac)*4(t3)");

				if (array_value == 0)///< 주소값 계산.
				{
					A.val = ac; C.val = t3; B.val = t2;
					emit_three("add", A, B, C, 4, "t2+t3 is address of arr[exp] ");
				}
				else///< 값 계산.
				{
					// $t = MEM[$s + offset]; lw $t, offset($s)
					A.val = ac; C.val = t3; B.val = t2;
					emit_three("add", A, B, C, 4, "ac=t2+t3 is address of arr[exp] ");
					A.val = ac; B.val = 0; C.val = ac;
					emit_three("lw", A, B, C, 2, "ac=[ac] is value of arr[exp]");
				}
			}
			else
			{

				if (array_value != 0)
				{
					A.val = ac;
					B.val = 0;
					C.val = t2;
					emit_three("lw", A, B, C, 2, "ac=[t2] is value of val");

				}
				else if (array_value == 0)   ///< 주소값 계산.
				{
					 B.val = t2;
					A.val = ac;//ac <- t2
					emit_two("move", A, B, 4, "ac <- t2(left is value and need address)");
				}
			
				
			}
			if (g_traceCode && spflag)emitComment("!!!End!!!  <Variable> !!!End!!!");
			break; 

		case RELOP:
		case ADDOP:
		case MULOP:
			if (g_traceCode && spflag)emitComment("!!!Start!!! <operation> !!!Start!!! ");

		
			p2 = tree->child[1];
			p1 = tree->child[0];
			/* 1 gen code for ac = left arg */
			cGen(p1, 1);
			/* 2 gen code to push left operand*/
			ee_push(ac);
			/* 3 gen code for ac = right operand */
			cGen(p2, 1);
			/* 4 now load left operand */
			ee_pop(t2);

			// left - t2, right - ac.
			switch (tree->attr.what)
			{
				case PLUS :
					if(spflag)
				        B.val = t2; C.val = ac; A.val = ac;
					
					emit_three("add",A,B,C,4,"op +");
					break;
				case MINUS :
					if(spflag)
					B.val = t2; C.val = ac; A.val = ac;
					emit_three("sub", A, B, C, 4, "op -");
					break;
				case MUL :
					if(spflag)
					B.val = t2; C.val = ac; A.val = ac;
					emit_three("mul", A, B, C, 0, "t2 = t2*ac");
					break;
				case DIV :
					if(spflag)
					B.val = t2; C.val = ac; A.val = ac;
					emit_three("div", A, B, C, 0, "t2 = t2/ac");
					break;

					//LT, LTE, GT, GTE, EQ, NEQ
				case LT:
					if(spflag)
					 B.val = t2; C.val = ac; A.val = ac;
					emit_three("slt",A,B,C,0,"slt(<) t2,ac");
					break;
				case LET:
					if(spflag)
					B.val = t2; C.val = ac; A.val = ac;
					emit_three("sle", A, B, C, 0, "sle(<=) t2,ac");
					break;
				case GT:
					if(spflag)
					B.val = t2; C.val = ac; A.val = ac;
					emit_three("sgt", A, B, C, 0, "sgt(>) t2,ac");
					break;
				case GET:
					if(spflag)
					A.val = ac; B.val = t2; C.val = ac;
					emit_three("sge", A, B, C, 0, "sge(>=) t2,ac");
					break;
				case EQ:
					if(spflag)
					A.val = ac; B.val = t2; C.val = ac;
					emit_three("seq", A, B, C, 0, "seq(==) t2,ac");
					break;
				case NEQ:

					if(spflag)
					A.val = ac; B.val = t2; C.val = ac;
					emit_three("sne", A, B, C, 0, "sne(!=) t2,ac");
					break;
			}
			if (g_traceCode)emitComment("!!!End!!!  <operation> !!!End!!!");
			break; /* OpK */
		case VARDECL:
			break;
		case FUNDECL:
			if( strcmp(tree->attr.name, "input") == 0 || strcmp(tree->attr.name, "output") == 0)
			{
				break;
			}
			if (g_traceCode && spflag)emitComment("!!!Start!!! <function>  !!!Start!!!");
			if(spflag)sc_point1 = searchScopeByFunc(maintain_global, tree->attr.name);
			
			if (strcmp(tree->attr.name, "main") == 0)emit_global(".globl main\n", 0);

			if(spflag)emit_function(tree->attr.name);

			if(strcmp(tree->attr.name,"main")==0)//check set
			{
				if(spflag)
				A.val = ac; B.val = 0;
				emit_two("li", A, B, 2, "load const");

				ee_push(fp);
				A.val = fp; B.val = sp;
				emit_two("move", A, B, 4, "fp = sp");
				ee_push(ra);
			}
			else { ee_push(ra); }// ra까지 쌓음 이제 local쌓아야함
			//sp = sp + local개수

			if(spflag)
			A.val = sp; B.val = sp; C.val = sc_point1->min;
			if (sc_point1->min != 0)C.val += 4;
			emit_three("addi", A, B, C, 3, "sp = sp + (minus)local mem size");

			p2 = tree->child[2];
			cGen(p2,1);
			
			if(spflag)
			A.val = sp; B.val = fp; C.val = -4;
			emit_three("addi", A, B, C, 3, "sp = fp - 4");

			ee_pop(ra);
			ee_pop(fp);

			A.name = "$ra";
			emit_two("jr", A, B, 3, "jr ra");
			if (g_traceCode)emitComment("!!!End!!!  <function> !!!End!!!");
			break;
		default:
			emitComment("BUG: Unknown operator");
			break;
	}
}

static void cGen(AST_node* tree, int array_value)
{ 
	for (; tree != NULL; tree = tree->sibling)
	{
		genStmt(tree,  array_value);
	}
}


void global_code(scopeHash * global)
{
	int i;
	declInfo *ret = NULL;
	emit_global(".data\n", 0);
	for (i = 0; i < HASH_SIZE; i++)
	{
		ret = global->decls[i];
		while (ret)
		{
			if (ret->vpf == V)
			{
				if (ret->aux.vpInfo.isArr)emit_global(ret->id, ret->aux.vpInfo.arrSize);
				else emit_global(ret->id, -1);
			}
			ret = ret->link;
		}
	}
	make_input_output();
}

void emit2()
{

	emit_global("input:\n", 0);
}

void emitsyscall()
{
	
	if(spflag)
	{
	emit_global("li $v0, 4\n", 0);
	emit_global("la $a0, inin\n", 0);
	emit_global("syscall\n", 0);
	emit_global("li $v0, 5\n", 0);
	emit_global("syscall\n", 0);
	}

}

void emit1()
{
	if(spflag)
	{	
	emit_global(" inin: .asciiz \"input: \"\n", 0);
	emit_global(" outout: .asciiz \"output: \"\n", 0);
	emit_global(" newline: .asciiz \"\\n\"\n", 0);
	emit_global(".text\n",0);
	}
}


void emit_after()
{

emit_global("li $v0, 4\n", 0);
emit_global("la $a0, outout\n", 0);
emit_global("syscall\n", 0);

emit_global("li $v0, 1\n", 0);
emit_global("lw $a0, 4($fp)\n", 0);
emit_global("syscall\n", 0);

emit_global("li $v0, 4\n", 0);
emit_global("la $a0, newline\n", 0);
emit_global("syscall\n", 0);




}


void make_input_output()//".data밑에 있다"
{
	vn A, B, C;

	
	if(spflag)
	emit1();
	
	

	//input---------------------------------------------------------------------
	emit2();

	//-----fp, ra하면되고----------------------------
	//fp는 call에서 했고
	ee_push(ra);
	//ra는 안바뀌니까 stack에 넣고 빼고 할 필요 없고
	//-----------------------------------------------
	emitsyscall();

	A.val = ac; B.val = v0;
	emit_two("move", A, B, 4, "move ac to v0");

	//-----fp, ra 다시 초기화----------------------------
	A.val = sp; B.val = fp; C.val = -4;
	emit_three("addi", A, B, C, 3, "sp = fp - 4");
	ee_pop(ra);
	ee_pop(fp);
	//----------------------------------------------------
	emit_global("jr $ra\n", 0);


	//output-------------------------------------------------------------------
	emit_global("output:\n", 0);

	//-----fp, ra하면되고----------------------------
	//fp는 call에서 했고
	ee_push(ra);
	//ra는 안바뀌니까 stack에 넣고 빼고 할 필요 없고
	//-----------------------------------------------


	emit_after();


	//-----fp, ra 다시 초기화----------------------------
	A.val = sp; B.val = fp; C.val = -4;
	emit_three("addi", A, B, C, 3, "sp = fp - 4");
	ee_pop(ra);
	ee_pop(fp);
	//----------------------------------------------------
	emit_global("jr $ra\n", 0);
}

void codeGen(AST_node * syntaxTree, char * codefile, scopeHash * global)
{ 
	int i;
	maintain_global = global;
	char * s = malloc(strlen(codefile) + 7);

	vn A, B;
	strcpy(s, "File: ");
	strcat(s, codefile);
	emitComment("C- Compilation to SPIM Code");
	emitComment(s);
	emitComment("Start of execution.");
	global_code(global);
	//generateInOut();
	cGen(syntaxTree, 1);
	emitComment("End of execution.");
}
