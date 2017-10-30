int orderG(){
    return 36;
}

int sizeG(){
    return  594;}

int are_adjacent(int u, int v){
    if(0<=u && 0<=v && u<orderG() && v<orderG()){
        if (u>v)
            return (u-v)%12;
        else
            return (v-u)%12;
    }
    return 0;
}





