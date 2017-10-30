int orderG(){
return 10;}

int sizeG(){
return 15;
}

int are_adjacent(int u, int v){
 if(0<= u && 0<=v && u<10 && v<10){
  int i,j,index=0;
  int T[10];
  for(i=0;i<5;i++)
   for(j=i+1;j<5;j++)
    {
     T[index]= (1 << i) + (1 << j);
     index++;
    }
  return (!(T[u] & T[v]));
 }	
 else return 0;
}


