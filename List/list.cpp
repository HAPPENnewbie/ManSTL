#include<iostream>
#include<stdexcept> 
using namespace std;

template<typename T>class list{
public:
// 缺省函数
    list():m_head{nullptr}, m_tail{nullptr}{}
// 拷贝构造，用存在的链表拷贝构造新的链表
    list(list const& that):m_head{nullptr}, m_tail{nullptr}{
        for (node* pnode = that.m_head; pnode; pnode = pnode -> next) {
            push_back(pnode -> m_data);
        }
    }
// 析构函数
    ~list() {
        clear();
    }
// 链表判空  
    bool empty() {
        return m_head==nullptr && m_tail==nullptr;
    }
// 添加头结点
    void push_front(const T& data) {
        m_head = new node(nullptr,m_head,data);
        if (m_head -> m_next) {
            m_head -> m_next -> m_prev = m_head; 
        }else {
            m_tail = m_head; 
        }
    }
// 删除链表头结点
    void pop_front() {
        if (empty()) return;
        node* pnode = m_head -> m_next;
        delete m_head;
        if (pnode)
            pnode -> m_prev = nullptr;
        else
            m_tail = pnode;
        m_head = pnode;
    }
// 获取头结点元素,添加引用是为了方便更改头结点，两种版本
    T& front() {  //  非常属性的链表调用这个
        if (empty()) { // 抛出异常，链表为空没有节点
            throw underflow_error("null node");
        }
        return m_head -> m_data;
    }

    const T& front() const{  //  常属性的链表调用这个（不可改任何节点数据）
        return const_cast<list*>(this)->front();
    }
// 添加尾节点
    void push_back(T const& data) {
        m_tail = new node(m_tail,nullptr,data);
        if (m_tail -> m_prev) {
            m_tail -> m_prev -> m_next = m_tail;
        }else {
            m_head = m_tail;
        }
    }
// 删除尾节点
    void pop_back() {
        if (empty()) return;
        node* pnode = m_tail -> m_prev;
        delete m_tail;
        if (pnode) 
            pnode -> m_next = nullptr;
        else 
            m_head = nullptr;
        m_tail = pnode;
    }
// 获取尾节点数据
    T& back() {
        if (empty()) { // 抛出异常，链表为空没有节点
            throw underflow_error("null node");
        }
        return m_tail -> m_data;
    }

    T const& back() const{
        return const_cast<list*>(this)->back();
    }
// 清空链表
    void clear() {
        while(!empty()) {
            pop_front();
        }
    }
// 获取链表大小
    size_t size() {
        size_t count = 0;
        for (node* pnode = m_head; pnode; pnode = pnode -> m_next) {
            ++count;
        }
        return count;
    }


private:
    // 节点类，双向链表的单个节点
    class node{
        public:
            node(node* prev, node* next, const T& data)
            :m_prev{prev}, m_next{next},m_data{data} 
            {}
            node* m_prev;  // 前指针
            node* m_next;  // 后指针
            T m_data;      // 节点数据
    };
    node* m_head; // 链表头
    node* m_tail; // 链表尾
    friend ostream& operator<<(ostream& os, list<int>& l);
};





// -------------------------------------模拟用户---------------------------------
ostream& operator<<(ostream& os, list<int>& l) {  // 输出流重载，告诉out怎么打印链表
    for (list<int>::node* pnode = l.m_head; pnode; pnode = pnode -> m_next) {
        os << pnode -> m_data << ' ';
    }
    return os;
}



int main() {
    list<int> ls;

    // 添加节点测试
    ls.push_back(1);
    ls.push_back(2);
    ls.push_back(3);
    ls.push_back(4);
    ls.push_front(0);
    ls.push_front(-1);
    ls.push_front(-2);
    ls.push_front(-3);
    cout << ls << endl;


    // 删除节点测试
    ls.pop_front();
    ls.pop_front();
    ls.pop_front();
    ls.pop_front();

    ls.pop_back();
    cout << ls << endl;

    // 获取节点测试1
    int head_num = ls.front();
    cout << head_num << endl;
    int tail_num = ls.back();
    cout << tail_num << endl;

    // 清空链表测试
    ls.clear();
    cout << ls << endl;

    // 获取节点测试2，验证空链表时抛出异常
    int head_num2 = ls.front();  


    return 0;
}