# homework 2 Red Black Tree
- RBTree::insert()
    - 基本上是跟binary tree的insert同樣做法，把<=的丟去left child，>的丟去right child
    - insert完了之後丟進fix_tree_insert()，把它弄成是valid的red black tree
- RBtree::fix_tree_insert()
    - 當insert進去的node的parent是紅色的時候，不符合red black tree，fix_tree會根據node和parent的位置，把它弄成是符合的red black tree
    - parent是黑色,符合red black tree
    - case1：parent的sibling也是red的(不論parent在left or right)
        - 把parent和parent的sibling弄成black
        - 把parent的parent弄成red
        - 以parent的parent做多一次fix_tree()
    - case2：parent的sibling是black，且new node在right (以parent在left的情況)
        - 做一次right rotate讓parent的parent，new node， parent 在一條線上
        - 做完之後會變成case3的情況
        - parent在right的話就變成case3的情況
    - case3: parent的sibling是black，且new node在left (以parent在left的情況)
        - 把parent變成black，parent的parent變成red
        - 做一次left rotate 就balance了
        - parent在right的話，就是case2的情況
- RBtree::del()
    - 是 leaf 直接改成valid=false
    - 不是有3種case
        - case1： left child not valid
            - 把right child 拉到要刪的node的位置
        - case2： right child not valid
            - 把left child 拉到要刪的node的位置
        - case3： left && right not valid
            - 把predecessor拉上來
            - 把predecessor丟進del多一次
    - 刪掉的node如果是black的一定會使tree不符合red black tree
        - 會使tree中的某一條路徑上的black node的數量比其他路徑的少
        - 丟進fix_tree_del()
- RBtree::fix_tree_del()
    - 根據被刪的node的sibling和sibling的child的顏色可以分成4種case
        - case0： now是red
            - 改成black， return;
        - case1： sibling是red，兩個child都是black
            - sibling改成black
            - now->parent改成red
            - sibling 做left_rotate（若是sibling在left，反向rotate）
            - loop多一次fix_tree_del check 有沒有符合red black tree
        - case2： sibling是black，兩個child都是black
            - sibling改成red
            - now->parent是red，改成black,return
            - now->parent是black，繼續loop
        - case3： sibling是black，left child是red，right是black
            - 以sibling在right的情況
            - sibling left child改成black
            - sibling改成red
            - sibling->left 做一次right_rotate
            - 進入case4
            - sibling 在left的情況 == case4
        - case4： sibling是black，不管left child，right child是red
            - 以sibling在left的情況
            - sibling改成parent的顏色
            - parent改成black
            - sibling right child 改成black
            - sibling做left_rotate
            - 把root改成black, return
            - sibling在right的話 == case3

- RBtree::show(node*, fstream&)
    - recurcive的tree inorder traversal function
    - fstream& <- 讓output可以用recurcive跑的關鍵

- RBtree::Root()
    - return RBtree的root
    - 爲了讓show可以直接做recursive

