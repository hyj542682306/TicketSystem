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
BPlus_Tree users("_user",2000000);
BPlus_Tree trains("_train",6000000);
BPlus_Tree orders("_order",2000000);
BPlus_Tree queues("_queue",2000000);
BPlus_Tree first_user("_first",100);
BPlus_Tree dqueues("_dqueue",2000000);
BPlus_Tree stations("_station",6000000);
BPlus_Tree hashtable1("_login",10000);
BPlus_Tree hashtable2("_release",10000);

std::fstream file_user("user",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_train("train",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_station("station",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_order("order",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_queue("queue",std::ios::in|std::ios::out|std::ios::binary);
std::fstream file_seat("seat",std::ios::in|std::ios::out|std::ios::binary);
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
	if(!file_seat.is_open()){
		file_seat.open("seat",std::ios::out|std::ios::binary);
		file_seat.close();
		file_seat.open("seat",std::ios::in|std::ios::out|std::ios::binary);
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
	for(int i=0;i<len;++i)res=res*297+s[i];
	return res;
}

//sort
struct sort_node{
	int key,st,tt,p,s,x[2];
	char i[25];
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
void sort(int l,int r,unsigned int *a,short *b){
	if(l>=r)return ;
	unsigned int i=l,j=r,base=a[l],tmp;
	short _base=b[l],_tmp;
	while(i<j){
		while(a[j]>=base&&i<j)j--;
		while(a[i]<=base&&i<j)i++;
		if(i<j){
			std::swap(a[i],a[j]);
			std::swap(b[i],b[j]);
		}
	}
	a[l]=a[i];a[i]=base;
	b[l]=b[i];b[i]=_base;
	sort(l,i-1,a,b),sort(i+1,r,a,b);
}
int find_pos(int l,int r,unsigned int *a,unsigned int x){
	int ans=-1;
	while(l<=r){
		int mid=(l+r)>>1;
		if(a[mid]>=x)ans=mid,r=mid-1;
		else l=mid+1;
	}
	return a[ans]==x?ans:-1;
}

//user
class user{
private:
	char username[25],password[35],name[25],mailAddr[35];
	int privilege;
public:
	user(){}
	user(const char *_username,const char *_password,const char *_name,const char *_mailAddr,int p) {
		strcpy(username,_username);
		strcpy(password,_password);
		strcpy(name,_name);
		strcpy(mailAddr,_mailAddr);
		privilege=p;
	}
	int Get_privilege(){
		return privilege;
	}
	bool Match_password(const char *s){
		return string_same(password,s);
	}
	void Update_password(const char *s){
		strcpy(password,s);
	}
	void Update_name(const char *s){
		strcpy(name,s);
	}
	void Update_mailAddr(const char *s){
		strcpy(mailAddr,s);
	}
	void Update_privilege(int p){
		privilege=p;
	}
	void output(){
		printf("%s %s %s %d\n",username,name,mailAddr,privilege);
	}
};
int add_user(){
	char now[5],u[25],p[35],n[25],m[35],c[25];
	int g;
	for(int i=1;i<=6;++i){
		scanf("%s",now);
		if(now[1]=='c')scanf("%s",c);
		else if(now[1]=='u')scanf("%s",u);
		else if(now[1]=='p')scanf("%s",p);
		else if(now[1]=='n')scanf("%s",n);
		else if(now[1]=='m')scanf("%s",m);
		else scanf("%d",&g);
	}
	//the first user
	if(first_user.query(0)==-1){
		g=10;
		users.insert(hash_calc(u),0);
		user _user(u,p,n,m,g);
		file_user.seekg(0,std::ios::beg);
		file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
		first_user.insert(0,1);
		return 0;
	}
	//login
	unsigned int chash=hash_calc(c),uhash=hash_calc(u);
	if(hashtable1.query(chash)==-1)return -1;
	//privilege
	int curpos=users.query(chash);
	file_user.seekg(curpos,std::ios::beg);
	user ucur;
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	if(ucur.Get_privilege()<=g)return -1;
	//id conflict
	if(users.query(uhash)!=-1)return -1;
	//legal
	file_user.seekg(0,std::ios::end);
	int nowpos=file_user.tellg();
	users.insert(uhash,nowpos);
	user _user(u,p,n,m,g);
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
	int pos=users.query(uhash);
	//username error
	if(pos==-1)return -1;
	//have logged in
	if(hashtable1.query(uhash)==1)return -1;
	file_user.seekg(pos,std::ios::beg);
	user ucur;
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	//password error
	if(ucur.Match_password(p)){
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
	int cpos=users.query(chash),upos=users.query(uhash);
	//query itself
	if(chash==uhash){
		file_user.seekg(upos,std::ios::beg);
		user _user;
		file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
		_user.output();
		return 0;
	}
	//username error
	if(upos==-1)return -1;
	//legal
	user ucur,_user;
	file_user.seekg(cpos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	file_user.seekg(upos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
	if(ucur.Get_privilege()>_user.Get_privilege()){
		_user.output();
		return 0;
	} else return -1;
}
int modify_profile(){
	char now[20],c[25],u[25],p[35],n[25],m[35];
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
	int cpos=users.query(chash),upos=users.query(uhash);
	if(chash==uhash){
		file_user.seekg(upos,std::ios::beg);
		user _user;
		file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
		if(fg&&_user.Get_privilege()<=g)return -1;
		if(fp)_user.Update_password(p);
		if(fn)_user.Update_name(n);
		if(fm)_user.Update_mailAddr(m);
		if(fg)_user.Update_privilege(g);
		file_user.seekg(upos,std::ios::beg);
		file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
		_user.output();
		return 0;
	}
	//username error
	if(upos==-1)return -1;
	user ucur,_user;
	file_user.seekg(cpos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&ucur),sizeof(ucur));
	file_user.seekg(upos,std::ios::beg);
	file_user.read(reinterpret_cast<char * >(&_user),sizeof(_user));
	//privilege error
	int cg=ucur.Get_privilege(),ug=_user.Get_privilege();
	if(cg<=ug||(fg&&cg<=g))return -1;
	if(fp)_user.Update_password(p);
	if(fn)_user.Update_name(n);
	if(fm)_user.Update_mailAddr(m);
	if(fg)_user.Update_privilege(g);
	file_user.seekg(upos,std::ios::beg);
	file_user.write(reinterpret_cast<char * >(&_user),sizeof(_user));
	_user.output();
	return 0;
}

//time
int months[13]={inf,31,28,31,30,31,30,31,31,30,31,30,31};
int minsum[13]={inf,0,44640,84960,129600,172800,217440,260640,305280,349920,393120,437760,480960};
struct ntime{
	int month,day,hour,minute;
	void output(char c='\0'){
		if(month<10)printf("0");
		printf("%d-",month);
		if(day<10)printf("0");
		printf("%d ",day);
		if(hour<10)printf("0");
		printf("%d:",hour);
		if(minute<10)printf("0");
		printf("%d",minute);
		if(c!='\0')putchar(c);
	}
	bool operator < (const ntime &A) const {
		return month<A.month||(month==A.month&&day<A.day)||
			(month==A.month&&day==A.day&&hour<A.hour)||
			(month==A.month&&day==A.day&&hour==A.hour&&minute<=A.minute);
	}
	int operator - (const ntime &A) const {
		return minsum[month]+day*24*60+hour*60+minute-
			   minsum[A.month]-A.day*24*60-A.hour*60-A.minute;
	}
};
ntime timecalc(ntime p,int t){
	int month=p.month,day=p.day,hour=p.hour,minute=p.minute;
	minute+=t;
	hour+=minute/60;minute%=60;
	day+=hour/24;hour%=24;
	while(day>months[month]){
		day-=months[month],month++;
		if(month>12)month-=12;
	}
	return (ntime){month,day,hour,minute};
}
int timeid(int m,int d){
	if(m==6)return d;
	else if(m==7)return d+months[6];
	else if(m==8)return d+months[6]+months[7];
	else if(m==9)return d+months[6]+months[7]+months[8];
	else return d+months[6]+months[7]+months[8]+months[9];
}
int Decode_month(int d){
	int res=6;
	while(d>months[res]){
		d-=months[res],res++;
		if(res>12)res-=12;
	}
	return res;
}
int Decode_day(int d){
	int res=6;
	while(d>months[res]){
		d-=months[res],res++;
		if(res>12)res-=12;
	}
	return d;
}

//train
struct seat{
	int day_seat[105];
	void fill(int n,int x){
		for(int i=1;i<n;++i)day_seat[i]=x;
	}
}_seat,__seat;
struct train{
	char trainID[25],stations[105][50],type;
	int stationNum,seatNum;
	int seatPos[100],stopoverTimes[105],prices[105],startTime[2],travelTimes[105],saleDate[4];
	int arriveTime[105],leaveTime[105];
	unsigned int stations_hash[105];
	short original_pos[105];
}_train,__train;
struct station{
	int trainNum;
	unsigned int trainHash[10005];
	short stationOrder[10005];
}_station,__station;
struct order{
	int prePos;
	int status,seatNum,date;
	int startOrder,endOrder,cnt,price;
	ntime startTime,endTime;
	char trainID[25],startStation[50],endStation[50];
}_order;
struct queue{
	int nxtPos;
	int orderPos,seatNum,date,startOrder,endOrder;
	void operator = (const queue &A) {
		orderPos=A.orderPos;
		seatNum=A.seatNum;
		date=A.date;
		startOrder=A.startOrder;
		endOrder=A.endOrder;
		nxtPos=A.nxtPos;
	}
}_queue,__queue;
int add_train(){
	//read the information
	char now[5];
	for(int i=1;i<=10;++i){
		scanf("%s",now);
		if(now[1]=='i')scanf("%s",_train.trainID);
		else if(now[1]=='n')scanf("%d",&_train.stationNum);
		else if(now[1]=='m')scanf("%d",&_train.seatNum);
		else if(now[1]=='s'){
			char stations[6005];scanf("%s",stations);
			int cnt=0,len=strlen(stations);
			for(int j=0;j<len;++j){
				++cnt;int pos=0;
				while(stations[j]!='|'&&j<len)_train.stations[cnt][pos++]=stations[j],++j;
				_train.stations[cnt][pos]='\0';
			}
		}
		else if(now[1]=='p'){
			char prices[1005];scanf("%s",prices);
			int cnt=0,len=strlen(prices);
			for(int j=0;j<len;++j){
				++cnt;int tmp=0;
				while(prices[j]!='|'&&j<len)tmp=tmp*10+prices[j]-'0',++j;
				_train.prices[cnt]=tmp;
			}
		}
		else if(now[1]=='x')scanf("%d:%d",&_train.startTime[0],&_train.startTime[1]);
		else if(now[1]=='t'){
			char times[1005];scanf("%s",times);
			int cnt=0,len=strlen(times);
			for(int j=0;j<len;++j){
				++cnt;int tmp=0;
				while(times[j]!='|'&&j<len)tmp=tmp*10+times[j]-'0',++j;
				_train.travelTimes[cnt]=tmp;
			}
		}
		else if(now[1]=='o'){
			char stops[1005];scanf("%s",stops);
			if(stops[0]!='_'){
				int cnt=0,len=strlen(stops);
				for(int j=0;j<len;++j){
					++cnt;int tmp=0;
					while(stops[j]!='|'&&j<len)tmp=tmp*10+stops[j]-'0',++j;
					_train.stopoverTimes[cnt]=tmp;
				}
			}
		}
		else if(now[1]=='d')scanf("%d-%d|%d-%d",&_train.saleDate[0],&_train.saleDate[1],&_train.saleDate[2],&_train.saleDate[3]);
		else std::cin>>_train.type;
	}
	//illegal
	unsigned int ihash=hash_calc(_train.trainID);
	if(trains.query(ihash)!=-1)return -1;
 	//process the information
	_train.leaveTime[1]=0;
	for(int i=2;i<=_train.stationNum;++i){
		_train.arriveTime[i]=_train.leaveTime[i-1]+_train.travelTimes[i-1];
		if(i!=_train.stationNum)_train.leaveTime[i]=_train.arriveTime[i]+_train.stopoverTimes[i-1];
	}
	//write train
	file_train.seekg(0,std::ios::end);
	int nowpos=file_train.tellg();
	trains.insert(ihash,nowpos);
	file_train.write(reinterpret_cast<char * >(&_train),sizeof(_train));
	return 0;
}
int release_train(){
	char now[5],i[25];
	scanf("%s%s",now,i);
	unsigned int ihash=hash_calc(i);
	//not exist
	if(trains.query(ihash)==-1)return -1;
	//have been released
	if(hashtable2.query(ihash)==1)return -1;
	//legal
	hashtable2.insert(ihash,1);
	int ipos=trains.query(ihash);
	file_train.seekg(ipos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	//write itself
	for(int i=1;i<=_train.stationNum;++i){
		_train.stations_hash[i]=hash_calc(_train.stations[i]);
		_train.original_pos[i]=i;
	}
	sort(1,_train.stationNum,_train.stations_hash,_train.original_pos);
	//write seat
	int dl=timeid(_train.saleDate[0],_train.saleDate[1]);
	int dr=timeid(_train.saleDate[2],_train.saleDate[3]);
	for(int i=dl;i<=dr;++i){
		file_seat.seekg(0,std::ios::end);
		_train.seatPos[i]=file_seat.tellg();
		_seat.fill(_train.stationNum,_train.seatNum);
		file_seat.write(reinterpret_cast<char * >(&_seat),sizeof(_seat));
	}
	file_train.seekg(ipos,std::ios::beg);
	file_train.write(reinterpret_cast<char *>(&_train),sizeof(_train));
	//write station
	for(int i=1;i<=_train.stationNum;++i){
		unsigned int shash=hash_calc(_train.stations[i]);
		int spos=stations.query(shash);
		if(spos==-1){
			file_station.seekg(0,std::ios::end);
			stations.insert(shash,file_station.tellg());
			_station.trainNum=1;
			_station.trainHash[_station.trainNum]=ihash;
			_station.stationOrder[_station.trainNum]=i;
			file_station.write(reinterpret_cast<char * >(&_station),sizeof(_station));
		} else {
			file_station.seekg(spos,std::ios::beg);
			file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
			_station.trainNum++;
			_station.trainHash[_station.trainNum]=ihash;
			_station.stationOrder[_station.trainNum]=i;
			file_station.seekg(spos,std::ios::beg);
			file_station.write(reinterpret_cast<char * >(&_station),sizeof(_station));
		}
	}
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
	int nowpos=trains.query(ihash),di=timeid(d[0],d[1]);
	//not exist
	if(nowpos==-1)return -1;
	file_train.seekg(nowpos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	int ld=timeid(_train.saleDate[0],_train.saleDate[1]);
	int rd=timeid(_train.saleDate[2],_train.saleDate[3]);
	if(di<ld||di>rd)return -1;
	if(hashtable2.query(ihash)==-1)_seat.fill(_train.stationNum,_train.seatNum);
	else {
		file_seat.seekg(_train.seatPos[di],std::ios::beg);
		file_seat.read(reinterpret_cast<char * >(&_seat),sizeof(_seat));
	}
	printf("%s %c\n",_train.trainID,_train.type);
	ntime et=(ntime){d[0],d[1],_train.startTime[0],_train.startTime[1]};
	int psum=0;
	for(int i=1;i<=_train.stationNum;++i){
		printf("%s ",_train.stations[i]);
		if(i==1)printf("xx-xx xx:xx");
		else timecalc(et,_train.arriveTime[i]).output();
		printf(" -> ");
		if(i==_train.stationNum)printf("xx-xx xx:xx ");
		else timecalc(et,_train.leaveTime[i]).output(' ');
		if(i!=1)psum+=_train.prices[i-1];
		printf("%d ",psum);
		if(i==_train.stationNum)printf("x\n");
		else printf("%d\n",_seat.day_seat[i]);
	}
	return 0;
}
int delete_train(){
	char now[5],_i[25];
	scanf("%s%s",now,_i);
	unsigned int ihash=hash_calc(_i);
	int nowpos=trains.query(ihash);
	//not exist
	if(nowpos==-1)return -1;
	//have been released
	if(hashtable2.query(ihash)==1)return -1;
	//delete train
	trains.erase(ihash,nowpos);
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
	int spos=stations.query(shash),tpos=stations.query(thash);
	if(spos==-1||tpos==-1){
		puts("0");
		return ;
	}
	file_station.seekg(spos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
	file_station.seekg(tpos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&__station),sizeof(__station));
	sort(1,_station.trainNum,_station.trainHash,_station.stationOrder);
	sort(1,__station.trainNum,__station.trainHash,__station.stationOrder);
	int i=1,j=1;
	while(i<=_station.trainNum&&j<=__station.trainNum){
		if(_station.trainHash[i]==__station.trainHash[j]){
			int ipos=trains.query(_station.trainHash[i]);
			file_train.seekg(ipos,std::ios::beg);
			file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
			int st,tt,p=0,seat=inf,mark=0,di=timeid(d[0],d[1]);
			for(int k=_station.stationOrder[i];k<=__station.stationOrder[j];++k){
				if(k==_station.stationOrder[i]){
					mark=1;
					st=_train.leaveTime[k];
					ntime tmp=(ntime){0,0,_train.startTime[0],_train.startTime[1]};
					tmp=timecalc(tmp,st);
					di-=tmp.day;
					int ld=timeid(_train.saleDate[0],_train.saleDate[1]);
					int rd=timeid(_train.saleDate[2],_train.saleDate[3]);
					if(di<ld||di>rd){
						mark=0;
						break;
					}
					file_seat.seekg(_train.seatPos[di],std::ios::beg);
					file_seat.read(reinterpret_cast<char * >(&_seat),sizeof(_seat));
				} else if(k==__station.stationOrder[j]){
					tt=_train.arriveTime[k];
					break;
				}
				if(mark)p+=_train.prices[k],seat=std::min(seat,_seat.day_seat[k]);
			}
			++i,++j;
			if(!mark)continue;
			++cnt;
			memcpy(sort_a[cnt].i,_train.trainID,sizeof(_train.trainID));
			sort_a[cnt].p=p;
			sort_a[cnt].s=seat;
			sort_a[cnt].st=st,sort_a[cnt].tt=tt;
			sort_a[cnt].x[0]=_train.startTime[0],sort_a[cnt].x[1]=_train.startTime[1];
			if(q[0]=='t'||!fq)sort_a[cnt].key=tt-st;
			else sort_a[cnt].key=p;
		} else {
			if(_station.trainHash[i]<__station.trainHash[j])++i;
			else ++j;
		}
	}
	sort(1,cnt,sort_a);
	ntime tmp;
	printf("%d\n",cnt);
	for(i=1;i<=cnt;++i){
		printf("%s %s ",sort_a[i].i,s);
		tmp=(ntime){d[0],d[1],sort_a[i].x[0],sort_a[i].x[1]};
		tmp=timecalc(tmp,sort_a[i].st);
		tmp.month=d[0],tmp.day=d[1];
		tmp.output();
		printf(" -> %s ",t);
		timecalc(tmp,sort_a[i].tt-sort_a[i].st).output(' ');
		printf("%d %d\n",sort_a[i].p,sort_a[i].s);
	}
}
void query_transfer(){
	char now[20],s[50],t[50],q[6];
	int d[2];
	while(scanf("%s",now)!=EOF){
		if(now[0]!='-'){
			nextorder=1;
			memcpy(odr,now,sizeof(odr));
			break;
		}
		if(now[1]=='s')scanf("%s",s);
		else if(now[1]=='t')scanf("%s",t);
		else if(now[1]=='d')scanf("%d-%d",&d[0],&d[1]);
		else scanf("%s",q);
	}
	if(d[0]<6){
		puts("0");
		return ;
	}
	int key=inf,_key=inf,gap=0;
	char mids[50];
	unsigned int shash=hash_calc(s),thash=hash_calc(t),mhash,ihash,_ihash;
	int spos=stations.query(shash),tpos=stations.query(thash),mpos,ipos,_ipos;
	if(spos==-1||tpos==-1){
		puts("0");
		return ;
	}
	file_station.seekg(spos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&_station),sizeof(_station));
	file_station.seekg(tpos,std::ios::beg);
	file_station.read(reinterpret_cast<char * >(&__station),sizeof(__station));
	//enumerate the train passing by the station S
	for(int i=1;i<=_station.trainNum;++i){
		ihash=_station.trainHash[i];
		ipos=trains.query(ihash);
		file_train.seekg(ipos,std::ios::beg);
		file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
		int mark=0,p=0,s=inf,di=timeid(d[0],d[1]);
		//enumerate the mid station M
		for(int j=_station.stationOrder[i];j<=_train.stationNum;++j){
			unsigned int nowhash=hash_calc(_train.stations[j]);
			if(nowhash==shash){
				ntime tmp=(ntime){0,0,_train.startTime[0],_train.startTime[1]};
				tmp=timecalc(tmp,_train.leaveTime[j]);
				di-=tmp.day;
				mark=j;
				int ld=timeid(_train.saleDate[0],_train.saleDate[1]);
				int rd=timeid(_train.saleDate[2],_train.saleDate[3]);
				if(di<ld||di>rd)break;
				file_seat.seekg(_train.seatPos[di],std::ios::beg);
				file_seat.read(reinterpret_cast<char * >(&_seat),sizeof(_seat));
			} else if(mark){
				if(nowhash==thash){
					p+=_train.prices[j],s=std::min(s,_seat.day_seat[j]);
					continue;
				}
				mhash=nowhash;
				mpos=stations.query(mhash);
				for(int k=1;k<=__station.trainNum;++k){
					_ihash=__station.trainHash[k];
					if(_ihash==ihash)continue;
					_ipos=trains.query(_ihash);
					file_train.seekg(_ipos,std::ios::beg);
					file_train.read(reinterpret_cast<char * >(&__train),sizeof(__train));
					ntime midtime=(ntime){Decode_month(di),Decode_day(di),_train.startTime[0],_train.startTime[1]};
					midtime=timecalc(midtime,_train.arriveTime[j]);
					int ti=__station.stationOrder[k],si=find_pos(1,__train.stationNum,__train.stations_hash,mhash);
					if(si==-1)continue;
					si=__train.original_pos[si];
					int _mark=0,_p=0,_s=inf,_di=timeid(midtime.month,midtime.day),_gap=0;
					for(int l=si;l<=ti;++l){
						if(l==si){
							_mark=l;
							ntime _tmp=(ntime){0,0,__train.startTime[0],__train.startTime[1]};
							_tmp=timecalc(_tmp,__train.leaveTime[l]);
							int _dil=timeid(__train.saleDate[0]+_tmp.month,__train.saleDate[1]+_tmp.day);
							int _dir=timeid(__train.saleDate[2]+_tmp.month,__train.saleDate[3]+_tmp.day);
							if(_di>_dir){
								_mark=0;
								break;
							}
							_di=std::max(_di,_dil)-_tmp.day;
							_tmp=(ntime){Decode_month(_di),Decode_day(_di),__train.startTime[0],__train.startTime[1]};
							_tmp=timecalc(_tmp,__train.leaveTime[l]);
							_gap=_tmp-midtime;
							if(_gap<0)_gap+=1440,_di++;
							int _ld=timeid(__train.saleDate[0],__train.saleDate[1]);
							int _rd=timeid(__train.saleDate[2],__train.saleDate[3]);
							if(_di<_ld||_di>_rd){
								_mark=0;
								break;
							}
							file_seat.seekg(__train.seatPos[_di],std::ios::beg);
							file_seat.read(reinterpret_cast<char * >(&__seat),sizeof(__seat));
						}
						else if(l==ti){
							if(!_mark)break;
							//s (i) -> m (_i) -> t
							int nowkey=0,_nowkey=0;
							if(q[0]=='c')nowkey=p+_p;
							else {
								nowkey+=_train.arriveTime[j]-_train.leaveTime[mark];
								nowkey+=__train.arriveTime[l]-__train.leaveTime[_mark];
								nowkey+=_gap;
							}
							_nowkey=_train.arriveTime[j]-_train.leaveTime[mark];
							if(nowkey<key||(nowkey==key&&_nowkey<_key)){
								memcpy(sort_a[0].i,_train.trainID,sizeof(_train.trainID));
								memcpy(sort_a[1].i,__train.trainID,sizeof(__train.trainID));
								sort_a[0].p=p,sort_a[1].p=_p;
								sort_a[0].s=s,sort_a[1].s=_s;
								sort_a[0].st=_train.leaveTime[mark];
								sort_a[0].tt=_train.arriveTime[j];
								sort_a[0].x[0]=_train.startTime[0],sort_a[0].x[1]=_train.startTime[1];
								sort_a[1].st=__train.leaveTime[_mark];
								sort_a[1].tt=__train.arriveTime[l];
								sort_a[1].x[0]=__train.startTime[0],sort_a[1].x[1]=__train.startTime[1];
								memcpy(mids,_train.stations[j],sizeof(_train.stations[j]));
								key=nowkey;_key=_nowkey;
								gap=_gap;
							}
							break;
						}
						if(_mark)_p+=__train.prices[l],_s=std::min(_s,__seat.day_seat[l]);
					}
					if(!_mark)continue;
				}
			}
			if(mark)p+=_train.prices[j],s=std::min(s,_seat.day_seat[j]);
		}
	}
	if(key==inf)puts("0");
	else {
		printf("%s %s ",sort_a[0].i,s);
		ntime tmp=(ntime){d[0],d[1],sort_a[0].x[0],sort_a[0].x[1]};
		tmp=timecalc(tmp,sort_a[0].st);
		tmp.month=d[0],tmp.day=d[1];
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
	int ipos=trains.query(ihash);
	file_train.seekg(ipos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	if(n>_train.seatNum)return -1;
	int di=timeid(d[0],d[1]),mark=0,s=inf,p=0;
	int si=find_pos(1,_train.stationNum,_train.stations_hash,fhash);
	int ti=find_pos(1,_train.stationNum,_train.stations_hash,thash);
	if(si==-1||ti==-1)return -1;
	si=_train.original_pos[si],ti=_train.original_pos[ti];
	ntime tmp=(ntime){0,0,_train.startTime[0],_train.startTime[1]};
	for(int i=si;i<ti;++i){
		if(i==si){
			tmp=timecalc(tmp,_train.leaveTime[i]);
			di-=tmp.day;
			mark=1;
			si=i;
			int ld=timeid(_train.saleDate[0],_train.saleDate[1]);
			int rd=timeid(_train.saleDate[2],_train.saleDate[3]);
			if(di<ld||di>rd){
				mark=0;
				break;
			}
			file_seat.seekg(_train.seatPos[di],std::ios::beg);
			file_seat.read(reinterpret_cast<char * >(&_seat),sizeof(_seat));
		}
		p+=_train.prices[i],s=std::min(s,_seat.day_seat[i]);
	}
	if(!mark)return -1;
	if(s<n&&(q[0]=='f'||!fq))return -1;
	if(si==-1||ti==-1)return -1;
	if(s>=n){
		for(int i=si;i<ti;++i)_seat.day_seat[i]-=n;
		file_seat.seekg(_train.seatPos[di],std::ios::beg);
		file_seat.write(reinterpret_cast<char * >(&_seat),sizeof(_seat));
		int opos=orders.query(uhash),precnt=0;
		if(opos!=-1){
			file_order.seekg(opos,std::ios::beg);
			file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
			precnt=_order.cnt;
		}
		memcpy(_order.trainID,_train.trainID,sizeof(_train.trainID));
		_order.prePos=opos;_order.cnt=precnt+1;
		_order.status=1;
		_order.date=di;
		_order.startOrder=si;
		_order.endOrder=ti;
		_order.seatNum=n;
		_order.price=p;
		memcpy(_order.startStation,f,sizeof(f));
		memcpy(_order.endStation,t,sizeof(t));
		_order.startTime=(ntime){d[0],d[1],tmp.hour,tmp.minute};
		_order.endTime=timecalc(_order.startTime,_train.arriveTime[ti]-_train.leaveTime[si]);
		orders.erase(uhash,opos);
		file_order.seekg(0,std::ios::end);
		opos=file_order.tellg();
		orders.insert(uhash,opos);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
		return 1ll*p*n;
	} else {
		int opos=orders.query(uhash),precnt=0;
		if(opos!=-1){
			file_order.seekg(opos,std::ios::beg);
			file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
			precnt=_order.cnt;
		}
		memcpy(_order.trainID,_train.trainID,sizeof(_train.trainID));
		_order.prePos=opos;_order.cnt=precnt+1;
		_order.status=2;
		_order.date=di;
		_order.startOrder=si;
		_order.endOrder=ti;
		_order.seatNum=n;
		_order.price=p;
		memcpy(_order.startStation,f,sizeof(f));
		memcpy(_order.endStation,t,sizeof(t));
		_order.startTime=(ntime){d[0],d[1],tmp.hour,tmp.minute};
		_order.endTime=timecalc(_order.startTime,_train.arriveTime[ti]-_train.leaveTime[si]);
		orders.erase(uhash,opos);
		file_order.seekg(0,std::ios::end);
		opos=file_order.tellg();
		orders.insert(uhash,opos);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
		//queue
		int qpos=dqueues.query(ihash);
		if(qpos==-1){
			file_queue.seekg(0,std::ios::end);
			queues.insert(ihash,file_queue.tellg());
		} else {
			dqueues.erase(ihash,qpos);
			file_queue.seekg(0,std::ios::end);
			int _qpos=file_queue.tellg();
			file_queue.seekg(qpos,std::ios::beg);
			file_queue.read(reinterpret_cast<char * >(&_queue),sizeof(_queue));
			_queue.nxtPos=_qpos;
			file_queue.seekg(qpos,std::ios::beg);
			file_queue.write(reinterpret_cast<char * >(&_queue),sizeof(_queue));
		}
		_queue.nxtPos=-1;
		_queue.orderPos=opos;
		_queue.startOrder=si;
		_queue.endOrder=ti;
		_queue.date=di;
		_queue.seatNum=n;
		file_queue.seekg(0,std::ios::end);
		dqueues.insert(ihash,file_queue.tellg());
		file_queue.write(reinterpret_cast<char * >(&_queue),sizeof(_queue));
		return -2;
	}
}
int query_order(){
	char now[5],u[25];scanf("%s%s",now,u);
	unsigned int uhash=hash_calc(u);
	if(hashtable1.query(uhash)==-1)return -1;
	int cnt=0,opos=orders.query(uhash);
	if(opos==-1){
		puts("0");
		return 0;
	}
	while(opos!=-1){
		file_order.seekg(opos,std::ios::beg);
		file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
		if(!cnt)printf("%d\n",cnt=_order.cnt);
		if(_order.status==1)printf("[success] ");
		else if(_order.status==2)printf("[pending] ");
		else printf("[refunded] ");
		printf("%s %s ",_order.trainID,_order.startStation);
		_order.startTime.output();
		printf(" -> %s ",_order.endStation);
		_order.endTime.output(' ');
		printf("%d %d\n",_order.price,_order.seatNum);
		opos=_order.prePos;
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
	int opos=orders.query(uhash);
	while(opos!=-1){
		file_order.seekg(opos,std::ios::beg);
		file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
		n--;
		if(n<=0)break;
		opos=_order.prePos;
	}
	if(n)return -1;
	if(_order.status==3)return -1;
	if(_order.status==2){
		_order.status=3;
		file_order.seekg(opos,std::ios::beg);
		file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
		unsigned int ihash=hash_calc(_order.trainID);
		int qpos=queues.query(ihash),_qpos=-1;
		while(qpos!=-1){
			file_queue.seekg(qpos,std::ios::beg);
			file_queue.read(reinterpret_cast<char * >(&_queue),sizeof(_queue));
			if(_queue.orderPos==opos){
				//delete node
				if(_qpos==-1){
					queues.erase(ihash,qpos);
					if(_queue.nxtPos!=-1)queues.insert(ihash,_queue.nxtPos);
				} else {
					__queue.nxtPos=_queue.nxtPos;
					file_queue.seekg(_qpos,std::ios::beg);
					file_queue.write(reinterpret_cast<char * >(&__queue),sizeof(__queue));
				}
				if(_queue.nxtPos==-1){
					dqueues.erase(ihash,qpos);
					if(_qpos!=-1)dqueues.insert(ihash,_qpos);
				}
			} else {
				__queue=_queue,_qpos=qpos;
			}
			qpos=_queue.nxtPos;
		}
		return 0;
	}
	_order.status=3;
	file_order.seekg(opos,std::ios::beg);
	file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
	unsigned int ihash=hash_calc(_order.trainID);
	int ipos=trains.query(ihash);
	file_train.seekg(ipos,std::ios::beg);
	file_train.read(reinterpret_cast<char * >(&_train),sizeof(_train));
	file_seat.seekg(_train.seatPos[_order.date],std::ios::beg);
	file_seat.read(reinterpret_cast<char * >(&_seat),sizeof(_seat));
	for(int i=_order.startOrder;i<_order.endOrder;++i)
		_seat.day_seat[i]+=_order.seatNum;
	file_seat.seekg(_train.seatPos[_order.date],std::ios::beg);
	file_seat.write(reinterpret_cast<char * >(&_seat),sizeof(_seat));
	//queue
	int qpos=queues.query(ihash),_qpos=-1;
	while(qpos!=-1){
		file_queue.seekg(qpos,std::ios::beg);
		file_queue.read(reinterpret_cast<char * >(&_queue),sizeof(_queue));
		int s=inf;
		file_seat.seekg(_train.seatPos[_queue.date],std::ios::beg);
		file_seat.read(reinterpret_cast<char * >(&_seat),sizeof(_seat));
		for(int i=_queue.startOrder;i<_queue.endOrder;++i)s=std::min(s,_seat.day_seat[i]);
		if(s>=_queue.seatNum){
			for(int i=_queue.startOrder;i<_queue.endOrder;++i)_seat.day_seat[i]-=_queue.seatNum;
			file_seat.seekg(_train.seatPos[_queue.date],std::ios::beg);
			file_seat.write(reinterpret_cast<char * >(&_seat),sizeof(_seat));
			file_order.seekg(_queue.orderPos,std::ios::beg);
			file_order.read(reinterpret_cast<char * >(&_order),sizeof(_order));
			_order.status=1;
			file_order.seekg(_queue.orderPos,std::ios::beg);
			file_order.write(reinterpret_cast<char * >(&_order),sizeof(_order));
			//delete node
			if(_qpos==-1){
				queues.erase(ihash,qpos);
				if(_queue.nxtPos!=-1)queues.insert(ihash,_queue.nxtPos);
			} else {
				__queue.nxtPos=_queue.nxtPos;
				file_queue.seekg(_qpos,std::ios::beg);
				file_queue.write(reinterpret_cast<char * >(&__queue),sizeof(__queue));
			}
			if(_queue.nxtPos==-1){
				dqueues.erase(ihash,qpos);
				if(_qpos!=-1)dqueues.insert(ihash,_qpos);
			}
		} else {
			__queue=_queue,_qpos=qpos;
		}
		qpos=_queue.nxtPos;
	}
	return 0;
}

//clean
void clean(){
	hashtable1.clear();
	hashtable2.clear();
	users.clear();
	trains.clear();
	orders.clear();
	queues.clear();
	dqueues.clear();
	stations.clear();
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
	file_seat.close();
	file_seat.open("seat",std::fstream::out|std::ios::trunc);
	file_seat.close();
	file_seat.open("seat",std::ios::in|std::ios::out|std::ios::binary);
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