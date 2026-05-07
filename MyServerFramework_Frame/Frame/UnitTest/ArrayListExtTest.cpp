#include "FrameHeader.h"
struct ArrayListExtTestImpl
{
    // ============================================================
    // ArrayListExtTest — ArrayList<Length,T> 全方法深度测试
    // STLExtTest 已覆盖：add(1值)/removeAt/clear/size/isEmpty/contains/operator[]/迭代器
    // ContainerTest 无 ArrayList 完整测试
    // 本文件覆盖所有未测试的方法：
    //   add(2~6值批量) / addNotEqual / addUnique / add(&&移动语义)
    //   remove / removeLast / remove
    //   isAll / addRange(ArrayList) / addRange(ptr,count) / setRange
    //   data() / data(offset) / get() / random() / setSize / resize
    //   zero() / setAll(val) / setAll(start,val) / maxSize / cbegin/cend
    // ============================================================

#define ArrayListExtTestImpl_ALE_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("ArrayListExtTest FAILED: ") + (msg)); }
#define ArrayListExtTestImpl_ALE_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("ArrayListExtTest FAILED: ") + (msg) + \
        " | got=" + IToS((int)(a))); }

// ============================================================
// add(2值) / add(3值) / add(4值) / add(5值) / add(6值)
// ============================================================
    static void testALE_AddMulti_Two()
    {
        ArrayList<16, int> al;
        bool r = al.add(10, 20);
        ArrayListExtTestImpl_ALE_ASSERT(r, "add(2) success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "add(2) size==2");
        ArrayListExtTestImpl_ALE_EQ(al[0], 10, "add(2) [0]==10");
        ArrayListExtTestImpl_ALE_EQ(al[1], 20, "add(2) [1]==20");
    }

    static void testALE_AddMulti_Three()
    {
        ArrayList<16, int> al;
        bool r = al.add(1, 2, 3);
        ArrayListExtTestImpl_ALE_ASSERT(r, "add(3) success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "add(3) size==3");
        ArrayListExtTestImpl_ALE_EQ(al[2], 3, "add(3) [2]==3");
    }

    static void testALE_AddMulti_Four()
    {
        ArrayList<16, int> al;
        bool r = al.add(10, 20, 30, 40);
        ArrayListExtTestImpl_ALE_ASSERT(r, "add(4) success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 4, "add(4) size==4");
        ArrayListExtTestImpl_ALE_EQ(al[3], 40, "add(4) [3]==40");
    }

    static void testALE_AddMulti_Five()
    {
        ArrayList<16, int> al;
        bool r = al.add(1, 2, 3, 4, 5);
        ArrayListExtTestImpl_ALE_ASSERT(r, "add(5) success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 5, "add(5) size==5");
        ArrayListExtTestImpl_ALE_EQ(al[4], 5, "add(5) [4]==5");
    }

    static void testALE_AddMulti_Six()
    {
        ArrayList<16, int> al;
        bool r = al.add(1, 2, 3, 4, 5, 6);
        ArrayListExtTestImpl_ALE_ASSERT(r, "add(6) success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 6, "add(6) size==6");
        ArrayListExtTestImpl_ALE_EQ(al[5], 6, "add(6) [5]==6");
    }

    static void testALE_AddMulti_Full()
    {
        ArrayList<4, int> al;
        bool r1 = al.add(1, 2);
        bool r2 = al.add(3, 4);
        ArrayListExtTestImpl_ALE_ASSERT(r1 && r2, "add two pairs success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 4, "two pairs size==4");
        // 容量已满，再加 2 个应失败
        bool r3 = al.add(5, 6);
        ArrayListExtTestImpl_ALE_ASSERT(!r3, "add(2) full fails");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 4, "size unchanged after fail");
    }

    static void testALE_AddMulti_OverflowThree()
    {
        ArrayList<5, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        // 剩余 2 个，add(3) 需要 3 个位置应失败
        bool r = al.add(4, 5, 6);
        ArrayListExtTestImpl_ALE_ASSERT(!r, "add(3) overflow fails");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "size unchanged after overflow");
    }

    // ============================================================
    // addNotEqual — 仅当 value != other 时才 add
    // ============================================================
    static void testALE_AddNotEqual_AddWhenDiff()
    {
        ArrayList<8, int> al;
        bool r = al.addNotEqual(5, 0);  // 5 != 0，添加
        ArrayListExtTestImpl_ALE_ASSERT(r, "addNotEqual adds when diff");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 1, "addNotEqual size==1");
        ArrayListExtTestImpl_ALE_EQ(al[0], 5, "addNotEqual [0]==5");
    }

    static void testALE_AddNotEqual_SkipWhenSame()
    {
        ArrayList<8, int> al;
        bool r = al.addNotEqual(5, 5);  // 相等，不添加
        ArrayListExtTestImpl_ALE_ASSERT(!r, "addNotEqual skips when same");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 0, "addNotEqual size==0");
    }

    static void testALE_AddNotEqual_Mixed()
    {
        ArrayList<8, int> al;
        al.addNotEqual(1, 0);  // adds 1
        al.addNotEqual(0, 0);  // skips
        al.addNotEqual(2, 0);  // adds 2
        al.addNotEqual(0, 0);  // skips
        al.addNotEqual(3, 0);  // adds 3
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "addNotEqual mixed size==3");
        ArrayListExtTestImpl_ALE_EQ(al[0], 1, "addNotEqual [0]==1");
        ArrayListExtTestImpl_ALE_EQ(al[1], 2, "addNotEqual [1]==2");
        ArrayListExtTestImpl_ALE_EQ(al[2], 3, "addNotEqual [2]==3");
    }

    static void testALE_AddNotEqual_Full()
    {
        ArrayList<2, int> al;
        al.addNotEqual(1, 0);
        al.addNotEqual(2, 0);
        bool r = al.addNotEqual(3, 0);  // full
        ArrayListExtTestImpl_ALE_ASSERT(!r, "addNotEqual full fails");
    }

    // ============================================================
    // addUnique — 不重复添加
    // ============================================================
    static void testALE_AddUnique_Basic()
    {
        ArrayList<8, int> al;
        bool r1 = al.addUnique(10);
        bool r2 = al.addUnique(20);
        bool r3 = al.addUnique(10);  // dup
        ArrayListExtTestImpl_ALE_ASSERT(r1, "addUnique 10 first true");
        ArrayListExtTestImpl_ALE_ASSERT(r2, "addUnique 20 true");
        ArrayListExtTestImpl_ALE_ASSERT(!r3, "addUnique 10 dup false");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "addUnique size==2");
    }

    static void testALE_AddUnique_Empty()
    {
        ArrayList<8, int> al;
        bool r = al.addUnique(0);
        ArrayListExtTestImpl_ALE_ASSERT(r, "addUnique to empty true");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 1, "addUnique size==1");
    }

    static void testALE_AddUnique_AllUnique()
    {
        ArrayList<10, int> al;
        for (int i = 0; i < 10; ++i)
        {
            bool r = al.addUnique(i);
            ArrayListExtTestImpl_ALE_ASSERT(r, "addUnique all unique");
        }
        ArrayListExtTestImpl_ALE_EQ(al.size(), 10, "addUnique all unique size==10");
    }

    static void testALE_AddUnique_Full()
    {
        ArrayList<3, int> al;
        al.addUnique(1);
        al.addUnique(2);
        al.addUnique(3);
        bool r = al.addUnique(4);  // full
        ArrayListExtTestImpl_ALE_ASSERT(!r, "addUnique full returns false");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "addUnique full size unchanged");
    }

    // ============================================================
    // add(T&&) — 移动语义
    // ============================================================
    static void testALE_AddMove_String()
    {
        ArrayList<8, string> al;
        string s1 = "moved_str1";
        string s2 = "moved_str2";
        bool r1 = al.add(move(s1));
        bool r2 = al.add(move(s2));
        ArrayListExtTestImpl_ALE_ASSERT(r1, "add(move) r1 true");
        ArrayListExtTestImpl_ALE_ASSERT(r2, "add(move) r2 true");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "add(move) size==2");
        ArrayListExtTestImpl_ALE_ASSERT(al[0] == "moved_str1", "add(move) [0]==moved_str1");
        ArrayListExtTestImpl_ALE_ASSERT(al[1] == "moved_str2", "add(move) [1]==moved_str2");
    }

    static void testALE_AddMove_Int()
    {
        ArrayList<4, int> al;
        int v = 99;
        al.add(move(v));
        ArrayListExtTestImpl_ALE_EQ(al[0], 99, "add(move int) [0]==99");
    }

    // ============================================================
    // remove — 删除所有匹配元素
    // ============================================================
    static void testALE_EraseElement_Single()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        al.removeAll(2);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "removeAll single size==2");
        ArrayListExtTestImpl_ALE_ASSERT(!al.contains(2), "removeAll single 2 gone");
        ArrayListExtTestImpl_ALE_EQ(al[0], 1, "removeAll [0]==1");
        ArrayListExtTestImpl_ALE_EQ(al[1], 3, "removeAll [1]==3");
    }

    static void testALE_EraseElement_Multiple()
    {
        ArrayList<16, int> al;
        al.add(1);
        al.add(2);
        al.add(1);
        al.add(3);
        al.add(1);
        al.removeAll(1);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "removeAll all 1s size==2");
        ArrayListExtTestImpl_ALE_ASSERT(!al.contains(1), "removeAll 1 gone all");
        ArrayListExtTestImpl_ALE_EQ(al[0], 2, "removeAll [0]==2");
        ArrayListExtTestImpl_ALE_EQ(al[1], 3, "removeAll [1]==3");
    }

    static void testALE_EraseElement_NotExist()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.removeAll(99);  // not found, no-op
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "removeAll notExist size unchanged");
    }

    static void testALE_EraseElement_AllSame()
    {
        ArrayList<8, int> al;
        for (int i = 0; i < 5; ++i)
        {
            al.add(7);
        }
        al.removeAll(7);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 0, "removeAll all same cleared");
        ArrayListExtTestImpl_ALE_ASSERT(al.isEmpty(), "removeAll all same isEmpty");
    }

    // ============================================================
    // removeLast — 删除最后一个匹配元素
    // ============================================================
    static void testALE_EraseLastElement_Basic()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        al.add(2);
        bool r = al.removeLast(2);
        ArrayListExtTestImpl_ALE_ASSERT(r, "removeLast success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "removeLast size==3");
        ArrayListExtTestImpl_ALE_EQ(al[1], 2, "removeLast first 2 still at [1]");
        ArrayListExtTestImpl_ALE_EQ(al[2], 3, "removeLast [2]==3");
    }

    static void testALE_EraseLastElement_NotExist()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        bool r = al.removeLast(99);
        ArrayListExtTestImpl_ALE_ASSERT(!r, "removeLast notExist false");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "removeLast size unchanged");
    }

    static void testALE_EraseLastElement_Single()
    {
        ArrayList<8, int> al;
        al.add(5);
        bool r = al.removeLast(5);
        ArrayListExtTestImpl_ALE_ASSERT(r, "removeLast single success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 0, "removeLast single size==0");
    }

    // ============================================================
    // remove — 删除第一个匹配元素
    // ============================================================
    static void testALE_EraseFirstElement_Basic()
    {
        ArrayList<8, int> al;
        al.add(2);
        al.add(1);
        al.add(2);
        al.add(3);
        bool r = al.remove(2);
        ArrayListExtTestImpl_ALE_ASSERT(r, "remove success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "remove size==3");
        ArrayListExtTestImpl_ALE_EQ(al[0], 1, "remove [0]==1");
        ArrayListExtTestImpl_ALE_EQ(al[1], 2, "remove second 2 still at [1]");
    }

    static void testALE_EraseFirstElement_NotExist()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        bool r = al.remove(99);
        ArrayListExtTestImpl_ALE_ASSERT(!r, "remove notExist false");
    }

    static void testALE_EraseFirstElement_OnlyOne()
    {
        ArrayList<8, int> al;
        al.add(42);
        bool r = al.remove(42);
        ArrayListExtTestImpl_ALE_ASSERT(r, "remove only one success");
        ArrayListExtTestImpl_ALE_ASSERT(al.isEmpty(), "remove only one isEmpty");
    }

    // ============================================================
    // isAll — 全部等于指定值
    // ============================================================
    static void testALE_IsAll_True()
    {
        ArrayList<8, int> al;
        al.add(5);
        al.add(5);
        al.add(5);
        ArrayListExtTestImpl_ALE_ASSERT(al.isAll(5), "isAll 5 true");
    }

    static void testALE_IsAll_False()
    {
        ArrayList<8, int> al;
        al.add(5);
        al.add(5);
        al.add(3);
        ArrayListExtTestImpl_ALE_ASSERT(!al.isAll(5), "isAll 5 false");
    }

    static void testALE_IsAll_Empty()
    {
        ArrayList<8, int> al;
        // 空列表，isAll 应返回 true（无元素则全部满足）
        ArrayListExtTestImpl_ALE_ASSERT(al.isAll(0), "isAll empty true");
    }

    static void testALE_IsAll_Single()
    {
        ArrayList<8, int> al;
        al.add(99);
        ArrayListExtTestImpl_ALE_ASSERT(al.isAll(99), "isAll single true");
        ArrayListExtTestImpl_ALE_ASSERT(!al.isAll(0), "isAll single false");
    }

    // ============================================================
    // addRange(ArrayList&) — 从另一 ArrayList 批量追加
    // ============================================================
    static void testALE_AddRange_ArrayList()
    {
        ArrayList<16, int> dst;
        dst.add(1);
        dst.add(2);
        ArrayList<8, int> src;
        src.add(3);
        src.add(4);
        src.add(5);
        bool r = dst.addRange(src);
        ArrayListExtTestImpl_ALE_ASSERT(r, "addRange(ArrayList) success");
        ArrayListExtTestImpl_ALE_EQ(dst.size(), 5, "addRange size==5");
        ArrayListExtTestImpl_ALE_EQ(dst[2], 3, "addRange [2]==3");
        ArrayListExtTestImpl_ALE_EQ(dst[4], 5, "addRange [4]==5");
    }

    static void testALE_AddRange_ArrayList_Empty()
    {
        ArrayList<8, int> dst;
        dst.add(1);
        ArrayList<4, int> src;  // empty
        bool r = dst.addRange(src);
        ArrayListExtTestImpl_ALE_ASSERT(r, "addRange empty src success");
        ArrayListExtTestImpl_ALE_EQ(dst.size(), 1, "addRange empty src size unchanged");
    }

    // ============================================================
    // addRange(ptr, count) — 从裸指针批量追加
    // ============================================================
    static void testALE_AddRange_Ptr()
    {
        ArrayList<16, int> dst;
        int src[] = { 10, 20, 30, 40, 50 };
        bool r = dst.addRange(src, 5);
        ArrayListExtTestImpl_ALE_ASSERT(r, "addRange(ptr,5) success");
        ArrayListExtTestImpl_ALE_EQ(dst.size(), 5, "addRange ptr size==5");
        ArrayListExtTestImpl_ALE_EQ(dst[0], 10, "addRange ptr [0]==10");
        ArrayListExtTestImpl_ALE_EQ(dst[4], 50, "addRange ptr [4]==50");
    }

    static void testALE_AddRange_Ptr_Partial()
    {
        ArrayList<16, int> dst;
        int src[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        bool r = dst.addRange(src, 4);  // only first 4
        ArrayListExtTestImpl_ALE_ASSERT(r, "addRange ptr partial success");
        ArrayListExtTestImpl_ALE_EQ(dst.size(), 4, "addRange ptr partial size==4");
        ArrayListExtTestImpl_ALE_EQ(dst[3], 4, "addRange ptr partial [3]==4");
    }

    static void testALE_AddRange_Ptr_AppendToExisting()
    {
        ArrayList<16, int> dst;
        dst.add(1);
        dst.add(2);
        int src[] = { 3, 4, 5 };
        bool r = dst.addRange(src, 3);
        ArrayListExtTestImpl_ALE_ASSERT(r, "addRange ptr append success");
        ArrayListExtTestImpl_ALE_EQ(dst.size(), 5, "addRange ptr append size==5");
        ArrayListExtTestImpl_ALE_EQ(dst[2], 3, "addRange ptr append [2]==3");
    }

    // ============================================================
    // setRange — 清空后 addRange
    // ============================================================
    static void testALE_SetRange_Basic()
    {
        ArrayList<16, int> al;
        al.add(100);
        al.add(200);
        int src[] = { 1, 2, 3 };
        bool r = al.setRange(src, 3);
        ArrayListExtTestImpl_ALE_ASSERT(r, "setRange success");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "setRange size==3");
        ArrayListExtTestImpl_ALE_EQ(al[0], 1, "setRange [0]==1");
        ArrayListExtTestImpl_ALE_EQ(al[2], 3, "setRange [2]==3");
    }

    static void testALE_SetRange_ClearsFirst()
    {
        ArrayList<16, int> al;
        for (int i = 0; i < 10; ++i)
        {
            al.add(i);
        }
        ArrayListExtTestImpl_ALE_EQ(al.size(), 10, "before setRange size==10");
        int src[] = { 99 };
        al.setRange(src, 1);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 1, "setRange clears then adds size==1");
        ArrayListExtTestImpl_ALE_EQ(al[0], 99, "setRange [0]==99");
    }

    // ============================================================
    // data() — 基础版本
    // ============================================================
    static void testALE_Data_Basic()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        const int* ptr = al.data();
        ArrayListExtTestImpl_ALE_ASSERT(ptr != nullptr, "data() non-null");
        ArrayListExtTestImpl_ALE_EQ(ptr[0], 1, "data()[0]==1");
        ArrayListExtTestImpl_ALE_EQ(ptr[1], 2, "data()[1]==2");
        ArrayListExtTestImpl_ALE_EQ(ptr[2], 3, "data()[2]==3");
    }

    static void testALE_Data_Mutable()
    {
        ArrayList<8, int> al;
        al.add(10);
        al.add(20);
        int* ptr = al.data();
        ptr[0] = 100;
        ArrayListExtTestImpl_ALE_EQ(al[0], 100, "data mutable modify [0]==100");
    }

    // ============================================================
    // data(offset) — 带偏移量版本
    // ============================================================
    static void testALE_Data_Offset()
    {
        ArrayList<8, int> al;
        for (int i = 0; i < 5; ++i)
        {
            al.add(i * 10);
        }
        const int* p = al.data(2);
        ArrayListExtTestImpl_ALE_ASSERT(p != nullptr, "data(2) non-null");
        ArrayListExtTestImpl_ALE_EQ(p[0], 20, "data(2)[0]==20");
        ArrayListExtTestImpl_ALE_EQ(p[1], 30, "data(2)[1]==30");
        ArrayListExtTestImpl_ALE_EQ(p[2], 40, "data(2)[2]==40");
    }

    static void testALE_Data_Offset_Zero()
    {
        ArrayList<4, int> al;
        al.add(5);
        al.add(6);
        const int* p = al.data(0);
        ArrayListExtTestImpl_ALE_ASSERT(p != nullptr, "data(0) non-null");
        ArrayListExtTestImpl_ALE_EQ(p[0], 5, "data(0)[0]==5");
    }

    // ============================================================
    // get() — 无边界检查版本
    // ============================================================
    static void testALE_Get_Basic()
    {
        ArrayList<8, int> al;
        al.add(10);
        al.add(20);
        al.add(30);
        ArrayListExtTestImpl_ALE_EQ(al.get(0), 10, "get(0)==10");
        ArrayListExtTestImpl_ALE_EQ(al.get(1), 20, "get(1)==20");
        ArrayListExtTestImpl_ALE_EQ(al.get(2), 30, "get(2)==30");
    }

    static void testALE_Get_OutOfBounds()
    {
        ArrayList<4, int> al;
        al.add(42);
        // get() 越界返回 mDefault（int 的 0）
        int v = al.get(99);
        ArrayListExtTestImpl_ALE_EQ(v, 0, "get(OOB) returns default 0");
    }

    static void testALE_Get_Negative()
    {
        ArrayList<4, int> al;
        al.add(99);
        int v = al.get(-1);
        ArrayListExtTestImpl_ALE_EQ(v, 0, "get(-1) returns default 0");
    }

    // ============================================================
    // random() — 随机返回一个元素（单元素验证确定性）
    // ============================================================
    static void testALE_Random_Single()
    {
        ArrayList<4, int> al;
        al.add(42);
        ArrayListExtTestImpl_ALE_EQ(al.random(), 42, "random single==42");
    }

    static void testALE_Random_Multiple()
    {
        ArrayList<4, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        // 多次调用，结果都应在集合内
        for (int i = 0; i < 20; ++i)
        {
            int v = al.random();
            ArrayListExtTestImpl_ALE_ASSERT(v >= 1 && v <= 3, "random in [1,3]");
        }
    }

    static void testALE_Random_Empty()
    {
        ArrayList<4, int> al;
        int v = al.random();
        ArrayListExtTestImpl_ALE_EQ(v, 0, "random empty returns default 0");
    }

    // ============================================================
    // setSize — 扩展到指定大小（用默认值填充）
    // ============================================================
    static void testALE_SetSize_Expand()
    {
        ArrayList<16, int> al;
        al.add(1);
        al.add(2);
        al.setSize(5);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 5, "setSize(5) size==5");
        ArrayListExtTestImpl_ALE_EQ(al[0], 1, "setSize [0]==1 unchanged");
        ArrayListExtTestImpl_ALE_EQ(al[1], 2, "setSize [1]==2 unchanged");
        ArrayListExtTestImpl_ALE_EQ(al[2], 0, "setSize [2]==default 0");
        ArrayListExtTestImpl_ALE_EQ(al[4], 0, "setSize [4]==default 0");
    }

    static void testALE_SetSize_SameSize()
    {
        ArrayList<8, int> al;
        al.add(10);
        al.add(20);
        al.setSize(2);  // same size, no-op
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "setSize same size unchanged");
    }

    static void testALE_SetSize_Smaller()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        al.setSize(1);  // smaller than current, no effect per implementation
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "setSize smaller no effect");
    }

    // ============================================================
    // resize — 直接设置 mSize
    // ============================================================
    static void testALE_Resize_Up()
    {
        ArrayList<16, int> al;
        al.add(1);
        al.add(2);
        al.resize(8);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 8, "resize(8) size==8");
        // 扩展部分仍是内存中的原有值（可能是 0）
    }

    static void testALE_Resize_Down()
    {
        ArrayList<8, int> al;
        for (int i = 0; i < 6; ++i)
        {
            al.add(i);
        }
        al.resize(3);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 3, "resize(3) size==3");
        ArrayListExtTestImpl_ALE_EQ(al[0], 0, "resize down [0]==0");
        ArrayListExtTestImpl_ALE_EQ(al[2], 2, "resize down [2]==2");
    }

    static void testALE_Resize_Zero()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.resize(0);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 0, "resize(0) size==0");
        ArrayListExtTestImpl_ALE_ASSERT(al.isEmpty(), "resize(0) isEmpty");
    }

    // ============================================================
    // zero() — 全部置零（适用于 POD 类型）
    // ============================================================
    static void testALE_Zero_Int()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        al.zero();
        // zero 只清零内存，不改变 mSize
        for (int i = 0; i < 8; ++i)
        {
            ArrayListExtTestImpl_ALE_EQ(al.mValue[i], 0, "zero int all zero");
        }
    }

    static void testALE_Zero_Float()
    {
        ArrayList<4, float> al;
        al.add(1.5f);
        al.add(2.5f);
        al.zero();
        ArrayListExtTestImpl_ALE_ASSERT(fabsf(al.mValue[0]) < 1e-7f, "zero float [0]==0");
        ArrayListExtTestImpl_ALE_ASSERT(fabsf(al.mValue[1]) < 1e-7f, "zero float [1]==0");
    }

    // ============================================================
    // setAll(val) — 全部填充为指定值
    // ============================================================
    static void testALE_FillArray_Int()
    {
        ArrayList<8, int> al;
        al.setAll(99);
        for (int i = 0; i < 8; ++i)
        {
            ArrayListExtTestImpl_ALE_EQ(al.mValue[i], 99, "setAll 99");
        }
    }

    static void testALE_FillArray_Float()
    {
        ArrayList<4, float> al;
        al.setAll(3.14f);
        for (int i = 0; i < 4; ++i)
        {
            ArrayListExtTestImpl_ALE_ASSERT(fabsf(al.mValue[i] - 3.14f) < 1e-4f, "setAll 3.14f");
        }
    }

    static void testALE_FillArray_AfterAdd()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.setAll(0);
        for (int i = 0; i < 8; ++i)
        {
            ArrayListExtTestImpl_ALE_EQ(al.mValue[i], 0, "setAll after add all 0");
        }
        // mSize 未变（setAll 不修改 size）
        ArrayListExtTestImpl_ALE_EQ(al.size(), 2, "setAll size unchanged");
    }

    // ============================================================
    // setAll(start, val) — 从 start 开始填充
    // ============================================================
    static void testALE_FillArrayStart_Mid()
    {
        ArrayList<8, int> al;
        al.setAll(1);
        al.setAll(4, 9);
        for (int i = 0; i < 4; ++i)
        {
            ArrayListExtTestImpl_ALE_EQ(al.mValue[i], 1, "setAll(4,9) first 4 == 1");
        }
        for (int i = 4; i < 8; ++i)
        {
            ArrayListExtTestImpl_ALE_EQ(al.mValue[i], 9, "setAll(4,9) last 4 == 9");
        }
    }

    static void testALE_FillArrayStart_Zero()
    {
        ArrayList<8, int> al;
        al.setAll(5);
        al.setAll(0, 7);  // from start, replaces all
        for (int i = 0; i < 8; ++i)
        {
            ArrayListExtTestImpl_ALE_EQ(al.mValue[i], 7, "setAll(0,7) all 7");
        }
    }

    // ============================================================
    // maxSize / cbegin / cend
    // ============================================================
    static void testALE_MaxSize()
    {
        ArrayList<16, int> al;
        ArrayListExtTestImpl_ALE_EQ(al.maxSize(), 16, "maxSize==16");
        ArrayList<128, int> al2;
        ArrayListExtTestImpl_ALE_EQ(al2.maxSize(), 128, "maxSize==128");
    }

    static void testALE_CBeginCEnd()
    {
        ArrayList<8, int> al;
        al.add(10);
        al.add(20);
        al.add(30);
        const ArrayList<8, int>& cal = al;
        int sum = 0;
        for (auto it = cal.cbegin(); it != cal.cend(); ++it)
        {
            sum += *it;
        }
        ArrayListExtTestImpl_ALE_EQ(sum, 60, "cbegin/cend sum==60");
    }

    static void testALE_BeginEnd_Mutable()
    {
        ArrayList<8, int> al;
        al.add(1);
        al.add(2);
        al.add(3);
        for (int* it = al.begin(); it != al.end(); ++it)
        {
            *it *= 2;
        }
        ArrayListExtTestImpl_ALE_EQ(al[0], 2, "begin/end mutable [0]==2");
        ArrayListExtTestImpl_ALE_EQ(al[1], 4, "begin/end mutable [1]==4");
        ArrayListExtTestImpl_ALE_EQ(al[2], 6, "begin/end mutable [2]==6");
    }

    // ============================================================
    // 综合场景：模拟批量处理
    // ============================================================
    static void testALE_BatchProcess()
    {
        ArrayList<32, int> al;
        // 批量添加
        int batch[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        al.addRange(batch, 8);
        ArrayListExtTestImpl_ALE_EQ(al.size(), 8, "batch size==8");

        // 移除所有偶数
        ArrayList<16, int> toRemove;
        for (int i = 0; i < al.size(); ++i)
        {
            if (al[i] % 2 == 0)
            {
                toRemove.add(al[i]);
            }
        }
        for (int i = 0; i < toRemove.size(); ++i)
        {
            al.removeAll(toRemove[i]);
        }
        // 只剩奇数：1,3,5,7
        ArrayListExtTestImpl_ALE_EQ(al.size(), 4, "batch after erase evens size==4");
        ArrayListExtTestImpl_ALE_ASSERT(al.isAll(1) || !al.isAll(1), "isAll check not crash");
    }

    static void testALE_SortLikePattern()
    {
        ArrayList<16, int> al;
        al.add(5);
        al.add(3);
        al.add(8);
        al.add(1);
        al.add(9);
        al.add(2);
        // 手工冒泡排序
        for (int i = 0; i < al.size() - 1; ++i)
        {
            for (int j = 0; j < al.size() - 1 - i; ++j)
            {
                if (al[j] > al[j + 1])
                {
                    int tmp = al[j];
                    al[j] = al[j + 1];
                    al[j + 1] = tmp;
                }
            }
        }
        ArrayListExtTestImpl_ALE_EQ(al[0], 1, "sort [0]==1");
        ArrayListExtTestImpl_ALE_EQ(al[5], 9, "sort [5]==9");
    }

    static void testALE_AddRangeFromPtr_ThenSearch()
    {
        ArrayList<32, int> al;
        int data[] = { 10, 20, 30, 40, 50, 60, 70, 80 };
        al.addRange(data, 8);
        ArrayListExtTestImpl_ALE_ASSERT(al.contains(50), "contains 50");
        ArrayListExtTestImpl_ALE_ASSERT(!al.contains(99), "not contains 99");
        al.removeAll(50);
        ArrayListExtTestImpl_ALE_ASSERT(!al.contains(50), "after erase 50 gone");
        ArrayListExtTestImpl_ALE_EQ(al.size(), 7, "after erase size==7");
    }

    // ============================================================
    // 主入口
    // ============================================================
    static void test()
    {
        testALE_AddMulti_Two();
        testALE_AddMulti_Three();
        testALE_AddMulti_Four();
        testALE_AddMulti_Five();
        testALE_AddMulti_Six();
        testALE_AddMulti_Full();
        testALE_AddMulti_OverflowThree();
        testALE_AddNotEqual_AddWhenDiff();
        testALE_AddNotEqual_SkipWhenSame();
        testALE_AddNotEqual_Mixed();
        testALE_AddNotEqual_Full();
        testALE_AddUnique_Basic();
        testALE_AddUnique_Empty();
        testALE_AddUnique_AllUnique();
        testALE_AddUnique_Full();
        testALE_AddMove_String();
        testALE_AddMove_Int();
        testALE_EraseElement_Single();
        testALE_EraseElement_Multiple();
        testALE_EraseElement_NotExist();
        testALE_EraseElement_AllSame();
        testALE_EraseLastElement_Basic();
        testALE_EraseLastElement_NotExist();
        testALE_EraseLastElement_Single();
        testALE_EraseFirstElement_Basic();
        testALE_EraseFirstElement_NotExist();
        testALE_EraseFirstElement_OnlyOne();
        testALE_IsAll_True();
        testALE_IsAll_False();
        testALE_IsAll_Empty();
        testALE_IsAll_Single();
        testALE_AddRange_ArrayList();
        testALE_AddRange_ArrayList_Empty();
        testALE_AddRange_Ptr();
        testALE_AddRange_Ptr_Partial();
        testALE_AddRange_Ptr_AppendToExisting();
        testALE_SetRange_Basic();
        testALE_SetRange_ClearsFirst();
        testALE_Data_Basic();
        testALE_Data_Mutable();
        testALE_Data_Offset();
        testALE_Data_Offset_Zero();
        testALE_Get_Basic();
        testALE_Get_OutOfBounds();
        testALE_Get_Negative();
        testALE_Random_Single();
        testALE_Random_Multiple();
        testALE_Random_Empty();
        testALE_SetSize_Expand();
        testALE_SetSize_SameSize();
        testALE_SetSize_Smaller();
        testALE_Resize_Up();
        testALE_Resize_Down();
        testALE_Resize_Zero();
        testALE_Zero_Int();
        testALE_Zero_Float();
        testALE_FillArray_Int();
        testALE_FillArray_Float();
        testALE_FillArray_AfterAdd();
        testALE_FillArrayStart_Mid();
        testALE_FillArrayStart_Zero();
        testALE_MaxSize();
        testALE_CBeginCEnd();
        testALE_BeginEnd_Mutable();
        testALE_BatchProcess();
        testALE_SortLikePattern();
        testALE_AddRangeFromPtr_ThenSearch();
    }
};

struct ArrayListExt2TestImpl
{
#define ArrayListExt2TestImpl_ALE2_ASSERT(expr) \
	if (!(expr)) { ERROR(string("ArrayListExt2Test FAILED: ") + (#expr)); }

    // ========== removeAt ==========

    static void testALE2_EraseAtBasic()
    {
        ArrayList<8, int> a;
        a.add(10);
        a.add(20);
        a.add(30);
        a.removeAt(1);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 10);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 30);
    }

    static void testALE2_EraseAtFirst()
    {
        ArrayList<8, int> a;
        a.add(1);
        a.add(2);
        a.add(3);
        a.removeAt(0);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 3);
    }

    static void testALE2_EraseAtLast()
    {
        ArrayList<8, int> a;
        a.add(100);
        a.add(200);
        a.add(300);
        a.removeAt(2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 100);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 200);
    }

    // ========== contains ==========

    static void testALE2_ContainsTrue()
    {
        ArrayList<8, int> a;
        a.add(5);
        a.add(10);
        a.add(15);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.contains(5));
        ArrayListExt2TestImpl_ALE2_ASSERT(a.contains(10));
        ArrayListExt2TestImpl_ALE2_ASSERT(a.contains(15));
    }

    static void testALE2_ContainsFalse()
    {
        ArrayList<8, int> a;
        a.add(5);
        a.add(10);
        ArrayListExt2TestImpl_ALE2_ASSERT(!a.contains(3));
        ArrayListExt2TestImpl_ALE2_ASSERT(!a.contains(100));
    }

    static void testALE2_ContainsEmpty()
    {
        ArrayList<8, int> a;
        ArrayListExt2TestImpl_ALE2_ASSERT(!a.contains(0));
        ArrayListExt2TestImpl_ALE2_ASSERT(!a.contains(42));
    }

    // ========== maxSize ==========

    static void testALE2_MaxSize()
    {
        ArrayList<16, int> a;
        ArrayListExt2TestImpl_ALE2_ASSERT(a.maxSize() == 16);
        ArrayList<4, float> b;
        ArrayListExt2TestImpl_ALE2_ASSERT(b.maxSize() == 4);
        ArrayList<100, short> c;
        ArrayListExt2TestImpl_ALE2_ASSERT(c.maxSize() == 100);
    }

    // ========== clear and reuse ==========

    static void testALE2_ClearAndReuse()
    {
        ArrayList<8, int> a;
        a.add(1);
        a.add(2);
        a.add(3);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 3);
        a.clear();
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 0);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.isEmpty());
        a.add(99);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 1);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 99);
    }

    // ========== multi-argument add ==========

    static void testALE2_AddSixElements()
    {
        ArrayList<8, int> a;
        bool ok = a.add(1, 2, 3, 4, 5, 6);
        ArrayListExt2TestImpl_ALE2_ASSERT(ok);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 6);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 1);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[5] == 6);
    }

    static void testALE2_AddFiveElements()
    {
        ArrayList<8, int> a;
        bool ok = a.add(10, 20, 30, 40, 50);
        ArrayListExt2TestImpl_ALE2_ASSERT(ok);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 5);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[4] == 50);
    }

    static void testALE2_AddFourElements()
    {
        ArrayList<8, int> a;
        bool ok = a.add(1, 2, 3, 4);
        ArrayListExt2TestImpl_ALE2_ASSERT(ok);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 4);
    }

    static void testALE2_AddThreeElements()
    {
        ArrayList<8, int> a;
        bool ok = a.add(11, 22, 33);
        ArrayListExt2TestImpl_ALE2_ASSERT(ok);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 3);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[2] == 33);
    }

    static void testALE2_AddTwoElements()
    {
        ArrayList<8, int> a;
        bool ok = a.add(7, 8);
        ArrayListExt2TestImpl_ALE2_ASSERT(ok);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 7);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 8);
    }

    // ========== addNotEqual ==========

    static void testALE2_AddNotEqualMixed()
    {
        ArrayList<8, int> a;
        // addNotEqual(val, other): add val only if val != other
        a.addNotEqual(5, 3);
        a.addNotEqual(3, 3);
        a.addNotEqual(7, 0);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 5);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 7);
    }

    // ========== addUnique ==========

    static void testALE2_AddUniqueAllSame()
    {
        ArrayList<8, int> a;
        a.addUnique(5);
        a.addUnique(5);
        a.addUnique(5);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 1);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 5);
    }

    static void testALE2_AddUniqueAllDifferent()
    {
        ArrayList<8, int> a;
        a.addUnique(1);
        a.addUnique(2);
        a.addUnique(3);
        a.addUnique(4);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 4);
    }

    // ========== get out-of-bounds default ==========

    static void testALE2_GetOutOfBoundsDefault()
    {
        ArrayList<8, int> a;
        a.add(10);
        // get with out-of-bounds index returns mDefault (0 for int)
        int v = a.get(99);
        ArrayListExt2TestImpl_ALE2_ASSERT(v == 0);
        int v2 = a.get(-1);
        ArrayListExt2TestImpl_ALE2_ASSERT(v2 == 0);
    }

    // ========== random single element ==========

    static void testALE2_RandomSingleElem()
    {
        ArrayList<8, int> a;
        a.add(42);
        int v = a.random();
        ArrayListExt2TestImpl_ALE2_ASSERT(v == 42);
    }

    // ========== operator[] write ==========

    static void testALE2_OperatorBracketWrite()
    {
        ArrayList<8, int> a;
        a.add(1);
        a.add(2);
        a.add(3);
        a[1] = 99;
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 99);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 1);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[2] == 3);
    }

    // ========== isAll ==========

    static void testALE2_IsAllMixed()
    {
        ArrayList<8, int> a;
        a.add(5);
        a.add(5);
        a.add(5);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.isAll(5));
        a.add(6);
        ArrayListExt2TestImpl_ALE2_ASSERT(!a.isAll(5));
    }

    // ========== addRange from same-type ==========

    static void testALE2_AddRangeFromSelf()
    {
        ArrayList<8, int> src;
        src.add(10);
        src.add(20);
        ArrayList<16, int> dst;
        dst.add(1);
        dst.addRange(src);
        ArrayListExt2TestImpl_ALE2_ASSERT(dst.size() == 3);
        ArrayListExt2TestImpl_ALE2_ASSERT(dst[0] == 1);
        ArrayListExt2TestImpl_ALE2_ASSERT(dst[1] == 10);
        ArrayListExt2TestImpl_ALE2_ASSERT(dst[2] == 20);
    }

    // ========== setRange clears first ==========

    static void testALE2_SetRangeClearsFirst()
    {
        ArrayList<8, int> a;
        a.add(1);
        a.add(2);
        a.add(3);
        int arr[] = { 99, 88 };
        a.setRange(arr, 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a.size() == 2);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[0] == 99);
        ArrayListExt2TestImpl_ALE2_ASSERT(a[1] == 88);
    }

    // ========== setAll partial ==========

    static void testALE2_FillArrayPartial()
    {
        ArrayList<8, int> a;
        a.resize(8);
        a.setAll(2, -1);
        for (int i = 2; i < 8; ++i)
        {
            ArrayListExt2TestImpl_ALE2_ASSERT(a[i] == -1);
        }
    }

    // ========== iterator mutable ==========

    static void testALE2_IteratorMutable()
    {
        ArrayList<8, int> a;
        a.add(1);
        a.add(2);
        a.add(3);
        int sum = 0;
        for (int* it = a.begin(); it != a.end(); ++it)
        {
            sum += *it;
        }
        ArrayListExt2TestImpl_ALE2_ASSERT(sum == 6);
    }

    static void test()
    {
        testALE2_EraseAtBasic();
        testALE2_EraseAtFirst();
        testALE2_EraseAtLast();
        testALE2_ContainsTrue();
        testALE2_ContainsFalse();
        testALE2_ContainsEmpty();
        testALE2_MaxSize();
        testALE2_ClearAndReuse();
        testALE2_AddSixElements();
        testALE2_AddFiveElements();
        testALE2_AddFourElements();
        testALE2_AddThreeElements();
        testALE2_AddTwoElements();
        testALE2_AddNotEqualMixed();
        testALE2_AddUniqueAllSame();
        testALE2_AddUniqueAllDifferent();
        testALE2_GetOutOfBoundsDefault();
        testALE2_RandomSingleElem();
        testALE2_OperatorBracketWrite();
        testALE2_IsAllMixed();
        testALE2_AddRangeFromSelf();
        testALE2_SetRangeClearsFirst();
        testALE2_FillArrayPartial();
        testALE2_IteratorMutable();
    }
};

void ArrayListExtTest::test()
{
    ArrayListExtTestImpl::test();
    ArrayListExt2TestImpl::test();
}