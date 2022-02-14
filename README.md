# 开发文档

### 模块

#### 后端逻辑 - main.cpp

##### BPT

+ `users`：记录用户的保存位置
+ `trains`：记录车辆的保存位置
+ `stations`：记录车站的保存位置
+ `orders`：记录某用户最后一个订单的保存位置
+ `queues`：记录某车辆第一个候补的保存位置
+ `dqueues`：记录某车辆最后一个候补的保存位置
+ `hashtable1`：功能为哈希表，记录用户是否登录
+ `hashtable2`：功能为哈希表，记录车辆是否发布

##### 类

+ `user`：记录用户信息。其中信息数组均为私有，公有函数有构造、取优先级、输出以及各种更新

##### 结构体

+ `sort_node`：为 `query_ticket` 排序开的结构体
+ `ntime`：记录时间——月、日、时、分。其中内置时间输出函数、重载小于号以及重载等号
+ `seat`：保存某一天发车的车辆在每一站的座位数
+ `train`：保存火车信息
+ `station`：保存车站信息，记录当前车站有哪些车经过以及经过车的数量
+ `order`：保存订单信息。其中保存有和当前订单用户相同的上一个订单的保存位置
+ `queue`：保存候补信息。其中内置赋值重载，保存有和当前候补车辆相同的上一个候补的保存位置

##### 函数

+ `int main()`：主函数，分配指令
+ `void create_file()`：新建外存文件
+ `bool string_same(const char *A,const char *B)`：判断串 $A$ 和串 $B$ 是否相同
+ `bool string_less(const char *A,const char *B)`：判断串 $A$ 是否字典序小于串 $B$
+ `unsigned int hash_calc(char *s)`：求串 $s$ 的哈希值
+ `void sort(int l,int r,sort_node *a)`：对 `sort_node` 的 $l$ 到 $r$ 从小到大排序
+ `void sort(int l,int r,unsigned int *a,short *b)`：对 $a$ 数组从小到大排序，$b$ 的相应位置一起改变
+ `int find_pos(int l,int r,unsigned int *a,unsigned int x)`：寻找哈希值为 $x$ 的车站是某辆火车的第几站
**实现**：每个车辆在读入的时候将所有经过的车站记录下哈希值并进行排序，这样每次就可以二分查找
+ `int add_user()`：添加用户
+ `int login()`：登录
+ `int logout()`：登出
+ `int query_profile()`：查询用户信息
+ `int modify_profile()`：修改用户信息
+ `ntime timecalc(ntime p,int t)`：计算时间 $p$ 在 $t$ 分钟之后的时间
+ `int timeid(int m,int d)`：计算月份为 $m$ ，日期为 $d$ 的 $id$ ，其中 $id$ 以 $6$ 月 $1$ 号为 $1$，一直到 $8$ 月 $31$ 为 $92$
+ `int Decode_month(int d)`：对 $id$ 进行解码，求出月份
+ `int Decode_day(int d)`：对 $id$ 进行解码，求出日期
+ `int add_train()`：添加车辆
+ `int release_train()`：发布车辆
**实现**：发布车辆后，枚举它经过的所有车站，给每个车站进行信息修改，加上该趟列车。同时，对发车期间的所有天数都新建 `seat`
+ `int query_train()`:查询车辆
+ `int delete_train()`：删除车辆
+ `void query_ticket()`:查询车票
**实现**：取出起点和终点的车站信息，可以知道经过他们的车辆集合，两者交集则有可能满足条件。对两个集合排序，用两个指针寻找相同值。找到一个相同值之后，取出这辆车的信息，暴力判断是否满足条件（经过顺序，时间等）
+ `void query_transfer()`：查询换乘
**实现**：枚举起点经过的车作为第一辆车，再枚举车经过的站作为中间站，再枚举终点站经过的车作为第二辆车，判断是否满足条件
+ `long long buy_ticket()`：买票
**实现**：买票后，记录当前订单或候补
+ `int query_order()`：查询订单
**实现**：通过 `order` 这颗BPT，可以知道该用户最新的订单。而每个订单都记录了上一个订单的位置，于是循环实现，每次跳到上一个订单并输出
+ `int refund_ticket()`：退票
**实现**：推完票后，修改订单状态。退票会导致候补发生变化，于是要看当前车的候补。找候补与找订单的方法相同也是通过链表进行跳，不过这个链表是向后的链表。如果最老的候补满足条件，则进行买票操作，并从链表中删除
+ `void clean()`：清空

#### 数据结构 - BPlus_Tree.cpp

##### 数据成员

+ `class Node`：内嵌类，记录BPT的节点信息

  |    bool isLeaf = true；    |            是否为叶节点            |
  | :------------------------: | :--------------------------------: |
  |        int num = 0;        |           孩子节点的个数           |
  |       int pre = -1;        |         当前节点的前一节点         |
  |       int nxt = -1;        |         当前节点的后一节点         |
  | int childPos[max_num + 1]; |       孩子节点在文件中的位置       |
  |    UI key[max_num + 1];    | 孩子节点的关键词信息(unsigned int) |


+ `fs`：文件流对象
+ `size`：元素个数
+ `root_pos`：根节点在文件中的位置

##### 公有函数

+ `explicit BPlus_Tree(const char *s, int sz = 0)`：构造函数，`s`是文件名，`sz`是分配文件大小

+ `~BPlus_Tree()`：析构函数，更新`size`和`root_pos`

+ `bool insert(UI s, int pos, int node_pos = -1, Node *parent = nullptr)`：插入键值为`s`，文件写入位置为`pos`的元素

  **实现**：从根节点向下查找，找到元素对应节点后插入，并自下向上平衡

+ `explicit BPlus_Tree(const char *s, int sz = 0)`：构造函数，`s`是文件名，`sz`是分配文件大小

+ `~BPlus_Tree()`：析构函数，更新`size`和`root_pos`

+ `bool insert(UI s, int pos, int node_pos = -1, Node *parent = nullptr)`：插入键值为`s`，文件写入位置为`pos`的元素

  **实现**：从根节点向下查找，找到对应叶节点后插入，并自下向上平衡

+ `bool erase(UI s, int pos = -1, int node_pos = -1, Node *parent = nullptr)`：删除键值为`s`，文件写入位置为`pos`的元素

  **实现**：从根节点向下查找，找到元素所在节点后删除，并自下向上平衡

+ `int query(UI s, int node_pos = -1)`：查询键值为`s`的元素在文件中的写入位置

  **实现**：从根节点向下查找

+ `void clear()`：把`size`和`root_pos`重置成初始值

##### 私有函数

+ `int getChildIndex_insert(Node &node, UI s)`：辅助`insert`，查询元素应在节点
+ `int getChildIndex_erase_query(Node &node, UI s)`：辅助`erase`和`query`，查询元素所在节点
+ `int queryLeaf(Node &node, UI s)`：辅助`query`，在叶节点中查询元素
+ `bool empty()`：判断BPT是否为空
+ `void insertNode(Node &node, UI s, int pos, int idx)`：辅助`insert`，在叶节点中插入元素
+ `void split(Node &node, int cur_pos, Node *parent)`：辅助`insert`，将当前节点分裂成两个节点，若当前节点为根节点，生成新的根节点作为新节点的父亲，更新`root_pos`
+ `void insertAdjust(Node &node, int cur_pos, Node *parent)`：辅助`insert`，更新父亲节点，若当前节点元素`num`过大则分裂
+ `void eraseAdjust(Node &node, int cur_pos, Node *parent)`：辅助`erase`，若当前元素`num`过小，则向后（前）节点借一个元素，若后（前 ）节点`num`也小，则与当前节点合并
+ `void eraseNode(Node &node, UI s, int pos, int idx)`：辅助`erase`，在叶节点中删除元素
+ `void borrowPre(Node &curNode, int cur_pos, Node &preNode, int pre_pos, Node *parent)`：辅助`earseAdjust`，将前一个节点的最大元素搬到当前节点
+ `void borrowNxt(Node &curNode, int cur_pos, Node &nxtNode, int nxt_pos, Node *parent)`：辅助`earseAdjust`，将后一个节点的最小元素搬到当前节点
+ `void mergeNxt(Node &node, int cur_pos, Node &nxt_node, int nxt_pos, Node *parent)`：辅助`earseAdjust`，将后一个节点和当前节点合并
+ `void mergePre(Node &node, int cur_pos, Node &pre_node, int pre_pos, Node *parent)`：辅助`earseAdjust`，将前一个节点和当前节点合并
+ `void read(T &obj, int pos)`：文件读入
+ `void write(T &obj, int pos)`：文件写入
+ `int end()`：查询文件末位置

### 分工

洪熠佳：后端逻辑

凌逸飏：数据结构
