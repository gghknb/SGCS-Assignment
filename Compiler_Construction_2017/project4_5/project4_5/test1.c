int data;
int a;

void func(int a){
	
	if (a == 0) return;
	func(a / data);
	output(a-a/data*data);

}
void main(void){
	
	data = input();
	func(input());
	
}
