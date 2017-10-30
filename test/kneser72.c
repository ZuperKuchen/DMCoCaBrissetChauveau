int orderG(){
    return 21;}

    int sizeG(){
        return 105;
    }

int are_adjacent(int u, int v){
    if(0<= u && 0<=v && u<21 && v<21){
        int i,j,index=0;
        int T[21];
        for(i=0;i<7;i++)
            for(j=i+1;j<7;j++)
                {
                    T[index]= (1 << i) + (1 << j);
                    index++;
                }
        return (!(T[u] & T[v]));
    }	
    else return 0;
}


