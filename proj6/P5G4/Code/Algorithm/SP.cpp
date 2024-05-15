#include<bits/stdc++.h>
#define N 1000005

using namespace std;

int n,W,cnt;

struct node{int w,h,id;}a[N];

struct strips{
    int xposition,yposition,width,lower,upper,itemWidth;
}s[N];

struct node1{
    int x,y,id;
}q[N];

// Comparison function for sorting items by width and height in non-increasing order
inline bool cmp(node a,node b){if(a.w==b.w)return a.h>b.h;return a.w>b.w;}

// Comparison function for sorting positions by item ID
inline bool cmp1(node1 a,node1 b){return a.id<b.id;}

int main(){
    // Read the number of items
    scanf("%d",&n);
    
    // Read the width and height of each item
    for (int i=1;i<=n;i++) scanf("%d%d",&a[i].w,&a[i].h),a[i].id=i;
    
    // Read the width of the strip
    scanf("%d",&W);
    
    // Sort the items in non-increasing order of width and height
    sort(a+1,a+n+1,cmp);
    
    int ans=0;
    s[cnt=1].xposition=0;
    s[cnt].yposition=s[cnt].lower=s[cnt].upper=0;
    s[cnt].width=s[cnt].itemWidth=W;
    
    for (int i=1;i<=n;i++){
        int fd=0;
        
        // Find a suitable strip to place the item
        for (int j=1;j<=cnt;j++){
            if (s[j].width-s[j].itemWidth>=a[i].w){
                if (!fd) fd=j;
                else if (s[fd].width>s[j].width) fd=j;
            }
        }
        
        if (!fd){
            // If no suitable strip is found, place the item in the lowest strip
            
            for (int j=1;j<=cnt;j++){
                if (!fd) fd=j;
                else if (s[fd].upper>s[j].upper) fd=j;
            }
            
            q[++ans].x=s[fd].xposition;
            q[ans].y=s[fd].upper;
            q[ans].id=a[i].id;
            s[fd].lower=s[fd].upper;
            s[fd].upper=s[fd].upper+a[i].h;
            s[fd].itemWidth=a[i].w;
        }
        else {
            // If a suitable strip is found, place the item in that strip
            
            q[++ans].id=a[i].id;
            q[ans].x=s[fd].xposition+s[fd].itemWidth;
            q[ans].y=s[fd].lower;
            
            strips s1,s2;
            s1.xposition=s[fd].xposition;
            s1.yposition=s[fd].upper;
            s1.width=s[fd].itemWidth;
            s1.lower=s[fd].upper;
            s1.upper=s[fd].upper;
            s1.itemWidth=s[fd].itemWidth;
            
            s2.xposition=s[fd].xposition+s[fd].itemWidth;
            s2.yposition=s[fd].lower;
            s2.width=s[fd].width-s[fd].itemWidth;
            s2.lower=s[fd].lower;
            s2.upper=s[fd].lower+a[i].h;
            s2.itemWidth=a[i].w;
            
            // Adjust the strips
            
            for (int j=fd;j<cnt;j++) s[j]=s[j+1];cnt--;
            s[++cnt]=s1;
            s[++cnt]=s2;
        }
    }
    
    // Sort the positions in ascending order of item ID
    sort(q+1,q+n+1,cmp1);
    
    // Print the positions of each item
    for (int i=1;i<=n;i++) printf("Item %d:(%d, %d)\n",q[i].id,q[i].x,q[i].y);
    
    return 0;
}
