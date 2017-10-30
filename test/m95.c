int orderG(){
    return 95;}

    int sizeG(){
        return  755;
    }

int adj(int u, int v, int n){
    int x,y;
    if (u==v)
        return 0;
    if(u>v){
        x=u;
        y=v;
    }
    else{
        x=v;
        y=u;
    }
    if(x==1)
        return 1;
    if(x<n/2)
        return adj(x,y,n/2);
    if(x==n-1){
        if(y>=n/2) 
            return 1;
        else return 0;
    }
    if(x>=n/2 && y<n/2)
        return adj(x-n/2,y,n/2);
    return 0;
}




int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG())
        return adj(u,v,orderG());
    return 0;
}





