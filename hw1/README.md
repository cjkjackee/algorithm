# hw1

## young tableau
- 特化型的heap sort 
    - h=1 的 binery tree
    - 抓周圍的element當作是child
- insert
    - 找第一個找到的x，換成input的value
    - 對比上面一個和左邊一個，抓他們之中最到的交換
    - 當自己是最大的停下
- extrat-min
    - pop掉第一個號碼
    - 找下面跟右邊的最小的那個，把他的值拉上來
    - 重復直到遇到兩邊都是x或最後一個位置