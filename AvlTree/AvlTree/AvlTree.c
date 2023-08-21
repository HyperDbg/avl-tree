#include <Windows.h>
#include <stdio.h>

/**
 * @brief Maximum number of array for test cases
 *
 */
#define MAX_AVL_TEST_NODES 200

/**
 * @brief AVL Node structure
 *
 */
typedef struct _OPTIMIZATION_AVL_NODE
{
    UINT64 Data;
    UINT32 Left, Right;
    UINT32 Height;
} OPTIMIZATION_AVL_NODE, *POPTIMIZATION_AVL_NODE;

/**
 * @brief AVL context
 *
 */
typedef struct _OPTIMIZATION_AVL_CONTEXT
{
    OPTIMIZATION_AVL_NODE Tree[MAX_AVL_TEST_NODES];
    UINT32                Root; // Use a value that is not a valid index
    UINT32                FreeIndex;

} OPTIMIZATION_AVL_CONTEXT, *POPTIMIZATION_AVL_CONTEXT;

/**
 * @brief Initialize an empty AVL tree
 * @param AvlContext
 *
 * @return VOID
 */
VOID
AvlTreeInit(OPTIMIZATION_AVL_CONTEXT * AvlContext)
{
    AvlContext->Root      = MAX_AVL_TEST_NODES;
    AvlContext->FreeIndex = 0;
}

/**
 * @brief Get the height of a node
 * @param AvlContext
 * @param Index
 *
 * @return UINT32
 */
UINT32
AvlTreeGetHeight(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    if (Index == MAX_AVL_TEST_NODES)
    {
        return 0;
    }
    return AvlContext->Tree[Index].Height;
}

/**
 * @brief Update height of a node
 * @param AvlContext
 * @param Index
 *
 * @return VOID
 */
VOID
AvlTreeUpdateHeight(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    AvlContext->Tree[Index].Height =
        1 + ((AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Left) >
              AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Right))
                 ? AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Left)
                 : AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Right));
}

/**
 * @brief Rotate node to the left
 * @param AvlContext
 * @param Index
 *
 * @return UINT32
 */
UINT32
AvlTreeRotateLeft(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    UINT32 NewRoot                 = AvlContext->Tree[Index].Right;
    AvlContext->Tree[Index].Right  = AvlContext->Tree[NewRoot].Left;
    AvlContext->Tree[NewRoot].Left = Index;
    AvlTreeUpdateHeight(AvlContext, Index);
    AvlTreeUpdateHeight(AvlContext, NewRoot);
    return NewRoot;
}

/**
 * @brief Rotate node to the right
 * @param AvlContext
 * @param Index
 *
 * @return UINT32
 */
UINT32
AvlTreeRotateRight(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    UINT32 NewRoot                  = AvlContext->Tree[Index].Left;
    AvlContext->Tree[Index].Left    = AvlContext->Tree[NewRoot].Right;
    AvlContext->Tree[NewRoot].Right = Index;
    AvlTreeUpdateHeight(AvlContext, Index);
    AvlTreeUpdateHeight(AvlContext, NewRoot);
    return NewRoot;
}

/**
 * @brief Balance the AVL tree
 * @param AvlContext
 * @param Index
 *
 * @return UINT32
 */
UINT32
AvlTreeBalance(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    if (AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Left) >
        AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Right) + 1)
    {
        if (AvlTreeGetHeight(AvlContext,
                             AvlContext->Tree[AvlContext->Tree[Index].Left].Left) >=
            AvlTreeGetHeight(
                AvlContext,
                AvlContext->Tree[AvlContext->Tree[Index].Left].Right))
        {
            Index = AvlTreeRotateRight(AvlContext, Index);
        }
        else
        {
            AvlContext->Tree[Index].Left =
                AvlTreeRotateLeft(AvlContext, AvlContext->Tree[Index].Left);
            Index = AvlTreeRotateRight(AvlContext, Index);
        }
    }
    else if (AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Right) >
             AvlTreeGetHeight(AvlContext, AvlContext->Tree[Index].Left) + 1)
    {
        if (AvlTreeGetHeight(
                AvlContext,
                AvlContext->Tree[AvlContext->Tree[Index].Right].Right) >=
            AvlTreeGetHeight(
                AvlContext,
                AvlContext->Tree[AvlContext->Tree[Index].Right].Left))
        {
            Index = AvlTreeRotateLeft(AvlContext, Index);
        }
        else
        {
            AvlContext->Tree[Index].Right =
                AvlTreeRotateRight(AvlContext, AvlContext->Tree[Index].Right);
            Index = AvlTreeRotateLeft(AvlContext, Index);
        }
    }
    return Index;
}

/**
 * @brief Insert a new element into the AVL tree
 *
 * @param AvlContext
 * @param Index
 * @param Data
 *
 * @return UINT32
 */
UINT32
AvlTreeInsertItem(OPTIMIZATION_AVL_CONTEXT * AvlContext,
                  UINT32                     Index,
                  UINT64                     Data)
{
    if (Index == MAX_AVL_TEST_NODES)
    {
        AvlContext->Tree[AvlContext->FreeIndex].Data   = Data;
        AvlContext->Tree[AvlContext->FreeIndex].Left   = MAX_AVL_TEST_NODES;
        AvlContext->Tree[AvlContext->FreeIndex].Right  = MAX_AVL_TEST_NODES;
        AvlContext->Tree[AvlContext->FreeIndex].Height = 1;
        return AvlContext->FreeIndex++;
    }

    if (Data < AvlContext->Tree[Index].Data)
    {
        AvlContext->Tree[Index].Left =
            AvlTreeInsertItem(AvlContext, AvlContext->Tree[Index].Left, Data);
    }
    else
    {
        AvlContext->Tree[Index].Right =
            AvlTreeInsertItem(AvlContext, AvlContext->Tree[Index].Right, Data);
    }
    AvlTreeUpdateHeight(AvlContext, Index);

    return AvlTreeBalance(AvlContext, Index);
}

/**
 * @brief Find the node with the minimum value in the subtree rooted at index
 *
 * @param AvlContext
 * @param Index
 *
 * @return UINT32
 */
UINT32
AvlTreeFindMin(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    if (Index == MAX_AVL_TEST_NODES ||
        AvlContext->Tree[Index].Left == MAX_AVL_TEST_NODES)
    {
        return Index;
    }
    return AvlTreeFindMin(AvlContext, AvlContext->Tree[Index].Left);
}

/**
 * @brief Delete a node with the given data from the AVL tree
 *
 * @param AvlContext
 * @param Index
 * @param Data
 *
 * @return UINT32
 */
UINT32
AvlTreeDeleteItem(OPTIMIZATION_AVL_CONTEXT * AvlContext,
                  UINT32                     Index,
                  UINT64                     Data)
{
    if (Index == MAX_AVL_TEST_NODES)
    {
        return MAX_AVL_TEST_NODES;
    }

    if (Data < AvlContext->Tree[Index].Data)
    {
        AvlContext->Tree[Index].Left =
            AvlTreeDeleteItem(AvlContext, AvlContext->Tree[Index].Left, Data);
    }
    else if (Data > AvlContext->Tree[Index].Data)
    {
        AvlContext->Tree[Index].Right =
            AvlTreeDeleteItem(AvlContext, AvlContext->Tree[Index].Right, Data);
    }
    else
    {
        if (AvlContext->Tree[Index].Left == MAX_AVL_TEST_NODES ||
            AvlContext->Tree[Index].Right == MAX_AVL_TEST_NODES)
        {
            UINT32 temp = (AvlContext->Tree[Index].Left != MAX_AVL_TEST_NODES)
                              ? AvlContext->Tree[Index].Left
                              : AvlContext->Tree[Index].Right;
            if (temp == MAX_AVL_TEST_NODES)
            {
                temp  = Index;
                Index = MAX_AVL_TEST_NODES;
            }
            else
            {
                AvlContext->Tree[Index] = AvlContext->Tree[temp];
            }
            AvlContext->FreeIndex--;
        }
        else
        {
            UINT32 temp                   = AvlTreeFindMin(AvlContext, AvlContext->Tree[Index].Right);
            AvlContext->Tree[Index].Data  = AvlContext->Tree[temp].Data;
            AvlContext->Tree[Index].Right = AvlTreeDeleteItem(
                AvlContext,
                AvlContext->Tree[Index].Right,
                AvlContext->Tree[temp].Data);
        }
    }

    if (Index == MAX_AVL_TEST_NODES)
    {
        return Index;
    }

    AvlTreeUpdateHeight(AvlContext, Index);
    return AvlTreeBalance(AvlContext, Index);
}

/**
 * @brief Print the AVL tree in-order
 *
 * @param AvlContext
 * @param Index
 *
 * @return VOID
 */
VOID
AvlTreePrintInOrder(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT32 Index)
{
    if (Index == MAX_AVL_TEST_NODES)
    {
        return;
    }
    AvlTreePrintInOrder(AvlContext, AvlContext->Tree[Index].Left);
    printf("%lld ", AvlContext->Tree[Index].Data);
    AvlTreePrintInOrder(AvlContext, AvlContext->Tree[Index].Right);
}

/**
 * @brief Find the target item in AVL tree
 *
 * @param AvlContext
 * @param Data
 *
 * @return UINT32
 */
UINT32
AvlTreeFindIndex(OPTIMIZATION_AVL_CONTEXT * AvlContext, UINT64 Data)
{
    UINT32 Index = AvlContext->Root;

    while (Index != MAX_AVL_TEST_NODES)
    {
        printf("%d->", Index);
        if (Data < AvlContext->Tree[Index].Data)
        {
            Index = AvlContext->Tree[Index].Left;
        }
        else if (Data > AvlContext->Tree[Index].Data)
        {
            Index = AvlContext->Tree[Index].Right;
        }
        else
        {
            break;
        }
    }
    return Index;
}

/**
 * @brief Example of using AVL tree insertion and deletion
 *
 * @param AvlContext
 * @param Data
 *
 * @return UINT32
 */
int
main()
{
    OPTIMIZATION_AVL_CONTEXT AvlContext = {0};

    //
    // Initialize AVL tree
    //
    AvlTreeInit(&AvlContext);

    //
    // Insert elements into the AVL tree
    //
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 70);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 2);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 33);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 50);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 45);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 25);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 5);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 100);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 150);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 115);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 35);
    AvlContext.Root = AvlTreeInsertItem(&AvlContext, AvlContext.Root, 80);

    printf("\n......................................\n");

    //
    // Print the AVL tree in-order
    //
    printf("In-order traversal: ");
    AvlTreePrintInOrder(&AvlContext, AvlContext.Root);

    printf("\n......................................\n");
    printf("index50 : %d \n", AvlTreeFindIndex(&AvlContext, 50));
    printf("index80 : %d \n", AvlTreeFindIndex(&AvlContext, 80));
    printf("index33 : %d \n", AvlTreeFindIndex(&AvlContext, 33));
    printf("index2 : %d \n", AvlTreeFindIndex(&AvlContext, 2));
    printf("index35 : %d \n", AvlTreeFindIndex(&AvlContext, 35));
    printf("index70 : %d \n", AvlTreeFindIndex(&AvlContext, 70));
    printf("index115 : %d \n", AvlTreeFindIndex(&AvlContext, 115));
    printf("index5 : %d \n", AvlTreeFindIndex(&AvlContext, 5));
    printf("index25 : %d \n", AvlTreeFindIndex(&AvlContext, 25));
    printf("index150 : %d \n", AvlTreeFindIndex(&AvlContext, 150));
    printf("index100 : %d \n", AvlTreeFindIndex(&AvlContext, 100));
    printf("index4 : %d \n", AvlTreeFindIndex(&AvlContext, 4));
    printf("index47 : %d \n", AvlTreeFindIndex(&AvlContext, 47));
    printf("index45 : %d \n", AvlTreeFindIndex(&AvlContext, 45));
    printf("index50 : %d \n", AvlTreeFindIndex(&AvlContext, 50));

    AvlTreeDeleteItem(&AvlContext, AvlContext.Root, 4);
    AvlTreeDeleteItem(&AvlContext, AvlContext.Root, 5);
    AvlTreeDeleteItem(&AvlContext, AvlContext.Root, 6);
    AvlTreeDeleteItem(&AvlContext, AvlContext.Root, 25);
    AvlTreeDeleteItem(&AvlContext, AvlContext.Root, 50);
    AvlTreeDeleteItem(&AvlContext, AvlContext.Root, 80);

    printf("\n......................................\n");

    printf("index50 : %d \n", AvlTreeFindIndex(&AvlContext, 50));
    printf("index80 : %d \n", AvlTreeFindIndex(&AvlContext, 80));
    printf("index33 : %d \n", AvlTreeFindIndex(&AvlContext, 33));
    printf("index2 : %d \n", AvlTreeFindIndex(&AvlContext, 2));
    printf("index35 : %d \n", AvlTreeFindIndex(&AvlContext, 35));
    printf("index70 : %d \n", AvlTreeFindIndex(&AvlContext, 70));
    printf("index115 : %d \n", AvlTreeFindIndex(&AvlContext, 115));
    printf("index5 : %d \n", AvlTreeFindIndex(&AvlContext, 5));
    printf("index25 : %d \n", AvlTreeFindIndex(&AvlContext, 25));
    printf("index150 : %d \n", AvlTreeFindIndex(&AvlContext, 150));
    printf("index100 : %d \n", AvlTreeFindIndex(&AvlContext, 100));
    printf("index4 : %d \n", AvlTreeFindIndex(&AvlContext, 4));
    printf("index47 : %d \n", AvlTreeFindIndex(&AvlContext, 47));
    printf("index45 : %d \n", AvlTreeFindIndex(&AvlContext, 45));
    printf("index50 : %d \n", AvlTreeFindIndex(&AvlContext, 50));

    printf("\n......................................\n");

    return 0;
}
