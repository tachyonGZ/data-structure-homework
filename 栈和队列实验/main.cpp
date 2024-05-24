#include <iostream>  
  
struct ParkInfo  
{  
    unsigned int id, inTime, outTime;  
};  
  
struct HStack  
{  
    ParkInfo *aParkInfo;  
    size_t maxSize;  
    size_t top;  
};  
  
struct QNode  
{  
    ParkInfo parkInfo;  
    QNode *pNext;     
};  
  
struct Queue  
{  
    QNode *pRear;  
    QNode *pHead;
};  
  
void init_stack(HStack &s, size_t size)  
{  
    s.aParkInfo = new ParkInfo[size];  
    s.maxSize = size;  
    s.top = 0;  
}

void uninit_stack(HStack &s)
{
    delete[] s.aParkInfo;
    s.maxSize = 0;
    s.top = 0;
}
  
void push_stack(HStack &s, ParkInfo &info)  
{  
    if (s.maxSize == s.top)  
    {  
        return;  
    }  
    s.aParkInfo[s.top++] = info;  
}  
  
void pop_stack(HStack &s, ParkInfo &info)  
{  
    if (0 == s.top)  
    {  
        return;  
    }  
  
    info = s.aParkInfo[--s.top];  
}  
  
size_t stack_size(HStack &s)  
{  
    return s.top + 1;  
}  
  
bool stack_full(HStack &s)  
{  
    return s.maxSize == s.top;  
}  
  
bool stack_empty(HStack &s)  
{  
    return 0 == s.top;  
}  
  
void init_queue(Queue &q)  
{  
    q.pHead = new QNode;  
    q.pHead->pNext = q.pHead;  
    q.pRear = nullptr;
}

void uninit_queue(Queue &q)
{
    QNode *pQNode = q.pHead->pNext;
    while(pQNode != q.pHead)
    {
        QNode *temp = pQNode;
        pQNode = pQNode->pNext;
        delete temp;
    }

    delete q.pHead;
    q.pHead = nullptr;
    q.pRear = nullptr;
}
  
void en_queue(Queue &q, ParkInfo &info)  
{  
    QNode *pQNode = new QNode;  
    pQNode->parkInfo = info;  
  
    pQNode->pNext = q.pHead;  
    if (nullptr == q.pRear)  
    {  
        q.pRear = q.pHead;  
        q.pHead->pNext = pQNode;  
    }  
    else  
    {  
        q.pRear->pNext = pQNode;  
    }  
}

void de_queue(Queue& q, ParkInfo &info)
{
    if (nullptr == q.pRear)
    {
        return;
    }

    QNode *pQNode = q.pHead->pNext;
    q.pHead->pNext = q.pHead->pNext->pNext;
    
    info = pQNode->parkInfo;
    if (pQNode == q.pRear)
    {
        q.pRear = nullptr;
    }
    delete pQNode;
}

bool queue_empty(Queue &q)
{
    return q.pRear == nullptr;
}

static size_t g_parkSize;
static double g_parkPrice;
  
void process(Queue &q, HStack &s, unsigned int flag, unsigned int id, unsigned int time)  
{  
    if (0 == flag)  
    {  
        ParkInfo info;  
        info.id = id;  
        info.inTime = time;  
        // 进入
        if (stack_full(s))  
        {  
            // 停车场满了  
            en_queue(q, info);  
        }  
        else  
        {  
            // 停车场未满  
            push_stack(s, info);  
        }  
    }  
    else  
    {  
        HStack tempStack;  
        init_stack(tempStack, stack_size(s));  
  
        ParkInfo destInfo;

        bool bInStack = false;

        while(!stack_empty(s))  
        {  
            pop_stack(s, destInfo);
            if (destInfo.id == id)
            {  
                bInStack = true;
                break;  
            }  
            push_stack(tempStack, destInfo);
        }
        
        ParkInfo tempInfo;
        while(!stack_empty(tempStack))  
        {  
            pop_stack(tempStack, tempInfo);  
            push_stack(s, tempInfo);  
        }

        uninit_stack(tempStack);
        
        if (bInStack)
        {
            de_queue(q, tempInfo);
            push_stack(s, tempInfo);
        }
        else
        {
            while(true)
            {
                de_queue(q, destInfo);
                if (destInfo.id == id)
                {
                    break;
                }
                en_queue(q, destInfo);
            }
        }

        destInfo.outTime = time;

        std::cout << destInfo.id << (destInfo.outTime - destInfo.inTime) * g_parkPrice << std::endl;
    }
}

void print_stack(HStack& s)
{
    unsigned int* aId = new unsigned int[stack_size(s)];
    ParkInfo info;

    size_t i = 0;
    while (!stack_empty(s))
    {
        pop_stack(s, info);
        aId[i++] = info.id;
    }

    while (i != 0)
    {
        std::cout << aId[i-- - 1] << ' ';
    }
    std::cout << std::endl;
}

void print_queue(Queue &q)
{
    ParkInfo info;
    while(!queue_empty(q))
    {
        de_queue(q, info);
        std::cout << info.id << ' ';
    }
    std::cout << std::endl;
}


int main(int argc, char **argv)  
{  
    std::cin >> g_parkSize >> g_parkPrice;  
    HStack stack;  
    init_stack(stack, g_parkSize);  
      
    Queue queue;  
    init_queue(queue);  
    while(true)  
    {  
        unsigned int flag, id, time;  
        std::cin >> flag >> id >> time;  
        if (0 == id)  
        {  
            break;  
        }  
        process(queue, stack, flag, id, time);  
    }

    print_stack(stack);
    uninit_stack(stack);

    print_queue(queue);
    uninit_queue(queue);
    return 0;  
}