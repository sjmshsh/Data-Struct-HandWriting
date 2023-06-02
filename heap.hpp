#include <vector>

using namespace std;

// 大堆
class Heap {
public:
    Heap() {
        elem.resize(10);
    }

    // 向下调整
    // parent代表每棵树的根节点, len代表每棵树调整的结束位置
    void shiftDown(int parent, int len) {
        int child = 2 * parent + 1;
        // 1. 最起码是有左孩子的, 至少有一个孩子
        while (child < len) {
            // child必须是较大的那个, 否则向下调整之后无法维持堆的结构特性
            if (child + 1 < len && elem[child] < elem[child + 1]) {
                child++;
            }
            // 如果child的值比parent的大, 需要交换child和parent的位置维持大根堆
            if (elem[child] > elem[parent]) {
                swap(elem[child], elem[parent]);
                parent = child;
                child = 2 * parent + 1;
            } else {
                break;
            }
        }
    }

    void shiftUp(int child) {
        int parent = (child - 1) / 2;
        while (child > 0) {
            if (elem[child] > elem[parent]) {
                swap(elem[child], elem[parent]);
                child = parent;
                parent = (child - 1) / 2;
            } else {
                break;
            }
        }
    }

    void offer(int val) {
        if (isFull()) {
            // 扩容
            elem.resize(2 * elem.size());
        }
        elem[usedSize++] = val;
        shiftUp(usedSize - 1);
    }

    bool isFull() {
        return usedSize == elem.size();
    }

    // 大堆
    void createHeap(const int array[], int N) {
        for (int i = 0; i < N; ++i) {
            elem[i] = array[i];
            usedSize++;
        }
        for (int parent = (usedSize - 1 - 1) / 2; parent >= 0; parent--) {
            shiftDown(parent, usedSize);
        }
    }

    int poll() {
        if (isEmpty()) {
            return -1;
        }
        int temp = elem[0];
        elem[0] = elem[usedSize - 1];
        elem[usedSize - 1] = temp;
        usedSize--;
        shiftDown(0, usedSize);
        return temp;
    }

    bool isEmpty() {
        return usedSize == 0;
    }

    // 在array数组里面找到最小的K个元素
    void TopK(int array[], int N, int k) {
        // 前K个元素建堆
        for (int i = 0; i < k; ++i) {
            // 建堆
            offer(array[i]);
        }
        // 剩下n - k个数字进行比较
        for (int i = k; i < N; ++i) {
            if (array[i] < elem[0]) {
                elem[0] = array[i];
                shiftDown(0, usedSize - 1);
            }
        }
    }

private:
    vector<int> elem;
    int usedSize;
};
