def swap(lst,x,y):
    tmp =lst[ x ]
    lst[x] = lst [y]
    lst[y] = tmp

def bubble_sort(lst):
    swapped = True
    while swapped:
        swapped = False
        for x in range(1,len(lst)):
            if lst[x] < lst[x-1]:
                swapped = True
                swap(lst,x, x-1)

x = [1,3,5,3,98,3,1,2,1]
print(x)
bubble_sort(x)
print(x)
