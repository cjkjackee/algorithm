#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class node
{
    friend class RBtree;
    public:
        node(){valid=false;};
        ~node(){};
        node(node*);
    private:
        int value;
        node* left;
        node* right;
        node* parent;
        bool is_red;
        bool valid;
};

node::node(node* p)
{
    value = 0;
    left = NULL;
    right = NULL;
    parent = p;
    is_red = false;
    valid = false;
}

class RBtree
{
    public:
        RBtree(){root=new node(NULL);};
        ~RBtree();
        void insert(int);
        void del(int);
        void show(node*, fstream&);
        node* Root(){return root;};
        void debug(node*);
    private:
        node* root;
        void D_all(node*&);
        void left_rotate(node*);
        void right_rotate(node*);
        void fix_tree_insert(node*);
        void fix_tree_del(node*);
        void deletion(node*);
};

void RBtree::debug(node* now)
{
    if (!now->valid)
        return;
    
    debug(now->left);
    cout << "key: " << now->value;
    cout << " parent: ";
    if (now->parent == NULL)
        cout << "0";
    else
        cout << now->parent->value;
    cout << " color: ";
    if (now->is_red)
        cout << "red" << endl;
    else
        cout << "black" << endl;
    debug(now->right);

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
    node* now = root;

    while(now->valid)
    {
        if (n <= now->value)
            now = now->left;
        else
            now = now->right;
    }

    now->value = n;
    now->left = new node(now);
    now->right = new node(now);
    now->valid = true;
    now->is_red = true;

    if (now == root)
    {
        now->parent = new node(NULL);
        now->is_red = false;
    }

    if (now->parent->is_red)
        fix_tree_insert(now);

    cout << endl;
    debug(root);
    cout << endl;

    return;
}

void RBtree::del(int n)
{
    node* now = root;

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
    if (!now->valid)
        return;

    show(now->left,fout);
    fout << "key: " << now->value;
    fout << " parent: ";
    if (!now->parent->valid)
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

void RBtree::D_all(node*& now)
{
    if (now!=NULL)
    {
        D_all(now->left);
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
        now->parent = root->parent;
        this->root = now;
    }

    parent->right = now->left;
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
        now->parent = root->parent;
        this->root = now;
    }

    parent->left = now->right;
    now->right->parent = parent;
    now->right = parent;
    parent->parent = now;

    return;
}

void RBtree::fix_tree_insert(node* now)
{
    node* parent;
    node* uncle;

    while(1)
    {
        if (now==this->root)
        {
            now->is_red = false;
            return;
        }

        parent = now->parent;
        if (parent == root)
            return;
        
        if (parent == parent->parent->left)
            uncle = parent->parent->right;
        else 
            uncle = parent->parent->left;

        if (uncle->is_red)
        {
            parent->is_red = false;
            uncle->is_red = false;
            parent->parent->is_red = true;
            now = parent->parent;
            if (!now->parent->is_red)
                return;
        }
        else 
        {
            if (parent == parent->parent->left)
            {
                if (now == parent->right)
                {
                    left_rotate(now);
                    parent = now;
                    now = parent->left;
                }
                parent->is_red = false;
                parent->parent->is_red = true;
                right_rotate(parent);
                return;
            }
            else
            {
                if (now == parent->left)
                {
                    right_rotate(now);
                    parent = now;
                    now = parent->right;
                }
                parent->is_red = false;
                parent->parent->is_red = true;
                left_rotate(parent);
                return;
            }
        }
    }

    return;
}

void RBtree::fix_tree_del(node* now)
{
    node* sibling;
    
    while(1)
    {
        if (now->is_red || now==root)
        {
            now->is_red = false;
            return;
        }

        if (now == now->parent->left)
            sibling = now->parent->right;
        else
            sibling = now->parent->left;
        
        if (sibling->is_red)
        {
            sibling->is_red = false;
            now->parent->is_red = true;
            if (sibling == sibling->parent->right)
                left_rotate(sibling);
            else
                right_rotate(sibling);
        }
        else
        {
            if ((!sibling->left->is_red || sibling->left==NULL) && (!sibling->right->is_red || sibling->right==NULL))
            {
                sibling->is_red = true;
                now = now->parent;
            }
            else
            {
                if (sibling == now->parent->right)
                {
                    if (sibling->left->is_red)
                    {
                        sibling->left->is_red = false;
                        sibling->is_red = true;
                        right_rotate(sibling->left);
                        sibling = now->parent->right;
                    }
                    sibling->is_red = now->parent->is_red;
                    now->parent->is_red = false;
                    sibling->right->is_red = false;
                    left_rotate(sibling);
                    sibling->is_red = false;
                    return;
                }
                else
                {
                    if (sibling->right->is_red)
                    {
                        sibling->right->is_red = false;
                        sibling->is_red = true;
                        left_rotate(sibling->right);
                        sibling = now->parent->left;
                    }
                    sibling->is_red = now->parent->is_red;
                    now->parent->is_red = false;
                    sibling->left->is_red = false;
                    right_rotate(sibling);
                    sibling->is_red = false;
                    return;
                }
            }
        }
    }

    return;
}

void RBtree::deletion(node* now)
{
    bool now_is_red;
    node* replace;

    now_is_red = now->is_red;

    if (!now->left->valid && !now->right->valid)
    {
        now->is_red = now->left->is_red;
        now->valid = false;
    }
    else if (!now->left->valid)
    {
        replace = now->right;
        if (now == now->parent->left)
        {
            now->parent->left = replace;
            replace->parent = now->parent;
        }
        else 
        {
            now->parent->right = replace;
            replace->parent = now->parent;
        }
        delete now->left;
        delete now;
        now = replace;
    }
    else if (!now->right->valid)
    {
        replace = now->left;
        if (now == now->parent->left)
        {
            now->parent->left = replace;
            replace->parent = now->parent;
        }
        else 
        {
            now->parent->right = replace;
            replace->parent = now->parent;
        }
        delete now->right;
        delete now;
        now = replace;
    }
    else 
    {
        replace = now->left;
        while(replace->right->valid)
            replace = replace->right;
        now->value = replace->value;
        now = replace;
        deletion(now);
    }

    if (!now_is_red)
        fix_tree_del(now);

    return;
}



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
    fout.open("output.txt",fstream::out);

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