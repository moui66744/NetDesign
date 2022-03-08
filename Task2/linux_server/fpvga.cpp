#include<verilator.h>
using namespace std;

/*
 * FPVGA:Field Programmable Virtual Gate Array
 */

TOP_NAME *top;
VerilatorContext *contextp;
#define GET_BIT(x,num) (num>>x)&1

static (void*) sw;
static (void*) led;

void fgvpa_get_data();
void fgvpa_init() {
	contextp = new VerilatorContext;
	top = new TOP_NAME{contextp };
}
int get_sw(int i){

}

void fgvpa_single(void p){

	for (int i = 0;i < SW_NUM; i ++){
		top->sw[i] = get_sw(i);
	}
	for (int i = 0;i < LED_NUM; i++){
		out[i] = top->led[i];
	}
}
