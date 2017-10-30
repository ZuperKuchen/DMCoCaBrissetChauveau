int orderG(){
return 20;}

int sizeG(){
return 10;}

int are_adjacent(int u, int v){
if(0<= u && 0<=v && u<orderG() && v<orderG()){
  return (u+v==19);
 }	
 else return 0;
}

