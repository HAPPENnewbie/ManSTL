#include<iostream>
#include<stdexcept> 
using namespace std;
#include<vector>
template<typename T>class list{
public:
// 缺省函数
    list():m_head{nullptr}, m_tail{nullptr}{}
// 拷贝构造，用存在的链表拷贝构造新的链表
    list(list const& that):m_head{nullptr}, m_tail{nullptr}{
        for (node* pnode = that.m_head; pnode; pnode = pnode -> m_next) {
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

    
public:
// 迭代类
    class iterator{
        public:
            iterator(node* start, node* cur, node* end)
            :m_start{start}, m_cur{cur}, m_end{end}
            {}
            // 重载 *， 获取迭代器指向节点的数据
            T& operator*() {  
                if (m_cur == nullptr)
                    throw underflow_error("null node");
                return m_cur -> m_data;
            }
            // 重载 ++ , 这里实现的是循环迭代器
            iterator operator++() {
                if (m_cur == nullptr) 
                    m_cur = m_start;
                else
                    m_cur = m_cur -> m_next;
                return *this; 
            }
            // 重载 --
            iterator operator--() {
                if (m_cur == nullptr) 
                    m_cur = m_end;
                else
                    m_cur = m_cur -> m_prev;
                return *this;
            }
            // 重载==
            bool operator==(const iterator& that)const {
                return m_start == that.m_start && m_cur == that.m_cur && m_end == that.m_end;
            }
            // 重载!=
            bool operator!=(const iterator& that)const {
                return !(*this==that);
            }
        private:
            node* m_start;  //开始指向
            node* m_cur;    //当前指向
            node* m_end;    //终止指向
            friend class list;
    };
// 获取起始迭代器
    iterator begin() {  
        return iterator(m_head, m_head, m_tail);
    } 
// 获取结尾迭代器
    iterator end() {  
        return iterator(m_head, nullptr, m_tail);
    }
// 迭代器指向位置添加节点
    void insert(const iterator& loc, const T& data) {
        if (loc == end()) {
            push_back(data);
        }else {
            node* pnode = new node(loc.m_cur -> m_prev, loc.m_cur, data);
            if (pnode -> m_prev)
                pnode -> m_prev -> m_next = pnode;
            else
                m_head = pnode;
            pnode -> m_next -> m_prev = pnode;
        }
    }
// 删除迭代器指向的节点
    void erase(const iterator& loc) {
        if (loc == end()) return;
        node* pnode = loc.m_cur;
        if (pnode -> m_prev)
            pnode -> m_prev -> m_next = pnode -> m_next;
        else
            m_head = pnode -> m_next;

        if (pnode -> m_next) 
            pnode -> m_next -> m_prev = pnode -> m_prev;
        else 
            m_tail = pnode -> m_prev;
        delete pnode;
    }


// 正向常迭代类
    class const_iterator{
        public:
            const_iterator(const iterator& it)
            :m_it{it} 
            {}
            const T& operator* () {
                return *m_it; 
            }
            const_iterator& operator++() {
                ++m_it;
                return *this;
            }
            const_iterator& operator--() {
                --m_it;
                return *this;
            }
            bool operator== (const const_iterator& that) const {
                return m_it == that.m_it;
            }
            bool operator!= (const const_iterator& that) const {
                return !(m_it==that.m_it);
            }
        private:
            iterator m_it;
    };
// 获取常起始迭代器
    const_iterator begin() const {
        return iterator(m_head, m_head, m_tail);
    }
// 获取常终止迭代器
const_iterator end() const {
        return iterator(m_head, nullptr, m_tail);
    }
};




 
// 利用迭代器遍历链表
void print(const string& str, list<int>& l) {
    cout << str << endl;
    typedef list<int>::iterator IT;
    for (IT it = l.begin(); it != l.end(); ++it) {
        cout << *it << ' ';
    }
    cout << endl;
}



int main() {
    list<int> ls;
    for (int i = 1; i <= 5; i++) {
        ls.push_back(i);
    }
    print("打印链表为", ls);

    const list<int> cls(ls);
    typedef list<int>::const_iterator CIT;
    for (CIT cit = cls.begin(); cit != cls.end(); ++cit) {
        cout << *cit << ' ';
    }
    
    return 0;
}