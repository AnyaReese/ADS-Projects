#include<bits/stdc++.h>
#define N 1000005

using namespace std;

int n,W,hmax;
map<int,int>mp[N];

// Function to place a shape in the strip
void push(int x){
    if (x==1){
        // Place shape of type 1: 1x4 rectangle
        for (int i=1;i<=hmax;i++)
            for (int j=1;j<=W;j++){
                if (j+3>W) continue;
                int fg=1;
                for (int k=j;k<=j+3;k++)
                    if (mp[i].count(k)){fg=0;break;}
                if (!fg) continue;
                for (int k=j;k<=j+3;k++) mp[i][k]=1;
                return;
            }
        // If no suitable position is found, create a new row and place the shape
        hmax++;
        for (int j=1;j<=4;j++) mp[hmax][j]=1;
        return;
    }
    if (x==2){
        // Place shape of type 2: 2x2 square
        for (int i=1;i<=hmax;i++){
            for (int j=1;j<W;j++){
                int fg=1;
                for (int k=i;k<=i+1;k++)
                    for (int l=j;l<=j+1;l++)
                        if (mp[k].count(l)){fg=0;break;}
                if (!fg) continue;
                for (int k=i;k<=i+1;k++)
                    for (int l=j;l<=j+1;l++) mp[k][l]=1;
                if (i==hmax) hmax++;
                return;
            }
        }
        // If no suitable position is found, create two new rows and place the shape
        hmax+=2;
        for (int i=hmax-1;i<=hmax;i++) mp[i][1]=mp[i][2]=1;
        return;
    }
    if (x==3){
        // Place shape of type 3: 3x2 rectangle
        for (int i=1;i<=hmax;i++){
            for (int j=1;j<W;j++){
                int fg=1;
                for (int k=i;k<=i+2;k++)
                    if (mp[k].count(j)){fg=0;break;}
                if (mp[i].count(j+1)) fg=0;
                if (!fg) continue;
                for (int k=i;k<=i+2;k++) mp[k][j]=1;
                mp[i][j+1]=1;
                hmax=max(hmax,i+2);
                return;
            }
        }
        // If no suitable position is found, create three new rows and place the shape
        hmax+=3;
        for (int i=hmax-2;i<=hmax;i++) mp[i][1]=1;
        mp[hmax-2][2]=1;
        return;
    }
    if (x==4){
        // Place shape of type 4: L shape
        for (int i=1;i<=hmax;i++){
            for (int j=2;j<=W;j++){
                if (mp[i].count(j)) continue;
                if (mp[i+1].count(j)) continue;
                if (mp[i+1].count(j-1)) continue;
                if (mp[i+2].count(j-1)) continue;
                mp[i][j]=mp[i+1][j]=mp[i+1][j-1]=mp[i+2][j-1]=1;
                hmax=max(hmax,i+2);
                return;
            }
        }
        // If no suitable position is found, create three new rows and place the shape
        hmax+=3;
        mp[hmax-2][W]=mp[hmax-1][W]=mp[hmax-1][W-1]=mp[hmax][W-1]=1;
        return;
    }
    // Place shape of type T: T shape
    for (int i=1;i<=hmax;i++){
        for (int j=2;j<=W-1;j++){
            if (mp[i].count(j)) continue;
            if (mp[i+1].count(j+1)) continue;
            if (mp[i+1].count(j)) continue;
            if (mp[i+1].count(j-1)) continue;
            mp[i][j]=mp[i+1][j+1]=mp[i+1][j]=mp[i+1][j-1]=1;
            hmax=max(hmax,i+1);
            return;
        }
    }
    // If no suitable position is found, create two new rows and place the shape
    hmax+=2;
    mp[hmax-1][3]=mp[hmax][3]=mp[hmax][2]=mp[hmax][4]=1;
}

int main(){
    scanf("%d%d",&n,&W);
    hmax=1;
    for (int i=1;i<=n;i++){
        int x;
        scanf("%d",&x);
        push(x);  // Place each shape
    }
    printf("%d\n",hmax);  // Print the number of rows used
    return 0;
}
