#include <stdlib.h>
#include <stdio.h>
#include <time.h>     //per usare rand()
#include <windows.h>  //per usare Sleep()

void riempiMat(char mov,char map[][21]); //riempie la matrice "map" per farla stampare
void stampaMat(char map[20][21]);  //stampa la matrice "map" raddoppiandone il contenuto (per farla diventare quadrata)
void muovi(char mov,char map[20][21]); //fa spostare la testa del serpente secondo alla mossa
void pushArray(); //sposta l'array delle coordinate di tutto il corpo del serpenmte di una posizione per far spazio al nuovo pezzo
void mela(char map[20][21]); //fa comparire la mela nella mappa
int controlloMela(char map[20][21]); //controlla se c'� gi� la mela nella mappa
int controllo(int time,char map[20][21]); //controlla se sei morto
int contrario(char mov,char Pmov); //controlla se la mossa che hai inserito � contraria alla mossa precedente (se si non te la lascia fare)
int refreshTime(); //funzione che calcola il tempo per il frame

int x=9,y=9; //coordinate testa snake
int body[50][2]; //coordinate di tutto il corpo //0=x,1=y
int len=3; //lunghezza del corpo

int main()
{
    char Pmov='0'; //previous move
    int time=0; //punteggio
    int Fmela=0; //flag che dice se c'� la mela o no
    int i,j;
    for(i=0;i<2;i++)
        for(j=0;j<50;j++)
            body[i][j]=-1; //inizializzo tutta la matrica a '-1' perch� si
    char map[20][21]; //mappa 20x20 (21 cos� la si trasforna in un array di stringhe per stamparlo pi� velocemente
    char mov='0'; //mossa (w,a,s,d)
    for(i=0;i<20;i++)
        map[i][20]='\0'; //si fa diventare la matrice un array di stringhe
    riempiMat(mov,map);
    stampaMat(map);
    do{
        fflush(stdin);                                                              //input
        printf("%d\n\n",clock());
        Sleep(refreshTime()); //refresh time
        if(kbhit()!=0) //se si � cliccato un tasto cambia mov nel tasto cliccato
            mov=getch(stdin);   //se no, lascia quello precedente
        if(contrario(mov,Pmov)!=0) //controlla che non stai facendo una mossa "illegale"
            mov=Pmov;
        else
            Pmov=mov;
        muovi(mov,map);
        riempiMat(mov,map);

                                                                                    //mela
        if(controlloMela(map) == 0) //controlla se c'� la mela o no
            mela(map);  //se no la mette
          
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);           //output
        stampaMat(map);
        if(mov!='0') //se non si � ancora fatta la prima mossa non fa andare avanti il tempo
            time++;
        printf("\t\tTEMPO: %d\tPUNTEGGIO:%d\n",time,len);

    }while( controllo(time,map) == 0 ); //quando � diverso da 0 restituisce false
    printf("\n\n\tHAI PERSO\n\n");                                                  //fine
    system("PAUSE");
}

void riempiMat(char mov,char map[][21]){
    int i,j;
    for(i=0;i<20;i++)       //fa tornare tutta la matrice a ' '
        for(j=0;j<20;j++)
            if(map[i][j]!='O') //per� se c'� la mela lalascia
                map[i][j]=' ';

    for(i=0;body[i+1][0]!=-1;i++)
        map[body[i][1]][body[i][0]]=219; //mette tutto il serpente nella mappa

    switch(mov){    //mette la testa nella mappa
    case 'w':       //in base a dove il serpente sta "guardando"
        map[y][x]=218;
    break;case 'a':
        map[y][x]='<';
    break;case 's':
        map[y][x]=217;
    break;case 'd':
        map[y][x]='>';
    }


}
void muovi(char mov,char map[20][21]){
    pushArray(); //pusha l'array del corpo del serpente
    body[0][0]=x; //e mette nella prima posizione la posizione della testa
    body[0][1]=y;
    switch(mov){    //sposta le coordinate della testa in base alla mossa
    case 'w':
        map[--y][x]=219;
    break;case 'a':
        map[y][--x]=219;
    break;case 's':
        map[++y][x]=219;
    break;case 'd':
        map[y][++x]=219;
    }
}
#define riga 45
void stampaMat(char map[20][21]){
    int i,j;
    char mapx2[20*riga+1];
    for(i=0;i<20;i++)
        for(j=0;j<20;j++){
            mapx2[i*riga+j*2+3]=map[i][j];
            mapx2[i*riga+j*2+3+1]=map[i][j];
        }
    for(i=0;i<20;i++){
        mapx2[i*riga+riga-1]='\n';
        mapx2[i*riga+riga-43]='|';
        mapx2[i*riga+riga-2]='|';
        mapx2[i*riga+riga-riga]='\t';
        mapx2[i*riga+riga-riga+1]='\t';}

    mapx2[20*riga]='\0';

    printf("\n\n\t\t------------------------------------------");
    printf("\n%s",mapx2);
    printf("\t\t------------------------------------------\n\n");
}
void pushArray(){
    int i;
    for(i=len-1;i>=0;i--){  //sposta l'array di una casella in avanti
        body[i+1][0]=body[i][0];
        body[i+1][1]=body[i][1];
    }
    body[len][0]=-1; //nell'ultima casella ci rimette '-1'
    body[len][1]=-1;
}
void mela(char map[20][21]){
    srand(time(NULL));
    int i,j,a,b;
    short flag; //dice se le coordinate generate sono occupate o no
    do{
        a=rand()%20; //genera coordinate random perlamela
        b=rand()%20;
        for(i=0;i<len+1;i++)
            if(( body[i][0]==a && body[i][1]==b )||( a==x && b==y ))    //controlla che non siano occupate dal serpente
                flag=1;
    }while(flag==1);
    map[b][a]='O'; //mette la mela
}
int controlloMela(char map[20][21]){
    int i,j;
    for(i=0;i<20;i++)  //controlla se c'�lamelanella mappa
        for(j=0;j<20;j++)
            if(map[i][j]=='O')
                return 1; //se si ritorna 1
    len++;    //se no vuol dire che � stata mangiata
    return 0; //e quindi ritorna 0 e aumenta 'len'
}
int controllo(int time,char map[20][21]){
    if(time<len+2) //se no all'inizio le coordinate della testa coincidono con quelle del corpo
        return 0;
    int i,j;
    for(i=0;i<len-1;i++)
        if(body[i][0]==x && body[i][1]==y) //se ti sei scontrato conro il corpo
            return 1;
    if(x==-1 || x==20) //muro a destra e sinistra
        return 1;
    if(y==-1 || y==20) //muro sopra e sotto
        return 1;
    return 0;
}
int contrario(char mov,char Pmov){
    switch(Pmov){ //controlla se la mossa precedente � contraria alla mossa appena inserita e se si ritorna 1
    case 'w':
        if(mov=='s'){
            return 1;}
    break;case 'a':
        if(mov=='d')
            return 1;
    break;case 's':
        if(mov=='w')
            return 1;
    break;case 'd':
        if(mov=='a')
            return 1;
    break;
    }
    return 0;
}
int refreshTime(){
    if(len*3>150)               //formula per calcolare il tempo di ogni frame
        return 50;              //(diminuisce con l'avanzamento del gioco)
    return 200-len*3;
}
