#include<bits/stdc++.h>
#define N 1000005

using namespace std;

int n,W,cnta,cntb,ipt1[N],ipt2[N];

struct node{int w,h,id;}a[N],b[N];

bool operator < (const node a,const node b){return a.h>b.h;}

struct node1{int x,y,id;}q[N];

inline bool cmp(node1 a,node1 b){return a.id<b.id;} 

int main(){
    // Read input
    scanf("%d",&n);
    for (int i=1;i<=n;i++) scanf("%d%d",&ipt1[i],&ipt2[i]);
    scanf("%d",&W);
    
    // Divide the items into two groups based on their width
    for (int i=1;i<=n;i++){
        int w,h;
        w=ipt1[i];h=ipt2[i];
        if (2*w>W) a[++cnta]=(node){w,h,i}; // Group A: width is greater than half of the strip width
        else b[++cntb]=(node){w,h,i}; // Group B: width is less than or equal to half of the strip width
    }
    
    // Randomly shuffle items in Group A
    random_shuffle(a+1,a+cnta+1);
    
    int nowh=0;
    int cnt=0;
    // Place items from Group A vertically at the left side of the strip
    for (int i=1;i<=cnta;i++) q[++cnt].x=0,q[cnt].y=nowh,q[cnt].id=a[i].id,nowh+=a[i].h;
    
    // Sort items in Group B in non-increasing order of height
    sort(b+1,b+cntb+1);
    
    int j=1,hl=nowh,hr=nowh;
    // Place items from Group B horizontally at the bottom or top of the strip
    for (int i=1;i<=cntb;i=j+1){
        int now_w=b[i].w,hmax=b[i].h;
        j=i;
        while (j<cntb&&2*(b[j+1].w+now_w)<=W) now_w+=b[j+1].w,j++;
        if (hl<=hr){
            int ww=0;
            // Place items at the bottom of the strip
            for (int k=i;k<=j;k++) q[++cnt].x=ww,q[cnt].y=hl,q[cnt].id=b[k].id,ww+=b[k].w;
            hl+=hmax;
        }
        else{
            int ww=W/2;
            // Place items at the top of the strip
            for (int k=i;k<=j;k++) q[++cnt].x=ww,q[cnt].y=hr,q[cnt].id=b[k].id,ww+=b[k].w;
            hr+=hmax;
        }
    }
    
    // Sort the final positions of all items by their IDs
    sort(q+1,q+cnt+1,cmp);
    
    // Print the positions of each item
    for (int i=1;i<=cnt;i++) printf("Item %d:(%d, %d)\n",q[i].id,q[i].x,q[i].y);
    
    return 0;
}
