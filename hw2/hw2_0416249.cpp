#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

// datatype: node
class node
{
    friend class RBtree;
public:
    node(){};
    ~node(){};
    node(int,node*);
private:
    int value;
    node* left;
    node* right;
    node* parent;
    bool is_red;
};

node::node(int num, node* p)
{
    value = num;
    left = NULL;
    right = NULL;
    parent = p;
    is_red = true;
}
// end node

// datatype red black tree
class RBtree
{
public:
    RBtree(){root = NULL;};
    ~RBtree();
    void insert(int);
    void del(int);
    void show(node*,fstream&);
    node* Root();
private:
    node* root;
    void D_all(node*&);
    void left_rotate(node*);
    void right_rotate(node*);
    void fix_tree_insert(node*);
    void fix_tree_del(node*);
    void deletion(node*);
};

void RBtree::D_all(node*& now)
{
    if (now!=NULL)
    {
        D_all(now->right);
        D_all(now->right);
        delete now;
        now = NULL;
    }
    return;
}

void RBtree::left_rotate(node* now)
{
    node* parent = now->parent;

    if (parent!=root)
    {
        if (parent == parent->parent->left)
        {
            parent->parent->left = now;
            now->parent = parent->parent;
        }
        else 
        {
            parent->parent->right = now;
            now->parent = parent->parent;
        }
    }
    else
    {
        now->parent = NULL;
        this->root = now;
    }

    parent->right = now->left;
    if (now->left!=NULL)
        now->left->parent = parent;
    now->left = parent;
    parent->parent = now;

    return;
}

void RBtree::right_rotate(node* now)
{
    node* parent = now->parent;

    if (parent!=root)
    {
        if (parent == parent->parent->left)
        {
            parent->parent->left = now;
            now->parent = parent->parent;
        }
        else 
        {
            parent->parent->right = now;
            now->parent = parent->parent;
        }
    }
    else
    {
        now->parent = NULL;
        this->root = now;
    }

    parent->left = now->right;
    if (now->right!=NULL)
        now->right->parent = parent;
    now->right = parent;
    parent->parent = now;

    return;
}

void RBtree::fix_tree_insert(node* now)
{
    node* p;
    node* uncle;

    // valid when parent==black
    if (now == this->root)
    {
        if (now->is_red)
            now->is_red = false;
        return;
    }

    if (now->parent->is_red==false)
        return;

    // init now->parent && parent->sibling
    p = now->parent;
    if (p != root)
    {
        if (p == p->parent->left)
        {
            if (p->parent->right==NULL)
                uncle = NULL;
            else
                uncle = p->parent->right;
        }
        else 
        {
            if (p->parent->left==NULL)
                uncle = NULL;
            else 
                uncle = p->parent->left;
        }
    }
    else 
        return;

    // case 1: when parent && uncle node == red
    if (uncle!=NULL && uncle->is_red)
    {
        p->is_red = false;
        uncle->is_red = false;
        p->parent->is_red = true;
        fix_tree_insert(p->parent);
    }
    // case2 && case3 either parent node on grandparent node left side or right side
    else
    {
        if (p == p->parent->left)
        {
            // case 2:
            if (now == p->right) 
            {
                left_rotate(now);
                p = now;
                now = p->left;
            }
            // case 3:
            p->is_red = false;
            p->parent->is_red = true;
            right_rotate(p);
        }
        else 
        {
            //case 2: 
            if (now == p->left)
            {
                right_rotate(now);
                p = now;
                now = p->right;
            }
            // case 3:
            p->is_red = false;
            p->parent->is_red = true;
            left_rotate(p);
        }
    }

    return;
}

void RBtree::fix_tree_del(node* now)
{
    node* sibling;

    while(1)
    {
        if (now == this->root)
        {
            if (now->is_red)
                now->is_red = false;
            return;
        }

        if (now == now->parent->left)
        {
            if (now->parent->right)
                sibling = now->parent->right;
            else 
                sibling = NULL;
        }
        else 
        {
            if (now->parent->left)
                sibling = now->parent->left;
            else 
                sibling = NULL;
        }

        if (sibling!=NULL && sibling->is_red)
        {
            sibling->is_red = false;
            now->parent->is_red = true;
            if (now == now->parent->left)
                left_rotate(sibling);
            else
                right_rotate(sibling);
        }
        else 
        {
            if (sibling==NULL || (sibling->left==NULL && sibling->right==NULL) || (!sibling->left->is_red && !sibling->right->is_red))
            {
                if (sibling)
                    sibling->is_red = true;
                if (now->parent->is_red)
                {
                    now->parent->is_red = false;
                    return;
                }
                now = now->parent;
            }
            else if (sibling->right!=NULL && sibling->right->is_red)
            {
                if (now == now->parent->left)
                {
                    sibling->is_red = now->parent->is_red;
                    now->parent->is_red = false;
                    if (sibling->right)
                        sibling->right->is_red = false;
                    left_rotate(sibling);
                    return;
                }
                else 
                {
                    sibling->right->is_red = false;
                    sibling->is_red = true;
                    left_rotate(sibling->right);
                    now = sibling->parent;
                }
            }
            else if (sibling->left!=NULL && sibling->left->is_red)
            {
                if (now == now->parent->left)
                {
                    sibling->left->is_red = false;
                    sibling->is_red = true;
                    right_rotate(sibling->left);
                    now = sibling->parent;
                }
                else 
                {
                    sibling->is_red = now->parent->is_red;
                    now->parent->is_red = false;
                    if (sibling->left)
                        sibling->left->is_red = false;
                    right_rotate(sibling);
                    return;
                }
            }
        }
    }
    
    return;
}

void RBtree::deletion(node* now)
{
    bool now_is_red, replace_is_red;
    node* replace;
    
    now_is_red = now->is_red;

    if (now->left==NULL && now->right==NULL)
        delete now;
    else if (now->left==NULL)
    {
        replace = now->right;
        if (now->parent->left == now)
            now->parent->left = replace;
        else
            now->parent->right = replace;
        replace_is_red = replace->is_red;
        replace->parent = now->parent;
        replace->is_red = now->is_red;
        delete now;
    }
    else if (now->right==NULL)
    {
        replace = now->left;
        if (now->parent->left == now)
            now->parent->left = replace;
        else
            now->parent->right = replace;
        replace_is_red = replace->is_red;
        replace->parent = now->parent;
        replace->is_red = now->is_red;
        delete now;
    }
    else
    {
        replace = now->left;
        while(replace->right!=NULL)
            replace = replace->right;
        now->value = replace->value;

        deletion(replace);
    }

    if (!now_is_red && !replace_is_red)
        fix_tree_del(now);
    
    return;
}

RBtree::~RBtree()
{
    node* now = root;
    if (now != NULL)
        D_all(now);
}

void RBtree::insert(int n)
{
    bool is_left;
    node* p = NULL;
    node* now = root;

    if (this->root == NULL)
    {
        root = new node(n,p);
        root->is_red = false;
        return;
    }

    while(now != NULL)
    {
        p = now;
        if (n <= now->value)
        {
            is_left = true;
            now = now->left;
        }
        else 
        {
            is_left = false;
            now = now->right;
        }
    }
    now = new node(n,p);
    
    if (is_left)
        p->left = now;
    else
        p->right = now;

    fix_tree_insert(now);

    return;
}

void RBtree::del(int n)
{
    node* now = this->root;

    while(1)
    {
        if (n == now->value)
            break;
        else if (n < now->value)
            now = now->left;
        else if (n > now->value)
            now = now->right;
    }

    deletion(now);
    
    return;
}

void RBtree::show(node* now, fstream& fout)
{
    if (now == NULL)
        return;

    show(now->left,fout);
    fout << "key: " << now->value;
    fout << " parent: ";
    if (now->parent == NULL)
        fout << "0";
    else
        fout << now->parent->value;
    fout << " color: ";
    if (now->is_red)
        fout << "red" << endl;
    else
        fout << "black" << endl;
    show(now->right,fout);

    return;
}

node* RBtree::Root()
{
    return this->root;
}
// end red black tree

int main ()
{
    RBtree tree;
    fstream fin, fout;
    int T;
    int trig, n;
    bool first;
    string str;
    stringstream ss;

    fin.open("input.txt",fstream::in);
    fout.open("log.txt",fstream::out);

    getline(fin,str);
    ss << str;
    ss >> T;

    for (int x=0;x<T;++x)
    {
        first = true;
        
        // get method number;
        ss.clear();
        getline(fin,str);
        ss << str;
        ss >> trig;
        ss.clear();

        // get input number in string;
        getline(fin,str);
        ss << str;

        while(ss >> n)
        {
            if (trig == 1)
            {
                if (first)
                {
                    fout << "Insert: " << n;
                    first = false;
                }
                else 
                    fout << ", " << n;

                tree.insert(n);
            }
            else if (trig == 2)
            {
                if (first)
                {
                    fout << "Delete: " << n;
                    first = false;
                }
                else
                    fout << ", " << n;
                tree.del(n);
            }
            else 
                fout << "unknow method" << endl;
        }
        fout << endl;

        tree.show(tree.Root(),fout);
    }

    return 0;
}