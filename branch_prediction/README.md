# How to compile

In Linux or OS X:

```
$ g++ branch_prediction_with_sorted_array.cpp -o branch_prediction_with_sorted_array
$ g++ branch_prediction_with_unsorted_array.cpp -o branch_prediction_with_unsorted_array
```

# How to profile the program

In Linux, we can use `perf`:

```
$ sudo perf stat ./branch_prediction_with_sorted_array
$ sudo perf stat ./branch_prediction_with_unsorted_array
```
