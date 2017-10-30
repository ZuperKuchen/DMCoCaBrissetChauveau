int orderG(){
return 15;}

int sizeG(){
return 45;
}

int are_adjacent(int u, int v){
 if(0<= u && 0<=v && u<15 && v<15){
  int i,j,index=0;
  int T[15];
  for(i=0;i<6;i++)
   for(j=i+1;j<6;j++)
    {
     T[index]= (1 << i) + (1 << j);
     index++;
    }
  return (!(T[u] & T[v]));
 }	
 else return 0;
}


