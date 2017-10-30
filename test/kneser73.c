int orderG(){
    return 35;}

    int sizeG(){
        return 70;
    }

int are_adjacent(int u, int v){
    if(0<= u && 0<=v && u<35 && v<35){
        int i,j,k,index=0;
        int T[35];
        for(i=0;i<7;i++)
            for(j=i+1;j<7;j++)
                for(k=j+1;k<7;k++)
                {
                    T[index]= (1 << i) + (1 << j)+(1<<k);
                    index++;
                }
        return (!(T[u] & T[v]));
    }	
    else return 0;
}


