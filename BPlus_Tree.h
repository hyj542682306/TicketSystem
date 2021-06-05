#ifndef SECOND_BPLUS_TREE_H
#define SECOND_BPLUS_TREE_H

#include <fstream>
#include <cstring>
#include <iostream>

class BPlus_Tree
{
    typedef unsigned int UI;
    static constexpr int max_num = 50; //TODO
    static constexpr int min_num = max_num / 2;

private:
    class Node
    {
    public:
        bool isLeaf = true;
        int num = 0;
        int pre = -1;
        int nxt = -1;
        int childPos[max_num + 1];
        UI key[max_num + 1];

        Node()
        {
            memset(childPos, 0, sizeof(childPos));
            memset(key, 0, sizeof(key));
        }

        Node(const Node &o)
        {
            isLeaf = o.isLeaf;
            num = o.num;
            pre = o.pre;
            nxt = o.nxt;
            for (int i = 0; i < num; ++i)
            {
                childPos[i] = o.childPos[i];
                key[i] = o.key[i];
            }
            for (int i = num; i <= max_num; ++i)
                childPos[i] = key[i] = 0;
        }

        Node &operator=(const Node &o)
        {
            if (this == &o) return *this;

            isLeaf = o.isLeaf;
            num = o.num;
            pre = o.pre;
            nxt = o.nxt;
            for (int i = 0; i < num; ++i)
            {
                childPos[i] = o.childPos[i];
                key[i] = o.key[i];
            }
            for (int i = num; i <= max_num; ++i)
                childPos[i] = key[i] = 0;

            return *this;
        }
    };

    std::fstream fs;
    int size = 0;
    int root_pos = 2 * sizeof(int);

public:
    /**
     * 构造一颗新的b+树，s是文件名，sz是分配文件大小
     */
    explicit BPlus_Tree(const char *s, int sz = 0);

    ~BPlus_Tree();

    /**
     * 插入键值为s，文件写入位置为pos
     * 如果插入成功返1，否则为0
     */
    bool insert(UI s, int pos, int node_pos = -1, Node *parent = nullptr);

    /**
     * 删除键值为s，文件写入位置为pos
     * 如果删除成功返1，否则为0
     */
    bool erase(UI s, int pos = -1, int node_pos = -1, Node *parent = nullptr);//TODO remove =-1

    /**
     * 查询键值为s的信息在文件中的写入位置
     * 如果不存在查询的节点的话返回-1
     */
    int query(UI s, int node_pos = -1);

    int get_size() const;

    void clear();

private:
    int getChildIndex_insert(Node &node, UI s);

    int getChildIndex_erase_query(Node &node, UI s);

    int queryLeaf(Node &node, UI s);

    bool empty() const;

    void insertNode(Node &node, UI s, int pos, int idx);

    void split(Node &node, int cur_pos, Node *parent);

    void insertAdjust(Node &node, int cur_pos, Node *parent);

    void eraseAdjust(Node &node, int cur_pos, Node *parent);

    void eraseNode(Node &node, UI s, int pos, int idx);

    void borrowPre(Node &curNode, int cur_pos, Node &preNode, int pre_pos, Node *parent);

    void borrowNxt(Node &curNode, int cur_pos, Node &nxtNode, int nxt_pos, Node *parent);

    void mergeNxt(Node &node, int cur_pos, Node &nxt_node, int nxt_pos, Node *parent);

    void mergePre(Node &node, int cur_pos, Node &pre_node, int pre_pos, Node *parent);

    template<typename T>
    void read(T &obj, int pos)
    {
        fs.seekg(pos, std::ios::beg);
        fs.read(reinterpret_cast<char *>(&obj), sizeof(obj));
    }

    template<typename T>
    void write(T &obj, int pos)
    {
        fs.seekp(pos, std::ios::beg);
        fs.write(reinterpret_cast<char *>(&obj), sizeof(obj));
    }

    int end()
    {
        fs.seekg(0, std::ios::end);
        int pos = fs.tellp();
        return pos;
    }
};


#endif //SECOND_BPLUS_TREE_H
