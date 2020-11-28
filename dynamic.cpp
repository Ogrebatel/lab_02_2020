#include <iostream>
#include <vector>

using namespace std;

static void merge(vector<int>& buf, size_t l, size_t r, size_t m)
{
    if (l >= r || m < l || m > r) return;
    if (r == l + 1 && buf[l] > buf[r]) {
        swap(buf[l], buf[r]);
        return;
    }

    vector<int> tmp(&buf[l], &buf[l] + (r + 1));

    for (size_t i = l, j = 0, k = m - l + 1; i <= r; ++i) {
        if (j > m - l) {
            buf[i] = tmp[k++];
        } else if(k > r - l) {
            buf[i] = tmp[j++];
        } else {
            buf[i] = (tmp[j] < tmp[k]) ? tmp[j++] : tmp[k++];
        }
    }
}

void MergeSort(vector<int>& buf, size_t l, size_t r)
{
    if(l >= r) return;

    size_t m = (l + r) / 2;

    MergeSort(buf, l, m);
    MergeSort(buf, m+1, r);
    merge(buf, l, r, m);
}

void printing_array(vector<int>& arr, int size)
{
    for (int i=0; i<=size-1; i++)
        printf("%d ", arr[i]);
}

void r_filling_array(vector<int>& arr, int size, int maximum)
{
    for (int i=0; i<=size-1; i++)
        arr.emplace_back(rand()%maximum);
}

void h_filling_array(vector<int>& arr, int size)
{
    int tmp;
    for (int i=0; i<=size-1; i++) {
        scanf("%d", &tmp);
        arr.emplace_back(tmp);
    }
}

int main() {
    vector<int> buf;
    int tmp;
    int size;
    printf("%s", "Input size of array:");
    scanf("%d", &size);

    printf("Choose the method of filling:\n"
                 "1.Random filling\n"
                 "2.Hand filling");
    scanf("%d", &tmp);
    switch (tmp)
    {
        case 1:
            printf("Choose the module:");
            scanf("%d", &tmp);
            r_filling_array(buf, size, tmp);
            break;
        case 2:
            h_filling_array(buf, size);
            break;
    }

    while(true){
        printf("Choose action:\n"
               "1.Sort array\n"
               "2.Print array\n"
               "3.Exit");
        scanf("%d", &tmp);
        switch (tmp){
            case 1:
                MergeSort(buf, 0, size-1);
                break;
            case 2:
                printing_array(buf, size);
                break;
            default:
                return 0;
        }
    }
}
