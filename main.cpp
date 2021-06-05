#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include<sstream>
#include<fstream>
#include "BPlus_Tree.cpp"


const int inf=0x3f3f3f3f;

int nextorder=0;
char odr[20];
BPlus_Tree user("_user",2000000),train("_train",60000000),station("_station",60000000);
BPlus_Tree order("_order",200000000),queue("_queue",200000000);
BPlus_Tree hashtable1("_login",10000),hashtable2("_release",10000);
BPlus_Tree first_user("_first",100);

std::fstream file_user("user",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_train("train",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_station("station",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_order("order",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_queue("queue",std::ios::in|std::ios::out|std::ios::binary);
//create file
void create_file(){
	if(!file_user.is_open()){
		file_user.open("user",std::ios::out|std::ios::binary);
		file_user.close();
		file_user.open("user",std::ios::in|std::ios::out|std::ios::binary);
	}
	if(!file_train.is_open()){
		file_train.open("train",std::ios::out|std::ios::binary);
		file_train.close();
		file_train.open("train",std::ios::in|std::ios::out|std::ios::binary);
	}
	if(!file_station.is_open()){
		file_station.open("station",std::ios::out|std::ios::binary);
		file_station.close();
		file_station.open("station",std::ios::in|std::ios::out|std::ios::binary);
	}
	if(!file_order.is_open()){
		file_order.open("order",std::ios::out|std::ios::binary);
		file_order.close();
		file_order.open("order",std::ios::in|std::ios::out|std::ios::binary);
	}
	if(!file_queue.is_open()){
		file_queue.open("queue",std::ios::out|std::ios::binary);
		file_queue.close();
		file_queue.open("queue",std::ios::in|std::ios::out|std::ios::binary);
	}
}

//string compare
bool string_same(const char *A,const char *B){
	int al=strlen(A),bl=strlen(B);
	if(al!=bl)return false;
	for(int i=0;i<al;++i)
		if(A[i]!=B[i])return false;
	return true;
}
bool string_less(const char *A,const char *B){
	int al=strlen(A),bl=strlen(B),l=std::min(al,bl);
	for(int i=0;i<l;++i)
		if(A[i]!=B[i])return A[i]<B[i];
	return al<bl;
}

//hash
unsigned int hash_calc(char *s){
	int len=strlen(s);
	unsigned int res=0;
	for(int i=0;i<len;++i)res=res*19260817+s[i];
	return res;
}

//sort
struct sort_node{
	int key;
	char i[25];
	int st,tt,p,s,x[2];
	bool operator <= (const sort_node &A) const {
		return key<A.key||(key==A.key&&(string_less(i,A.i)||string_same(i,A.i)));
	};
	void operator = (const sort_node &A) {
		key=A.key;
		memcpy(i,A.i,sizeof(A.i));
		st=A.st,tt=A.tt,p=A.p,s=A.s,x[0]=A.x[0],x[1]=A.x[1];
	}
}sort_a[10005];
void sort(int l,int r,sort_node *a){
	if(l>=r)return ;
	int i,j;
	sort_node base,tmp;
	i=l,j=r,base=a[l];
	while(i<j){
		while(base<=a[j]&&i<j)j--;
		while(a[i]<=base&&i<j)i++;
		if(i<j)tmp=a[i],a[i]=a[j],a[j]=tmp;
	}
	a[l]=a[i];a[i]=base;
	sort(l,i-1,a),sort(i+1,r,a);
}
void sort(int l,int r,unsigned int *a){
	if(l>=r)return ;
	int i,j,base,tmp;
	i=l,j=r,base=a[l];
	while(i<j){
		while(a[j]>=base&&i<j)j--;
		while(a[i]<=base&&i<j)i++;
		if(i<j)tmp=a[i],a[i]=a[j],a[j]=tmp;
	}
	a[l]=a[i];a[i]=base;
	sort(l,i-1,a),sort(i+1,r,a);
}

//user
struct user_node{
	char u[25],p[35],n[25],m[35];
	int g;
}_user,ucur;
int add_user(){
	char now[5],c[25];
	for(int i=1;i<=6;++i){
		scanf("%s",now);
		if(now[1]=='c')scanf("%s",c);
		else if(now[1]=='u')scanf("%s",_user.u);
		else if(now[1]=='p')scanf("%s",_user.p);
		else if(now[1]=='n')scanf("%s",_user.n);
		else if(now[1]=='m')scanf("%s",_user.m);
		else scanf("%d",&_user.g);
	}
	//the first user
	if(first_user.query(0)==-1){
		_user.g=10;
		user.insert(hash_calc(_user.u),0);
		file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
		first_user.insert(0,1);
		return 0;
	}
	//login
	unsigned int curh=hash_calc(c),uhash=hash_calc(_user.u);
	if(hashtable1.query(curh)==-1)return -1;
	//privilege
	int curpos=user.query(curh);
	file_user.seekg(curpos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	if(ucur.g<=_user.g)return -1;
	//id conflict
	if(user.query(uhash)!=-1)return -1;
	//legal
	file_user.seekg(0,std::ios::end);
	int nowpos=file_user.tellg();
	user.insert(uhash,nowpos);
	file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
	return 0;
}
int login(){
	char now[5],u[25],p[35];
	for(int i=1;i<=2;++i){
		scanf("%s",now);
		if(now[1]=='u')scanf("%s",u);
		else scanf("%s",p);
	}
	unsigned int uhash=hash_calc(u);
	int pos=user.query(uhash);
	//username error
	if(pos==-1)return -1;
	//have logged in
	if(hashtable1.query(uhash)==1)return -1;
	file_user.seekg(pos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
//	std::cout<<u<<' '<<ucur.u<<' '<<ucur.p<<' '<<p<<'\n';
	//password error
	if(string_same(ucur.p,p)){
		hashtable1.insert(uhash,1);
		return 0;
	} else return -1;
}
int logout(){
	char now[5],u[25];
	scanf("%s%s",now,u);
	unsigned int uhash=hash_calc(u);
	if(hashtable1.query(uhash)==-1)return -1;
	hashtable1.erase(uhash,1);
	return 0;
}
int query_profile(){
	char now[5],c[25],u[25];
	for(int i=1;i<=2;++i){
		scanf("%s",now);
		if(now[1]=='c')scanf("%s",c);
		else scanf("%s",u);
	}
	unsigned int chash=hash_calc(c),uhash=hash_calc(u);
	//login
	if(hashtable1.query(chash)==-1)return -1;
	int cpos=user.query(chash),upos=user.query(uhash);
	//query itself
	if(string_same(c,u)){
		file_user.seekg(upos,std::ios::beg);
		file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
		return 0;
	}
	//username error
	if(upos==-1)return -1;
	//legal
	file_user.seekg(cpos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	file_user.seekg(upos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
	return ucur.g>_user.g?0:-1;
}
int modify_profile(){
	char now[20],c[25],u[25],p[35],n[25],m[25];
	int g,fm=0,fn=0,fp=0,fg=0;
	while(scanf("%s",now)!=EOF){
		if(now[0]!='-'){
			nextorder=1;
			memcpy(odr,now,sizeof(odr));
			break;
		}
		if(now[1]=='c')scanf("%s",c);
		else if(now[1]=='u')scanf("%s",u);
		else if(now[1]=='p')scanf("%s",p),fp=1;
		else if(now[1]=='n')scanf("%s",n),fn=1;
		else if(now[1]=='m')scanf("%s",m),fm=1;
		else scanf("%d",&g),fg=1;
	}
	unsigned int chash=hash_calc(c),uhash=hash_calc(u);
	//login error
	if(hashtable1.query(chash)==-1)return -1;
	int cpos=user.query(chash),upos=user.query(uhash);
	if(string_same(u,c)){
		file_user.seekg(upos,std::ios::beg);
		file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
		if(fg&&_user.g<=g)return -1;
		if(fp)memcpy(_user.p,p,sizeof(_user.p));
		if(fn)memcpy(_user.n,n,sizeof(_user.n));
		if(fm)memcpy(_user.m,m,sizeof(_user.m));
		if(fg)_user.g=g;
		file_user.seekg(upos,std::ios::beg);
		file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
		return 0;
	}
	//username error
	if(upos==-1)return -1;
	file_user.seekg(cpos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	file_user.seekg(upos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
	//privilege error
	if(ucur.g<=_user.g||(fg&&ucur.g<=g))return -1;
	if(fp)memcpy(_user.p,p,sizeof(_user.p));
	if(fn)memcpy(_user.n,n,sizeof(_user.n));
	if(fm)memcpy(_user.m,m,sizeof(_user.m));
	if(fg)_user.g=g;
	file_user.seekg(upos,std::ios::beg);
	file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
	return 0;
}

//time
int months[13]={inf,31,28,31,30,31,30,31,31,30,31,30,31};
int minsum[13]={inf,0,44640,84960,129600,172800,217440,260640,305280,349920,393120,437760,480960};
struct ntime{
	int m,d,h,n;
	void output(char c='\0'){
		if(m<10)printf("0");printf("%d-",m);
		if(d<10)printf("0");printf("%d ",d);
		if(h<10)printf("0");printf("%d:",h);
		if(n<10)printf("0");printf("%d",n);
		if(c!='\0')putchar(c);
	}
	bool operator < (const ntime &A) const {
		return m<A.m||(m==A.m&&d<A.d)||
			(m==A.m&&d==A.d&&h<A.h)||(m==A.m&&d==A.d&&h==A.h&&n<=A.n);
	}
	int operator - (const ntime &A) const {
		return minsum[m]+d*24*60+h*60+n-minsum[A.m]-A.d*24*60-A.h*60-A.n;
	}
};
ntime timecalc(ntime p,int t){
	int m=p.m,d=p.d,h=p.h,n=p.n;
	n+=t;
	h+=n/60;n%=60;
	d+=h/24;h%=24;
	while(d>months[m])d-=months[m],m++;
	if(m>12)m-=12;
	return (ntime){m,d,h,n};
}
int timeid(int m,int d){
	if(m==6)return d;
	else if(m==7)return d+months[6];
	else if(m==8)return d+months[6]+months[7];
	else return d+months[6]+months[7]+months[8];
}
int timedecode1(int d){
	int res=6;
	while(d>months[res])d-=months[res],res++;
	return res;
}
int timedecode2(int d){
	int res=6;
	while(d>months[res])d-=months[res],res++;
	return d;
}

//train
struct train_node{
	char i[25],s[105][50],y;
	int n,m[100][105],o[105],p[105],x[2],t[105],d[4],ms;
	int arrive[105],leave[105];
}_train,__train;
struct station_node{
	int cnt;
	unsigned int i[10005];
}_station,__station;
struct order_node{
	int sta,n,pre,di,si,ti,cnt,p;
	ntime s,t;
	char i[25],sp[50],tp[50];
}_order;
struct queue_node{
	int opos,n,pre,di,si,ti,done;
	void operator = (const queue_node &A) {
		opos=A.opos,n=A.n,di=A.di,si=A.si,ti=A.ti,pre=A.pre,done=A.done;
	}
}_queue,__queue;
int add_train(){
	//read the information
	char now[5];int m;
	for(int i=1;i<=10;++i){
		scanf("%s",now);
		if(now[1]=='i')scanf("%s",_train.i);
		else if(now[1]=='n')scanf("%d",&_train.n);
		else if(now[1]=='m')scanf("%d",&m);
		else if(now[1]=='s'){
			char stations[5005];scanf("%s",stations);
			int cnt=0,len=strlen(stations);
			for(int j=0;j<len;++j){
				++cnt;int pos=0;
				while(stations[j]!='|'&&j<len)_train.s[cnt][pos++]=stations[j],++j;
				_train.s[cnt][pos]='\0';
			}
		}
		else if(now[1]=='p'){
			char prices[1005];scanf("%s",prices);
			int cnt=0,len=strlen(prices);
			for(int j=0;j<len;++j){
				++cnt;int tmp=0;
				while(prices[j]!='|'&&j<len)tmp=tmp*10+prices[j]-'0',++j;
				_train.p[cnt]=tmp;
			}
		}
		else if(now[1]=='x')scanf("%d:%d",&_train.x[0],&_train.x[1]);
		else if(now[1]=='t'){
			char times[1005];scanf("%s",times);
			int cnt=0,len=strlen(times);
			for(int j=0;j<len;++j){
				++cnt;int tmp=0;
				while(times[j]!='|'&&j<len)tmp=tmp*10+times[j]-'0',++j;
				_train.t[cnt]=tmp;
			}
		}
		else if(now[1]=='o'){
			char stops[1005];scanf("%s",stops);
			if(stops[0]!='_'){
				int cnt=0,len=strlen(stops);
				for(int j=0;j<len;++j){
					++cnt;int tmp=0;
					while(stops[j]!='|'&&j<len)tmp=tmp*10+stops[j]-'0',++j;
					_train.o[cnt]=tmp;
				}
			}
		}
		else if(now[1]=='d')scanf("%d-%d|%d-%d",&_train.d[0],&_train.d[1],&_train.d[2],&_train.d[3]);
		else std::cin>>_train.y;
	}
	//illegal
	unsigned int ihash=hash_calc(_train.i);
	if(train.query(ihash)!=-1)return -1;
 	//process the information
	_train.leave[0]=0;_train.ms=m;
	for(int j=1;j<=92;++j)_train.m[j][1]=m;
	for(int i=2;i<=_train.n;++i){
		for(int j=1;j<=92;++j)_train.m[j][i]=m;
		_train.arrive[i]=_train.leave[i-1]+_train.t[i-1];
		if(i!=_train.n)_train.leave[i]=_train.arrive[i]+_train.o[i-1];
	}
	//write train
	file_train.seekg(0,std::ios::end);
	int nowpos=file_train.tellg();
	train.insert(ihash,nowpos);
	file_train.write(reinterpret_cast<char * >(&_train),sizeof(_train));
	//write station
	for(int i=1;i<=_train.n;++i){
		unsigned int shash=hash_calc(_train.s[i]);
		int spos=station.query(shash);
		if(spos==-1){
			_station.cnt=1;
			_station.i[_station.cnt]=ihash;
			file_station.seekg(0,std::ios::end);
			spos=file_station.tellg();
			station.insert(shash,spos);
			file_station.write(reinterpret_cast<char * >(&_station),sizeof(_station));
		} else {
			file_station.seekg(spos,std::ios::beg);
			file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
			_station.cnt++;
			_station.i[_station.cnt]=ihash;
			file_station.seekg(spos,std::ios::beg);
			file_station.write(reinterpret_cast<char * >(&_station),sizeof(_station));
		}
	}
	return 0;
}
int release_train(){
	char now[5],i[25];
	scanf("%s%s",now,i);
	unsigned int ihash=hash_calc(i);
	//not exist
	if(train.query(ihash)==-1)return -1;
	//have been released
	if(hashtable2.query(ihash)==1)return -1;
	//legal
	hashtable2.insert(ihash,1);
	return 0;
}
int query_train(){
	char now[5],_i[25];
	int d[2];
	for(int i=1;i<=2;++i){
		scanf("%s",now);
		if(now[1]=='i')scanf("%s",_i);
		else scanf("%d-%d",&d[0],&d[1]);
	}
	if(d[0]<6)return -1;
	unsigned int ihash=hash_calc(_i);
	int nowpos=train.query(ihash),di=timeid(d[0],d[1]);
	//not exist
	if(nowpos==-1)return -1;
	file_train.seekg(nowpos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	int ld=timeid(_train.d[0],_train.d[1]),rd=timeid(_train.d[2],_train.d[3]);
	if(di<ld||di>rd)return -1;
	printf("%s %c\n",_train.i,_train.y);
	ntime et=(ntime){d[0],d[1],_train.x[0],_train.x[1]};
	int psum=0;
	for(int i=1;i<=_train.n;++i){
		printf("%s ",_train.s[i]);
		if(i==1)printf("xx-xx xx:xx");
		else timecalc(et,_train.arrive[i]).output();
		printf(" -> ");
		if(i==_train.n)printf("xx-xx xx:xx ");
		else timecalc(et,_train.leave[i]).output(' ');
		if(i!=1)psum+=_train.p[i-1];
		printf("%d ",psum);
		if(i==_train.n)printf("x\n");
		else printf("%d\n",_train.m[di][i]);
	}
	return 0;
}
int delete_train(){
	char now[5],i[25];
	scanf("%s%s",now,i);
	unsigned int ihash=hash_calc(i);
	int nowpos=train.query(ihash);
	//not exist
	if(nowpos==-1)return -1;
	//have been released
	if(hashtable2.query(ihash)==1)return -1;
	//delete train
	train.erase(ihash,nowpos);
	//delete station
	file_train.seekg(nowpos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	for(int i=1;i<=_train.n;++i){
		unsigned int shash=hash_calc(_train.s[i]);
		int spos=station.query(shash);
		file_station.seekg(spos,std::ios::beg);
		file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
		for(int j=1;j<=_station.cnt;++j)
			if(_station.i[j]==ihash){
				_station.i[j]=_station.i[_station.cnt];
				_station.cnt--;
				break;
			}
		file_station.seekg(spos,std::ios::beg);
		file_station.write(reinterpret_cast<char * >(&_station),sizeof(_station));
	}
	return 0;
}
void query_ticket(){
	char now[20],s[50],t[50],q[6];
	int d[2],fq=0;
	while(scanf("%s",now)!=EOF){
		if(now[0]!='-'){
			nextorder=1;
			memcpy(odr,now,sizeof(odr));
			break;
		}
		if(now[1]=='s')scanf("%s",s);
		else if(now[1]=='t')scanf("%s",t);
		else if(now[1]=='d')scanf("%d-%d",&d[0],&d[1]);
		else scanf("%s",q),fq=1;
	}
	if(d[0]<6){
		puts("0");
		return ;
	}
	int cnt=0;
	unsigned int shash=hash_calc(s),thash=hash_calc(t);
	int spos=station.query(shash),tpos=station.query(thash);
	if(spos==-1||tpos==-1){
		puts("0");
		return ;
	}
	file_station.seekg(spos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
	file_station.seekg(tpos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&__station),sizeof(__station));
	sort(1,_station.cnt,_station.i);
	sort(1,__station.cnt,__station.i);
	int i=1,j=1;
	while(i<=_station.cnt&&j<=__station.cnt){
		if(_station.i[i]==__station.i[j]){
			if(hashtable2.query(_station.i[i])==-1){
				++i;++j;continue;
			}
			int tpos=train.query(_station.i[i]);
			++i,++j;
			file_train.seekg(tpos,std::ios::beg);
			file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
			int st,tt,p=0,seat=inf,mark=0,di=timeid(d[0],d[1]);
			for(int k=1;k<=_train.n;++k){
				if(string_same(s,_train.s[k])){
					mark=1;
					st=_train.leave[k];
					ntime tmp=(ntime){0,0,_train.x[0],_train.x[1]};
					tmp=timecalc(tmp,st);
					di-=tmp.d;
					int ld=timeid(_train.d[0],_train.d[1]),rd=timeid(_train.d[2],_train.d[3]);
					if(di<ld||di>rd){
						mark=0;
						break;
					}
				} else if(string_same(t,_train.s[k])){
					tt=_train.arrive[k];
					break;
				}
				if(mark)p+=_train.p[k],seat=std::min(seat,_train.m[di][k]);
			}
			if(!mark)continue;
			++cnt;
			memcpy(sort_a[cnt].i,_train.i,sizeof(_train.i));
			sort_a[cnt].p=p;
			sort_a[cnt].s=seat;
			sort_a[cnt].st=st,sort_a[cnt].tt=tt;
			sort_a[cnt].x[0]=_train.x[0],sort_a[cnt].x[1]=_train.x[1];
			if(q[0]=='t'||!fq)sort_a[cnt].key=tt-st;
			else sort_a[cnt].key=p;
		} else {
			if(_station.i[i]<__station.i[j])++i;
			else ++j;
		}
	}
	sort(1,cnt,sort_a);
	printf("%d\n",cnt);
	for(i=1;i<=cnt;++i){
		printf("%s %s ",sort_a[i].i,s);
		ntime tmp=(ntime){d[0],d[1],sort_a[i].x[0],sort_a[i].x[1]};
		tmp=timecalc(tmp,sort_a[i].st);
		tmp.m=d[0],tmp.d=d[1];
		tmp.output();
		printf(" -> %s ",t);
		timecalc(tmp,sort_a[i].tt-sort_a[i].st).output(' ');
		printf("%d %d\n",sort_a[i].p,sort_a[i].s);
	}
}
void query_transfer(){
	char now[20],s[50],t[50],q[6];
	int d[2],fq=0;
	while(scanf("%s",now)!=EOF){
		if(now[0]!='-'){
			nextorder=1;
			memcpy(odr,now,sizeof(odr));
			break;
		}
		if(now[1]=='s')scanf("%s",s);
		else if(now[1]=='t')scanf("%s",t);
		else if(now[1]=='d')scanf("%d-%d",&d[0],&d[1]);
		else scanf("%s",q),fq=1;
	}
	if(d[0]<6){
		puts("0");
		return ;
	}
	int key=inf,_key=inf,gap=0;
	char mids[50];
	unsigned int shash=hash_calc(s),thash=hash_calc(t),mhash,ihash,_ihash;
	int spos=station.query(shash),tpos=station.query(thash),mpos,ipos,_ipos;
	if(spos==-1||tpos==-1){
		puts("0");
		return ;
	}
	file_station.seekg(spos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
	//enumerate the train passing by the station S
	for(int i=1;i<=_station.cnt;++i){
		ihash=_station.i[i];
		if(hashtable2.query(ihash)==-1)continue;
		ipos=train.query(ihash);
		file_train.seekg(ipos,std::ios::beg);
		file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
		int mark=0,p=0,s=inf,di=timeid(d[0],d[1]);
		//enumerate the mid station M
		for(int j=1;j<=_train.n;++j){
			int nowhash=hash_calc(_train.s[j]);
			if(!mark&&nowhash==shash){
				ntime tmp=(ntime){0,0,_train.x[0],_train.x[1]};
				tmp=timecalc(tmp,_train.leave[j]);
				di-=tmp.d;
				mark=j;
				int ld=timeid(_train.d[0],_train.d[1]),rd=timeid(_train.d[2],_train.d[3]);
				if(di<ld||di>rd)break;
			}
			else if(mark){
				if(nowhash==thash)continue;
				mhash=hash_calc(_train.s[j]);
				mpos=station.query(mhash);
				file_station.seekg(mpos,std::ios::beg);
				file_station.read(reinterpret_cast<char * >(&__station),sizeof(__station));
				//enumerate the train passing by the station M
				for(int k=1;k<=__station.cnt;++k){
					_ihash=__station.i[k];
					if(_ihash==ihash)continue;
					if(hashtable2.query(_ihash)==-1)continue;
					_ipos=train.query(_ihash);
					file_train.seekg(_ipos,std::ios::beg);
					file_train.read(reinterpret_cast<char * >(&__train),sizeof(__train));
					ntime midtime=(ntime){timedecode1(di),timedecode2(di),_train.x[0],_train.x[1]};
					midtime=timecalc(midtime,_train.arrive[j]);
					int _mark=0,_p=0,_s=inf,_di=timeid(midtime.m,midtime.d),_gap=0;
					//find the route
					for(int l=1;l<=__train.n;++l){
						if(hash_calc(__train.s[l])==mhash){
							_mark=l;
							ntime _tmp=(ntime){0,0,__train.x[0],__train.x[1]};
							_tmp=timecalc(_tmp,__train.leave[l]);
							int _dil=timeid(__train.d[0]+_tmp.m,__train.d[1]+_tmp.d);
							int _dir=timeid(__train.d[2]+_tmp.m,__train.d[3]+_tmp.d);
							if(_di>_dir){
								_mark=0;
								break;
							}
							_di=std::max(_di,_dil)-_tmp.d;
							_tmp=(ntime){timedecode1(_di),timedecode2(_di),__train.x[0],__train.x[1]};
							_tmp=timecalc(_tmp,__train.leave[l]);
							_gap=_tmp-midtime;
							if(_gap<0)_gap+=1440,_di++;
							int _ld=timeid(__train.d[0],__train.d[1]),_rd=timeid(__train.d[2],__train.d[3]);
							if(_di<_ld||_di>_rd){
								_mark=0;
								break;
							}
						}
						else if(hash_calc(__train.s[l])==thash){
							if(!_mark)break;
							//s (i) -> m (_i) -> t
							int nowkey=0,_nowkey=0;
							if(q[0]=='c')nowkey=p+_p;
							else {
								nowkey+=_train.arrive[j]-_train.leave[mark];
								nowkey+=__train.arrive[l]-__train.leave[_mark];
								nowkey+=_gap;
							}
							_nowkey=_train.arrive[j]-_train.leave[mark];
							if(nowkey<key||(nowkey==key&&_nowkey<_key)){
								memcpy(sort_a[0].i,_train.i,sizeof(_train.i));
								memcpy(sort_a[1].i,__train.i,sizeof(__train.i));
								sort_a[0].p=p,sort_a[1].p=_p;
								sort_a[0].s=s,sort_a[1].s=_s;
								sort_a[0].st=_train.leave[mark];
								sort_a[0].tt=_train.arrive[j];
								sort_a[0].x[0]=_train.x[0],sort_a[0].x[1]=_train.x[1];
								sort_a[1].st=__train.leave[_mark];
								sort_a[1].tt=__train.arrive[l];
								sort_a[1].x[0]=__train.x[0],sort_a[1].x[1]=__train.x[1];
								memcpy(mids,_train.s[j],sizeof(_train.s[j]));
								key=nowkey;_key=_nowkey;
								gap=_gap;
							}
							break;
						}
						if(_mark)_p+=__train.p[l],_s=std::min(_s,__train.m[_di][l]);
					}
					if(!_mark)continue;
				}
			}
			if(mark)p+=_train.p[j],s=std::min(s,_train.m[di][j]);
		}
	}
	if(key==inf)puts("0");
	else {
		printf("%s %s ",sort_a[0].i,s);
		ntime tmp=(ntime){d[0],d[1],sort_a[0].x[0],sort_a[0].x[1]};
		tmp=timecalc(tmp,sort_a[0].st);
		tmp.m=d[0],tmp.d=d[1];
		tmp.output();
		printf(" -> %s ",mids);
		tmp=timecalc(tmp,sort_a[0].tt-sort_a[0].st);
		tmp.output(' ');
		printf("%d %d\n",sort_a[0].p,sort_a[0].s);
		printf("%s %s ",sort_a[1].i,mids);
		tmp=timecalc(tmp,gap);
		tmp.output();
		printf(" -> %s ",t);
		tmp=timecalc(tmp,sort_a[1].tt-sort_a[1].st);
		tmp.output(' ');
		printf("%d %d\n",sort_a[1].p,sort_a[1].s);
	}
}
long long buy_ticket(){
	char now[20],u[25],i[25],f[50],t[50],q[6];
	int d[2],n,fq=0;
	while(scanf("%s",now)!=EOF){
		if(now[0]!='-'){
			nextorder=1;
			memcpy(odr,now,sizeof(odr));
			break;
		}
		if(now[1]=='u')scanf("%s",u);
		else if(now[1]=='i')scanf("%s",i);
		else if(now[1]=='d')scanf("%d-%d",&d[0],&d[1]);
		else if(now[1]=='n')scanf("%d",&n);
		else if(now[1]=='f')scanf("%s",f);
		else if(now[1]=='t')scanf("%s",t);
		else scanf("%s",q),fq=1;
	}
	//not login in
	unsigned int uhash=hash_calc(u);
	if(hashtable1.query(uhash)==-1)return -1;
	unsigned int ihash=hash_calc(i),fhash=hash_calc(f),thash=hash_calc(t);
	if(hashtable2.query(ihash)==-1)return -1;
	int ipos=train.query(ihash);
	file_train.seekg(ipos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	if(n>_train.ms)return -1;
	int di=timeid(d[0],d[1]),mark=0,s=inf,p=0,si=-1,ti=-1;
	ntime tmp=(ntime){0,0,_train.x[0],_train.x[1]};
	for(int i=1;i<=_train.n;++i){
		unsigned int nowhash=hash_calc(_train.s[i]);
		if(nowhash==fhash){
			tmp=timecalc(tmp,_train.leave[i]);
			di-=tmp.d;
			mark=1;
			si=i;
			int ld=timeid(_train.d[0],_train.d[1]),rd=timeid(_train.d[2],_train.d[3]);
			if(di<ld||di>rd){
				mark=0;
				break;
			}
		} else if(nowhash==thash){
			ti=i;
			break;
		}
		if(mark)p+=_train.p[i],s=std::min(s,_train.m[di][i]);
	}
	if(!mark)return -1;
	if(s<n&&(q[0]=='f'||!fq))return -1;
	if(si==-1||ti==-1)return -1;
	if(s>=n){
		for(int i=si;i<ti;++i)_train.m[di][i]-=n;
		file_train.seekg(ipos,std::ios::beg);
		file_train.write(reinterpret_cast<char * >(&_train),sizeof(_train));
		int opos=order.query(uhash),precnt=0;
		if(opos!=-1){
			file_order.seekg(opos,std::ios::beg);
			file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
			precnt=_order.cnt;
		}
		memcpy(_order.i,_train.i,sizeof(_train.i));
		_order.pre=opos;_order.cnt=precnt+1;
		_order.sta=1;
		_order.di=di;_order.si=si;_order.ti=ti;
		_order.n=n;_order.p=p;
		memcpy(_order.sp,f,sizeof(f));
		memcpy(_order.tp,t,sizeof(t));
		_order.s=(ntime){d[0],d[1],tmp.h,tmp.n};
		_order.t=timecalc(_order.s,_train.arrive[ti]-_train.leave[si]);
		order.erase(uhash,opos);
		file_order.seekg(0,std::ios::end);
		opos=file_order.tellg();
		order.insert(uhash,opos);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
		return 1ll*p*n;
	} else {
		int opos=order.query(uhash),precnt=0;
		if(opos!=-1){
			file_order.seekg(opos,std::ios::beg);
			file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
			precnt=_order.cnt;
		}
		memcpy(_order.i,_train.i,sizeof(_train.i));
		_order.pre=opos;_order.cnt=precnt+1;
		_order.sta=2;
		_order.di=di;_order.si=si;_order.ti=ti;
		_order.n=n;_order.p=p;
		memcpy(_order.sp,f,sizeof(f));
		memcpy(_order.tp,t,sizeof(t));
		_order.s=(ntime){d[0],d[1],tmp.h,tmp.n};
		_order.t=timecalc(_order.s,_train.arrive[ti]-_train.leave[si]);
		order.erase(uhash,opos);
		file_order.seekg(0,std::ios::end);
		opos=file_order.tellg();
		order.insert(uhash,opos);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
		//queue
		int qpos=queue.query(ihash);
		_queue.pre=qpos;_queue.done=0;
		_queue.opos=opos;
		_queue.si=si,_queue.ti=ti;
		_queue.di=di;
		_queue.n=n;
		queue.erase(ihash,qpos);
		file_queue.seekg(0,std::ios::end);
		qpos=file_queue.tellg();
		queue.insert(ihash,qpos);
		file_queue.write(reinterpret_cast<char * >(&_queue),sizeof(_queue));
		return -2;
	}
}
int query_order(){
	char now[5],u[25];scanf("%s%s",now,u);
	unsigned int uhash=hash_calc(u);
	if(hashtable1.query(uhash)==-1)return -1;
	int cnt=0,opos=order.query(uhash);
	if(opos==-1){
		puts("0");
		return 0;
	}
	while(opos!=-1){
		file_order.seekg(opos,std::ios::beg);
		file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
		if(!cnt)printf("%d\n",cnt=_order.cnt);
		if(_order.sta==1)printf("[success] ");
		else if(_order.sta==2)printf("[pending] ");
		else printf("[refunded] ");
		printf("%s %s ",_order.i,_order.sp);
		_order.s.output();
		printf(" -> %s ",_order.tp);
		_order.t.output(' ');
		printf("%d %d\n",_order.p,_order.n);
		opos=_order.pre;
	}
	return 0;
}
int refund_ticket(){
	char now[20],u[25];int n,fn=0;
	while(scanf("%s",now)!=EOF){
		if(now[0]!='-'){
			nextorder=1;
			memcpy(odr,now,sizeof(odr));
			break;
		}
		if(now[1]=='u')scanf("%s",u);
		else scanf("%d",&n),fn=1;
	}
	if(!fn)n=1;
	unsigned int uhash=hash_calc(u);
	//not login
	if(hashtable1.query(uhash)==-1)return -1;
	//refund
	int opos=order.query(uhash);
	while(opos!=-1){
		file_order.seekg(opos,std::ios::beg);
		file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
		n--;
		if(n<=0)break;
		opos=_order.pre;
	}
	if(n)return -1;
	if(_order.sta==3)return -1;
	if(_order.sta==2){
		_order.sta=3;
		file_order.seekg(opos,std::ios::beg);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
		unsigned int ihash=hash_calc(_order.i);
		int qpos=queue.query(ihash);
		while(qpos!=-1){
			file_queue.seekg(qpos,std::ios::beg);
			file_queue.read(reinterpret_cast<char * >(&_queue),sizeof(_queue));
			if(_queue.done){
				qpos=_queue.pre;
				continue;
			}
			if(_queue.opos==opos){
				_queue.done=1;
				file_queue.seekg(qpos,std::ios::beg);
				file_queue.write(reinterpret_cast<char * >(&_queue),sizeof(_queue));
				break;
			}
			qpos=_queue.pre;
		}
		return 0;
	}
	_order.sta=3;
	file_order.seekg(opos,std::ios::beg);
	file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
	unsigned int ihash=hash_calc(_order.i);
	int ipos=train.query(ihash);
	file_train.seekg(ipos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	for(int i=_order.si;i<_order.ti;++i)
		_train.m[_order.di][i]+=_order.n;
	file_train.seekg(ipos,std::ios::beg);
	file_train.write(reinterpret_cast<char * >(&_train),sizeof(_train));
	//queue
	while(true){
		int qpos=queue.query(ihash),mark=0,_qpos;
		if(qpos==-1)return 0;
		while(qpos!=-1){
			file_queue.seekg(qpos,std::ios::beg);
			file_queue.read(reinterpret_cast<char * >(&_queue),sizeof(_queue));
			if(_queue.done){
				qpos=_queue.pre;
				continue;
			}
			int s=inf;
			for(int i=_queue.si;i<_queue.ti;++i)s=std::min(s,_train.m[_queue.di][i]);
			if(s>=_queue.n){
				mark=1,__queue=_queue,_qpos=qpos;
			}
			qpos=_queue.pre;
		}
		if(!mark)return 0;
		for(int i=__queue.si;i<__queue.ti;++i)
			_train.m[__queue.di][i]-=__queue.n;
		__queue.done=1;
		file_queue.seekg(_qpos,std::ios::beg);
		file_queue.write(reinterpret_cast<char * >(&__queue),sizeof(__queue));
		file_train.seekg(ipos,std::ios::beg);
		file_train.write(reinterpret_cast<char * >(&_train),sizeof(_train));
		file_order.seekg(__queue.opos,std::ios::beg);
		file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
		_order.sta=1;
		file_order.seekg(__queue.opos,std::ios::beg);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
	}
	return 0;
}

//clean
void clean(){
	hashtable1.clear();hashtable2.clear();
	user.clear();train.clear();station.clear();
	order.clear();queue.clear();
	first_user.clear();
	file_user.close();
	file_user.open("user",std::fstream::out|std::ios::trunc);
	file_user.close();
	file_user.open("user",std::ios::in|std::ios::out|std::ios::binary);
	file_train.close();
	file_train.open("train",std::fstream::out|std::ios::trunc);
	file_train.close();
	file_train.open("train",std::ios::in|std::ios::out|std::ios::binary);
	file_station.close();
	file_station.open("station",std::fstream::out|std::ios::trunc);
	file_station.close();
	file_station.open("station",std::ios::in|std::ios::out|std::ios::binary);
	file_order.close();
	file_order.open("order",std::fstream::out|std::ios::trunc);
	file_order.close();
	file_order.open("order",std::ios::in|std::ios::out|std::ios::binary);
	file_queue.close();
	file_queue.open("queue",std::fstream::out|std::ios::trunc);
	file_queue.close();
	file_queue.open("queue",std::ios::in|std::ios::out|std::ios::binary);
}

//main
int main(){
	create_file();
	while(true){
		if(!nextorder){
			if(scanf("%s",odr)==EOF){
				break;
			}
		}
		nextorder=0;
		if(odr[0]=='e'){   //exit
			hashtable1.clear();
			puts("bye");
			break;
		} else if(odr[0]=='a'){   //add_
			if(odr[4]=='u'){   //add_user
				printf("%d\n",add_user());
			} else {   //add_train
				printf("%d\n",add_train());
			}
		} else if(odr[0]=='l'){   //log
			if(odr[3]=='i'){   //login
				printf("%d\n",login());
			} else {    //logout
				printf("%d\n",logout());
			}
		} else if(odr[0]=='q'){   //query_
			if(odr[6]=='p'){   //query_profile
				int x=query_profile();
				if(x==-1)puts("-1");
				else printf("%s %s %s %d\n",_user.u,_user.n,_user.m,_user.g);
			} else if(odr[9]=='i'){   //query_train
				int x=query_train();
				if(x==-1)puts("-1");
			} else if(odr[7]=='i'){   //query_ticket
				query_ticket();
			} else if(odr[9]=='n'){   //query_transfer
				query_transfer();
			} else {   //query_order
				int x=query_order();
				if(x==-1)puts("-1");
			}
		} else if(odr[0]=='m'){    //modify_profile
			int x=modify_profile();
			if(x==-1)puts("-1");
			else printf("%s %s %s %d\n",_user.u,_user.n,_user.m,_user.g);
		} else if(odr[0]=='r'){
			if(odr[2]=='l'){   //release_train
				printf("%d\n",release_train());
			} else {   //refund_ticket
				printf("%d\n",refund_ticket());
			}
		} else if(odr[0]=='d'){   //delete_train
			printf("%d\n",delete_train());
		} else if(odr[0]=='b'){   //buy_ticket
			long long x=buy_ticket();
			if(x==-1)puts("-1");
			else if(x==-2)puts("queue");
			else printf("%lld\n",x);
		} else {   //clean
			clean();
			puts("0");
		}
	}
	file_user.close();
	file_train.close();
	file_station.close();
	file_order.close();
	file_queue.close();
	return 0;
}
