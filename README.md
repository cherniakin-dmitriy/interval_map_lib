# interval_map_lib
interval_map is data structure which is similar to std::map, but provides assign for not single value, but range

Example of code:

```
...
interval_map<int, char> im('a');
std::cout << im[0] << std::endl; // 'a'
im.assign(1, 10, 'b');
std::cout << im[5] << std::endl; // 'b' as for any value in [1, 10)
...
```

Requirements were fulfilled by implementation:
* Best perfomance
* Requires only operator< implemented for Key type of interval_map
* Count of usages of operator< is minimized

Covered with GTest.
CI pipeline described in config.yml
