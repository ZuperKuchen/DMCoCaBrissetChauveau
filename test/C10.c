int orderG(){
return 10;}

int sizeG(){
return 10;}

int are_adjacent(int u, int v){
 if(0<= u && 0<=v && u<10 && v<10){
  return (((10+u-v)%10==1)||((10+v-u)%10==1));
 }	
 else return 0;
}

