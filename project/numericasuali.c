#include <Libreria.h>

int main(){
    
    int tot = SO_ROUND_SCORE;
    int max_rand;                                           
    int flag = (rand() % (SO_FLAG_MAX - SO_FLAG_MIN + 1)) + SO_FLAG_MIN;
    int band;
    int i;
    int tmp= flag;

    /*printf("%d\n",flag);*/
    srand(time(NULL));
    for(i=0;i<flag;i++){
    	max_rand = tot-(tmp-1);
    	if(tmp != 1)
        band = (rand() % (max_rand))+1;
    	else band = tot;
        tot= tot - band;
    	printf("TOT:%d -- FLAG:%d -- BAND: %d\n",tot,tmp,band);
    	tmp--;
    }

}