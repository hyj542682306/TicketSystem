#include "BPlus_Tree.h"
#include <iostream>

BPlus_Tree::BPlus_Tree(const char *s, int sz)
{//TODO why has to be like this
    std::fstream fi(s, std::ios::in | std::ios::binary);
    if (!fi.is_open())
    {
        std::fstream fo(s, std::ios::out | std::ios::binary);
        fo.write(reinterpret_cast<char *>(&size), sizeof(size));
        fo.write(reinterpret_cast<char *>(&root_pos), sizeof(root_pos));
        fo.close();
    } else
    {
        fi.read(reinterpret_cast<char *>(&size), sizeof(size));
        fi.read(reinterpret_cast<char *>(&root_pos), sizeof(root_pos));
        fi.close();
    }
    fs.open(s, std::ios::in | std::ios::out | std::ios::binary);
}

BPlus_Tree::~BPlus_Tree()
{
    write(size, 0);
    write(root_pos, sizeof(int));
    fs.close();
}

bool BPlus_Tree::insert(UI s, int pos, int node_pos, Node *parent)
{
    if (empty())
    {
        Node root;
        write(root, 2 * sizeof(int));
    }

    if (node_pos == -1) node_pos = root_pos;
    Node node;
    read(node, node_pos);

    int idx = getChildIndex_insert(node, s);
    if (idx == -1) return 0;

    if (node.isLeaf)
    {
        insertNode(node, s, pos, idx);
        insertAdjust(node, node_pos, parent);

        size++;
    } else
    {
        if (idx == node.num) idx--;
        if (!insert(s, pos, node.childPos[idx], &node)) return 0;
        else insertAdjust(node, node_pos, parent);
    }

    write(node, node_pos);
    return 1;
}

bool BPlus_Tree::erase(UI s, int pos, int node_pos, Node *parent)
{
    if (empty()) return 0;

    if (node_pos == -1) node_pos = root_pos;
    Node node;
    read(node, node_pos);

    if (node.isLeaf)
    {//TODO different here
        int idx = -1;
        for (int i = 0; i < node.num; ++i)
        {
            if (s != node.key[i] || pos != node.childPos[i]) continue;
            idx = i;
            break;
        }
        if (idx == -1) return 0;
        eraseNode(node, s, pos, idx);
        eraseAdjust(node, node_pos, parent);

        size--;
    } else
    {
        int idx = getChildIndex_erase_query(node, s);
        if (idx == -1) return 0;
        if (!erase(s, pos, node.childPos[idx], &node)) return 0;
        else eraseAdjust(node, node_pos, parent);
    }

    if (node_pos != -1) write(node, node_pos);//TODO whether we need !=-1
    return 1;
}

int BPlus_Tree::query(UI s, int node_pos)
{
    if (empty()) return -1;

    if (node_pos == -1) node_pos = root_pos;
    Node node;
    read(node, node_pos);

    if (node.isLeaf) return queryLeaf(node, s);
    else
    {
        int idx = getChildIndex_erase_query(node, s);
        if (idx == -1) return -1;
        else return query(s, node.childPos[idx]);
    }
}

int BPlus_Tree::getChildIndex_insert(Node &node, UI s)
{
    for (int i = 0; i < node.num; ++i)
    {
        if (s == node.key[i]) return -1;
        if (s < node.key[i]) return i;
    }
    return node.num;
}

int BPlus_Tree::queryLeaf(Node &node, UI s)
{
    for (int i = 0; i < node.num; ++i)
        if (s == node.key[i])
            return node.childPos[i];
    return -1;
}

bool BPlus_Tree::empty() const
{
    return size == 0;
}

void BPlus_Tree::insertNode(Node &node, UI s, int pos, int idx)
{
    for (int i = node.num; i > idx; --i)
    {
        node.childPos[i] = node.childPos[i - 1];
        node.key[i] = node.key[i - 1];
    }
    node.num++;
    node.childPos[idx] = pos;
    node.key[idx] = s;
}

void BPlus_Tree::split(Node &node, int cur_pos, Node *parent)
{
    Node newNode;
    newNode.isLeaf = node.isLeaf;
    newNode.num = node.num - min_num;
    newNode.pre = cur_pos;
    newNode.nxt = node.nxt;
    for (int i = 0; i < newNode.num; ++i)
    {
        newNode.childPos[i] = node.childPos[i + min_num];
        newNode.key[i] = node.key[i + min_num];
    }

    int newPos = end();
    node.nxt = newPos;
    node.num = min_num;
    write(node, cur_pos);
    write(newNode, newPos);

    if (newNode.nxt != -1)
    {
        Node newNode_nxt;
        read(newNode_nxt, newNode.nxt);
        newNode_nxt.pre = newPos;
        write(newNode_nxt, newNode.nxt);
    }

    if (parent)
    {
        int idx = -1;
        for (int i = 0; i < parent->num; ++i)
        {
            if (cur_pos != parent->childPos[i]) continue;
            idx = i;
            parent->childPos[i] = newPos;
            parent->key[i] = newNode.key[newNode.num - 1];
            break;
        }

        Node &par = *parent;
        insertNode(par, node.key[node.num - 1], cur_pos, idx);
    } else
    {
        Node root;
        int rootPos = end();
        root_pos = rootPos;
        root.isLeaf = false;
        root.num = 2;
        root.childPos[0] = cur_pos;
        root.childPos[1] = newPos;
        root.key[0] = node.key[node.num - 1];
        root.key[1] = newNode.key[newNode.num - 1];
        write(root, rootPos);
    }
}

void BPlus_Tree::insertAdjust(Node &node, int cur_pos, Node *parent)
{
    if (parent)
    {
        for (int i = 0; i < parent->num; ++i)
        {
            if (parent->childPos[i] != cur_pos) continue;
            parent->key[i] = node.key[node.num - 1];
            break;
        }
    }

    if (node.num > max_num) split(node, cur_pos, parent);
}

void BPlus_Tree::eraseAdjust(Node &node, int cur_pos, Node *parent)
{
    int prePos = -1, nxtPos = -1;
    Node preNode, nxtNode;
    if (parent)
    {
        int idx = -1;
        for (int i = 0; i < parent->num; ++i)
        {
            if (cur_pos != parent->childPos[i]) continue;
            idx = i;
            break;
        }
        if (idx != 0) prePos = parent->childPos[idx - 1];
        if (idx != parent->num - 1) nxtPos = parent->childPos[idx + 1];
    }

    if (node.num < min_num)
    {
        if (nxtPos != -1)
        {
            read(nxtNode, node.nxt);

            if (nxtNode.num > min_num) borrowNxt(node, cur_pos, nxtNode, nxtPos, parent);
            else mergeNxt(node, cur_pos, nxtNode, nxtPos, parent);
        } else if (prePos != -1)
        {
            read(preNode, node.pre);

            if (preNode.num > min_num) borrowPre(node, cur_pos, preNode, prePos, parent);
            else mergePre(node, cur_pos, preNode, prePos, parent);
        }
    }

    if (parent)
    {
        if (parent->num <= 1 && node.pre == -1 && node.nxt == -1)
        {
            (*parent) = node;//TODO what to do with root_pos
        } else
        {
            for (int i = 0; i < parent->num; ++i)
            {
                if (cur_pos != parent->childPos[i]) continue;
                parent->key[i] = node.key[node.num - 1];
                break;
            }
        }
    }
}

void BPlus_Tree::eraseNode(Node &node, UI s, int pos, int idx)
{
    for (int i = idx; i < node.num - 1; ++i)
    {
        node.childPos[i] = node.childPos[i + 1];
        node.key[i] = node.key[i + 1];
    }
    node.num--;
}

void BPlus_Tree::borrowPre(Node &curNode, int cur_pos, Node &preNode, int pre_pos, Node *parent)
{
    for (int i = curNode.num; i > 0; --i)
    {
        curNode.childPos[i] = curNode.childPos[i - 1];
        curNode.key[i] = curNode.key[i - 1];
    }
    curNode.num++;

    curNode.childPos[0] = preNode.childPos[preNode.num - 1];
    curNode.key[0] = preNode.key[preNode.num - 1];
    preNode.num--;
    write(preNode, pre_pos);

    if (parent)
    {
        for (int i = 0; i < parent->num; ++i)
        {
            if (pre_pos != parent->childPos[i]) continue;
            parent->key[i] = preNode.key[preNode.num - 1];
            break;
        }
    }
}

void BPlus_Tree::borrowNxt(Node &curNode, int cur_pos, Node &nxtNode, int nxt_pos, Node *parent)
{
    curNode.childPos[curNode.num] = nxtNode.childPos[0];
    curNode.key[curNode.num] = nxtNode.key[0];
    curNode.num++;

    for (int i = 1; i < nxtNode.num; ++i)
    {
        nxtNode.childPos[i - 1] = nxtNode.childPos[i];
        nxtNode.key[i - 1] = nxtNode.key[i];
    }
    nxtNode.num--;
    write(nxtNode, nxt_pos);
}

void BPlus_Tree::mergeNxt(Node &node, int cur_pos, Node &nxt_node, int nxt_pos, Node *parent)
{
    for (int i = 0; i < nxt_node.num; ++i)
    {
        node.childPos[i + node.num] = nxt_node.childPos[i];
        node.key[i + node.num] = nxt_node.key[i];
    }
    node.num += nxt_node.num;

    for (int i = 0; i < parent->num; ++i)
    {
        if (nxt_pos != parent->childPos[i]) continue;
        for (int j = i; j < parent->num - 1; ++j)
        {
            parent->childPos[j] = parent->childPos[j + 1];
            parent->key[j] = parent->key[j + 1];
        }
        parent->num--;
        break;
    }

    node.nxt = nxt_node.nxt;
    if (nxt_node.nxt != -1)
    {
        Node nxtNode_nxt;
        read(nxtNode_nxt, nxt_node.nxt);
        nxtNode_nxt.pre = cur_pos;
        write(nxtNode_nxt, nxt_node.nxt);
    }
}

void BPlus_Tree::mergePre(Node &node, int cur_pos, Node &pre_node, int pre_pos, Node *parent)
{
    for (int i = node.num - 1; i >= 0; --i)
    {
        node.childPos[i + pre_node.num] = node.childPos[i];
        node.key[i + pre_node.num] = node.key[i];
    }
    node.num += pre_node.num;
    for (int i = 0; i < pre_node.num; ++i)
    {
        node.childPos[i] = pre_node.childPos[i];
        node.key[i] = pre_node.key[i];
    }

    for (int i = 0; i < parent->num; ++i)
    {
        if (pre_pos != parent->childPos[i]) continue;
        for (int j = i; j < parent->num - 1; ++j)
        {
            parent->childPos[j] = parent->childPos[j + 1];
            parent->key[j] = parent->key[j + 1];
        }
        parent->num--;
        break;
    }

    node.pre = pre_node.pre;
    if (pre_node.pre != -1)
    {
        Node preNode_pre;
        read(preNode_pre, pre_node.pre);
        preNode_pre.nxt = cur_pos;
        write(preNode_pre, pre_node.pre);
    }
}

int BPlus_Tree::get_size() const
{
    return size;
}

void BPlus_Tree::clear()
{
    size = 0;
    write(size, 0);
    root_pos = 2 * sizeof(int);
    write(root_pos, sizeof(int));
}

int BPlus_Tree::getChildIndex_erase_query(BPlus_Tree::Node &node, BPlus_Tree::UI s)
{
    for (int i = 0; i < node.num; ++i)
        if (s <= node.key[i]) return i;
    return -1;
}

