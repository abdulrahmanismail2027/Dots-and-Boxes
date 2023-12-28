void emtpyArray(int arr[], char size, char startIndex) // Changes values of the array from startIndex to the end to 0
{
    int i;
    for (i = startIndex; i < size; i++)
    {
        arr[i]=0;
    }
}
int nonZero(int array[],int size)
{
    int sum = 0; int i;
    for (i = 0; i < size; i++)
    {
        if (array[i]) sum++;
    }
    return sum;
}